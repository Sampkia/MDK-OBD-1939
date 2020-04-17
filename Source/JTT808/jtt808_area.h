#ifndef JTT808_AREA_H_
#define JTT808_AREA_H_

#include "jtt808.h"

void JTT808_Areacycleset(JTT808t_Msg* msg);              //0x8600	设置圆形区域
void JTT808_Areacycledel(JTT808t_Msg* msg);              //0x8601	删除圆形区域
void JTT808_Arearectangleset(JTT808t_Msg* msg);          //0x8602	设置矩形区域
void JTT808_Arearectangledel(JTT808t_Msg* msg);          //0x8603	删除矩形区域
void JTT808_Areapolygonset(JTT808t_Msg* msg);            //0x8604	设置多边形区域
void JTT808_Areapolygondel(JTT808t_Msg* msg);            //0x8605	删除多边形区域
void JTT808_Routeset(JTT808t_Msg* msg);                  //0x8606	设置路线
void JTT808_Routedel(JTT808t_Msg* msg);                  //0x8607	删除路线

void JTT808_Area_Alarm_check(void);                      //区域扫描



#endif


