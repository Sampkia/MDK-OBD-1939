#ifndef __INCLUDE_H
#define __INCLUDE_H

/*------------Stand C Library-------------*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

/*-------------STM32 Library--------------*/
//#include "stm32f10x.h"


/*----------Self Define Library-----------*/
#include "debug.h"

#include "Driver/bsp.h"
#include "Driver/systick.h"
#include "Driver/usart.h"
#include "Driver/wdg.h"
#include "Driver/flash.h"
#include "Driver/can.h"
#include "Driver/timer.h"

#include "Peripheral/w25x.h"
#include "Peripheral/gps.h"
#include "Peripheral/gprs.h"
#include "Peripheral/agps.h"

#include "Common/common.h"
#include "Common/Config_Parameter.h"

#include "OBD_1939/obd_1939.h"

#include "JTT808/jtt808.h"
#include "JTT808/jtt808_alarm_state.h"
#include "JTT808/jtt808_area.h"
#include "JTT808/jtt808_config.h"
#include "JTT808/jtt808_initiative_send.h"
#include "JTT808/jtt808_list_prc.h"
#include "JTT808/jtt808_location.h"
#include "JTT808/jtt808_Param.h"
#include "JTT808/jtt808_point_in_polygon.h"
#include "JTT808/jtt808_rec.h"

#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "usb_prop.h"
#include "usb_istr.h"
#include "memory.h"

#include "fatfs/ff.h"
#include "fatfs/diskio.h"
#endif
