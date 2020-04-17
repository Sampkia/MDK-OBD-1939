#ifndef JTT808_AREA_H_
#define JTT808_AREA_H_

#include "jtt808.h"

void JTT808_Areacycleset(JTT808t_Msg* msg);              //0x8600	����Բ������
void JTT808_Areacycledel(JTT808t_Msg* msg);              //0x8601	ɾ��Բ������
void JTT808_Arearectangleset(JTT808t_Msg* msg);          //0x8602	���þ�������
void JTT808_Arearectangledel(JTT808t_Msg* msg);          //0x8603	ɾ����������
void JTT808_Areapolygonset(JTT808t_Msg* msg);            //0x8604	���ö��������
void JTT808_Areapolygondel(JTT808t_Msg* msg);            //0x8605	ɾ�����������
void JTT808_Routeset(JTT808t_Msg* msg);                  //0x8606	����·��
void JTT808_Routedel(JTT808t_Msg* msg);                  //0x8607	ɾ��·��

void JTT808_Area_Alarm_check(void);                      //����ɨ��



#endif


