#include "include.h"


//#define PASSIVE_TRANSMISSION_TIMEOUT    (30*60)      //�������͵ķְ���ʱʱ��  ��ʱ��ɾ���ڵ㣬Ϊ�����ְ��������
//
//                                               
//struct NODE HEAD={0};                               //����ͷ���
//int node_num=0;                                     //������Ϣ�����ܸ���
//int JTT808_MSG_WRITE_ADDR=0;                        //��Ϣ�洢��ʼ��ַ
//
//
//int get_repeat_timeout_time(int repeat_times)     //�����ش��������ش����ȷ���ش���ʱʱ�� ���õݹ����
//{
//    if(repeat_times<1)return 0;                                       //���ش�����Ϊ0�����״η���ʱ������0
//    if(repeat_times==1)return JTT808_Terminal_Param.tcp_timeout;      //���ش�����Ϊ1�����ڶ��η���ʱ������tcp��ʱʱ��
//    else return repeat_times*get_repeat_timeout_time(repeat_times-1); //���ش���������1ʱ���ݹ�
//}
//
//
//void JTT808_Ruerypackage_proc(char* body,int size)
//{
//    u16 first_package_seral_no;
//    u8 total;
//    struct NODE* node;
//    u16 ID;
//    first_package_seral_no=ntohs(*(u16*)body);     //��һ���ְ�����ˮ��
//    body+=2;
//    total=*body++;                                            //��Ҫ�ش��İ�����
//    for(node=&HEAD;node->next&&node->next->msg_serial_no!=first_package_seral_no;node=node->next); //�ҵ���һ���ְ���ˮ�Ŷ�Ӧ�Ľڵ�
//    if(!node->next)return;                                                                         //����ڵ�Ϊ���򷵻�
//    
//    while(total--)
//    {
//        ID=ntohs(*(u16*)body);     //��ȡ�ش�����ID
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
//*  ������Ϣ��ˮ�ź���ϢIDɾ���ڵ�
//*  serial_no: Ϊ-1��ʾ��Ϣ��ˮ�Ų����ã����ڵ���0������ʾ��Ϣ��ˮ�ſ���
//*  msg_id����ϢID
//*****************************************/
//
//JTT808t_Err jtt808_msg_node_del(u16 serial_no,u16 msg_id)//��Ϣ���ͳɹ���ɾ����Ӧ�Ĵ�����Ϣ�ڵ㣬�ú�������������Ϣ��ˮ��ɾ���ڵ�,���֪����ɾ�ڵ����ˮ����������д��ˮ��
//{
//    struct NODE *node,*node_tmp;
//    if(serial_no!=(u16)-1)for(node=&HEAD;node->next&&node->next->msg_serial_no!=serial_no;node=node->next);  //���֪��������Ϣ����ˮ�ţ����ҵ���Ӧ��ˮ�ŵĽڵ�
//    else if(msg_id)for(node=&HEAD;node->next&&node->next->msg_id!=msg_id;node=node->next);              //�����֪��������Ϣ����ˮ�ţ����ҵ���Ӧ����ϢID
//
//    if(node->next)
//    {
//        node_tmp=node->next;              //����ǰ�ڵ����һ���ڵ㱣����tmp��
//        node->next=node->next->next;      //��ǰ�ڵ����һ���ڵ㸳ֵΪ��ǰ�ڵ����һ���ڵ����һ���ڵ�
//        free(node_tmp);                   //�ͷ�tmp
//        node_num--;                       //������Ϣ����-1
//        printf("\r\n ɾ���ڵ㣺��Ϣ��ˮ��:0x%05x,��ϢID��0x%04x \r\n",serial_no,msg_id);
//        printf("\r\n �ڵ�������%d \r\n",node_num);
//        return JTT808_OK;
//    }
//    return JTT808_ERR;
//}
//
//
//JTT808t_Err create_list_write_flash(JTT808t_Msg* msg,char* msg_buf,int msg_buf_len,u8 prio) //���������������������Ϣ��flash
//{
//    struct NODE* node=malloc(sizeof(struct NODE));
//    struct NODE* node_tmp;
//
//    if(!node)
//    {
//        /*�ڴ����ʧ�ܣ�����*/
//        printf("\r\n �ڴ����ʧ�ܣ� \r\n");
//        printf("\r\n �ڵ�������%d \r\n",node_num);
//
//        NVIC_SystemReset();
//
//        return JTT808_ERR;
//    } 
//    
//  
//    if((JTT808_MSG_WRITE_ADDR+msg_buf_len)>=JTT808_MSG_END_ADDR)JTT808_MSG_WRITE_ADDR=JTT808_MSG_START_ADDR;//��flash�洢���µ�ǰ��Ϣ��ʱ���洢��ַ��ͷ��ʼ
//    
//
//
//    W25X_Write((u8*)msg_buf,JTT808_MSG_WRITE_ADDR,msg_buf_len); //��msg_buf ��msg_buf_len���ֽ�д��JTT808_MSG_WRITE_ADDR��
//    node->next=NULL;                                       //��һ���ڵ㸳ֵΪ��
//    node->flash_address=JTT808_MSG_WRITE_ADDR;             //��Ӧ����Ϣ��ŵ�ַ
//    node->send_time=0;                                     //��δ���ͣ�����ʱ�丳ֵ0
//    node->repeat_times=0;                                  //��δ���ͣ��ش�����Ϊ0
//    node->initiative_send=prio;                            //��ֵ�������ԣ�������Ϣ��������Ϣ
//    node->msg_id=msg->head.id;                             //��ϢID
//    node->msg_serial_no=msg->head.serial_no;               //��Ϣ��ˮ��
//    node->msg_size=msg_buf_len;                            //��Ϣ����
//    node->package_cnt=msg->head.package.cnt;               //�ְ�����
//    node->package_num=msg->head.package.num;               //�ְ����
//
//    for(node_tmp=&HEAD;node_tmp->next;node_tmp=node_tmp->next);//�ҵ���ǰ���������β
//    node_tmp->next=node;                                       //��δ����ӵ�ǰ�ڵ�
//    node_num++;                                                //������Ϣ����+1
//
//    printf("\r\n �����ڵ㣺��Ϣ��ˮ��:0x%05x,��ϢID:0x%04x�� \r\n",msg->head.serial_no,msg->head.id);
//    printf("\r\n �ڵ�������%d \r\n",node_num);
//
//    JTT808_MSG_WRITE_ADDR+=msg_buf_len;                        //flash��ַ��λmsg_buf_len����С
//
//    BKP_WriteBackupRegister(BKP_DR7, JTT808_MSG_WRITE_ADDR>>16);
//    BKP_WriteBackupRegister(BKP_DR8, JTT808_MSG_WRITE_ADDR&0xFFFF);     //��Ϣ�洢��ʼ��ַ
//
//    return JTT808_OK;
//}



