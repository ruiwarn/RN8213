
#ifndef _DL645CLOCK_H
#define _DL645CLOCK_H

//
//*******************************************************************************
//**  Function define
//*******************************************************************************
//
extern void fnDl645MainClock_Init( void );
extern void fnDl645SubClock_Init( void );
extern	u32 fnDl645MainClock_Get( void );
extern void fnDl645RCClock_Init( void );
#endif
