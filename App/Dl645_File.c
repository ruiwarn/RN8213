
#define  _DL645_FILE_GLOBALS
#include <App.h>
//文件项信息，排列顺序必须与文件项编号保持一致
const sFileItemInfo_TypeDef Dl645FileItemInfo_GeneralPara[] = 
{
	{	Dl645FileItemInfoOffAddr_GeneralPara_ComPara 	,	Dl645FileItemInfoLen_GeneralPara_ComPara 	},
	{	Dl645FileItemInfoOffAddr_GeneralPara_DispPara 	,	Dl645FileItemInfoLen_GeneralPara_DispPara 	}
};//通用参数文件编号（5个校表脉冲宽度，通讯波特率，有功功率电压上下限，冻结时间等）

const sFileItemInfo_TypeDef Dl645FileItemInfo_HighPara[] = 
{
	{	Dl645FileItemInfoOffAddr_HighPara_PExpression  	,	Dl645FileItemInfoLen_HighPara_PExpression  	},
	{	Dl645FileItemInfoOffAddr_HighPara_ComAddr	,	Dl645FileItemInfoLen_HighPara_ComAddr	},
	{	Dl645FileItemInfoOffAddr_HighPara_MeterNo	,	Dl645FileItemInfoLen_HighPara_MeterNo	},
	{	Dl645FileItemInfoOffAddr_HighPara_PConstE	,	Dl645FileItemInfoLen_HighPara_PConstE	}
};//高级参数文件编号（30个表号，常数，客户编号，有功等级，生产日期，额定电压，额定电流，最大电流等参数）

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
};//校表参数文件编号（17个根据计量芯片可修改）

const sFileItemInfo_TypeDef Dl645FileItemInfo_NvRam[] = 
{
	{	Dl645FileItemInfoOffAddr_NvRam_FristPowOn 	,	Dl645FileItemInfoLen_NvRam_FristPowOn 	},
	{	Dl645FileItemInfoOffAddr_NvRam_pFile 	,	Dl645FileItemInfoLen_NvRam_pFile 	}
};//FRAM存储器参数编号（2个芯片标识及存储指针）

//文件信息，排列顺序必须与文件编号保持一致
const sDl645FileInfo_TypeDef Dl645FileInfo[] =
{
	{	Dl645FileAddr_NvRam		,	Dl645FileLen_NvRam	,	Dl645FileItemInfoNum_NvRam_Max	,	Dl645FileItemInfo_NvRam	},
	{	Dl645FileAddr_FirmPara	,	Dl645FileLen_FirmPara	,	Dl645FileItemInfoNum_FirmPara_Max	,	Dl645FileItemInfo_FirmPara	},
	{	Dl645FileAddr_HighPara	,	Dl645FileLen_HighPara	,	Dl645FileItemInfoNum_HighPara_Max	,	Dl645FileItemInfo_HighPara	},
	{	Dl645FileAddr_GeneralPara ,	Dl645FileLen_GeneralPara	,	Dl645FileItemInfoNum_GeneralPara_Max	,	Dl645FileItemInfo_GeneralPara	}
};

// 远程费控表数据内容
const sDl645GeneralParaFile_TypeDef GeneralParaDefault =
{
	{0x04 , 0x08 , 0x08 , 0x08 , 0x08} , 
	{
		0x05 ,   //循显屏数
		0x05 ,   //显示时间
		0x02 ,   //显示电能小数位数
		0x04 ,   //显示功率小数位数
		0x06 ,   //按键显示屏数
		
		{
			0x00,0x01,0x01,0x02,0x00,	//轮显项(默认五屏)
			0x00,0x01,0x02,0x02,0x00,
			0x00,0x00,0x03,0x02,0x00,
			0x01,0x01,0x00,0x04,0x00,
		    0x02,0x01,0x00,0x04,0x00
	    },
		
		{
			0x00,0x01,0x01,0x02,0x00,	//轮显项(默认五屏)
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




