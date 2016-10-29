/*
 * This is for all the tests related to logic bugs (e.g. bad dereferences,
 * bad alignment, bad loops, bad locking, bad scheduling, deep stacks, and
 * lockups) along with other things that don't fit well into existing LKDTM
 * test source files.
 */
#include "lkdtm.h"
#include <linux/debugfs.h>
#include <linux/sched.h>

/*
 * Make sure our attempts to over run the kernel stack doesn't trigger
 * a compiler warning when CONFIG_FRAME_WARN is set. Then make sure we
 * recurse past the end of THREAD_SIZE by default.
 */
#if defined(CONFIG_FRAME_WARN) && (CONFIG_FRAME_WARN > 0)
#define REC_STACK_SIZE (CONFIG_FRAME_WARN / 2)
#else
#define REC_STACK_SIZE (THREAD_SIZE / 8)
#endif
#define REC_NUM_DEFAULT ((THREAD_SIZE / REC_STACK_SIZE) * 2)

static int recur_count = REC_NUM_DEFAULT;

static DEFINE_SPINLOCK(lock_me_up);

static int recursive_loop(int remaining)
{
	char buf[REC_STACK_SIZE];

	/* Make sure compiler does not optimize this away. */
	memset(buf, (remaining & 0xff) | 0x1, REC_STACK_SIZE);
	if (!remaining)
		return 0;
	else
		return recursive_loop(remaining - 1);
}

void lkdtm_PANIC(void)
{
	panic("dumptest");
}

void lkdtm_BUG(void)
{
	BUG();
}

void lkdtm_WARNING(void)
{
	WARN_ON(1);
}

void lkdtm_EXCEPTION(void)
{
	*((int *) 0) = 0;
}

void lkdtm_LOOP(void)
{
	for (;;)
		;
}

void lkdtm_OVERFLOW(void)
{
	(void) recursive_loop(recur_count);
}

noinline void lkdtm_CORRUPT_STACK(void)
{
	/* Use default char array length that triggers stack protection. */
	char data[8];

	memset((void *)data, 0, 64);
}

void lkdtm_UNALIGNED_LOAD_STORE_WRITE(void)
{
	static u8 data[5] __attribute__((aligned(4))) = {1, 2, 3, 4, 5};
	u32 *p;
	u32 val = 0x12345678;

	p = (u32 *)(data + 1);
	if (*p == 0)
		val = 0x87654321;
	*p = val;
}

void lkdtm_SOFTLOCKUP(void)
{
	preempt_disable();
	for (;;)
		cpu_relax();
}

void lkdtm_HARDLOCKUP(void)
{
	local_irq_disable();
	for (;;)
		cpu_relax();
}

void lkdtm_SPINLOCKUP(void)
{
	/* Must be called twice to trigger. */
	spin_lock(&lock_me_up);
	/* Let sparse know we intended to exit holding the lock. */
	__release(&lock_me_up);
}

void lkdtm_HUNG_TASK(void)
{
	set_current_state(TASK_UNINTERRUPTIBLE);
	schedule();
}

#define GENERATE_LKDTM_ATOMIC_TYPE(ignore1, atomic_name, ignore2)	\
																	\
static atomic_name##_t atomic_name##_var;							\
																	\
static int debugfs_##atomic_name##_get(void *data, u64 *val)		\
{																	\
	*val = atomic_name##_read((atomic_name##_t *)data);				\
	return 0;														\
}																	\
																	\
DEFINE_DEBUGFS_ATTRIBUTE(fops_##atomic_name,						\
						 debugfs_##atomic_name##_get,				\
						 NULL, "%lld\n");							\
																	\
static struct dentry *debugfs_create_##atomic_name(const char *item,\
				umode_t mode,										\
				struct dentry *parent,								\
				atomic_name##_t *value)								\
{																	\
	return debugfs_create_file_unsafe(item, mode, parent, value,	\
					&fops_##atomic_name);							\
}

LKDTM_ATOMIC_TYPES(GENERATE_LKDTM_ATOMIC_TYPE, 0, 0)

#define ATOMIC_TEST(name, atomic_name, init_func, start, safe_func,	\
					test_func_proto, testfunc)						\
																	\
