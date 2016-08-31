#ifndef _ASM_X86_LOCAL_H
#define _ASM_X86_LOCAL_H

#include <linux/percpu.h>

#include <linux/atomic.h>
#include <asm/asm.h>

typedef struct {
	atomic_long_t a;
} local_t;

typedef struct {
	atomic_long_wrap_t a;
} local_wrap_t;

#define LOCAL_INIT(i)	{ ATOMIC_LONG_INIT(i) }

#define local_read(l)	atomic_long_read(&(l)->a)
#define local_read_wrap(l)	atomic_long_read_wrap(&(l)->a)
#define local_set(l, i)	atomic_long_set(&(l)->a, (i))
#define local_set_wrap(l, i)	atomic_long_set_wrap(&(l)->a, (i))

static inline void local_inc(local_t *l)
{
	asm volatile(_ASM_INC "%0\n"
#ifdef CONFIG_HARDENED_ATOMIC
		     "jno 0f\n"
		     _ASM_DEC "%0\n"
		     "int $4\n0:\n"
		     _ASM_EXTABLE(0b, 0b)
#endif
		     : "+m" (l->a.counter));
}

static inline void local_inc_wrap(local_wrap_t *l)
{
	asm volatile(_ASM_INC "%0"
		     : "+m" (l->a.counter));
}

static inline void local_dec(local_t *l)
{
	asm volatile(_ASM_DEC "%0\n"

#ifdef CONFIG_HARDENED_ATOMIC
		     "jno 0f\n"
		     _ASM_INC "%0\n"
		     "int $4\n0:\n"
		     _ASM_EXTABLE(0b, 0b)
#endif
		     : "+m" (l->a.counter));
}

static inline void local_dec_wrap(local_wrap_t *l)
{
	asm volatile(_ASM_DEC "%0"
		     : "+m" (l->a.counter));
}

static inline void local_add(long i, local_t *l)
{
	asm volatile(_ASM_ADD "%1,%0\n"
#ifdef CONFIG_HARDENED_ATOMIC
		     "jno 0f\n"
		     _ASM_SUB "%1,%0\n"
		     "int $4\n0:\n"
		     _ASM_EXTABLE(0b, 0b)
#endif
		     : "+m" (l->a.counter)
		     : "ir" (i));
}

static inline void local_add_wrap(long i, local_wrap_t *l)
{
	asm volatile(_ASM_ADD "%1,%0"
		     : "+m" (l->a.counter)
		     : "ir" (i));
}

static inline void local_sub(long i, local_t *l)
{
	asm volatile(_ASM_SUB "%1,%0\n"
#ifdef CONFIG_HARDENED_ATOMIC
		     "jno 0f\n"
		     _ASM_ADD "%1,%0\n"
		     "int $4\n0:\n"
		     _ASM_EXTABLE(0b, 0b)
#endif
		     : "+m" (l->a.counter)
		     : "ir" (i));
}

static inline void local_sub_wrap(long i, local_wrap_t *l)
{
	asm volatile(_ASM_SUB "%1,%0"
		     : "+m" (l->a.counter)
		     : "ir" (i));
}

/**
 * local_sub_and_test - subtract value from variable and test result
 * @i: integer value to subtract
 * @l: pointer to type local_t
 *
 * Atomically subtracts @i from @l and returns
 * true if the result is zero, or false for all
 * other cases.
 */
static inline bool local_sub_and_test(long i, local_t *l)
{
	GEN_BINARY_RMWcc(_ASM_SUB, _ASM_ADD, l->a.counter, "er", i, "%0", e);
}

static inline bool local_sub_and_test_wrap(long i, local_wrap_t *l)
{
	GEN_BINARY_RMWcc_wrap(_ASM_SUB, l->a.counter, "er", i, "%0", e);
}

/**
 * local_dec_and_test - decrement and test
 * @l: pointer to type local_t
 *
 * Atomically decrements @l by 1 and
 * returns true if the result is 0, or false for all other
 * cases.
 */
static inline bool local_dec_and_test(local_t *l)
{
	GEN_UNARY_RMWcc(_ASM_DEC, _ASM_INC, l->a.counter, "%0", e);
}

static inline bool local_dec_and_test_wrap(local_wrap_t *l)
{
	GEN_UNARY_RMWcc_wrap(_ASM_DEC, l->a.counter, "%0", e);
}

