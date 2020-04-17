#ifndef JTT808_REC_H_
#define JTT808_REC_H_


extern u8 to_lock_car;

void JTT808_terminal_act(u16 serial_no,u16 msg_id,char res);
int JTT808_get_msg(JTT808_RX_BUF *jtt808_rx_buf);
void JTT808_msg_prase(JTT808_RX_BUF* jtt808_rx_buf);

#endif
