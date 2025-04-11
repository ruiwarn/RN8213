
#ifndef  _DATA_TYPES_H
#define  _DATA_TYPES_H

#ifndef NULL
  	#define NULL          0
#endif 

typedef enum {ERROR = 0 , SUCCESS = !ERROR} ErrorStatus;
typedef enum {false = 0 , true = !false} bool;
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;


typedef unsigned char  		u8;
typedef signed char  		s8;
typedef unsigned short  	u16;
typedef signed short  		s16;
typedef unsigned long int  	u32;
typedef signed long int  	s32;


typedef volatile unsigned char  	vu8;
typedef volatile signed char  		vs8;
typedef volatile unsigned short  	vu16;
typedef volatile signed short  		vs16;
typedef volatile unsigned long int  vu32;
typedef volatile signed long int  	vs32;
typedef	unsigned char	BOOL;





#endif	//_DATA_TYPES_H


























