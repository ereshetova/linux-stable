#ifndef _LINUX_STATS_H_
#define _LINUX_STATS_H_

#include <linux/atomic.h>
#include <linux/types.h>

typedef struct stats_struct {
	atomic_t counter;
} stats_t;

#define STATS_INIT(n)	{ .counter = ATOMIC_INIT(n), }

static inline unsigned int stats_read(const stats_t *s) {
	return atomic_read(&s->counter);
}

static inline void stats_set(stats_t *s, const unsigned int val) {
	atomic_set(&s->counter, val);
}

static inline void stats_inc(stats_t *s) {
	atomic_inc(&s->counter);	
}

static inline void stats_dec(stats_t *s) {
	atomic_dec(&s->counter);
}

static inline void stats_add(stats_t *s, const unsigned int val) {
	atomic_add(val, &s->counter);
}

static inline void stats_sub(stats_t *s, const unsigned int val) {
	atomic_sub(val, &s->counter);
}

#endif