/********************************************
* ����ĳһ������Ϣ
* �����������Ľڵ�
* ����ֵ����ʾ�Ƿ��ͳɹ�
**********************************************/

//JTT808t_Err send_initiative_data(struct NODE* node)
//{
//    char msg_buf[JTT808_MSG_MAX_SIZE]={0};
//    if(TimerSecond-node->next->send_time>=get_repeat_timeout_time(node->next->repeat_times))//�жϵ�ǰ��Ϣ�Ƿ�ʱ
//    {
//        node->next->send_time=TimerSecond;                                 //����ʱ�丳ֵΪ��ǰʱ��
//        node->next->repeat_times++;                                        //�ط�����+1
//        
//        W25X_Read((u8*)msg_buf,node->next->flash_address,node->next->msg_size); //����flash�е�����
//        JTT808_TXD((u8*)msg_buf,node->next->msg_size);                          //��������
//
//
//        return JTT808_OK;                                                  //����OK
//    }
//    else                                                                   //�����ʾ�����ش�����������δ��ʱ
//    {
//        if((node->next->repeat_times-1)>JTT808_Terminal_Param.tcp_repeat_times)//���ش�������������ʱ������
//        {
////            jtt808_msg_node_del(node->next->msg_serial_no,0);//ɾ����Ϣ
//            API_GPRS_Reset();                           //�Ͽ�����
//        }
//        return JTT808_ERR;
//    }
//}


//
//JTT808t_Err send_passive_data(struct NODE* node)
//{
//    char msg_buf[JTT808_MSG_MAX_SIZE]={0};
//    if(node->next->send_time==0)                                           //������Ϣ�״η���
//    {
//        W25X_Read((u8*)msg_buf,node->next->flash_address,node->next->msg_size); //��ȡ����
//        JTT808_TXD((u8*)msg_buf,node->next->msg_size);                          //��������
//        if(node->next->package_cnt)                                        //���������Ϣ�Ƿְ���Ϣ
//        {
//            node->next->send_time=TimerSecond;                             //�Է���ʱ�丳ֵ
//        }
//        else
//        {
////            jtt808_msg_node_del(node->next->msg_serial_no,0);              //���������Ϣ���Ƿְ���Ϣ����ɾ���ڵ�
//        }
//        return JTT808_OK;    
//    }
//    else
//    {
//        if(TimerSecond-node->next->send_time>=PASSIVE_TRANSMISSION_TIMEOUT)//������Ϣ���泬ʱ��ɾ���ڵ�
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
//    if(!API_get_gprs_state())return;                 //GPRSδ����     ����
//    if(API_get_send_buf_len())return;                //��������δ���� ����
//
//    if(check_sec==TimerSecond)return;                 //ÿ����ɨ��һ������
//    else check_sec=TimerSecond;
//
//    if(strcmp((const char*)JTT808_Terminal_Param.authorization_no,"00000000000000000000000000000000")==0)//û��ע�ᣬ�ҵ�������ע����ϢID������
//    {
//        for(node=&HEAD;node->next&&(node->next->msg_id!=JTT808_CMD_TERMINAL_REG);node=node->next);//������������ϢIDΪ0x0100��
//        if(node->next&&node->next->msg_id==JTT808_CMD_TERMINAL_REG)                               //�����ע����Ϣ����
//        {
//            send_initiative_data(node);
//        }
//        return;
//    }
//    else if(!JTT808_Flag_Authentication)//û�м�Ȩ���ҵ������м�Ȩ��ϢID������
//    {
//        for(node=&HEAD;node->next&&(node->next->msg_id!=JTT808_CMD_TERMINAL_AUTHENTICATION);node=node->next);//������ϢIDΪ0x0102��
//        if(node->next&&node->next->msg_id==JTT808_CMD_TERMINAL_AUTHENTICATION)                               //����м�Ȩ��Ϣ������
//        {
//            send_initiative_data(node);  
//        }
//        return;
//    }
//    else                                                  //��ʾע�ᡢ��Ȩ�ɹ�����ʼ����������Ϣ
//    {
//        int len=0;
//
//        for(node=&HEAD;node->next;node=node->next)        //ɨ��������������
//        {
//            if(node->next->initiative_send==1)            //������Ϣ
//            {
//                if(send_initiative_data(node)==JTT808_OK)
//                {
//                    len+=node->next->msg_size;            //���ͳɹ�����++
//                }
//            }
//            else                                          //������Ϣ��
//            {
//                if(send_passive_data(node)==JTT808_OK)
//                {
//                    len+=node->next->msg_size;            //���ͳɹ�����++
//                }
//            }
//            if(len>=500)break;
//        }
//    }   
//}






