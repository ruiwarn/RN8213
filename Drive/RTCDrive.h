#ifndef  _RTCDrive_H
#define  _RTCDrive_H

/***************Ӳʱ��RX8025SA�Ĵ�������********************/




extern ErrorStatus fnRTCDrive_Read(u8 *Dst);
extern ErrorStatus fnRTCDrive_Write(u8 *Dst , u8 Repeat);
extern ErrorStatus fnRTCDrive_Init(void);
extern ErrorStatus fnSysClock_Read(sDF01 *Clock);
extern ErrorStatus fnSysClock_Write(sDF01 *Clock);

#endif	//_RX8025_H


