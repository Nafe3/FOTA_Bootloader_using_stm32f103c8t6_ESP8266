
#ifndef STD_TYPES_H_
#define STD_TYPES_H_

typedef  unsigned char           u8 ;
typedef  unsigned short int      u16;
typedef  unsigned long int       u32;

typedef  signed char             s8 ;
typedef  signed short int        s16;
typedef  signed long int         s32;

typedef  float                   f32;
typedef  double                  f64;
typedef  long double             f96;


#define  ErrorStatus            u8
#define  STD_TYPES_ERROR_OK     (ErrorStatus)1U
#define  STD_TYPES_ERROR_NOK    (ErrorStatus)2U

#define STATUS_OK			(u8)0
#define STATUS_NOK			(u8)1


#define NULL ((void*)0)


#endif
