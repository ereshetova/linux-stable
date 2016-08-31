#ifndef _ASM_X86_ATOMIC_H
#define _ASM_X86_ATOMIC_H

#include <linux/compiler.h>
#include <linux/types.h>
#include <asm/alternative.h>
#include <asm/cmpxchg.h>
#include <asm/rmwcc.h>
#include <asm/barrier.h>

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 */

#define ATOMIC_INIT(i)	{ (i) }

/**
 * atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.
 */
static __always_inline int atomic_read(const atomic_t *v)
{
	return READ_ONCE((v)->counter);
}

/**
 * atomic_read_wrap - read atomic variable
 * @v: pointer of type atomic_wrap_t
 *
 * Atomically reads the value of @v.
 */
static __always_inline int atomic_read_wrap(const atomic_wrap_t *v)
{
	return ACCESS_ONCE((v)->counter);
}

/**
 * atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.
 */
static __always_inline void atomic_set(atomic_t *v, int i)
{
	WRITE_ONCE(v->counter, i);
}

/**
 * atomic_set_wrap - set atomic variable
 * @v: pointer of type atomic_wrap_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.
 */
static __always_inline void atomic_set_wrap(atomic_wrap_t *v, int i)
{
	v->counter = i;
}

/**
 * atomic_add - add integer to atomic variable
 * @i: integer value to add
 * @v: pointer of type atomic_t
 *
 * Atomically adds @i to @v.
 */
static __always_inline void atomic_add(int i, atomic_t *v)
{
	asm volatile(LOCK_PREFIX "addl %1,%0\n"

#ifdef CONFIG_HARDENED_ATOMIC
		     "jno 0f\n"
		     LOCK_PREFIX "subl %1,%0\n"
		     "int $4\n0:\n"
		     _ASM_EXTABLE(0b, 0b)
#endif

		     : "+m" (v->counter)
		     : "ir" (i));
}

/**
 * atomic_add_wrap - add integer to atomic variable
 * @i: integer value to add
 * @v: pointer of type atomic_wrap_t
 *
 * Atomically adds @i to @v.
 */
static __always_inline void atomic_add_wrap(int i, atomic_wrap_t *v)
{
	asm volatile(LOCK_PREFIX "addl %1,%0\n"
		     : "+m" (v->counter)
		     : "ir" (i));
}

/**
 * atomic_sub - subtract integer from atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 *
 * Atomically subtracts @i from @v.
 */
static __always_inline void atomic_sub(int i, atomic_t *v)
{
	asm volatile(LOCK_PREFIX "subl %1,%0\n"

#ifdef CONFIG_HARDENED_ATOMIC
		     "jno 0f\n"
		     LOCK_PREFIX "addl %1,%0\n"
		     "int $4\n0:\n"
		     _ASM_EXTABLE(0b, 0b)
#endif

		     : "+m" (v->counter)
		     : "ir" (i));
}

/**
 * atomic_sub_wrap - subtract integer from atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic_wrap_t
 *
 * Atomically subtracts @i from @v.
 */
static __always_inline void atomic_sub_wrap(int i, atomic_wrap_t *v)
{
	asm volatile(LOCK_PREFIX "subl %1,%0\n"
		     : "+m" (v->counter)
		     : "ir" (i));
}

/**
 * atomic_sub_and_test - subtract value from variable and test result
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 *
 * Atomically subtracts @i from @v and returns
 * true if the result is zero, or false for all
 * other cases.
 */
static __always_inline bool atomic_sub_and_test(int i, atomic_t *v)
{
	GEN_BINARY_RMWcc(LOCK_PREFIX "subl", LOCK_PREFIX "addl", v->counter, "er", i, "%0", e);
}

/**
 * atomic_sub_and_test_wrap - subtract value from variable and test result
 * @i: integer value to subtract
 * @v: pointer of type atomic_wrap_t
 *
 * Atomically subtracts @i from @v and returns
 * true if the result is zero, or false for all
 * other cases.
 */
