#include "include.h"


//#define PASSIVE_TRANSMISSION_TIMEOUT    (30*60)      //被动发送的分包超时时间  超时后删除节点，为补传分包请求服务
//
//                                               
//struct NODE HEAD={0};                               //创建头结点
//int node_num=0;                                     //待发消息链表总个数
//int JTT808_MSG_WRITE_ADDR=0;                        //消息存储起始地址
//
//
//int get_repeat_timeout_time(int repeat_times)     //依据重传次数及重传间隔确定重传超时时间 采用递归调用
//{
//    if(repeat_times<1)return 0;                                       //当重传次数为0，即首次发送时，返回0
//    if(repeat_times==1)return JTT808_Terminal_Param.tcp_timeout;      //当重传次数为1，及第二次发送时，返回tcp超时时间
//    else return repeat_times*get_repeat_timeout_time(repeat_times-1); //当重传次数大于1时，递归
//}
//
//
//void JTT808_Ruerypackage_proc(char* body,int size)
//{
//    u16 first_package_seral_no;
//    u8 total;
//    struct NODE* node;
//    u16 ID;
//    first_package_seral_no=ntohs(*(u16*)body);     //第一条分包的流水号
//    body+=2;
//    total=*body++;                                            //需要重传的包总数
//    for(node=&HEAD;node->next&&node->next->msg_serial_no!=first_package_seral_no;node=node->next); //找到第一条分包流水号对应的节点
//    if(!node->next)return;                                                                         //如果节点为空则返回
//    
//    while(total--)
//    {
//        ID=ntohs(*(u16*)body);     //获取重传包的ID
//        body+=2;
//        for(;node->next&&node->next->package_num!=ID;node=node->next);
//        if(!node->next)return;
//        node->next->send_time=0;
//        node->next->repeat_times=0;
//    }
//}
//
//
//
///*****************************************
//*  依据消息流水号和消息ID删除节点
//*  serial_no: 为-1表示消息流水号不可用，大于等于0的数表示消息流水号可用
//*  msg_id：消息ID
//*****************************************/
//
//JTT808t_Err jtt808_msg_node_del(u16 serial_no,u16 msg_id)//消息发送成功，删除对应的待发消息节点，该函数优先依据消息流水号删除节点,如果知道待删节点的流水号请优先填写流水号
//{
//    struct NODE *node,*node_tmp;
//    if(serial_no!=(u16)-1)for(node=&HEAD;node->next&&node->next->msg_serial_no!=serial_no;node=node->next);  //如果知道发送消息的流水号，则找到对应流水号的节点
//    else if(msg_id)for(node=&HEAD;node->next&&node->next->msg_id!=msg_id;node=node->next);              //如果不知道发送消息的流水号，则找到对应的消息ID
//
//    if(node->next)
//    {
//        node_tmp=node->next;              //将当前节点的下一个节点保存在tmp中
//        node->next=node->next->next;      //当前节点的下一个节点赋值为当前节点的下一个节点的下一个节点
//        free(node_tmp);                   //释放tmp
//        node_num--;                       //待发消息总数-1
//        printf("\r\n 删除节点：消息流水号:0x%05x,消息ID：0x%04x \r\n",serial_no,msg_id);
//        printf("\r\n 节点总数：%d \r\n",node_num);
//        return JTT808_OK;
//    }
//    return JTT808_ERR;
//}
//
//
//JTT808t_Err create_list_write_flash(JTT808t_Msg* msg,char* msg_buf,int msg_buf_len,u8 prio) //创建待发链表，保存待发消息到flash
//{
//    struct NODE* node=malloc(sizeof(struct NODE));
//    struct NODE* node_tmp;
//
//    if(!node)
//    {
//        /*内存分配失败，重启*/
//        printf("\r\n 内存分配失败！ \r\n");
//        printf("\r\n 节点总数：%d \r\n",node_num);
//
//        NVIC_SystemReset();
//
//        return JTT808_ERR;
//    } 
//    
//  
//    if((JTT808_MSG_WRITE_ADDR+msg_buf_len)>=JTT808_MSG_END_ADDR)JTT808_MSG_WRITE_ADDR=JTT808_MSG_START_ADDR;//当flash存储不下当前消息包时，存储地址重头开始
//    
//
//
//    W25X_Write((u8*)msg_buf,JTT808_MSG_WRITE_ADDR,msg_buf_len); //将msg_buf 中msg_buf_len个字节写入JTT808_MSG_WRITE_ADDR中
//    node->next=NULL;                                       //下一个节点赋值为空
//    node->flash_address=JTT808_MSG_WRITE_ADDR;             //对应的消息存放地址
//    node->send_time=0;                                     //尚未发送，发送时间赋值0
//    node->repeat_times=0;                                  //尚未发送，重传次数为0
//    node->initiative_send=prio;                            //赋值发送属性，主发消息、被发消息
//    node->msg_id=msg->head.id;                             //消息ID
//    node->msg_serial_no=msg->head.serial_no;               //消息流水号
//    node->msg_size=msg_buf_len;                            //消息长度
//    node->package_cnt=msg->head.package.cnt;               //分包总数
//    node->package_num=msg->head.package.num;               //分包序号
//
//    for(node_tmp=&HEAD;node_tmp->next;node_tmp=node_tmp->next);//找到当前待发链表的尾
//    node_tmp->next=node;                                       //在未上添加当前节点
//    node_num++;                                                //待发消息总数+1
//
//    printf("\r\n 创建节点：消息流水号:0x%05x,消息ID:0x%04x！ \r\n",msg->head.serial_no,msg->head.id);
//    printf("\r\n 节点总数：%d \r\n",node_num);
//
//    JTT808_MSG_WRITE_ADDR+=msg_buf_len;                        //flash地址移位msg_buf_len个大小
//
//    BKP_WriteBackupRegister(BKP_DR7, JTT808_MSG_WRITE_ADDR>>16);
//    BKP_WriteBackupRegister(BKP_DR8, JTT808_MSG_WRITE_ADDR&0xFFFF);     //消息存储起始地址
//
//    return JTT808_OK;
//}



