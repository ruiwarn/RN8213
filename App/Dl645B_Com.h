#ifndef __DL645B_COM_H
#define __DL645B_COM_H

#ifdef   DL645B_COM_GLOBALS
	#define  DL645B_EXT 	
#else
	#define  DL645B_EXT  extern 
#endif


#define	DL645B_FRAME_MAXLEN		195
#define DL645B_LEADERCODE		0xFE	//前导码
#define DL645B_LEADERCODE_LEN	4		//前导码长度
#define	F_Comm1Event			1		// 正在通讯标志

typedef enum
{	
	DL645B_ERRINFO_MISC      = 0x01,
	DL645B_ERRINFO_DATA      = 0x02,
	DL645B_ERRINFO_PSW       = 0x04,
	DL645B_ERRINFO_BPS       = 0x08,
	DL645B_ERRINFO_TZONE     = 0x10,
	DL645B_ERRINFO_POT       = 0x20,
	DL645B_ERRINFO_FL        = 0x40,
	
	DL645B_ERRINFO_SEQDATA   = 0x80,
	
	DL645B_ERRINFO_OK        = 0x00
} eDL645B_ErrInfo_TypeDef;


typedef struct {
  __IO uint32_t Buf[107];
}sDl645B_EMU_TypeDef;

typedef struct {
  __IO uint32_t Buf[32];
}sDl645B_RTC_TypeDef;

#define Dl645B_EMUCom             ((sDl645B_EMU_TypeDef     *) EMU_BASE           )
#define	Dl645B_RTCCom			  ((sDl645B_RTC_TypeDef     *) RTC_BASE           )

extern   ErrorStatus	fnDl645B_FrameCheck(sFrmDl645B_TypeDef *pCheck);
extern   eDL645B_ErrInfo_TypeDef fnDl645_GetVar(u8 DI2 , u8 DI1 , u8 DI0 , u8 *Dst , u16 *Len);
extern  u16 fnDl645B_Exec(u8 ComPort , sFrmDl645B_TypeDef *RxFrm);
extern	eDL645B_ErrInfo_TypeDef fnDl645_GetParaVar(u8 DI2 , u8 DI1 , u8 DI0 , u8 *Dst , u16 *Len);
extern	eDL645B_ErrInfo_TypeDef fnDl645_GetFrz(u8 DI2 , u8 DI1 , u8 DI0 , u8 Seq , u8 *Dst , u16 *Len);
extern	void fnDl645B_Inverse(u8 *Buf, u8 Len);
extern	ErrorStatus fnDl645B_JudgeClockNoWeek( u8 Start, u8 *Ck, u8 Len );
#endif
