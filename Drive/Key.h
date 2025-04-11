
#ifndef  _KEY_H
#define  _KEY_H

#ifdef   _KEY_GLOBALS
	#define  KEY_EXT
#else
	#define  KEY_EXT  extern 
#endif



typedef enum
{
	KEY_VAL_PRG		= (1 << 0),	//��̰���
	KEY_VAL_TURN	= (1 << 1),	//���԰���
	KEY_VAL_SHELL	= (1 << 2),	//����ǰ���
	KEY_Relay_Ctrl= (1 << 3), //�̵��������բ����
	KEY_Open_Cover= (1 << 4),//����ǣ��ϱ��
	KEY_VAL_CARD	= (1 << 5),
}eKeyVal_TypeDef;



typedef struct {
	u8 		KeyTurn_Scan;      //���԰�������ɨ������
	u8 		KeyProg_Scan;     //��̰�������ɨ������	
	u8 		KeyShell_Scan;    //����ǲ���ɨ������
	u8 		Press;		      //��������ʶ��λ,�������԰����Ķ̰�����������̼��İ��£����ǲ�����
	u8 		Release;		  //�����ͷ�ʶ��λ,�������԰������ͷ�,��̼����ͷţ��ϸǲ���?
	u16     KeyTurn_Time;     //���԰�ť�İ���ʱ���ۼƼ������Է�����б����բ������
	    			
} sKey_TypeDef;				   //KEY_TYPE;

KEY_EXT sKey_TypeDef Key;

//==============================================
extern void fnKey_Init(void);
extern void fnKey_Scan(void);
extern void fnKey_Exec(void);


#endif	//_KEY_H




