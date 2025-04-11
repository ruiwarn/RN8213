/******************************************************************************
 * @file     rtc.h
 * @brief    rtc driver header file for RN821x series SOC
 * @author   Renergy Technology
 * @version  V1.00
 * @date     2012-11-28
 *
 * @note
 * Copyright (C) 2008, Renergy Technology Inc. All rights reserved.
 *
 *
 ******************************************************************************/
#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief RtcReadDota
 *
 * @param 
 *    none
 * @return 
 *    uint16_t dota: dota value of RTC
 *
 */
uint16_t  RtcReadDota(void);

/**
 * @brief RtcWriteDota
 *
 * @param 
 *    in uint16_t dota: dota value of RTC
 * @return 
 *    none
 *
 */
void   RtcWriteDota(uint16_t dota);

void sysUpdate(void);
#ifdef __cplusplusw
}
#endif

#endif
