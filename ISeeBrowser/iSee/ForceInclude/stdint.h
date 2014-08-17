#ifndef _STDINT_H
#define _STDINT_H

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#ifndef CASSERT
#define CASSERT(exp, name) typedef int dummy##name [(exp) ? 1 : -1];
#endif

CASSERT(sizeof(int8_t) == 1, int8_t_is_one_byte)
CASSERT(sizeof(uint8_t) == 1, uint8_t_is_one_byte)
CASSERT(sizeof(int16_t) == 2, int16_t_is_two_bytes)
CASSERT(sizeof(uint16_t) == 2, uint16_t_is_two_bytes)
CASSERT(sizeof(int32_t) == 4, int32_t_is_four_bytes)
CASSERT(sizeof(uint32_t) == 4, uint32_t_is_four_bytes)
CASSERT(sizeof(int64_t) == 8, int64_t_is_four_bytes)
CASSERT(sizeof(uint64_t) == 8, uint64_t_is_four_bytes)

#endif
