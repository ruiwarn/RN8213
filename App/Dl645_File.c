
#define  _DL645_FILE_GLOBALS
#include <App.h>
//�ļ�����Ϣ������˳��������ļ����ű���һ��
const sFileItemInfo_TypeDef Dl645FileItemInfo_GeneralPara[] = 
{
	{	Dl645FileItemInfoOffAddr_GeneralPara_ComPara 	,	Dl645FileItemInfoLen_GeneralPara_ComPara 	},
	{	Dl645FileItemInfoOffAddr_GeneralPara_DispPara 	,	Dl645FileItemInfoLen_GeneralPara_DispPara 	}
};//ͨ�ò����ļ���ţ�5��У�������ȣ�ͨѶ�����ʣ��й����ʵ�ѹ�����ޣ�����ʱ��ȣ�

const sFileItemInfo_TypeDef Dl645FileItemInfo_HighPara[] = 
{
	{	Dl645FileItemInfoOffAddr_HighPara_PExpression  	,	Dl645FileItemInfoLen_HighPara_PExpression  	},
	{	Dl645FileItemInfoOffAddr_HighPara_ComAddr	,	Dl645FileItemInfoLen_HighPara_ComAddr	},
	{	Dl645FileItemInfoOffAddr_HighPara_MeterNo	,	Dl645FileItemInfoLen_HighPara_MeterNo	},
	{	Dl645FileItemInfoOffAddr_HighPara_PConstE	,	Dl645FileItemInfoLen_HighPara_PConstE	}
};//�߼������ļ���ţ�30����ţ��������ͻ���ţ��й��ȼ����������ڣ����ѹ����������������Ȳ�����

const sFileItemInfo_TypeDef Dl645FileItemInfo_FirmPara[] = 
{
	{	Dl645FileItemInfoOffAddr_FirmPara_SYSCON	,	Dl645FileItemInfoLen_FirmPara_SYSCON	},
	{	Dl645FileItemInfoOffAddr_FirmPara_EMUCON	,	Dl645FileItemInfoLen_FirmPara_EMUCON	},
	{	Dl645FileItemInfoOffAddr_FirmPara_HFConst	,	Dl645FileItemInfoLen_FirmPara_HFConst	},
	{	Dl645FileItemInfoOffAddr_FirmPara_PStart	,	Dl645FileItemInfoLen_FirmPara_PStart	},
	{	Dl645FileItemInfoOffAddr_FirmPara_QStart	,	Dl645FileItemInfoLen_FirmPara_QStart	},
	{	Dl645FileItemInfoOffAddr_FirmPara_GPQA	,	Dl645FileItemInfoLen_FirmPara_GPQA	},
	{	Dl645FileItemInfoOffAddr_FirmPara_GPQB	,	Dl645FileItemInfoLen_FirmPara_GPQB	},
	{	Dl645FileItemInfoOffAddr_FirmPara_IAGAIN	,	Dl645FileItemInfoLen_FirmPara_IAGAIN	},
	{	Dl645FileItemInfoOffAddr_FirmPara_UGAIN	,	Dl645FileItemInfoLen_FirmPara_UGAIN	},
	{	Dl645FileItemInfoOffAddr_FirmPara_IBGAIN	,	Dl645FileItemInfoLen_FirmPara_IBGAIN	},
	{	Dl645FileItemInfoOffAddr_FirmPara_PhsA	,	Dl645FileItemInfoLen_FirmPara_PhsA	},
	{	Dl645FileItemInfoOffAddr_FirmPara_PhsB	,	Dl645FileItemInfoLen_FirmPara_PhsB	},
	{	Dl645FileItemInfoOffAddr_FirmPara_QPhsCal	,	Dl645FileItemInfoLen_FirmPara_QPhsCal	},
	{	Dl645FileItemInfoOffAddr_FirmPara_APOSA	,	Dl645FileItemInfoLen_FirmPara_APOSA	},
	{	Dl645FileItemInfoOffAddr_FirmPara_APOSB	,	Dl645FileItemInfoLen_FirmPara_APOSB	},
	{	Dl645FileItemInfoOffAddr_FirmPara_RPOSA	,	Dl645FileItemInfoLen_FirmPara_RPOSA	},
	{	Dl645FileItemInfoOffAddr_FirmPara_RPOSB	,	Dl645FileItemInfoLen_FirmPara_RPOSB	},
	{	Dl645FileItemInfoOffAddr_FirmPara_IARMSOS	,	Dl645FileItemInfoLen_FirmPara_IARMSOS	},
	{	Dl645FileItemInfoOffAddr_FirmPara_IBRMSOS	,	Dl645FileItemInfoLen_FirmPara_IBRMSOS	},
	{	Dl645FileItemInfoOffAddr_FirmPara_KUrms	,	Dl645FileItemInfoLen_FirmPara_KUrms	},
	{	Dl645FileItemInfoOffAddr_FirmPara_KIArms	,	Dl645FileItemInfoLen_FirmPara_KIArms	},
	{	Dl645FileItemInfoOffAddr_FirmPara_KIBrms	,	Dl645FileItemInfoLen_FirmPara_KIBrms	},
	{	Dl645FileItemInfoOffAddr_FirmPara_KPrms	,	Dl645FileItemInfoLen_FirmPara_KPrms	},
	{	Dl645FileItemInfoOffAddr_FirmPara_PStarDisp	,	Dl645FileItemInfoLen_FirmPara_PStarDisp	},
	{	Dl645FileItemInfoOffAddr_FirmPara_IStarDisp	,	Dl645FileItemInfoLen_FirmPara_IStarDisp	},
	{	Dl645FileItemInfoOffAddr_FirmPara_ChkSum	,	Dl645FileItemInfoLen_FirmPara_ChkSum	},
	{	Dl645FileItemInfoOffAddr_FirmPara_RTCDota0	,	Dl645FileItemInfoLen_FirmPara_RTCDota0	}
};//У������ļ���ţ�17�����ݼ���оƬ���޸ģ�

