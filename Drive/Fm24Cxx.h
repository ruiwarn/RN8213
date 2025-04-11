

#ifndef  _FM24CXX_H
#define  _FM24CXX_H

//---------------------------------------------------
//��д����
#define Read    0
#define Write   1
#define	Memory_Size 64
//---------------------------------------------------
//�洢�����Ͷ��壬ҳ��С����
#define EEPROM24C04MAXSize   	(4*128)  //ÿ1KbitΪ128�ֽ�
#define EEPROM24C128MAXSize  	(128*128)	//ÿ1KbitΪ128�ֽ�
#define EEPROM24C256MAXSize  	(256*128)	//ÿ1KbitΪ128�ֽ�
#define EEPROM_MAXSize   EEPROM24C256MAXSize
//---------------------
#define EEPROM24C04PageSize   	0x07
#define EEPROM24C128PageSize  	0x3f
//#define EEPROM24C128PageSize  	0x1f
#define EEPROM_PageSize   EEPROM24C128PageSize

#define Read_EEPROM              ((u8  *) EEPROM_MEMORY_BASE    )
#define	Read_Flash				 ((u8  *) FLASH_MEMORY_BASE    ) 
//---------------------------------------------------
//extern ee_test();
extern ErrorStatus fnFM24CXX_Read(u8 *Dst , u32 Src , u32 DatLen , u8 Repeat);
extern ErrorStatus fnFM24CXX_Write(u32 Dst , u8 *Src , u32 DatLen , u8 Repeat);

#endif	//_FM24CXX_H

//---------------------------------------------------





















