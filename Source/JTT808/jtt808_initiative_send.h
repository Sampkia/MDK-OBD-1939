#ifndef JTT808_INITIATIVE_SHEND_H_
#define JTT808_INITIATIVE_SHEND_H_

#include "stm32f10x.h"

void JTT808_TransData(char* src,u16 len);
void JTT808_InitiativeCmd(void);
void JTT808_TerminalReg_back(void);
void JTT808_TransPlace(char* src,u16 len);
void JTT808_CAN_data_upload(u32 can_id,u8 sample_type,u8 frame_type,u8 can_channel,char* buf);

#endif