const sFileItemInfo_TypeDef Dl645FileItemInfo_NvRam[] = 
{
	{	Dl645FileItemInfoOffAddr_NvRam_FristPowOn 	,	Dl645FileItemInfoLen_NvRam_FristPowOn 	},
	{	Dl645FileItemInfoOffAddr_NvRam_pFile 	,	Dl645FileItemInfoLen_NvRam_pFile 	}
};//FRAM�洢��������ţ�2��оƬ��ʶ���洢ָ�룩

//�ļ���Ϣ������˳��������ļ���ű���һ��
const sDl645FileInfo_TypeDef Dl645FileInfo[] =
{
	{	Dl645FileAddr_NvRam		,	Dl645FileLen_NvRam	,	Dl645FileItemInfoNum_NvRam_Max	,	Dl645FileItemInfo_NvRam	},
	{	Dl645FileAddr_FirmPara	,	Dl645FileLen_FirmPara	,	Dl645FileItemInfoNum_FirmPara_Max	,	Dl645FileItemInfo_FirmPara	},
	{	Dl645FileAddr_HighPara	,	Dl645FileLen_HighPara	,	Dl645FileItemInfoNum_HighPara_Max	,	Dl645FileItemInfo_HighPara	},
	{	Dl645FileAddr_GeneralPara ,	Dl645FileLen_GeneralPara	,	Dl645FileItemInfoNum_GeneralPara_Max	,	Dl645FileItemInfo_GeneralPara	}
};

// Զ�̷ѿر���������
const sDl645GeneralParaFile_TypeDef GeneralParaDefault =
{
	{0x04 , 0x08 , 0x08 , 0x08 , 0x08} , 
	{
		0x05 ,   //ѭ������
		0x05 ,   //��ʾʱ��
		0x02 ,   //��ʾ����С��λ��
		0x04 ,   //��ʾ����С��λ��
		0x06 ,   //������ʾ����
		
		{
			0x00,0x01,0x01,0x02,0x00,	//������(Ĭ������)
			0x00,0x01,0x02,0x02,0x00,
			0x00,0x00,0x03,0x02,0x00,
			0x01,0x01,0x00,0x04,0x00,
		    0x02,0x01,0x00,0x04,0x00
	    },
		
		{
			0x00,0x01,0x01,0x02,0x00,	//������(Ĭ������)
			0x00,0x01,0x02,0x02,0x00,
			0x00,0x00,0x03,0x02,0x00,
			0x00,0x00,0x06,0x02,0x00,
			0x01,0x01,0x00,0x04,0x00,
		    0x02,0x01,0x00,0x04,0x00
		},
		0x05
	}
};

const sDl645HighParaFile_TypeDef HighParaDefault =
{
	0x05 ,
	
	{0x11 , 0x11 , 0x11 , 0x11 , 0x11 , 0x11} , 
	{0x11 , 0x11 , 0x11 , 0x11 , 0x11 , 0x11} 
};


const sDl645FirmParaFile_TypeDef FirmParaDefault = 
{
	0x0007 ,							                               
	0x0000 , 	                                                       
	0x0f57 , 				                                           
	0x0060 , 				                                           
	0x0120 , 				                                           
	0x0000 ,									                       
	0x0000 , 															
	0x0000 ,
	0x0000 , 															   
	0x0000 , 															   
	0x0000 , 									                       
	0x0000 , 																						
	0x0000 , 															
	0x0000 , 															
	0x0000 , 															
	0x0000 ,                                                           
	0x0000 , 															
	0,
	0,
	47.66254,
	4.1943,
	0,
	610.08,
	0,
	0,
	0xe3ef21,
	0,
};

ErrorStatus fnDl645File_Read(u8 FileId , u32 OffAddr , void *Dst , u32 Len)
{
	if(FileId >= Dl645FileId_Max) return(ERROR);
	if( (OffAddr + Len) > Dl645FileInfo[FileId].Len) return(ERROR);	
	return(fnMemory_Read(Dst , Dl645FileInfo[FileId].StartAddr + OffAddr , Len) );
}

