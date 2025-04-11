#ifndef _DL645_FRONT_H
#define _DL645_FRONT_H

#ifdef   _DL645_FRONT_GLOBALS
	#define  DL645FRONT_EXT
#else
	#define  DL645FRONT_EXT  extern 
#endif

typedef enum{
	POSITIVE		= 0x00,    //����
	INVERSION		= 0x01    //����
} eDl645Front_Direction_TypeDef;//�����ֻҪö�������򣬸������������������

typedef enum{
	FRONT_FLAG_INIT			= 0x01,	
	FRONT_FLAG_RN8209RST	= 0x02	
} eDl645Front_Flag_TypeDef;


typedef struct
{
    u8		ChkErrCnt;
    u32 	Pw[2];   		    //pa,pb   
    u32 	UI[3];          // Ia=UI[0] Inal U         
    u16 	Frequency;   	//����Ƶ�ʣ���λ��                            	
    u32		Pulse;		    //ǰ̨����
    u16     Pstart;
    //---��������---	
    u32		Pulse_Eg;	    //�������
    u32 	PDirect;		//���ʷ���
    u32 	ChkSum1;		//����EMU��У�����У��
} sDl645FrontTmp_TypeDef;

typedef struct
{	
	struct sDl645FrontPubData_TypeDef  {
		u16		U;			    //---��ѹ---NNN.N
		u32	    Ia;			    //---����NNNN.NNNN(����ֵҪ��3��3С������ֵҪ��2��4С�����λ��ʾ����)---		
		u32	    In;             //---���ߵ���
		sDF09	Pw;			    //---˲ʱ�й�p
		u16		Pf;			    //---��������N.NNN---	���λ��ʾ����{Pf Pfa Pfb Pfc}	
		u16		Angle;		    //---���NNN.N---		
		u16		Frequency;		//---Ƶ��NN.NN
		u32		PPwave;			//---һ����ƽ������NN.NNNN
		u8      Chnsel;
		u16		Temperature;	//---NNN.N  �¶�
		u16		ClockBat;		//---NN.NN  ��ص�ѹ
		u32		tWorkBat;		//---NNNN  ʱ�ӹ���ʱ�䣨���ӣ�
		u8		PDirect;		//---ԭ���ʷ���
		
	    u16    	CfIn; 			//���������ƽ�ж�
	    u8    	CfTime;			//
	    u8    	Step;
	    u16   	FrontStamp;
	} PubData;
	
	struct sDl645FrontPriData_TypeDef  {		
		u8		Flag;			//---�����쳣��־---
	} PriData;	
	
	struct sDl645FrontPriPara_TypeDef  {		
		u32	 PConstE;			//�й�����
		u16	 Crc;
	} PriPara;		
} sDl645Front_TypeDef;	

extern	void fnDl645Front_Init(void);				//�ϵ��ʼ��У�������д��EMU
extern	void fnDl645Front_Exec(void);               //EMU�������������������˲��������ص�ѹ
extern  void fnFront_TemperIAGain(void);
extern	void fnFront_CurMaxPGain(void);
#endif
