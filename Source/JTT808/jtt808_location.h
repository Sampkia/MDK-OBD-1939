#ifndef JTT808_LOCATION_H_
#define JTT808_LOCATION_H_

extern JTT808t_LocationTrackCtrl track;
extern u32 LOACTION_READ_ADDR;                  //�ϵ���������ַ
extern u32 LOACTION_WRITE_ADDR;                 //�ϵ�����д��ַ


void JTT808_LocatParse(u8* msg_buf,u32 *msg_buf_len, int type);
void JTT808_LocationReport(void);
void JTT808_turning_report(void);


#endif
