#ifndef _ASM_GENERIC_ATOMIC_LONG_H
#define _ASM_GENERIC_ATOMIC_LONG_H
/*
 * Copyright (C) 2005 Silicon Graphics, Inc.
 *	Christoph Lameter
 *
 * Allows to provide arch independent atomic definitions without the need to
 * edit all arch specific atomic.h files.
 */

#include <asm/types.h>

/*
 * Suppport for atomic_long_t
 *
 * Casts for parameters are avoided for existing atomic functions in order to
 * avoid issues with cast-as-lval under gcc 4.x and other limitations that the
 * macros of a platform may have.
 */

#if BITS_PER_LONG == 64

typedef atomic64_t atomic_long_t;

#ifdef CONFIG_HARDENED_ATOMIC
typedef atomic64_wrap_t atomic_long_wrap_t;
#else
typedef atomic64_t atomic_long_wrap_t;
#endif

#define ATOMIC_LONG_INIT(i)	ATOMIC64_INIT(i)
#define ATOMIC_LONG_PFX(x)	atomic64 ## x

#else

typedef atomic_t atomic_long_t;

#ifdef CONFIG_HARDENED_ATOMIC
typedef atomic_wrap_t atomic_long_wrap_t;
#else
typedef atomic_t atomic_long_wrap_t;
#endif

#define ATOMIC_LONG_INIT(i)	ATOMIC_INIT(i)
#define ATOMIC_LONG_PFX(x)	atomic ## x

#endif