/********************************************
* 主发某一条的消息
* 参数：待发的节点
* 返回值：表示是否发送成功
**********************************************/

//JTT808t_Err send_initiative_data(struct NODE* node)
//{
//    char msg_buf[JTT808_MSG_MAX_SIZE]={0};
//    if(TimerSecond-node->next->send_time>=get_repeat_timeout_time(node->next->repeat_times))//判断当前消息是否超时
//    {
//        node->next->send_time=TimerSecond;                                 //发送时间赋值为当前时间
//        node->next->repeat_times++;                                        //重发次数+1
//        
//        W25X_Read((u8*)msg_buf,node->next->flash_address,node->next->msg_size); //读出flash中的数据
//        JTT808_TXD((u8*)msg_buf,node->next->msg_size);                          //发送数据
//
//
//        return JTT808_OK;                                                  //返回OK
//    }
//    else                                                                   //否则表示超过重传次数，或则还未超时
//    {
//        if((node->next->repeat_times-1)>JTT808_Terminal_Param.tcp_repeat_times)//当重传次数超过限制时。。。
//        {
////            jtt808_msg_node_del(node->next->msg_serial_no,0);//删除消息
//            API_GPRS_Reset();                           //断开连接
//        }
//        return JTT808_ERR;
//    }
//}


//
//JTT808t_Err send_passive_data(struct NODE* node)
//{
//    char msg_buf[JTT808_MSG_MAX_SIZE]={0};
//    if(node->next->send_time==0)                                           //被发消息首次发送
//    {
//        W25X_Read((u8*)msg_buf,node->next->flash_address,node->next->msg_size); //读取数据
//        JTT808_TXD((u8*)msg_buf,node->next->msg_size);                          //发送数据
//        if(node->next->package_cnt)                                        //如果被发消息是分包消息
//        {
//            node->next->send_time=TimerSecond;                             //对发送时间赋值
//        }
//        else
//        {
////            jtt808_msg_node_del(node->next->msg_serial_no,0);              //如果被发消息不是分包消息，则删除节点
//        }
//        return JTT808_OK;    
//    }
//    else
//    {
//        if(TimerSecond-node->next->send_time>=PASSIVE_TRANSMISSION_TIMEOUT)//被发消息保存超时，删除节点
//        {
////           jtt808_msg_node_del(node->next->msg_serial_no,0); 
//        }
//        return JTT808_ERR;
//    }
//}



//
//void JTT808_flash_msg_send(void)
//{
//    struct NODE* node;
////    char msg_buf[JTT808_MSG_MAX_SIZE]={0};
//    static u32 check_sec=0;
//
//    if(!API_get_gprs_state())return;                 //GPRS未连接     返回
//    if(API_get_send_buf_len())return;                //还有数据未发完 返回
//
//    if(check_sec==TimerSecond)return;                 //每秒钟扫描一次链表
//    else check_sec=TimerSecond;
//
//    if(strcmp((const char*)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000")==0)//没有注册，找到链表中注册消息ID并发送
//    {
//        for(node=&HEAD;node->next&&(node->next->msg_id!=JTT808_CMD_TERMINAL_REG);node=node->next);//查找链表中消息ID为0x0100的
//        if(node->next&&node->next->msg_id==JTT808_CMD_TERMINAL_REG)                               //如果有注册消息则发送
//        {
//            send_initiative_data(node);
//        }
//        return;
//    }
//    else if(!JTT808_Flag_Authentication)//没有鉴权，找到链表中鉴权消息ID并发送
//    {
//        for(node=&HEAD;node->next&&(node->next->msg_id!=JTT808_CMD_TERMINAL_AUTHENTICATION);node=node->next);//查找消息ID为0x0102的
//        if(node->next&&node->next->msg_id==JTT808_CMD_TERMINAL_AUTHENTICATION)                               //如果有鉴权消息，则发送
//        {
//            send_initiative_data(node);  
//        }
//        return;
//    }
//    else                                                  //表示注册、鉴权成功，开始发送其他消息
//    {
//        int len=0;
//
//        for(node=&HEAD;node->next;node=node->next)        //扫描链表，发送数据
//        {
//            if(node->next->initiative_send==1)            //主发消息
//            {
//                if(send_initiative_data(node)==JTT808_OK)
//                {
//                    len+=node->next->msg_size;            //发送成功长度++
//                }
//            }
//            else                                          //被发消息，
//            {
//                if(send_passive_data(node)==JTT808_OK)
//                {
//                    len+=node->next->msg_size;            //发送成功长度++
//                }
//            }
//            if(len>=500)break;
//        }
//    }   
//}






