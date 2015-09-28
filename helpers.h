//
//  helpers.h
//  print helper tools
//
//  Created by Sebastian Keller on 24.09.15.
//  Copyright © 2015 sebikeller. All rights reserved.
//
#ifndef DEBUG_HELPERS_DEFINED
#define DEBUG_HELPERS_DEFINED 1

#include <stdio.h>
#include <stdint.h>

#define PRINT(argfmt, arg) \
do { \
	printf(#arg": %"argfmt"\n", arg);\
} while(0);

#define PRINT_TITLE(title_str) \
do { \
	puts("");\
	puts("================================================================================");\
	printf("%s\n", title_str);\
	puts("================================================================================");\
	puts("");\
} while(0);

#ifdef DEBUG
#	define DEBUG_PRINT(argfmt, arg) \
	do { \
		printf(#arg": %"argfmt"\n", arg);\
	} while(0);
#else
#	define DEBUG_PRINT(argfmt, arg)
#endif

#ifdef DEBUG
#	define DEBUG_PRINT_HEX(lenfmt, arg) \
	do { \
		printf(#arg": 0x%"lenfmt"x\n", arg);\
	} while(0);
#else
#	define DEBUG_PRINT_HEX(argfmt, arg)
#endif
	
#ifdef DEBUG
#	define DEBUG_PRINT_PTR(arg) \
	do { \
		printf(#arg": %p\n", (arg));\
	} while(0);
#else
#	define DEBUG_PRINT_PTR(arg)
#endif
	
#ifdef DEBUG
#	define DEBUG_PRINT_BIN(arg) \
	do { \
		printf(#arg": ");\
	    unsigned char *b = (unsigned char*) &arg;\
	    unsigned char byte;\
	    int i, j;\
	    for (i=sizeof(arg)-1;i>=0;i--) {\
	        for (j=7;j>=0;j--) {\
	            byte = b[i] & (1<<j);\
	            byte >>= j;\
	            printf("%u", byte);\
	        }\
	    }\
	    puts("");\
	} while(0);
#else
#	define DEBUG_PRINT_BIN(arg)
#endif

#ifdef DEBUG
#	define __PRINT_ARRAY_ELEM(arg, idx, array_t, array_f) \
	do { \
		printf("%11llu (%p): %"array_f"\n", idx, (arg), (array_t)(*(arg)));\
	} while(0);
#	define PRINT_ARRAY(array_ref, array_length, array_type, array_fmt) \
	do { \
		array_type const *array_p = (array_type const *)(array_ref);\
		printf(#array_ref" (%p): [\n", array_p);\
		for(uint64_t i = 0; i < array_length; i++){\
			__PRINT_ARRAY_ELEM(array_p+i, i, array_type, array_fmt);\
		}\
		printf("]\n\n");\
	} while(0);
#else
#	define __PRINT_ARRAY_ELEM(arg, array_t, array_f) \
	do { \
		printf("%"array_f, (array_t)(*(arg)));\
	} while(0);
#	define PRINT_ARRAY(array_ref, array_length, array_type, array_fmt) \
	do { \
		array_type const *array_p = (array_type const *)(array_ref);\
		uint64_t al1 = array_length-1;\
		printf(#array_ref": [");\
		for(uint64_t i = 0; i < array_length; i++){\
			__PRINT_ARRAY_ELEM(array_p+i, array_type, array_fmt);\
			if(i != al1){printf(", ");}\
		}\
		printf("]\n");\
	} while(0);
#endif

#endif //DEBUG_HELPERS_DEFINED
