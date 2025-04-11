
#ifndef _DL645_MEMORY_H
#define _DL645_MEMORY_H



#define MALLOC_SIZE					100

#define AT24C256_START_ADDR 		0x92000000             //32K byte
#define AT24C256_SIZE				0x00008000

extern ErrorStatus fnMemory_Read(u8 *Dst , u32 Src , u32 DatLen);
extern ErrorStatus fnMemory_Write(u32 Dst , u8 *Src , u32 DatLen);


#endif // _DL645_MEMORY_H 










