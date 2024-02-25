/* 
 * File:   mcal_std_types.h
 * Author: Honey
 *
 * Created on August 26, 2023, 4:01 AM
 */

#ifndef MCAL_STD_TYPES_H
#define	MCAL_STD_TYPES_H

/*Section include*/

#include "compilier.h"
#include "std_libraries.h"

/*Section Macro declaration*/
#define STD_LOW                         0x0
#define STD_HIGH                        0x1

#define STD_ACTIVE                      0x1
#define STD_IDLE                        0x0

#define STD_ON                          0x1
#define STD_OFF                         0x1

#define E_OK                            (Std_ReturnType)0x0
#define E_NOT_OK                        (Std_ReturnType)0x1

#define ZERO_INT                       0x0

/*Section Macro function declaration*/

/*Section Data type declaration*/
                                        
typedef unsigned char                   uint8;
typedef unsigned short                  uint16;
typedef unsigned int                    uint32;
typedef unsigned long long              uint64;

typedef signed char                     sint8;
typedef signed short                    sint16;
typedef signed int                      sint32;
typedef signed long long                sint64;                           

typedef uint8                           Std_ReturnType;

/*Section fun prototype*/


#endif	/* MCAL_STD_TYPES_H */