/**
 * local_inc_and_test - increment and test
 * @l: pointer to type local_t
 *
 * Atomically increments @l by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
static inline bool local_inc_and_test(local_t *l)
{
	GEN_UNARY_RMWcc(_ASM_INC, _ASM_DEC, l->a.counter, "%0", e);
}

static inline bool local_inc_and_test_wrap(local_wrap_t *l)
{
	GEN_UNARY_RMWcc_wrap(_ASM_INC, l->a.counter, "%0", e);
}

/**
 * local_add_negative - add and test if negative
 * @i: integer value to add
 * @l: pointer to type local_t
 *
 * Atomically adds @i to @l and returns true
 * if the result is negative, or false when
 * result is greater than or equal to zero.
 */
static inline bool local_add_negative(long i, local_t *l)
{
	GEN_BINARY_RMWcc(_ASM_ADD, _ASM_SUB, l->a.counter, "er", i, "%0", s);
}

static inline bool local_add_negative_wrap(long i, local_wrap_t *l)
{
	GEN_BINARY_RMWcc_wrap(_ASM_ADD, l->a.counter, "er", i, "%0", s);
}

/**
 * local_add_return - add and return
 * @i: integer value to add
 * @l: pointer to type local_t
 *
 * Atomically adds @i to @l and returns @i + @l
 */
static inline long local_add_return(long i, local_t *l)
{
	long __i = i;
	asm volatile(_ASM_XADD "%0, %1\n"
#ifdef CONFIG_HARDENED_ATOMIC
		     "jno 0f\n"
		     _ASM_MOV "%0,%1\n"
		     "int $4\n0:\n"
		     _ASM_EXTABLE(0b, 0b)
#endif
		     : "+r" (i), "+m" (l->a.counter)
		     : : "memory");
	return i + __i;
}

/**
 * local_add_return_wrap - add and return
 * @i: integer value to add
 * @l: pointer to type local_wrap_t
 *
 * Atomically adds @i to @l and returns @i + @l
 */
static inline long local_add_return_wrap(long i, local_wrap_t *l)
{
	long __i = i;
	asm volatile(_ASM_XADD "%0, %1;"
		     : "+r" (i), "+m" (l->a.counter)
		     : : "memory");
	return i + __i;
}

static inline long local_sub_return(long i, local_t *l)
{
	return local_add_return(-i, l);
}

static inline long local_sub_return_wrap(long i, local_wrap_t *l)
{
	return local_add_return_wrap(-i, l);
}

#define local_inc_return(l)  (local_add_return(1, l))
#define local_dec_return(l)  (local_sub_return(1, l))
#define local_inc_return_wrap(l)  (local_add_return_wrap(1, l))
#define local_dec_return_wrap(l)  (local_sub_return_wrap(1, l))

#define local_cmpxchg(l, o, n) \
	(cmpxchg_local(&((l)->a.counter), (o), (n)))
#define local_cmpxchg_wrap(l, o, n) \
	(cmpxchg_local(&((l)->a.counter), (o), (n)))
/* Always has a lock prefix */
#define local_xchg(l, n) (xchg(&((l)->a.counter), (n)))

/**
 * local_add_unless - add unless the number is a given value
 * @l: pointer of type local_t
 * @a: the amount to add to l...
 * @u: ...unless l is equal to u.
 *
 * Atomically adds @a to @l, so long as it was not @u.
 * Returns non-zero if @l was not @u, and zero otherwise.
 */
static inline long local_add_unless(local_t *l, long a, long u)
{
	long c, old, new;
	c = local_read((l));
	for (;;) {
		if (unlikely(c == (u)))
			break;

		asm volatile(_ASM_ADD "%2,%0\n"
#ifdef CONFIG_HARDENED_ATOMIC
			     "jno 0f\n"
			     _ASM_SUB "%2,%0\n"
			     "int $4\n0:\n"
			     _ASM_EXTABLE(0b, 0b)
#endif /* CONFIG_HARDENED_ATOMIC */
			     : "=r" (new)
			     : "0" (c), "ir" (a));

		old = local_cmpxchg((l), c, new);
		if (likely(old == c))
			break;
		c = old;
	}
	return c != (u);
}

#define local_add_unless_wrap(l, a, u)				\
({								\
	long c, old;						\
	c = local_read_wrap((l));				\
	for (;;) {						\
		if (unlikely(c == (u)))				\
			break;					\
		old = local_cmpxchg_wrap((l), c, c + (a));	\
		if (likely(old == c))				\
			break;					\
		c = old;					\
	}							\
	c != (u);						\
})

#define local_inc_not_zero(l) local_add_unless((l), 1, 0)

/* On x86_32, these are no better than the atomic variants.
 * On x86-64 these are better than the atomic variants on SMP kernels
 * because they dont use a lock prefix.
 */
#define __local_inc(l)		local_inc(l)
#define __local_dec(l)		local_dec(l)
#define __local_add(i, l)	local_add((i), (l))
#define __local_sub(i, l)	local_sub((i), (l))

#endif /* _ASM_X86_LOCAL_H */
