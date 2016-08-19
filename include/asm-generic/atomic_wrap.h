#ifndef _LINUX_ATOMIC_WRAP_H
#define _LINUX_ATOMIC_WRAP_H

#ifndef CONFIG_HARDENED_ATOMIC

#include <asm/atomic.h>

#ifndef atomic_read_wrap
#define atomic_read_wrap(v)	READ_ONCE((v)->counter)
#endif
#ifndef atomic_set_wrap
#define atomic_set_wrap(v, i)	WRITE_ONCE(((v)->counter), (i))
#endif

#ifndef atomic_inc_return_wrap
static inline int atomic_inc_return_wrap(atomic_wrap_t *v)
{
	return atomic_inc_return((atomic_t *) v);
}
#define atomic_inc_return_wrap atomic_inc_return_wrap
#endif

#ifndef atomic_dec_return_wrap
static inline int atomic_dec_return_wrap(atomic_wrap_t *v)
{
	return atomic_dec_return((atomic_t *) v);
}
#define atomic_dec_return_wrap atomic_dec_return_wrap
#endif

#ifndef atomic_add_return_wrap
static inline int atomic_add_return_wrap(int i, atomic_wrap_t *v)
{
	return atomic_add_return(i, (atomic_t *) v);
}
#define atomic_add_return_wrap atomic_add_return_wrap
#endif

#ifndef atomic_sub_return_wrap
static inline int atomic_sub_return_wrap(int i, atomic_wrap_t *v)
{
	return atomic_sub_return(i, (atomic_t *) v);
}
#define atomic_sub_return_wrap atomic_sub_return_wrap
#endif

#ifndef atomic_xchg_wrap
#define atomic_xchg_wrap(ptr, v)	(xchg(&(ptr)->counter, (v)))
#endif
#ifndef atomic_cmpxchg_wrap
#define atomic_cmpxchg_wrap(v, o, n)	atomic_cmpxchg(((atomic_t *) v), (o), (n))
#endif

#ifndef atomic_add_negative_wrap
static inline int atomic_add_negative_wrap(int i, atomic_wrap_t *v)
{
	return atomic_add_return_wrap(i, v) < 0;
}
#define atomic_add_negative_wrap atomic_add_negative_wrap
#endif

#ifndef atomic_add_wrap
static inline void atomic_add_wrap(int i, atomic_wrap_t *v)
{
	atomic_add_return_wrap(i, v);
}
#define atomic_add_wrap atomic_add_wrap
#endif

#ifndef atomic_sub_wrap
static inline void atomic_sub_wrap(int i, atomic_wrap_t *v)
{
	atomic_sub_return_wrap(i, v);
}
#define atomic_sub_wrap atomic_sub_wrap
#endif

#ifndef atomic_inc_wrap
static inline void atomic_inc_wrap(atomic_wrap_t *v)
{
	atomic_add_return_wrap(1, v);
}
#define atomic_inc_wrap atomic_inc_wrap
#endif

#ifndef atomic_dec_wrap
static inline void atomic_dec_wrap(atomic_wrap_t *v)
{
	atomic_sub_return_wrap(1, v);
}
#define atomic_dec_wrap atomic_dec_wrap
#endif

#ifndef atomic_sub_and_test_wrap
#define atomic_sub_and_test_wrap(i, v)	(atomic_sub_return_wrap((i), (v)) == 0)
#endif
#ifndef atomic_dec_and_test_wrap
#define atomic_dec_and_test_wrap(v)	(atomic_dec_return_wrap(v) == 0)
#endif
#ifndef atomic_inc_and_test_wrap
#define atomic_inc_and_test_wrap(v)	(atomic_inc_return_wrap(v) == 0)
#endif

#ifndef atomic_add_unless_wrap
static inline int atomic_add_unless_wrap(atomic_wrap_t *v, int a, int u)
{
	return __atomic_add_unless((atomic_t *) v, a, u);
}
#define atomic_add_unless_wrap atomic_add_unless_wrap
#endif

#endif /* CONFIG_HARDENED_ATOMIC */

#endif /* _LINUX_ATOMIC_WRAP_H */
