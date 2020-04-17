#ifndef JTT808_LIST_PRC_
#define JTT808_LIST_PRC_
#include "include.h"

struct NODE
{
    struct NODE* next;      //下一个节点的地址
    int flash_address;      //消息包存放的flash地址
    int send_time;          //上次重传时间
    char repeat_times;      //重传次数
    char initiative_send;   //0：应答消息   1：主发消息
    uint16_t msg_id;         //消息ID
    uint16_t msg_serial_no;  //消息包流水号
    uint16_t msg_size;       //消息体大小
    uint16_t package_cnt;    //分包总大小
    uint16_t package_num;    //分包序号
};

extern struct NODE HEAD;
extern int JTT808_MSG_WRITE_ADDR;  //消息存储起始地址

void JTT808_Ruerypackage_proc(char* body,int size);

int get_repeat_timeout_time(int repeat_times);

JTT808t_Err jtt808_msg_node_del(u16 serial_no,u16 msg_id);

void JTT808_flash_msg_send(void);

JTT808t_Err create_list_write_flash(JTT808t_Msg*,char* ,int,u8);

#endif
