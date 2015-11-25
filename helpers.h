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
#include <stdarg.h>
#include <string.h>

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
	    unsigned char *_______b = (unsigned char*) &arg;\
	    unsigned char _______byte;\
	    int _______i, _______j;\
	    for (_______i=sizeof(arg)-1;_______i>=0;_______i--) {\
	        for (_______j=7;_______j>=0;_______j--) {\
	            _______byte = _______b[_______i] & (1<<_______j);\
	            _______byte >>= _______j;\
	            printf("%u", _______byte);\
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
		array_type const *_______array_p = (array_type const *)(array_ref);\
		printf(#array_ref" (%p): [\n", _______array_p);\
		for(uint64_t _______i = 0; _______i < array_length; i++){\
			__PRINT_ARRAY_ELEM(_______array_p+_______i, _______i, array_type, array_fmt);\
		}\
		puts("]\n");\
	} while(0);
#else
#	define __PRINT_ARRAY_ELEM(arg, array_t, array_f) \
	do { \
		printf("%"array_f, (array_t)(*(arg)));\
	} while(0);
#	define PRINT_ARRAY(array_ref, array_length, array_type, array_fmt) \
	do { \
		array_type const *_______array_p = (array_type const *)(array_ref);\
		uint64_t _______al1 = array_length-1;\
		printf(#array_ref": [");\
		for(uint64_t _______i = 0; _______i < array_length; i++){\
			__PRINT_ARRAY_ELEM(array_p+_______i, array_type, array_fmt);\
			if(_______i != _______al1){printf(", ");}\
		}\
		puts("]");\
	} while(0);
#endif

typedef struct print_struct_member_info_t {
	char *member_name, *type;
	int type_length, print_type;
} PrintStructMemberInfo;

#define PRINT_STRUCT_INTEGER_TYPE 0
#define PRINT_STRUCT_FLOATING_POINT_TYPE 1
#define PRINT_STRUCT_OTHER_TYPE 2

#ifdef DEBUG
#define __PRINT_STRUCT_NAME(struct_t, struct_n) printf("struct %s %s (%p) {\n", #struct_t, #struct_n, (&struct_n));
#define __CREATE_STRUCT_ENTRY_FORMAT(buf, type, address) snprintf(buf, sizeof(buf), "\t%%s => %%%s (%p),\n", type, (address));
#else
#define __PRINT_STRUCT_NAME(struct_t, struct_n) printf("struct %s %s {\n", #struct_t, #struct_n);
#define __CREATE_STRUCT_ENTRY_FORMAT(buf, type, address) snprintf(buf, sizeof(buf), "\t%%s => %%%s,\n", type);
#endif

#define __PRINT_STRUCT_UNKNOWN_TYPE(member_name, address) printf("\t%s => %p,\n", member_name, (address));

#define PRINT_SIMPLE_STRUCT(struct_n, struct_t, num_fields, print_struct_member_info_array) \
do { \
	char* _______struct_start = (char*)(&struct_n); \
	__PRINT_STRUCT_NAME(struct_t, struct_n); \
	unsigned long _______offset = 0; \
	PrintStructMemberInfo _______t; \
	int _______loop_term = (int)(num_fields); \
	for (int _______i = 0; _______i < _______loop_term; i++) { \
		_______t = (print_struct_member_info_array)[_______i]; \
		char* _______member_name = _______t.member_name; \
		char* _______type = _______t.type; \
		int _______type_len = _______t.type_length; \
		int _______fp_type = _______t.print_type; \
		if(_______fp_type >= PRINT_STRUCT_OTHER_TYPE){ \
			 __PRINT_STRUCT_UNKNOWN_TYPE(member_name, (_______struct_start+_______offset)); \
		} else { \
			char _______buf[50]; \
			__CREATE_STRUCT_ENTRY_FORMAT(_______buf, _______type, (_______struct_start+_______offset)); \
			if(_______fp_type == PRINT_STRUCT_FLOATING_POINT_TYPE){ \
				switch (_______type_len) { \
					case sizeof(float): \
						printf(_______buf, _______member_name, (float)*((float *)(_______struct_start+_______offset))); \
						break; \
					case sizeof(double): \
						printf(_______buf, _______member_name, (double)*((double *)(_______struct_start+_______offset))); \
						break; \
					default: \
						__PRINT_STRUCT_UNKNOWN_TYPE(_______member_name, (_______struct_start+_______offset)); \
						break; \
				} \
			} else { \
				switch (_______type_len) { \
					case sizeof(u_int8_t): \
						printf(_______buf, _______member_name, (u_int8_t)*((u_int8_t *)(_______struct_start+_______offset))); \
						break; \
					case sizeof(u_int16_t): \
						printf(_______buf, _______member_name, (u_int16_t)*((u_int16_t *)(_______struct_start+_______offset))); \
						break; \
					case sizeof(u_int32_t): \
						printf(_______buf, _______member_name, (u_int32_t)*((u_int32_t *)(_______struct_start+_______offset))); \
						break; \
					case sizeof(u_int64_t): \
						printf(_______buf, _______member_name, (u_int64_t)*((u_int64_t *)(_______struct_start+_______offset))); \
						break; \
					default: \
						__PRINT_STRUCT_UNKNOWN_TYPE(_______member_name, (_______struct_start+_______offset)); \
						break; \
				} \
			} \
		} \
		_______offset += _______type_len;  \
	} \
	puts("}"); \
} while(0);

#endif //DEBUG_HELPERS_DEFINED