static __always_inline bool atomic_sub_and_test_wrap(int i, atomic_wrap_t *v)
{
	GEN_BINARY_RMWcc_wrap(LOCK_PREFIX "subl", v->counter, "er", i, "%0", e);
}

/**
 * atomic_inc - increment atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1.
 */
static __always_inline void atomic_inc(atomic_t *v)
{
	asm volatile(LOCK_PREFIX "incl %0\n"

#ifdef CONFIG_HARDENED_ATOMIC
		     "jno 0f\n"
		     LOCK_PREFIX "decl %0\n"
		     "int $4\n0:\n"
		     _ASM_EXTABLE(0b, 0b)
#endif

		     : "+m" (v->counter));
}

/**
 * atomic_inc_wrap - increment atomic variable
 * @v: pointer of type atomic_wrap_t
 *
 * Atomically increments @v by 1.
 */
static __always_inline void atomic_inc_wrap(atomic_wrap_t *v)
{
	asm volatile(LOCK_PREFIX "incl %0\n"
		     : "+m" (v->counter));
}

/**
 * atomic_dec - decrement atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1.
 */
static __always_inline void atomic_dec(atomic_t *v)
{
	asm volatile(LOCK_PREFIX "decl %0\n"

#ifdef CONFIG_HARDENED_ATOMIC
		     "jno 0f\n"
		     LOCK_PREFIX "incl %0\n"
		     "int $4\n0:\n"
		     _ASM_EXTABLE(0b, 0b)
#endif

		     : "+m" (v->counter));
}

/**
 * atomic_dec_wrap - decrement atomic variable
 * @v: pointer of type atomic_wrap_t
 *
 * Atomically decrements @v by 1.
 */
static __always_inline void atomic_dec_wrap(atomic_wrap_t *v)
{
	asm volatile(LOCK_PREFIX "decl %0\n"
		     : "+m" (v->counter));
}

/**
 * atomic_dec_and_test - decrement and test
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1 and
 * returns true if the result is 0, or false for all other
 * cases.
 */
static __always_inline bool atomic_dec_and_test(atomic_t *v)
{
	GEN_UNARY_RMWcc(LOCK_PREFIX "decl", LOCK_PREFIX "incl", v->counter, "%0", e);
}

#ifdef CONFIG_HARDENED_ATOMIC
static __always_inline bool atomic_dec_and_test_wrap(atomic_wrap_t *v)
{
	GEN_UNARY_RMWcc_wrap(LOCK_PREFIX "decl", v->counter, "%0", e);
}
#endif /* CONFIG_HARDENED_ATOMIC */

/**
 * atomic_inc_and_test - increment and test
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
static __always_inline bool atomic_inc_and_test(atomic_t *v)
{
	GEN_UNARY_RMWcc(LOCK_PREFIX "incl", LOCK_PREFIX "decl", v->counter, "%0", e);
}

/**
 * atomic_inc_and_test_wrap - increment and test
 * @v: pointer of type atomic_wrap_t
 *
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
static __always_inline int atomic_inc_and_test_wrap(atomic_wrap_t *v)
{
	GEN_UNARY_RMWcc_wrap(LOCK_PREFIX "incl", v->counter, "%0", e);
}

/**
 * atomic_add_negative - add and test if negative
 * @i: integer value to add
 * @v: pointer of type atomic_t
 *
 * Atomically adds @i to @v and returns true
 * if the result is negative, or false when
 * result is greater than or equal to zero.
 */
static __always_inline bool atomic_add_negative(int i, atomic_t *v)
{
	GEN_BINARY_RMWcc(LOCK_PREFIX "addl", LOCK_PREFIX "subl", v->counter, "er", i, "%0", s);
}