void lkdtm_##name##_##testfunc(void)								\
{																	\
	atomic_name##_set(&atomic_name##_var, start);					\
																	\
	pr_info("attempting good " #testfunc "\n");						\
	safe_func(&atomic_name##_var);									\
	test_func_proto(testfunc, &atomic_name##_var);					\
																	\
	pr_info("attempting bad " #testfunc "\n");						\
	test_func_proto(testfunc, &atomic_name##_var);					\
}

/* Declare underflow test functions for atomic_t and atomic_long_t types. */
#define LKDTM_ATOMIC_UNDERFLOW(operation, test_func_proto)			\
	ATOMIC_TEST(UNDERFLOW, atomic, ATOMIC_INIT, INT_MIN,			\
				atomic_inc, test_func_proto, atomic_##operation)	\
	ATOMIC_TEST(UNDERFLOW, atomic_long, ATOMIC_LONG_INIT,			\
				LONG_MIN, atomic_long_inc, test_func_proto,			\
				atomic_long_##operation)							\
	ATOMIC_TEST(UNDERFLOW, atomic64, ATOMIC64_INIT,					\
				S64_MIN, atomic64_inc, test_func_proto,				\
				atomic64_##operation)								\
	ATOMIC_TEST(UNDERFLOW, local, LOCAL_INIT,						\
				LONG_MIN, local_inc, test_func_proto,				\
				local_##operation)

/* Declare overflow test functions for atomic_t and atomic_long_t types. */
#define LKDTM_ATOMIC_OVERFLOW(operation, test_func_proto)			\
	ATOMIC_TEST(OVERFLOW, atomic, ATOMIC_INIT, INT_MAX,				\
				atomic_dec, test_func_proto, atomic_##operation)	\
	ATOMIC_TEST(OVERFLOW, atomic_long, ATOMIC_LONG_INIT,			\
				LONG_MAX, atomic_long_dec, test_func_proto,			\
				atomic_long_##operation)							\
	ATOMIC_TEST(OVERFLOW, atomic64, ATOMIC64_INIT,					\
				S64_MAX, atomic64_dec, test_func_proto,				\
				atomic64_##operation)								\
	ATOMIC_TEST(OVERFLOW, local, LOCAL_INIT,						\
				LONG_MAX, local_dec, test_func_proto,				\
				local_##operation)

#define GENERATE_LKDTM_ATOMIC_TEST(name, operation, test_func_proto)	\
	LKDTM_ATOMIC_##name(operation, test_func_proto)

LKDTM_ATOMIC_OPERATIONS(GENERATE_LKDTM_ATOMIC_TEST)

#define GENERATE_LKDTM_ATOMIC_DEBUGFS(ignore1, atomic_name, ignore2)	\
	de = debugfs_create_##atomic_name(#atomic_name, 0644,				\
					  atomic_dir,										\
					  &atomic_name##_var);								\
	if (de == NULL)														\
		pr_err("could not create " #atomic_name " in debugfs\n");

void __init lkdtm_bugs_init(int *recur_param, struct dentry *parent) {
	struct dentry *atomic_dir, *de;

	/* If the depth is negative, use default, otherwise keep parameter. */
	if (*recur_param < 0)
		*recur_param = recur_count;
	else
		recur_count = *recur_param;

	/* Don't treat failures to create the atomic value tree as fatal. */
	atomic_dir = debugfs_create_dir("atomic", parent);
	if (atomic_dir) {
		LKDTM_ATOMIC_TYPES(GENERATE_LKDTM_ATOMIC_DEBUGFS, 0, 0);
	} else {
		pr_err("creating atomic dir failed\n");
 }
}
