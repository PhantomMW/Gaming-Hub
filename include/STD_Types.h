#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#include <stdint.h>


/*==========================================================
 *                   Standard Integer Types
 *==========================================================*/

typedef uint8_t   u8;
typedef int8_t    s8;

typedef uint16_t  u16;
typedef int16_t   s16;

typedef uint32_t  u32;
typedef int32_t   s32;

typedef uint64_t  u64;
typedef int64_t   s64;


/*==========================================================
 *                    Standard Values
 *==========================================================*/

#ifndef NULL_PTR
#define NULL_PTR    ((void *)0)
#endif

#define STD_LOW     0U
#define STD_HIGH    1U


#endif /* STD_TYPES_H_ */
