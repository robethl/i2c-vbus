#ifndef	_TYPE_H_
#define _TYPE_H_

#ifndef	TRUE
	#define TRUE 1
#endif

#ifndef	FALSE
	#define FALSE 0
#endif

#ifndef	NULL
	#define NULL 0
#endif

#ifndef	VOID
	#define VOID void
#endif

#ifndef	CHAR
	#define CHAR char
#endif

#ifndef	BOOL
	#define BOOL unsigned int
#endif

#ifndef	SINT
	#define SINT signed int
#endif

#ifndef	UINT
	#define UINT unsigned int
#endif

#ifndef	SI08
	#define SI08 signed char
#endif

#ifndef	UI08
	#define UI08 unsigned char
#endif

#ifdef __COMPILER_WORD_SIZE16__/* the 16 bit compiler */
	#ifndef	SI16
		#define SI16 signed int
	#endif

	#ifndef	SI32
		#define SI32 signed long
	#endif

	#ifndef	UI16
		#define UI16 unsigned int
	#endif

	#ifndef	UI32
		#define UI32 unsigned long
	#endif
#else/* the 32 bit compiler */
	#ifndef	SI16
		#define SI16 signed short
	#endif

	#ifndef	SI32
		#define SI32 signed int
	#endif

	#ifndef	UI16
		#define UI16 unsigned short
	#endif

	#ifndef	UI32
		#define UI32 unsigned int
	#endif
#endif

#if defined(__C51__) || defined(__CX51__)
#define PARAM_PAST_IN_STACK	large reentrant
#else
#define PARAM_PAST_IN_STACK
#endif

#endif
