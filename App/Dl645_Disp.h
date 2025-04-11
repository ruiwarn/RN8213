#ifndef __DL645_DISP_H
#define __DL645_DISP_H

#ifdef   DL645_DISP_GLOBALS
	#define  DL645DISP_EXT 	
#else
	#define  DL645DISP_EXT  extern 
#endif

//人机对话数据结构
typedef struct {
	u8  Mode;/*0自动循显模式；1按键显示；2关闭显示*/
	u8  AutoSerialNo;//0~99 自动连续号码
	u8  KeySeriaNo;//0~99 按键连续号码
	u8  Stamp; //时戳。
	u8	CardDispStamp;
	u8  EventErr1;
	u8  EventErr2;
	u8  EventKeyErr;
	u8	BkLightFlag;	//背光点亮标志
	u8  BkLightStamp;  //背光时戳。
	u8  RemoteStamp;   //遥控时戳。
  	u8  Item[5];       //项目
  	u8  lItem[5];      //1项目
	u8  LcdCount;		//
	u8  DigitPosition; //操作位置
	u8 	DisplyErrFlag;
	u8	AllOnDispTime;	// 上电全显时间	
} sDl645B_Session_TYPE;



#define DISPLAYAUTO 0
#define DISPLAYKEY  1
#define DISPLARYEMOTE  2  //遥控器代码
#define InvalidItem  0xff


extern void fnDl645Disp_Init(void);
extern void fnDl645Disp_SessionExec(void);

extern void fnDl645Disp_Data(u8 *Buf,u8 DataType,u8 Length);
extern void fnDl645Disp_RefreshStatus(void);
extern void fnDl645Disp_RefreshDispData(void);
extern void fnDl645Disp_Variable(void);
extern void fnDl645Disp_GeneralPara(void);
extern void fnDl645Disp_OtherMessage(const u16 *Buf);
extern void fnDl645Disp_RateMonthTimes(u8 Type,u8 Data);
#endif
