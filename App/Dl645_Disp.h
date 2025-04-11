#ifndef __DL645_DISP_H
#define __DL645_DISP_H

#ifdef   DL645_DISP_GLOBALS
	#define  DL645DISP_EXT 	
#else
	#define  DL645DISP_EXT  extern 
#endif

//�˻��Ի����ݽṹ
typedef struct {
	u8  Mode;/*0�Զ�ѭ��ģʽ��1������ʾ��2�ر���ʾ*/
	u8  AutoSerialNo;//0~99 �Զ���������
	u8  KeySeriaNo;//0~99 ������������
	u8  Stamp; //ʱ����
	u8	CardDispStamp;
	u8  EventErr1;
	u8  EventErr2;
	u8  EventKeyErr;
	u8	BkLightFlag;	//���������־
	u8  BkLightStamp;  //����ʱ����
	u8  RemoteStamp;   //ң��ʱ����
  	u8  Item[5];       //��Ŀ
  	u8  lItem[5];      //1��Ŀ
	u8  LcdCount;		//
	u8  DigitPosition; //����λ��
	u8 	DisplyErrFlag;
	u8	AllOnDispTime;	// �ϵ�ȫ��ʱ��	
} sDl645B_Session_TYPE;



#define DISPLAYAUTO 0
#define DISPLAYKEY  1
#define DISPLARYEMOTE  2  //ң��������
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