#ifdef CONFIG_HARDENED_ATOMIC
static __always_inline bool atomic_add_negative_wrap(int i, atomic_wrap_t *v)
{
	GEN_BINARY_RMWcc_wrap(LOCK_PREFIX "addl", v->counter, "er", i, "%0", s);
}
#endif /* CONFIG_HARDENED_ATOMIC */

/**
 * atomic_add_return - add integer and return
 * @i: integer value to add
 * @v: pointer of type atomic_t
 *
 * Atomically adds @i to @v and returns @i + @v
 */
static __always_inline int atomic_add_return(int i, atomic_t *v)
{
	return i + xadd_check_overflow(&v->counter, i);
}

/**
 * atomic_add_return_wrap - add integer and return
 * @i: integer value to add
 * @v: pointer of type atomic_wrap_t
 *
 * Atomically adds @i to @v and returns @i + @v
 */
static __always_inline int atomic_add_return_wrap(int i, atomic_wrap_t *v)
{
	return i + xadd(&v->counter, i);
}

/**
 * atomic_sub_return - subtract integer and return
 * @v: pointer of type atomic_t
 * @i: integer value to subtract
 *
 * Atomically subtracts @i from @v and returns @v - @i
 */
static __always_inline int atomic_sub_return(int i, atomic_t *v)
{
	return atomic_add_return(-i, v);
}

#ifdef CONFIG_HARDENED_ATOMIC
static __always_inline int atomic_sub_return_wrap(int i, atomic_wrap_t *v)
{
	return atomic_add_return_wrap(-i, v);
}
#endif /* CONFIG_HARDENED_ATOMIC */

#define atomic_inc_return(v)  (atomic_add_return(1, v))
static __always_inline int atomic_inc_return_wrap(atomic_wrap_t *v)
{
	return atomic_add_return_wrap(1, v);
}

#define atomic_dec_return(v)  (atomic_sub_return(1, v))
#ifdef CONFIG_HARDENED_ATOMIC
static __always_inline int atomic_dec_return_wrap(atomic_wrap_t *v)
{
	return atomic_sub_return_wrap(1, v);
}
#endif /* CONFIG_HARDENED_ATOMIC */

static __always_inline int atomic_fetch_add(int i, atomic_t *v)
{
	return xadd_check_overflow(&v->counter, i);
}

static __always_inline int atomic_fetch_add_wrap(int i, atomic_wrap_t *v)
{
	return xadd(&v->counter, i);
}

static __always_inline int atomic_fetch_sub(int i, atomic_t *v)
{
	return xadd_check_overflow(&v->counter, -i);
}

static __always_inline int atomic_fetch_sub_wrap(int i, atomic_wrap_t *v)
{
	return xadd(&v->counter, -i);
}

static __always_inline int atomic_cmpxchg(atomic_t *v, int old, int new)
{
	return cmpxchg(&v->counter, old, new);
}

static __always_inline int atomic_cmpxchg_wrap(atomic_wrap_t *v, int old, int new)
{
	return cmpxchg(&v->counter, old, new);
}

static inline int atomic_xchg(atomic_t *v, int new)
{
	return xchg(&v->counter, new);
}

static inline int atomic_xchg_wrap(atomic_wrap_t *v, int new)
{
	return xchg(&v->counter, new);
}

#define ATOMIC_OP(op)							\
static inline void atomic_##op(int i, atomic_t *v)			\
{									\
	asm volatile(LOCK_PREFIX #op"l %1,%0"				\
			: "+m" (v->counter)				\
			: "ir" (i)					\
			: "memory");					\
}

#define ATOMIC_FETCH_OP(op, c_op)					\
static inline int atomic_fetch_##op(int i, atomic_t *v)		\
{									\
	int old, val = atomic_read(v);					\
	for (;;) {							\
		old = atomic_cmpxchg(v, val, val c_op i);		\
		if (old == val)						\
			break;						\
		val = old;						\
	}								\
	return old;							\
}

#define ATOMIC_OPS(op, c_op)						\
	ATOMIC_OP(op)							\
	ATOMIC_FETCH_OP(op, c_op)

