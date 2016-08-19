#ifndef _LINUX_LOCAL_H
#define _LINUX_LOCAL_H

#include <asm/local.h>

#ifdef CONFIG_HARDENED_ATOMIC
typedef struct {
	atomic_long_wrap_t a;
} local_wrap_t;
#else /* CONFIG_HARDENED_ATOMIC */
typedef local_t local_wrap_t;
#endif /* CONFIG_HARDENED_ATOMIC */

/*
 * A signed long type for operations which are atomic for a single CPU. Usually
 * used in combination with per-cpu variables. This is a safeguard header that
 * ensures that local_wrap_* is available regardless of whether platform support
 * for HARDENED_ATOMIC is available.
 */

#ifndef CONFIG_HARDENED_ATOMIC
#define local_read_wrap(l)		local_read(l)
#define local_set_wrap(l,i)		local_set((l),(i))
#define local_inc_wrap(l)		local_inc(l)
#define local_inc_return_wrap(l)	local_return(l)
#define local_inc_and_test_wrap(l)	local_inc_and_test(l)
#define local_dec_wrap(l)		local_dec(l)
#define local_dec_return_wrap(l)	local_dec_return(l)
#define local_dec_and_test_wrap(l)	local_dec_and_test(l)
#define local_add_wrap(i,l)		local_add((i),(l))
#define local_add_return_wrap(i, l)	local_add_return((i), (l))
#define local_sub_wrap(i,l)		local_sub((i),(l))
#define local_sub_return_wrap(i, l)	local_sub_return((i), (l))
#define local_sub_and_test_wrap(i, l)	local_sub_and_test((i), (l))
#define local_cmpxchg_wrap(l, o, n)	local_cmpxchg((l), (o), (n))
#define local_add_unless_wrap(l, _a, u) local_add_unless((l), (_a), (u))
#define local_add_negative_wrap(i, l)	local_add_negative((i), (l))
#else /* CONFIG_HARDENED_ATOMIC */
#ifndef local_read_wrap
#define local_read_wrap(l)	atomic_long_read_wrap(&(l)->a)
#endif
#ifndef local_set_wrap
#define local_set_wrap(l,i)	atomic_long_set_wrap((&(l)->a),(i))
#endif
#ifndef local_inc_wrap
#define local_inc_wrap(l)	atomic_long_inc_wrap(&(l)->a)
#endif
#ifndef local_inc_return_wrap 
#define local_inc_return_wrap(l)	atomic_long_return_wrap(&(l)->a)
#endif
#ifndef local_inc_and_test_wrap
#define local_inc_and_test_wrap(l)	atomic_long_inc_and_test_wrap(&(l)->a)
#endif
#ifndef local_dec_wrap
#define local_dec_wrap(l)	atomic_long_dec_wrap(&(l)->a)
#endif
#ifndef local_dec_return_wrap
#define local_dec_return_wrap(l)	atomic_long_dec_return_wrap(&(l)->a)
#endif
#ifndef local_dec_and_test_wrap
#define local_dec_and_test_wrap(l)	atomic_long_dec_and_test_wrap(&(l)->a)
#endif
#ifndef local_add_wrap
#define local_add_wrap(i,l)	atomic_long_add_wrap((i),(&(l)->a))
#endif
#ifndef local_add_return_wrap
#define local_add_return_wrap(i, l) atomic_long_add_return_wrap((i), (&(l)->a))
#endif
#ifndef local_sub_wrap
#define local_sub_wrap(i,l)	atomic_long_sub_wrap((i),(&(l)->a))
#endif
#ifndef local_sub_return_wrap
#define local_sub_return_wrap(i, l)	atomic_long_sub_return_wrap((i), (&(l)->a))
#endif
#ifndef local_sub_and_test_wrap
#define local_sub_and_test_wrap(i, l) atomic_long_sub_and_test_wrap((i), (&(l)->a))
#endif
#ifndef local_cmpxchg_wrap
#define local_cmpxchg_wrap(l, o, n) atomic_long_cmpxchg_wrap((&(l)->a), (o), (n))
#endif
#ifndef local_add_unless_wrap
#define local_add_unless_wrap(l, _a, u) atomic_long_add_unless_wrap((&(l)->a), (_a), (u))
#endif
#ifndef local_add_negative_wrap
#define local_add_negative_wrap(i, l)	atomic_long_add_negative_wrap((i), (&(l)->a))
#endif
#endif /* CONFIG_HARDENED_ATOMIC */

#endif /* _LINUX_LOCAL_H */