ErrorStatus fnDl645File_Write(u8 FileId , u32 OffAddr , void *Src , u32 Len)
{
	if(FileId >= Dl645FileId_Max) return(ERROR);
	if( (OffAddr + Len) > Dl645FileInfo[FileId].Len) return(ERROR);	
	return(fnMemory_Write(Dl645FileInfo[FileId].StartAddr + OffAddr , Src , Len) );	
}

ErrorStatus fnDl645File_Clr(u8 FileId)
{
	u32 OffAddr,OnceLen;
	u8  *Buf;
	
	if(FileId >= Dl645FileId_Max) return(ERROR);
	
	Buf = malloc(MALLOC_SIZE);
	if(Buf == NULL) return(ERROR);
	
	memset(Buf , 0 , MALLOC_SIZE );
		
	OffAddr = 0;
	while(OffAddr < Dl645FileInfo[FileId].Len)
	{
		fnWDT_Restart();
		if( (OffAddr + MALLOC_SIZE) > Dl645FileInfo[FileId].Len) OnceLen = Dl645FileInfo[FileId].Len - OffAddr;
		else OnceLen = MALLOC_SIZE;
		fnDl645File_Write(FileId , OffAddr , Buf , OnceLen);	
		OffAddr += OnceLen;
		fnWDT_Restart();
	}		
	
	free(Buf);		
	return(SUCCESS);
}

//---------------------------------------------------------
ErrorStatus fnDl645FileItem_Read(u8 FileId , u8 nItem , void *Dst)
{
	if(FileId >= Dl645FileId_Max) return(ERROR);		
	if(nItem >= Dl645FileInfo[FileId].MaxItem) return(ERROR);
	return(fnMemory_Read(Dst , Dl645FileInfo[FileId].StartAddr + Dl645FileInfo[FileId].pItemInfo[nItem].OffAddr , Dl645FileInfo[FileId].pItemInfo[nItem].Len) );	
}

ErrorStatus fnDl645FileItem_Write(u8 FileId , u8 nItem , void *Src)
{
	if(FileId >= Dl645FileId_Max) return(ERROR);		
	if(nItem >= Dl645FileInfo[FileId].MaxItem) return(ERROR);
	return(fnMemory_Write(Dl645FileInfo[FileId].StartAddr + Dl645FileInfo[FileId].pItemInfo[nItem].OffAddr , Src , Dl645FileInfo[FileId].pItemInfo[nItem].Len) );
}

ErrorStatus fnDl645FileItem_Clr(u8 FileId , u8 nItem)
{
	u32 OffAddr,OnceLen;
	u8 	Buf[64];
	
	if(FileId >= Dl645FileId_Max) return(ERROR);
	if(nItem >= Dl645FileInfo[FileId].MaxItem) return(ERROR);
	
	memset(Buf , 0 , sizeof(Buf) );  
		
	OffAddr = 0;
	while(OffAddr < Dl645FileInfo[FileId].pItemInfo[nItem].Len)
	{
		if( (OffAddr + sizeof(Buf) ) > Dl645FileInfo[FileId].pItemInfo[nItem].Len) OnceLen = Dl645FileInfo[FileId].pItemInfo[nItem].Len - OffAddr;
		else OnceLen = sizeof(Buf);
		fnDl645File_Write(FileId , Dl645FileInfo[FileId].pItemInfo[nItem].OffAddr + OffAddr , Buf , OnceLen);	
		OffAddr += OnceLen;
	}		
	return(SUCCESS);
}

//---------------------------------------------------------
const u8 FristPowOn[4]={'1','y','D','B'};
void fnDl645File_Init(void)
{
	u8 Tmp[4];
	u8 i;
	for(i=0;i<3;i++)
	{
		fnDl645File_Read(Dl645FileId_NvRam , 0 , &Tmp[0] , 4);
		if(memcmp( &FristPowOn[0] , &Tmp[0] , 4) == 0) return;
		fnWDT_Restart();
	}
	
	for(i = 0 ; i < Dl645FileId_Max ; i++)
	{
		if(i == Dl645FileId_FirmPara) 
		{
			fnDl645File_Write(Dl645FileId_FirmPara , 0 , (u8 *)&FirmParaDefault , Dl645FileLen_FirmPara);
		}
		else if(i == Dl645FileId_HighPara) 
		{
			fnDl645File_Write(Dl645FileId_HighPara , 0 , (u8 *)&HighParaDefault , Dl645FileLen_HighPara);			
		}
		else if(i == Dl645FileId_GeneralPara) 
		{
			fnDl645File_Write(Dl645FileId_GeneralPara , 0 , (u8 *)&GeneralParaDefault , Dl645FileLen_GeneralPara);			
		}
		else fnDl645File_Clr(i);
		fnWDT_Restart();	
	}
	fnDl645File_Clr(Dl645FileId_NvRam);
	fnDl645File_Write(Dl645FileId_NvRam , 0 , (u8 *)&FristPowOn[0] , 4);
	fnWDT_Restart();
	return;
}