#define ATOMIC_LONG_READ_OP(mo, suffix)						\
static inline long atomic_long_read##mo##suffix(const atomic_long##suffix##_t *l)\
{									\
	ATOMIC_LONG_PFX(suffix##_t) *v = (ATOMIC_LONG_PFX(suffix##_t) *)l;\
									\
	return (long)ATOMIC_LONG_PFX(_read##mo##suffix)(v);		\
}
ATOMIC_LONG_READ_OP(,)
ATOMIC_LONG_READ_OP(_acquire,)

#ifdef CONFIG_HARDENED_ATOMIC
ATOMIC_LONG_READ_OP(,_wrap)
#else /* CONFIG_HARDENED_ATOMIC */
#define atomic_long_read_wrap(v) atomic_long_read((v))
#endif /* CONFIG_HARDENED_ATOMIC */

#undef ATOMIC_LONG_READ_OP

#define ATOMIC_LONG_SET_OP(mo, suffix)					\
static inline void atomic_long_set##mo##suffix(atomic_long##suffix##_t *l, long i)\
{									\
	ATOMIC_LONG_PFX(suffix##_t) *v = (ATOMIC_LONG_PFX(suffix##_t) *)l;\
									\
	ATOMIC_LONG_PFX(_set##mo##suffix)(v, i);			\
}
ATOMIC_LONG_SET_OP(,)
ATOMIC_LONG_SET_OP(_release,)

#ifdef CONFIG_HARDENED_ATOMIC
ATOMIC_LONG_SET_OP(,_wrap)
#else /* CONFIG_HARDENED_ATOMIC */
#define atomic_long_set_wrap(v, i) atomic_long_set((v), (i))
#endif /* CONFIG_HARDENED_ATOMIC */

#undef ATOMIC_LONG_SET_OP

#define ATOMIC_LONG_ADD_SUB_OP(op, mo, suffix)				\
static inline long							\
atomic_long_##op##_return##mo##suffix(long i, atomic_long##suffix##_t *l)\
{									\
	ATOMIC_LONG_PFX(suffix##_t) *v = (ATOMIC_LONG_PFX(suffix##_t) *)l;\
									\
	return (long)ATOMIC_LONG_PFX(_##op##_return##mo##suffix)(i, v);\
}
ATOMIC_LONG_ADD_SUB_OP(add,,)
ATOMIC_LONG_ADD_SUB_OP(add, _relaxed,)
ATOMIC_LONG_ADD_SUB_OP(add, _acquire,)
ATOMIC_LONG_ADD_SUB_OP(add, _release,)
ATOMIC_LONG_ADD_SUB_OP(sub,,)
ATOMIC_LONG_ADD_SUB_OP(sub, _relaxed,)
ATOMIC_LONG_ADD_SUB_OP(sub, _acquire,)
ATOMIC_LONG_ADD_SUB_OP(sub, _release,)

#ifdef CONFIG_HARDENED_ATOMIC
ATOMIC_LONG_ADD_SUB_OP(add,,_wrap)
ATOMIC_LONG_ADD_SUB_OP(sub,,_wrap)
#else /* CONFIG_HARDENED_ATOMIC */
#define atomic_long_add_return_wrap(i,v) atomic_long_add_return((i), (v))
#define atomic_long_sub_return_wrap(i,v) atomic_long_sub_return((i), (v))
#endif /* CONFIG_HARDENED_ATOMIC */

#undef ATOMIC_LONG_ADD_SUB_OP

#define atomic_long_cmpxchg_relaxed(l, old, new) \
	(ATOMIC_LONG_PFX(_cmpxchg_relaxed)((ATOMIC_LONG_PFX(_t) *)(l), \
					   (old), (new)))
#define atomic_long_cmpxchg_acquire(l, old, new) \
	(ATOMIC_LONG_PFX(_cmpxchg_acquire)((ATOMIC_LONG_PFX(_t) *)(l), \
					   (old), (new)))
#define atomic_long_cmpxchg_release(l, old, new) \
	(ATOMIC_LONG_PFX(_cmpxchg_release)((ATOMIC_LONG_PFX(_t) *)(l), \
					   (old), (new)))
#define atomic_long_cmpxchg(l, old, new) \
	(ATOMIC_LONG_PFX(_cmpxchg)((ATOMIC_LONG_PFX(_t) *)(l), (old), (new)))

#ifdef CONFIG_HARDENED_ATOMIC
#define atomic_long_cmpxchg_wrap(l, old, new) \
	(ATOMIC_LONG_PFX(_cmpxchg_wrap)((ATOMIC_LONG_PFX(_wrap_t) *)(l), (old), (new)))
#else /* CONFIG_HARDENED_ATOMIC */
#define atomic_long_cmpxchg_wrap(v, o, n) atomic_long_cmpxchg((v), (o), (n))
#endif /* CONFIG_HARDENED_ATOMIC */

#define atomic_long_xchg_relaxed(v, new) \
	(ATOMIC_LONG_PFX(_xchg_relaxed)((ATOMIC_LONG_PFX(_t) *)(v), (new)))
#define atomic_long_xchg_acquire(v, new) \
	(ATOMIC_LONG_PFX(_xchg_acquire)((ATOMIC_LONG_PFX(_t) *)(v), (new)))
#define atomic_long_xchg_release(v, new) \
	(ATOMIC_LONG_PFX(_xchg_release)((ATOMIC_LONG_PFX(_t) *)(v), (new)))
#define atomic_long_xchg(v, new) \
	(ATOMIC_LONG_PFX(_xchg)((ATOMIC_LONG_PFX(_t) *)(v), (new)))

#ifdef CONFIG_HARDENED_ATOMIC
#define atomic_long_xchg_wrap(v, new) \
	(ATOMIC_LONG_PFX(_xchg_wrap)((ATOMIC_LONG_PFX(_wrap_t) *)(v), (new)))
#else /* CONFIG_HARDENED_ATOMIC */
#define atomic_long_xchg_wrap(v, i) atomic_long_xchg((v), (i))
#endif /* CONFIG_HARDENED_ATOMIC */

static __always_inline void atomic_long_inc(atomic_long_t *l)
{
	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;

	ATOMIC_LONG_PFX(_inc)(v);
}

#ifdef CONFIG_HARDENED_ATOMIC
static __always_inline void atomic_long_inc_wrap(atomic_long_wrap_t *l)
{
	ATOMIC_LONG_PFX(_wrap_t) *v = (ATOMIC_LONG_PFX(_wrap_t) *)l;

	ATOMIC_LONG_PFX(_inc_wrap)(v);
}
#else
#define atomic_long_inc_wrap(v) atomic_long_inc(v)
#endif

static __always_inline void atomic_long_dec(atomic_long_t *l)
{
	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;

	ATOMIC_LONG_PFX(_dec)(v);
}

#ifdef CONFIG_HARDENED_ATOMIC
static __always_inline void atomic_long_dec_wrap(atomic_long_wrap_t *l)
{
	ATOMIC_LONG_PFX(_wrap_t) *v = (ATOMIC_LONG_PFX(_wrap_t) *)l;

	ATOMIC_LONG_PFX(_dec_wrap)(v);
}
#else
#define atomic_long_dec_wrap(v) atomic_long_dec(v)
#endif

#define ATOMIC_LONG_FETCH_OP(op, mo)					\
static inline long							\
atomic_long_fetch_##op##mo(long i, atomic_long_t *l)			\
{									\
	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;		\
									\
	return (long)ATOMIC_LONG_PFX(_fetch_##op##mo)(i, v);		\
}

ATOMIC_LONG_FETCH_OP(add, )
ATOMIC_LONG_FETCH_OP(add, _relaxed)
ATOMIC_LONG_FETCH_OP(add, _acquire)
ATOMIC_LONG_FETCH_OP(add, _release)
ATOMIC_LONG_FETCH_OP(sub, )
ATOMIC_LONG_FETCH_OP(sub, _relaxed)
ATOMIC_LONG_FETCH_OP(sub, _acquire)
ATOMIC_LONG_FETCH_OP(sub, _release)
ATOMIC_LONG_FETCH_OP(and, )
ATOMIC_LONG_FETCH_OP(and, _relaxed)
ATOMIC_LONG_FETCH_OP(and, _acquire)
ATOMIC_LONG_FETCH_OP(and, _release)
ATOMIC_LONG_FETCH_OP(andnot, )
ATOMIC_LONG_FETCH_OP(andnot, _relaxed)
ATOMIC_LONG_FETCH_OP(andnot, _acquire)
ATOMIC_LONG_FETCH_OP(andnot, _release)
ATOMIC_LONG_FETCH_OP(or, )
ATOMIC_LONG_FETCH_OP(or, _relaxed)
ATOMIC_LONG_FETCH_OP(or, _acquire)
ATOMIC_LONG_FETCH_OP(or, _release)
ATOMIC_LONG_FETCH_OP(xor, )
ATOMIC_LONG_FETCH_OP(xor, _relaxed)
ATOMIC_LONG_FETCH_OP(xor, _acquire)
ATOMIC_LONG_FETCH_OP(xor, _release)

#undef ATOMIC_LONG_FETCH_OP

#define ATOMIC_LONG_FETCH_INC_DEC_OP(op, mo)					\
static inline long							\
atomic_long_fetch_##op##mo(atomic_long_t *l)				\
{									\
	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;		\
									\
	return (long)ATOMIC_LONG_PFX(_fetch_##op##mo)(v);		\
}

ATOMIC_LONG_FETCH_INC_DEC_OP(inc,)
ATOMIC_LONG_FETCH_INC_DEC_OP(inc, _relaxed)
ATOMIC_LONG_FETCH_INC_DEC_OP(inc, _acquire)
ATOMIC_LONG_FETCH_INC_DEC_OP(inc, _release)
ATOMIC_LONG_FETCH_INC_DEC_OP(dec,)
ATOMIC_LONG_FETCH_INC_DEC_OP(dec, _relaxed)
ATOMIC_LONG_FETCH_INC_DEC_OP(dec, _acquire)
ATOMIC_LONG_FETCH_INC_DEC_OP(dec, _release)

#undef ATOMIC_LONG_FETCH_INC_DEC_OP

#define ATOMIC_LONG_OP(op, suffix)					\
static __always_inline void						\
atomic_long_##op##suffix(long i, atomic_long##suffix##_t *l)		\
{									\
	ATOMIC_LONG_PFX(suffix##_t) *v = (ATOMIC_LONG_PFX(suffix##_t) *)l;\
									\
	ATOMIC_LONG_PFX(_##op##suffix)(i, v);				\
}

ATOMIC_LONG_OP(add,)
ATOMIC_LONG_OP(sub,)
ATOMIC_LONG_OP(and,)
ATOMIC_LONG_OP(or,)
ATOMIC_LONG_OP(xor,)
ATOMIC_LONG_OP(andnot,)

#ifdef CONFIG_HARDENED_ATOMIC
ATOMIC_LONG_OP(add,_wrap)
ATOMIC_LONG_OP(sub,_wrap)
#else /* CONFIG_HARDENED_ATOMIC */
#define atomic_long_add_wrap(i,v) atomic_long_add((i),(v))
#define atomic_long_sub_wrap(i,v) atomic_long_sub((i),(v))
#endif /* CONFIG_HARDENED_ATOMIC */

#undef ATOMIC_LONG_OP

static inline int atomic_long_sub_and_test(long i, atomic_long_t *l)
{
	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;

	return ATOMIC_LONG_PFX(_sub_and_test)(i, v);
}

static inline int atomic_long_dec_and_test(atomic_long_t *l)
{
	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;

	return ATOMIC_LONG_PFX(_dec_and_test)(v);
}

static inline int atomic_long_inc_and_test(atomic_long_t *l)
{
	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;

	return ATOMIC_LONG_PFX(_inc_and_test)(v);
}

static inline int atomic_long_add_negative(long i, atomic_long_t *l)
{
	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;

	return ATOMIC_LONG_PFX(_add_negative)(i, v);
}

#ifdef CONFIG_HARDENED_ATOMIC
static inline int atomic_long_sub_and_test_wrap(long i, atomic_long_wrap_t *l)
{
	ATOMIC_LONG_PFX(_wrap_t) *v = (ATOMIC_LONG_PFX(_wrap_t) *)l;

	return ATOMIC_LONG_PFX(_sub_and_test_wrap)(i, v);
}

static inline int atomic_long_dec_and_test_wrap(atomic_long_wrap_t *l)
{
	ATOMIC_LONG_PFX(_wrap_t) *v = (ATOMIC_LONG_PFX(_wrap_t) *)l;

	return ATOMIC_LONG_PFX(_dec_and_test_wrap)(v);
}

static inline int atomic_long_inc_and_test_wrap(atomic_long_wrap_t *l)
{
	ATOMIC_LONG_PFX(_wrap_t) *v = (ATOMIC_LONG_PFX(_wrap_t) *)l;

	return ATOMIC_LONG_PFX(_inc_and_test_wrap)(v);
}

static inline int atomic_long_add_negative_wrap(long i, atomic_long_wrap_t *l)
{
	ATOMIC_LONG_PFX(_wrap_t) *v = (ATOMIC_LONG_PFX(_wrap_t) *)l;

	return ATOMIC_LONG_PFX(_add_negative_wrap)(i, v);
}
#else /* CONFIG_HARDENED_ATOMIC */
#define atomic_long_sub_and_test_wrap(i, v) atomic_long_sub_and_test((i), (v))
#define atomic_long_dec_and_test_wrap(i, v) atomic_long_dec_and_test((i), (v))
#define atomic_long_inc_and_test_wrap(i, v) atomic_long_inc_and_test((i), (v))
#define atomic_long_add_negative_wrap(i, v) atomic_long_add_negative((i), (v))
#endif /* CONFIG_HARDENED_ATOMIC */

#define ATOMIC_LONG_INC_DEC_OP(op, mo, suffix)				\
static inline long							\
atomic_long_##op##_return##mo##suffix(atomic_long##suffix##_t *l)	\
{									\
	ATOMIC_LONG_PFX(suffix##_t) *v = (ATOMIC_LONG_PFX(suffix##_t) *)l;\
									\
	return (long)ATOMIC_LONG_PFX(_##op##_return##mo##suffix)(v);	\
}
ATOMIC_LONG_INC_DEC_OP(inc,,)
ATOMIC_LONG_INC_DEC_OP(inc, _relaxed,)
ATOMIC_LONG_INC_DEC_OP(inc, _acquire,)
ATOMIC_LONG_INC_DEC_OP(inc, _release,)
ATOMIC_LONG_INC_DEC_OP(dec,,)
ATOMIC_LONG_INC_DEC_OP(dec, _relaxed,)
ATOMIC_LONG_INC_DEC_OP(dec, _acquire,)
ATOMIC_LONG_INC_DEC_OP(dec, _release,)

#ifdef CONFIG_HARDENED_ATOMIC
ATOMIC_LONG_INC_DEC_OP(inc,,_wrap)
ATOMIC_LONG_INC_DEC_OP(dec,,_wrap)
#else /* CONFIG_HARDENED_ATOMIC */
#define atomic_long_inc_return_wrap(v) atomic_long_inc_return((v))
#define atomic_long_dec_return_wrap(v) atomic_long_dec_return((v))
#endif /*  CONFIG_HARDENED_ATOMIC */

#undef ATOMIC_LONG_INC_DEC_OP

static inline long atomic_long_add_unless(atomic_long_t *l, long a, long u)
{
	ATOMIC_LONG_PFX(_t) *v = (ATOMIC_LONG_PFX(_t) *)l;

	return (long)ATOMIC_LONG_PFX(_add_unless)(v, a, u);
}

#ifdef CONFIG_HARDENED_ATOMIC
static inline long atomic_long_add_unless_wrap(atomic_long_wrap_t *l, long a, long u)
{
	ATOMIC_LONG_PFX(_wrap_t) *v = (ATOMIC_LONG_PFX(_wrap_t) *)l;

	return (long)ATOMIC_LONG_PFX(_add_unless_wrap)(v, a, u);
}
#else /* CONFIG_HARDENED_ATOMIC */
#define atomic_long_add_unless_wrap(v, i, j) atomic_long_add_unless((v), (i), (j))
#endif /* CONFIG_HARDENED_ATOMIC */

#define atomic_long_inc_not_zero(l) \
	ATOMIC_LONG_PFX(_inc_not_zero)((ATOMIC_LONG_PFX(_t) *)(l))

#ifndef CONFIG_HARDENED_ATOMIC
#ifndef atomic_read_wrap
#define atomic_read_wrap(v) atomic_read(v)
#endif /* atomic_read_wrap */
#ifndef atomic_set_wrap
#define atomic_set_wrap(v, i) atomic_set((v), (i))
#endif /* atomic_set_wrap */
#define atomic_add_wrap(i, v) atomic_add((i), (v))
#define atomic_sub_wrap(i, v) atomic_sub((i), (v))
#define atomic_inc_wrap(v) atomic_inc(v)
#define atomic_dec_wrap(v) atomic_dec(v)
#ifndef atomic_add_return_wrap
#define atomic_add_return_wrap(i, v) atomic_add_return((i), (v))
#endif /* atomic_add_return_wrap */
#ifndef atomic_sub_return_wrap
#define atomic_sub_return_wrap(i, v) atomic_sub_return((i), (v))
#endif /* atomic_sub_return_wrap */
#define atoimc_dec_return_wrap(v) atomic_dec_return(v)
#ifndef atomic_inc_return_wrap
#define atomic_inc_return_wrap(v) atomic_inc_return(v)
#endif /* atomic_inc_return */
#ifndef atomic_dec_and_test_wrap
#define atomic_dec_and_test_wrap(v) atomic_dec_and_test(v)
#endif /* atomic_dec_and_test_wrap */
#ifndef atomic_inc_and_test_wrap
#define atomic_inc_and_test_wrap(v) atomic_inc_and_test(v)
#endif /* atomic_inc_and_test_wrap */
#define atomic_sub_and_test_wrap(i, v) atomic_sub_and_test((v), (i))
#ifndef atomic_xchg_wrap
#define atomic_xchg_wrap(v, i) atomic_xchg((v), (i))
#endif /*  atomic_xchg_wrap(v, i) */
#ifndef atomic_cmpxchg_wrap
#define atomic_cmpxchg_wrap(v, o, n) atomic_cmpxchg((v), (o), (n))
#endif /* atomic_cmpxchg_wrap */
#define atomic_add_negative_wrap(i, v) atomic_add_negative((i), (v))
#define atomic_add_unless_wrap(v, i, j) atomic_add_unless((v), (i), (j))
#endif /* CONFIG_HARDENED_ATOMIC */

#endif  /*  _ASM_GENERIC_ATOMIC_LONG_H  */
