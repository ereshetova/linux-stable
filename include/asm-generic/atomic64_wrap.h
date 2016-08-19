#ifndef _ASM_GENERIC_ATOMIC64_WRAP_H
#define _ASM_GENERIC_ATOMIC64_WRAP_H

#ifndef CONFIG_HARDENED_ATOMIC

#ifndef atomic64_wrap_t
#define atomic64_wrap_t atomic64_wrap_t
typedef struct {
	long counter;
} atomic64_wrap_t;
#endif

/*
 * CONFIG_HARDENED_ATOMIC requires a non-generic implementation of ATOMIC64.
 * This only serves to make the wrap-functions available when HARDENED_ATOMIC is
 * either unimplemented or unset.
 */

static inline long long atomic64_read_wrap(const atomic64_wrap_t *v)
{
	return atomic64_read((atomic64_t *) v);
}
#define atomic64_read_wrap atomic64_read_wrap

static inline void atomic64_set_wrap(atomic64_wrap_t *v, long long i)
{
	atomic64_set((atomic64_t *) v, i);
}
#define atomic64_set_wrap atomic64_set_wrap

static inline void atomic64_add_wrap(long long a, atomic64_wrap_t *v)
{
	atomic64_add(a, (atomic64_t *) v);
}
#define atomic64_add_wrap atomic64_add_wrap

static inline long long atomic64_add_return_wrap(long long a, atomic64_wrap_t *v)
{
	return atomic64_add_return(a, (atomic64_t *) v);
}
#define atomic64_add_return_wrap atomic64_add_return_wrap

static inline void atomic64_sub_wrap(long long a, atomic64_wrap_t *v)
{
	atomic64_sub(a, (atomic64_t *) v);
}
#define atomic64_sub_wrap atomic64_sub_wrap

static inline long long atomic64_sub_return_wrap(long long a, atomic64_wrap_t *v)
{
	return atomic64_sub_return(a, (atomic64_t *) v);
}
#define atomic64_sub_return_wrap atomic64_sub_return_wrap

static inline long long atomic64_sub_and_test_wrap(long long a, atomic64_wrap_t *v)
{
	return atomic64_sub_and_test(a, (atomic64_t *) v);
}
#define atomic64_sub_and_test_wrap atomic64_sub_and_test_wrap

static inline void atomic64_inc_wrap(atomic64_wrap_t *v)
{
	atomic64_inc((atomic64_t *) v);
}
#define atomic64_inc_wrap atomic64_inc_wrap

static inline long long atomic64_inc_return_wrap(atomic64_wrap_t *v)
{
	return atomic64_inc_return((atomic64_t *) v);
}
#define atomic64_inc_return_wrap atomic64_inc_return_wrap

static inline long long atomic64_inc_and_test_wrap(atomic64_wrap_t *v)
{
	return atomic64_inc_and_test((atomic64_t *) v);
}
#define atomic64_inc_and_test_wrap atomic64_inc_and_test_wrap

static inline void atomic64_dec_wrap(atomic64_wrap_t *v)
{
	atomic64_dec((atomic64_t *) v);
}
#define atomic64_dec_wrap atomic64_dec_wrap

static inline long long atomic64_dec_return_wrap(atomic64_wrap_t *v)
{
	return atomic64_dec_return((atomic64_t *) v);
}
#define atomic64_dec_return_wrap atomic64_dec_return_wrap

static inline long long atomic64_dec_and_test_wrap(atomic64_wrap_t *v)
{
	return atomic64_dec_and_test((atomic64_t *) v);
}
#define atomic64_dec_and_test_wrap atomic64_dec_and_test_wrap

static inline long long atomic64_cmpxchg_wrap(atomic64_wrap_t *v, long long o, long long n)
{
	return atomic64_cmpxchg((atomic64_t *) v, o, n);
}
#define atomic64_cmpxchg_wrap atomic64_cmpxchg_wrap

static inline long long atomic64_xchg_wrap(atomic64_wrap_t *v, long long n)
{
	return atomic64_xchg((atomic64_t *) v, n);
}
#define atomic64_xchg_wrap atomic64_xchg_wrap

static inline bool atomic64_add_negative_wrap(long i, atomic64_wrap_t *v)
{
	return atomic64_add_negative(i, (atomic64_t *) v);
}
#define atomic64_add_negative_wrap atomic64_add_negative_wrap

static inline bool atomic64_add_unless_wrap(atomic64_wrap_t *v, long a, long u)
{
	return atomic64_add_unless((atomic64_t *) v, a, u);
}
#define atomic64_add_unless_wrap atomic64_add_unless_wrap

#endif /* CONFIG_HARDENED_ATOMIC */

#endif /* _ASM_GENERIC_ATOMIC64_WRAP_H */
