
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
	u8	Front500 : 1;				//0.5sǰ̨�����־
	u8	Event500 : 1;				//0.5���¼������־
	u8	Disp500 : 1;				//0.5����ʾˢ�´����־	
	u8	Timadd500 : 1;				//0.5��ʱ����±�־	
	//��Ҫ�õ�������0.5S��ʶ�����涨�壬��ඨ��32��
	
} sDl645GlobalsFlagBit_TypeDef;		
typedef union {
	u32		Flag;
	sDl645GlobalsFlagBit_TypeDef	FlagBit;		
} sDl645GlobalsFlag_TypeDef;
#define OS_TICKS_PER_SEC         1000    // Set the number of ticks in one second                          


#endif	//_Drive_ALLINT_H
//--------------------------------------------

