#ifndef JTT808_LIST_PRC_
#define JTT808_LIST_PRC_
#include "include.h"

struct NODE
{
    struct NODE* next;      //��һ���ڵ�ĵ�ַ
    int flash_address;      //��Ϣ����ŵ�flash��ַ
    int send_time;          //�ϴ��ش�ʱ��
    char repeat_times;      //�ش�����
    char initiative_send;   //0��Ӧ����Ϣ   1��������Ϣ
    uint16_t msg_id;         //��ϢID
    uint16_t msg_serial_no;  //��Ϣ����ˮ��
    uint16_t msg_size;       //��Ϣ���С
    uint16_t package_cnt;    //�ְ��ܴ�С
    uint16_t package_num;    //�ְ����
};

extern struct NODE HEAD;
extern int JTT808_MSG_WRITE_ADDR;  //��Ϣ�洢��ʼ��ַ

void JTT808_Ruerypackage_proc(char* body,int size);

int get_repeat_timeout_time(int repeat_times);

JTT808t_Err jtt808_msg_node_del(u16 serial_no,u16 msg_id);

void JTT808_flash_msg_send(void);

JTT808t_Err create_list_write_flash(JTT808t_Msg*,char* ,int,u8);

#endif
