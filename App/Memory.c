#define  _DL645_MEMORY_GLOBALS
#include <App.h>

//�洢����ȡ����
ErrorStatus fnMemory_Read(u8 *Dst , u32 Src , u32 DatLen)
{
	ErrorStatus err=SUCCESS;
	if( (AT24C256_START_ADDR <= Src) && 
		( (Src + DatLen) < (AT24C256_START_ADDR + AT24C256_SIZE) ) )
	{		
		#if  !DL645SOFT_DEBUG
		err = fnFM24CXX_Read(Dst , Src - AT24C256_START_ADDR , DatLen , 3);
		#endif
	}
	else
	{	
		err = ERROR;
	}
	
	return(err);
}

//�洢��д�뺯��
ErrorStatus fnMemory_Write(u32 Dst , u8 *Src , u32 DatLen)
{
	ErrorStatus err=SUCCESS;

	if( (AT24C256_START_ADDR <= Dst) && 
		( (Dst + DatLen) < (AT24C256_START_ADDR + AT24C256_SIZE) ) )
	{		
		#if  !DL645SOFT_DEBUG
		err = fnFM24CXX_Write( Dst - AT24C256_START_ADDR , Src , DatLen , 3);
		#endif
	}
	
	else
	{	
		err = ERROR;
	}
	return(err);	
}




