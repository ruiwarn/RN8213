
#ifndef  _Drive_ALLINT_H
#define  _Drive_ALLINT_H


#ifdef   _Drive_ALLINT
#define  Drive_ALLINT_EXT
#else
#define  Drive_ALLINT_EXT  extern 
#endif
//--------------------------------------------
/*
*******************************************************************************
**  Typee define
*******************************************************************************
*/

/*
*******************************************************************************
**  Extern variable define
*******************************************************************************
*/
typedef struct {
	u8	Front500 : 1;				//0.5s前台处理标志
	u8	Event500 : 1;				//0.5秒事件处理标志
	u8	Disp500 : 1;				//0.5秒显示刷新处理标志	
	u8	Timadd500 : 1;				//0.5秒时间更新标志	
	//需要用到的其他0.5S标识在下面定义，最多定义32个
	
} sDl645GlobalsFlagBit_TypeDef;		
typedef union {
	u32		Flag;
	sDl645GlobalsFlagBit_TypeDef	FlagBit;		
} sDl645GlobalsFlag_TypeDef;
#define OS_TICKS_PER_SEC         1000    // Set the number of ticks in one second                          


#endif	//_Drive_ALLINT_H
//--------------------------------------------