ATOMIC_OPS(and, &)
ATOMIC_OPS(or , |)
ATOMIC_OPS(xor, ^)

#undef ATOMIC_OPS
#undef ATOMIC_FETCH_OP
#undef ATOMIC_OP

/**
 * __atomic_add_unless - add unless the number is already a given value
 * @v: pointer of type atomic_t
 * @a: the amount to add to v...
 * @u: ...unless v is equal to u.
 *
 * Atomically adds @a to @v, so long as @v was not already @u.
 * Returns the old value of @v.
 */
static __always_inline int __atomic_add_unless(atomic_t *v, int a, int u)
{
	int c, old, new;
	c = atomic_read(v);
	for (;;) {
		if (unlikely(c == (u)))
			break;

		asm volatile("addl %2,%0\n"

#ifdef CONFIG_HARDENED_ATOMIC
			     "jno 0f\n"
			     "subl %2,%0\n"
			     "int $4\n0:\n"
			     _ASM_EXTABLE(0b, 0b)
#endif

			     : "=r" (new)
			     : "0" (c), "ir" (a));

		old = atomic_cmpxchg((v), c, new);
		if (likely(old == c))
			break;
		c = old;
	}
	return c;
}

/**
 * __atomic_add_unless_wrap - add unless the number is already a given value
 * @v: pointer of type atomic_wrap_t
 * @a: the amount to add to v...
 * @u: ...unless v is equal to u.
 *
 * Atomically adds @a to @v, so long as @v was not already @u.
 * Returns the old value of @v.
 */
static __always_inline int __atomic_add_unless_wrap(atomic_wrap_t *v,
						    int a, int u)
{
	int c, old, new;
	c = atomic_read_wrap(v);
	for (;;) {
		if (unlikely(c == (u)))
			break;

		asm volatile("addl %2,%0\n"
			     : "=r" (new)
			     : "0" (c), "ir" (a));

		old = atomic_cmpxchg_wrap((v), c, new);
		if (likely(old == c))
			break;
		c = old;
	}
	return c;
}

/**
+ * atomic_inc_not_zero_hint - increment if not null
+ * @v: pointer of type atomic_t
+ * @hint: probable value of the atomic before the increment
+ *
+ * This version of atomic_inc_not_zero() gives a hint of probable
+ * value of the atomic. This helps processor to not read the memory
+ * before doing the atomic read/modify/write cycle, lowering
+ * number of bus transactions on some arches.
+ *
+ * Returns: 0 if increment was not done, 1 otherwise.
+ */
#define atomic_inc_not_zero_hint atomic_inc_not_zero_hint
static inline int atomic_inc_not_zero_hint(atomic_t *v, int hint)
{
	int val, c = hint, new;

	/* sanity test, should be removed by compiler if hint is a constant */
	if (!hint)
		return __atomic_add_unless(v, 1, 0);

	do {
		asm volatile("incl %0\n"

#ifdef CONFIG_HARDENED_ATOMIC
			     "jno 0f\n"
			     "decl %0\n"
			     "int $4\n0:\n"
			     _ASM_EXTABLE(0b, 0b)
#endif

			     : "=r" (new)
			     : "0" (c));

		val = atomic_cmpxchg((v), c, new);
		if (val == c)
			return 1;
		c = val;
	} while (c);

	return 0;
}

/**
 * atomic_inc_short - increment of a short integer
 * @v: pointer to type int
 *
 * Atomically adds 1 to @v
 * Returns the new value of @u
 */
static __always_inline short int atomic_inc_short(short int *v)
{
	asm(LOCK_PREFIX "addw $1, %0" : "+m" (*v));
	return *v;
}

#ifdef CONFIG_X86_32
# include <asm/atomic64_32.h>
#else
# include <asm/atomic64_64.h>
#endif

#endif /* _ASM_X86_ATOMIC_H */
