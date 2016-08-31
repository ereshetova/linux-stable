#ifndef _ASM_X86_RMWcc
#define _ASM_X86_RMWcc

#if !defined(__GCC_ASM_FLAG_OUTPUTS__) && defined(CC_HAVE_ASM_GOTO)

/* Use asm goto */

#ifdef CONFIG_HARDENED_ATOMIC
#define __GEN_RMWcc(fullop, fullantiop, var, cc, ...)			\
do {									\
	asm_volatile_goto (fullop					\
			";jno 0f\n"					\
			fullantiop					\
			";int $4\n0:\n"					\
			_ASM_EXTABLE(0b, 0b)				\
			 ";j" #cc " %l[cc_label]"			\
			: : "m" (var), ## __VA_ARGS__ 			\
			: "memory" : cc_label);				\
	return 0;							\
cc_label:								\
	return 1;							\
} while (0)
#else
#define __GEN_RMWcc(fullop, fullantiop, var, cc, ...)			\
do {									\
	asm_volatile_goto (fullop ";j" #cc " %l[cc_label]"		\
			: : "m" (var), ## __VA_ARGS__ 			\
			: "memory" : cc_label);				\
	return 0;							\
cc_label:								\
	return 1;							\
} while (0)
#endif

#define __GEN_RMWcc_wrap(fullop, var, cc, ...)do {									\
	asm_volatile_goto (fullop "; j" #cc " %l[cc_label]"		\
			: : "m" (var), ## __VA_ARGS__ 			\
			: "memory" : cc_label);				\
	return 0;							\
cc_label:								\
	return 1;							\
} while (0)

#define GEN_UNARY_RMWcc(op, antiop, var, arg0, cc) 			\
	__GEN_RMWcc(op " " arg0, antiop " " arg0, var, cc)
#define GEN_UNARY_RMWcc_wrap(op, var, arg0, cc) 			\
	__GEN_RMWcc_wrap(op " " arg0, var, cc)
#define GEN_BINARY_RMWcc(op, antiop, var, vcon, val, arg0, cc)		\
	__GEN_RMWcc(op " %1, " arg0, antiop " %1, " arg0, var, cc, vcon (val))
#define GEN_BINARY_RMWcc_wrap(op, var, vcon, val, arg0, cc)	\
	__GEN_RMWcc_wrap(op " %1, " arg0, var, cc, vcon (val))
#else /* defined(__GCC_ASM_FLAG_OUTPUTS__) || !defined(CC_HAVE_ASM_GOTO) */

/* Use flags output or a set instruction */

#ifdef CONFIG_HARDENED_ATOMIC
#define __GEN_RMWcc(fullop, fullantiop, var, cc, ...)			\
do {									\
	char c;								\
	asm volatile (fullop 						\
			";jno 0f\n"					\
			fullantiop					\
			";int $4\n0:\n"					\
			_ASM_EXTABLE(0b, 0b)				\
			";" CC_SET(cc)				\
			: "+m" (var), CC_OUT(cc) (c)			\
			: __VA_ARGS__ : "memory");			\
	return c != 0;							\
} while (0)
#else
#define __GEN_RMWcc(fullop, fullantiop, var, cc, ...)			\
do {									\
	char c;								\
	asm volatile (fullop ";" CC_SET(cc)				\
			: "+m" (var), CC_OUT(cc) (c)			\
			: __VA_ARGS__ : "memory");			\
	return c != 0;							\
} while (0)
#endif

#define __GEN_RMWcc_wrap(fullop, var, cc, ...)do {									\
	bool c;								\
	asm volatile (fullop ";" CC_SET(cc)				\
			: "+m" (var), CC_OUT(cc) (c)			\
			: __VA_ARGS__ : "memory");			\
	return c;							\
} while (0)

#define GEN_UNARY_RMWcc(op, antiop, var, arg0, cc)			\
	__GEN_RMWcc(op " " arg0, antiop " " arg0, var, cc)
#define GEN_UNARY_RMWcc_wrap(op, var, arg0, cc)			\
	__GEN_RMWcc_wrap(op " " arg0, var, cc)
#define GEN_BINARY_RMWcc(op, antiop, var, vcon, val, arg0, cc)		\
	__GEN_RMWcc(op " %2, " arg0, antiop " %2, " arg0, var, cc, vcon (val))
#define GEN_BINARY_RMWcc_wrap(op, var, vcon, val, arg0, cc)	\
	__GEN_RMWcc_wrap(op " %2, " arg0, var, cc, vcon (val))
#endif /* defined(__GCC_ASM_FLAG_OUTPUTS__) || !defined(CC_HAVE_ASM_GOTO) */

#endif /* _ASM_X86_RMWcc */
