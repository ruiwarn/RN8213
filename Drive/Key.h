
#ifndef  _KEY_H
#define  _KEY_H

#ifdef   _KEY_GLOBALS
	#define  KEY_EXT
#else
	#define  KEY_EXT  extern 
#endif



typedef enum
{
	KEY_VAL_PRG		= (1 << 0),	//编程按键
	KEY_VAL_TURN	= (1 << 1),	//键显按键
	KEY_VAL_SHELL	= (1 << 2),	//开表壳按键
	KEY_Relay_Ctrl= (1 << 3), //继电器保电合闸控制
	KEY_Open_Cover= (1 << 4),//开表盖，合表盖
	KEY_VAL_CARD	= (1 << 5),
}eKeyVal_TypeDef;



typedef struct {
	u8 		KeyTurn_Scan;      //键显按键采样扫描结果。
	u8 		KeyProg_Scan;     //编程按键采样扫描结果。	
	u8 		KeyShell_Scan;    //开表壳采样扫描结果。
	u8 		Press;		      //按键按下识别位,包括键显按键的短按，长按，编程键的按下，开盖操作。
	u8 		Release;		  //按键释放识别位,包括键显按键的释放,编程键的释放，合盖操作?
	u16     KeyTurn_Time;     //键显按钮的按动时间累计计数。以方便进行保电合闸操作。
	    			
} sKey_TypeDef;				   //KEY_TYPE;

KEY_EXT sKey_TypeDef Key;

//==============================================
extern void fnKey_Init(void);
extern void fnKey_Scan(void);
extern void fnKey_Exec(void);


#endif	//_KEY_H




