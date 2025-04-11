

#ifndef  _LcdDrive_H
#define  _LcdDrive_H

#ifdef   _LcdDrive_GLOBALS
	#define  LcdDrive_EXT
#else
	#define  LcdDrive_EXT  extern 
#endif
//---------------------------------------------------
//---------------------------------------------------

extern void		   fnLcd_Init(void);
extern void 	   fnLcd_ClrScr(void);
extern void 	   fnLcd_SetDigit(u8 nDigit,u8 B);
extern void 	   fnLcd_AllOn(void);
extern void 	   fnLcd_SetSegment(u8 nSegment,u8 B);
extern void fnLcd_RefreshLcdBuf(void);
/**************************LCD 各段定义*********************/
//-------------8COM液晶段定义----------------------------------
#define SEG_dangqian    160  //当前
#define SEG_Qshang  	155	//上
#define SEG_Qyue    	152	//月
#define SEG_zuhe    	145	//组合
#define	SEG_Zheng		188	//正
#define	SEG_Fang		189	//反
#define	SEG_xiang 		190	//向
#define	SEG_S55			191	//实验室状态
#define SEG_Qzong  		144	//总
#define SEG_Qjian  		136 //尖
#define SEG_Qfeng  		137	//峰
#define SEG_Qping  		129	//平
#define SEG_Qgu    		128	//谷
#define SEG_COS  		168   //COS

#define SEG_JieTi   	170  //阶梯
#define SEG_ShengYu   	121	//剩余
#define SEG_Qdian 		153//电
#define SEG_Qliang 		154//量
#define SEG_SheQian 	161//赊
#define SEG_Qian 		162//欠

#define SEG_Jia			146//价
#define SEG_TIME		139//时
#define SEG_JIAN		138//间
#define SEG_Duan		130//段
#define SEG_KW    		7//k
#define SEG_h1			99//h


#define SEG_Yuan		96//7//元
#define SEG_N    		1//N
#define SEG_L    		185//L

#define SEG_A1    		98//A1
#define	SEG_Yong		163	//用
#define	SEG_V1			97	//V
#define SEG_FuHao 		9//-
#define SEG_TunJi 		32//囤积


#define SEG_JIANQ		184 //尖
#define SEG_FENGQ		0 //峰
#define SEG_PINGQ		186 //平
#define SEG_GUQ			2 //谷


#define SEG_Bat1		19 //电池

#define SEG_Phone  		171   //通讯指示标识
#define SEG_ZBPhone  	169   //载波通讯指示标识

#define SEG_Prog    	18	//编程
#define SEG_Lock    	17 //锁
#define SEG_Home    	191

#define SEG_DuKa    	16 //读卡
#define SEG_Zhong   	24	// 中
#define SEG_Success  	25	//成功
#define SEG_Fail    	26 //失败
#define SEG_Please    	27	// 请购电
#define SEG_TouZhi    	33 //透支
#define SEG_Break    	34	//拉闸

#define SEG_Hu    	    147	//户
#define SEG_JinE    	131	//金额
#define SEG_ChangShu    120	//常数
#define SEG_Biao    	123	//表
#define	SEG_Hao			122 //号

#define SEG_1ShiDuan	3	//1时区
#define SEG_2ShiDuan   	11	//2时区

#define SEG_1TaoJieTiDianJie  		8	//第1套阶梯电价
#define SEG_2TaoJieTiDianJie   	10	//第2套阶梯电价

#define SEG_JieTiDianJie1  		176	//第1阶梯电价
#define SEG_JieTiDianJie2   	177	//第2阶梯电价
#define SEG_JieTiDianJie3  		178	//第3阶梯电价
#define SEG_JieTiDianJie4   	179	//第4阶梯电价



#define SEG_FanXiang   	187	//反向

#define SEG_Qseg9a 		112
#define SEG_Qseg9b 		104
#define SEG_Qseg9c 		106
#define SEG_Qseg9d 		115
#define SEG_Qseg9e 		114
#define SEG_Qseg9f 		113
#define SEG_Qseg9g 		105

#define SEG_Qseg1a 		180
#define SEG_Qseg1b 		172
#define SEG_Qseg1c 		174
#define SEG_Qseg1d 		183
#define SEG_Qseg1e 		182
#define SEG_Qseg1f 		181
#define SEG_Qseg1g 		173
#define SEG_QsegDp1     175

#define SEG_Qseg2a 		164 
#define SEG_Qseg2b 		156 
#define SEG_Qseg2c 		158 
#define SEG_Qseg2d 		167	
#define SEG_Qseg2e 		166 
#define SEG_Qseg2f 		165 
#define SEG_Qseg2g 		157 
//#define SEG_QsegDp2     159 

#define SEG_Qseg3a 		148 
#define SEG_Qseg3b 		140 
#define SEG_Qseg3c 		142 
#define SEG_Qseg3d 		151 
#define SEG_Qseg3e 		150 
#define SEG_Qseg3f 		149 
#define SEG_Qseg3g 		141 
#define SEG_QsegDp3     143 

#define SEG_Qseg4a 		132
#define SEG_Qseg4b 		124
#define SEG_Qseg4c 		126
#define SEG_Qseg4d 		135
#define SEG_Qseg4e 		134
#define SEG_Qseg4f 		133
#define SEG_Qseg4g 		125
#define SEG_QsegDp4   	127

#define SEG_Qseg5a 		116
#define SEG_Qseg5b 		108
#define SEG_Qseg5c 		110
#define SEG_Qseg5d 		119
#define SEG_Qseg5e 		118
#define SEG_Qseg5f 		117
#define SEG_Qseg5g 		109
#define SEG_QsegDp5   	111


#define SEG_Qseg6a 		100
#define SEG_Qseg6b 		36 
#define SEG_Qseg6c 		38 
#define SEG_Qseg6d 		103
#define SEG_Qseg6e 		102
#define SEG_Qseg6f 		101
#define SEG_Qseg6g 		37 
#define SEG_QsegDp6     39 


#define SEG_Qseg7a		28	
#define SEG_Qseg7b		20
#define SEG_Qseg7c		22
#define SEG_Qseg7d		31
#define SEG_Qseg7e		30
#define SEG_Qseg7f		29
#define SEG_Qseg7g		21
#define SEG_QsegDp7		23



#define SEG_QsegDp8		35
#define SEG_QsegDp9		159

#define SEG_Qseg8a 		12
#define SEG_Qseg8b 		4 
#define SEG_Qseg8c 		6 
#define SEG_Qseg8d 		15
#define SEG_Qseg8e 		14
#define SEG_Qseg8f 		13
#define SEG_Qseg8g 		5 
#define	SEG_P1			107


LcdDrive_EXT    u8     				LCDMEM[35];
   
#endif
   
   




