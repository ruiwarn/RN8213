

#ifndef  _APP_H
#define  _APP_H

#define	GataFlag				0		// 继电器，为0 为内置继电器  为1 为外置继电器
#define	GataType				0		// 继电器型号，为0 拉合闸时间为200MS  为1 拉合闸时间为35MS
//------------------------------

#define DL645_MAX_CYCDISP                 99    //最大轮显项数
#define DL645_MAX_KEYDISP                 99    //最大键显项数

//------------------------------

#include <math.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <Drive_Lib.h>
#include <Memory.h>

#include <Scom.h>

#include <Dl645_Types.h>
#include <Dl645_File.h>

#include <Dl645_Front.h>
#include <Dl645_Bkgrd.h>
#include <Dl645B_Com.h>
#include <Dl645_Disp.h>
#include <Dl645_ICEsam.h>
#include <Dl645_Globals.h>

#endif	//_APP_H

