#ifndef BIT_MATH_H_
#define BIT_MATH_H_


/*==========================================================
 *                    Bit Operations
 *==========================================================*/

#define SET_BIT(REG, BIT)       ((REG) |=  (1UL << (BIT)))

#define CLR_BIT(REG, BIT)       ((REG) &= ~(1UL << (BIT)))

#define TOG_BIT(REG, BIT)       ((REG) ^=  (1UL << (BIT)))

#define GET_BIT(REG, BIT)       (((REG) >> (BIT)) & 1UL)


#endif /* BIT_MATH_H_ */
