#include "include.h"



void JTT808_Areacycleset(JTT808t_Msg* msg)              //0x8600	����Բ������
{
    u8 *pbody=msg->body;
    u8 type=*pbody++;
    u8 total=*pbody++;
    u8 *p_msg_buf=jtt808_global_buf;
    u8 *p_msg_buf_new=jtt808_global_buf_new;
    JTT808t_AreaCycle area={0};

	memset(jtt808_global_buf,0x00,sizeof(jtt808_global_buf));
	memset(jtt808_global_buf_new,0x00,sizeof(jtt808_global_buf_new));


    switch(type)
    {
        case 0:
            while(total--)
            {
                area.id=ntohl(*(u32*)pbody);                //����ID
                pbody+=4;
                memcpy((char*)&area.area_property,pbody,2); //��������
                *(u16*)&area.area_property=ntohs(*(u16*)&area.area_property);
                pbody+=2;
                area.latitude=ntohl(*(u32*)pbody);          //���ĵ�γ��
                pbody+=4;
                area.longitude=ntohl(*(u32*)pbody);         //���ĵ㾭��
                pbody+=4;
                area.radius=ntohl(*(u32*)pbody);            //�뾶
                pbody+=4;
                if(area.area_property.time)
                {
                    memcpy(area.start_time,pbody,6);        //��ʼʱ��
                    pbody+=6;
                    memcpy(area.end_time,pbody,6);          //����ʱ��
                    pbody+=6;
                }
                else
                {
                    memset(area.start_time,0x00,6);
                    memset(area.end_time,0x00,6);
                }

                if(area.area_property.limit_speed)
                {                                                  
                    area.top_speed=ntohs(*(u16*)pbody);     //����ٶ�
                    pbody+=2;
                    area.speeding_time=*pbody++;            //����ʱ��
                }
                else
                {
                    memset((char*)&area.top_speed,0x00,2);
                    memset((char*)&area.speeding_time,0x00,1);
                }
                memcpy(p_msg_buf_new,(char*)&area,sizeof(JTT808t_AreaCycle));
                p_msg_buf_new+=sizeof(JTT808t_AreaCycle);

                if(!area.radius||(p_msg_buf_new-jtt808_global_buf_new)>=4096)     //�������ݳ���
                {
                    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
                    return;
                }
            }

            if(msg->head.property.size!=(pbody-(u8*)msg->body))
            {
                JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
                return;
            }

            W25X_EraseSector(CYCLE_AREAR_ADDR/4096);  //����ҳ
            W25X_Write_NoCheck(jtt808_global_buf_new,CYCLE_AREAR_ADDR,4096);       //д������            
        break;

        case 1:
        case 2:
            W25X_Read(jtt808_global_buf,CYCLE_AREAR_ADDR,4096);              //��������ԭ������
            while((p_msg_buf-jtt808_global_buf)<=4096)                       //�ҳ����в�ƥ�������
            {
                int i=0;
                memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_AreaCycle));

                if(!area.id&&!area.latitude&&!area.longitude&&!area.radius)break;//ID�����ĵ�γ�ȡ����ȡ��뾶ȫ��Ϊ0
                if(area.id==0xFFFFFFFF&&area.latitude==0xFFFFFFFF&&area.longitude==0xFFFFFFFF&&area.radius==0xFFFFFFFF)break;//ȫ��ΪFF�����˳�
                
                for(i=0;i<total;i++)
                {
                    JTT808t_AreaCycle area_new={0};

                    area_new.id=ntohl(*(u32*)pbody);                //����ID
                    pbody+=4;
                    memcpy((char*)&area_new.area_property,pbody,2); //��������
                    *(u16*)&area_new.area_property=ntohs(*(u16*)&area_new.area_property);
                    pbody+=2;
                    area_new.latitude=ntohl(*(u32*)pbody);          //���ĵ�γ��
                    pbody+=4;
                    area_new.longitude=ntohl(*(u32*)pbody);         //���ĵ㾭��
                    pbody+=4;
                    area_new.radius=ntohl(*(u32*)pbody);            //�뾶
                    pbody+=4;

                    if(area_new.area_property.time)
                    {
                        memcpy(area_new.start_time,pbody,6);        //��ʼʱ��
                        pbody+=6;
                        memcpy(area_new.end_time,pbody,6);          //����ʱ��
                        pbody+=6;
                    }
                    if(area_new.area_property.limit_speed)
                    {                                                  
                        area_new.top_speed=ntohs(*(u16*)pbody);     //����ٶ�
                        pbody+=2;
                        area_new.speeding_time=*pbody++;            //����ʱ��
                    }

                    if(!area_new.radius)
                    {
                        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
                        return;
                    } 
                    if(area.id==area_new.id)break;   
                }
                pbody=msg->body;
                pbody+=2;

                if(i==total)                                           //��ƥ����
                {
                    memcpy(p_msg_buf_new,p_msg_buf,sizeof(JTT808t_AreaCycle));
                    p_msg_buf_new+=sizeof(JTT808t_AreaCycle);
                }
                p_msg_buf+=sizeof(JTT808t_AreaCycle);
            }

            pbody=msg->body;
            pbody+=2;

            while(total--)
            {
                area.id=ntohl(*(u32*)pbody);                //����ID
                pbody+=4;
                memcpy((char*)&area.area_property,pbody,2); //��������
                *(u16*)&area.area_property=ntohs(*(u16*)&area.area_property);
                pbody+=2;
                area.latitude=ntohl(*(u32*)pbody);          //���ĵ�γ��
                pbody+=4;
                area.longitude=ntohl(*(u32*)pbody);         //���ĵ㾭��
                pbody+=4;
                area.radius=ntohl(*(u32*)pbody);            //�뾶
                pbody+=4;
                if(area.area_property.time)
                {
                    memcpy(area.start_time,pbody,6);        //��ʼʱ��
                    pbody+=6;
                    memcpy(area.end_time,pbody,6);          //����ʱ��
                    pbody+=6;
                }
                else
                {
                    memset(area.start_time,0x00,6);
                    memset(area.end_time,0x00,6);
                }

                if(area.area_property.limit_speed)
                {                                                  
                    area.top_speed=ntohs(*(u16*)pbody);     //����ٶ�
                    pbody+=2;
                    area.speeding_time=*pbody++;            //����ʱ��
                }
                else
                {
                    memset((char*)&area.top_speed,0x00,2);
                    memset((char*)&area.speeding_time,0x00,1);
                }

                memcpy(p_msg_buf_new,(char*)&area,sizeof(JTT808t_AreaCycle));
                p_msg_buf_new+=sizeof(JTT808t_AreaCycle);

                if(p_msg_buf_new-jtt808_global_buf_new>=4096)
                {
                    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_FAIL); //ʧ��
                    return;
                }
            }
            
            if(msg->head.property.size!=(pbody-(u8*)msg->body))
            {
                JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
                return;
            }

            W25X_EraseSector(CYCLE_AREAR_ADDR/4096);  //����ҳ
            W25X_Write_NoCheck(jtt808_global_buf_new,CYCLE_AREAR_ADDR,4096);      //д������            
        break;

        default:
            JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
            return;
    }
    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
}


void JTT808_Areacycledel(JTT808t_Msg* msg)              //0x8601	ɾ��Բ������
{
    u8 *pbody=msg->body;
    u8 total=*pbody++;
    u8 *p_msg_buf=jtt808_global_buf;
    u8 *p_msg_buf_new=jtt808_global_buf_new;
    JTT808t_AreaCycle area={0};

	memset(jtt808_global_buf,0x00,sizeof(jtt808_global_buf));
	memset(jtt808_global_buf_new,0x00,sizeof(jtt808_global_buf_new));

    if(msg->head.property.size!=(1+total*4))
    {
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
        return;
    }

    if(total==0)
    {
        W25X_EraseSector(CYCLE_AREAR_ADDR/4096);  //����ҳ
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
        return;
    }

    W25X_Read(jtt808_global_buf,CYCLE_AREAR_ADDR,4096);              //��������ԭ������

    while(p_msg_buf-jtt808_global_buf<4096)                          //ȥ������IDƥ����
    {
        int i=0;

        memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_AreaCycle));

        if(!area.id&&!area.latitude&&!area.longitude&&!area.radius)break;//ID�����ĵ�γ�ȡ����ȡ��뾶ȫ��Ϊ0
        if(area.id==0xFFFFFFFF&&area.latitude==0xFFFFFFFF&&area.longitude==0xFFFFFFFF&&area.radius==0xFFFFFFFF)break;//ȫ��ΪFF�����˳�
        
        for(i=0;i<total;i++)
        {
            u32 ID=ntohl(*(u32*)pbody);
            pbody+=4;
            if(ID==area.id)break;
        }
        pbody=msg->body;
        pbody++;

        if(i==total)               //��IDƥ����
        {
            memcpy(p_msg_buf_new,p_msg_buf,sizeof(JTT808t_AreaCycle));
            p_msg_buf_new+=sizeof(JTT808t_AreaCycle);
        }

        p_msg_buf+=sizeof(JTT808t_AreaCycle);
    }

    W25X_EraseSector(CYCLE_AREAR_ADDR/4096);  //����ҳ
    W25X_Write_NoCheck(jtt808_global_buf_new,CYCLE_AREAR_ADDR,4096);     //д������            

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
}


void JTT808_AreacycleCheck(void)                         //Բ������ɨ��   ����ҪGPS�ж�λ��ɨ��
{
    JTT808t_AreaCycle area={0};
    u32 addr=CYCLE_AREAR_ADDR;

    while(addr-CYCLE_AREAR_ADDR<4096)
    {
        u8 is_in_tim=0;        //�Ƿ���ʱ�����
        u8 is_in_area=0;       //�Ƿ���Χ���ڲ�
        
        W25X_Read((u8*)&area,addr,sizeof(JTT808t_AreaCycle));
        addr+=sizeof(JTT808t_AreaCycle);

        if(!area.id||!area.radius)break;//ID�����ĵ�γ�ȡ����ȡ��뾶ȫ��Ϊ0
        if(area.id==0xFFFFFFFF&&area.latitude==0xFFFFFFFF&&area.longitude==0xFFFFFFFF&&area.radius==0xFFFFFFFF)break;//ȫ��ΪFF�����˳�
    

        /********************�ж��Ƿ���ʱ�����************************/
        if(area.area_property.time)     //�������ʱ�� ,�ж��Ƿ���ʱ�����
        {
            u8 start_tim[7]={0},end_tim[7]={0},gps_tim[7]={0};
            u8 i,start_len=0,end_len=0;          //ǰ���м����ֽ�Ϊ0
            
            memcpy(start_tim,area.start_time,  6);
            memcpy(end_tim,  area.end_time,    6);
            memcpy(gps_tim,  gps_location.bj_tim.bcd_tim,6);

            for(i=0;i<6;i++)
            {
                if(start_tim[i]==0x00)start_len++;
                if(start_tim[i])break;
            }
            for(i=0;i<6;i++)
            {
                if(end_tim[i]==0x00)end_len++;
                if(end_tim[i])break;
            }

            i=start_len<=end_len?start_len:end_len; //�ҳ�ǰ��Ϊ0��С����

            memcpy(start_tim,&start_tim[i],6-i);    //�ҳ���Чʱ��
            memcpy(end_tim,  &end_tim[i],  6-i);
            memcpy(gps_tim,  &gps_tim[i],  6-i);

            start_tim[6-i]=0;
            end_tim[6-i]=0;
            gps_tim[6-i]=0;

            if(strcmp((const char*)gps_tim,(const char*)start_tim)>0&&strcmp((const char*)gps_tim,(const char*)end_tim)<0)is_in_tim=1; //������ʼʱ�䣬С�ڽ���ʱ��
            else is_in_tim=0; 
        }
        else                         //���������ʱ�� ������Զ��ʱ�����
        {
            is_in_tim=1;
        }
        if(is_in_tim==0)continue;        //�������ʱ����� �����к�������


        /********************�ж��Ƿ���Χ����***********************/
        if(two_point_distance(gps_location.latitude, gps_location.latitude_type,
                              gps_location.longitude,gps_location.longitude_type,
                              area.latitude, area.area_property.latitude_type,
                              area.longitude,area.area_property.longitude_type)   <=area.radius)//��Χ���ڲ�
        {
            is_in_area=1;
        }
        else                                                                                    //��Χ���ⲿ
        {
            is_in_area=0;
        }


        /********************����Χ�������жϱ���***************************/
        if(area.area_property.in2driver&&is_in_area==1)   //�����򱨾�����ʻԱ
        {
//            static u32 TTS_Area=0;

//            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//            {
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);

//                TTS_Area=TimerSecond;
//				strcat(TTS,"���ѽ���Բ������");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//                API_Play_TTS(TTS);
//            }
        }
        if(area.area_property.in2platform&&is_in_area==1) //�����򱨾���ƽ̨
        {
            JTT808_Location_Alarm.across_area=1;
        }

        if(area.area_property.out2driver&&is_in_area==0)  //�����򱨾�����ʻԱ
        {
//		    static u32 TTS_Area=0;

//            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//            {
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);				

//                TTS_Area=TimerSecond;
//				strcat(TTS,"���ѳ�Բ������");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//                API_Play_TTS(TTS);
//            }
        }
        if(area.area_property.out2platform&&is_in_area==0)//�����򱨾���ƽ̨
        {
            JTT808_Location_Alarm.across_area=1;
        }

        if(area.area_property.doorcontrol&&is_in_area==1) //��ֹ����
        {
            JTT808_Forbidden_opendoor();
        }
        if(area.area_property.in2community&&is_in_area==1)//������ر�GPRSͨ��
        {
            JTT808_Gprs_Disconnect();
        }
        if(area.area_property.in2GNSSdate&&is_in_area==1) //������ɼ���ϸGNSS����
        {
            JTT808_GNSS_Sample();
        }

        /******************�ж��Ƿ���************************/
        if(area.area_property.limit_speed&&is_in_area==1)  //��������ٶ�
        {
            if((area.top_speed)&&((gps_location.speed/10)>area.top_speed))       //����
            {
//                static u32 TTS_Area=0;

                JTT808_Location_Alarm.speeding=1;          //���ٱ���

//	            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//	            {
//									char TTS[50]={0},tmp[10]={0};
//									u32 id=htonl(area.id);

//	                TTS_Area=TimerSecond;
//									strcat(TTS,"������Բ������");
//									strncpy(tmp,(char*)&id,4);
//									strcat(TTS,tmp);
//									strcat(TTS,"�ڳ���");
//	                API_Play_TTS(TTS);
//	            }
            }
        }
    }
}


void JTT808_Arearectangleset(JTT808t_Msg* msg)          //0x8602	���þ�������
{
    u8 *pbody=msg->body;
    u8 type=*pbody++;
    u8 total=*pbody++;
    u8 *p_msg_buf=jtt808_global_buf;
    u8 *p_msg_buf_new=jtt808_global_buf_new;
    JTT808t_AreaRectangle area={0};

	memset(jtt808_global_buf,0x00,sizeof(jtt808_global_buf));
	memset(jtt808_global_buf_new,0x00,sizeof(jtt808_global_buf_new));

    switch(type)
    {
        case 0:
            while(total--)
            {
                area.id=ntohl(*(u32*)pbody);                //����ID
                pbody+=4;
                memcpy((char*)&area.area_property,pbody,2); //��������
                *(u16*)&area.area_property=ntohs(*(u16*)&area.area_property);
                pbody+=2;
                area.upleft_latitude=ntohl(*(u32*)pbody);   //���ϵ�γ��
                pbody+=4;
                area.upleft_longitude=ntohl(*(u32*)pbody);  //���ϵ㾭��
                pbody+=4;
                area.lowright_latitude=ntohl(*(u32*)pbody); //���µ�γ��
                pbody+=4;
                area.lowright_longitude=ntohl(*(u32*)pbody);//���µ㾭��
                pbody+=4;
                if(area.area_property.time)
                {
                    memcpy(area.start_time,pbody,6);        //��ʼʱ��
                    pbody+=6;
                    memcpy(area.end_time,pbody,6);          //����ʱ��
                    pbody+=6;
                }
                else
                {
                    memset(area.start_time,0x00,6);
                    memset(area.end_time,0x00,6);
                }

                if(area.area_property.limit_speed)
                {                                                  
                    area.top_speed=ntohs(*(u16*)pbody);     //����ٶ�
                    pbody+=2;
                    area.speeding_time=*pbody++;            //����ʱ��
                }
                else
                {
                    memset((char*)&area.top_speed,0x00,2);
                    memset((char*)&area.speeding_time,0x00,1);
                }

                memcpy(p_msg_buf_new,(char*)&area,sizeof(JTT808t_AreaRectangle));
                p_msg_buf_new+=sizeof(JTT808t_AreaRectangle);

                if((!area.id&&!area.upleft_latitude&&!area.upleft_longitude&&!area.lowright_latitude&&!area.lowright_longitude)||(p_msg_buf_new-jtt808_global_buf_new)>=4096)     //�������ݳ���
                {
                    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
                    return;
                }
            }

            if(msg->head.property.size!=(pbody-(u8*)msg->body))
            {
                JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
                return;
            }

            W25X_EraseSector(RECTANGULAR_AREAR_ADDR/4096);  //����ҳ
            W25X_Write_NoCheck(jtt808_global_buf_new,RECTANGULAR_AREAR_ADDR,4096);   //д������          
        break;

        case 1:
        case 2:
            W25X_Read(jtt808_global_buf,RECTANGULAR_AREAR_ADDR,4096);        //��������ԭ������
            while(p_msg_buf-jtt808_global_buf<4096)                          //�ҳ����в�ƥ�������
            {
                int i=0;
                memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_AreaRectangle));

                if(!area.id&&!area.upleft_latitude&&!area.upleft_longitude&&!area.lowright_latitude&&!area.lowright_longitude)break;//ID�����ϵ�γ�ȡ����ȡ����µ�γ�ȡ�����ȫ��Ϊ0
                if(area.id==0xFFFFFFFF&&area.upleft_latitude==0xFFFFFFFF&&area.upleft_longitude==0xFFFFFFFF&&area.lowright_latitude==0xFFFFFFFF&&area.lowright_longitude==0xFFFFFFFF)break;//ȫ��ΪFF�����˳�
                
                for(i=0;i<total;i++)
                {
                    JTT808t_AreaRectangle area_new={0};

                    area_new.id=ntohl(*(u32*)pbody);                //����ID
                    pbody+=4;
                    memcpy((char*)&area_new.area_property,pbody,2); //��������
                    *(u16*)&area_new.area_property=ntohs(*(u16*)&area_new.area_property);
                    pbody+=2;
                    area_new.upleft_latitude=ntohl(*(u32*)pbody);   //���ϵ�γ��
                    pbody+=4;
                    area_new.upleft_longitude=ntohl(*(u32*)pbody);  //���ϵ㾭��
                    pbody+=4;
                    area_new.lowright_latitude=ntohl(*(u32*)pbody); //���µ�γ��
                    pbody+=4;
                    area_new.lowright_longitude=ntohl(*(u32*)pbody);//���µ㾭��
                    pbody+=4;

                    if(area_new.area_property.time)
                    {
                        memcpy(area_new.start_time,pbody,6);        //��ʼʱ��
                        pbody+=6;
                        memcpy(area_new.end_time,pbody,6);          //����ʱ��
                        pbody+=6;
                    }
                    if(area_new.area_property.limit_speed)
                    {                                                  
                        area_new.top_speed=ntohs(*(u16*)pbody);     //����ٶ�
                        pbody+=2;
                        area_new.speeding_time=*pbody++;            //����ʱ��
                    } 

                    if(!area_new.id&&!area_new.upleft_latitude&&!area_new.upleft_longitude&&!area_new.lowright_latitude&&!area_new.lowright_longitude)
                    {
                        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
                        return;
                    }

                    if(area.id==area_new.id)break;   
                }
                pbody=msg->body;
                pbody+=2;

                if(i==total)                                           //��ƥ����
                {
                    memcpy(p_msg_buf_new,p_msg_buf,sizeof(JTT808t_AreaRectangle));
                    p_msg_buf_new+=sizeof(JTT808t_AreaRectangle);
                }
                p_msg_buf+=sizeof(JTT808t_AreaRectangle);
            }

            pbody=msg->body;
            pbody+=2;

            while(total--)
            {
                area.id=ntohl(*(u32*)pbody);                //����ID
                pbody+=4;
                memcpy((char*)&area.area_property,pbody,2); //��������
                *(u16*)&area.area_property=ntohs(*(u16*)&area.area_property);
                pbody+=2;
                area.upleft_latitude=ntohl(*(u32*)pbody);   //���ϵ�γ��
                pbody+=4;
                area.upleft_longitude=ntohl(*(u32*)pbody);  //���ϵ㾭��
                pbody+=4;
                area.lowright_latitude=ntohl(*(u32*)pbody); //���µ�γ��
                pbody+=4;
                area.lowright_longitude=ntohl(*(u32*)pbody);//���µ㾭��
                pbody+=4;
                if(area.area_property.time)
                {
                    memcpy(area.start_time,pbody,6);        //��ʼʱ��
                    pbody+=6;
                    memcpy(area.end_time,pbody,6);          //����ʱ��
                    pbody+=6;
                }
                else
                {
                    memset(area.start_time,0x00,6);
                    memset(area.end_time,0x00,6);
                }

                if(area.area_property.limit_speed)
                {                                                  
                    area.top_speed=ntohs(*(u16*)pbody);     //����ٶ�
                    pbody+=2;
                    area.speeding_time=*pbody++;            //����ʱ��
                }
                else
                {
                    memset((char*)&area.top_speed,0x00,2);
                    memset((char*)&area.speeding_time,0x00,1);
                }
                memcpy(p_msg_buf_new,(char*)&area,sizeof(JTT808t_AreaRectangle));
                p_msg_buf_new+=sizeof(JTT808t_AreaRectangle);

                if(p_msg_buf_new-jtt808_global_buf_new>=4096)
                {
                    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_FAIL); //ʧ��
                    return;
                }
            }

            if(msg->head.property.size!=(pbody-(u8*)msg->body))
            {
                JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
                return;
            }

            W25X_EraseSector(RECTANGULAR_AREAR_ADDR/4096);  //����ҳ
            W25X_Write_NoCheck(jtt808_global_buf_new,RECTANGULAR_AREAR_ADDR,4096);      //д������             
        break;

        default:
            JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
            return;
    }
    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
}


void JTT808_Arearectangledel(JTT808t_Msg* msg)          //0x8603	ɾ����������
{
    u8 *pbody=msg->body;
    u8 total=*pbody++;
    u8 *p_msg_buf=jtt808_global_buf;
    u8 *p_msg_buf_new=jtt808_global_buf_new;
    JTT808t_AreaRectangle area={0};

	memset(jtt808_global_buf,0x00,sizeof(jtt808_global_buf));
	memset(jtt808_global_buf_new,0x00,sizeof(jtt808_global_buf_new));

    if(msg->head.property.size!=(1+total*4))
    {
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
        return;
    }

    if(total==0)
    {
        W25X_EraseSector(RECTANGULAR_AREAR_ADDR/4096);  //����ҳ
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
        return;
    }

    W25X_Read(jtt808_global_buf,RECTANGULAR_AREAR_ADDR,4096);              //��������ԭ������

    while(p_msg_buf-jtt808_global_buf<4096)                                //ȥ������IDƥ����
    {
        int i=0;

        memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_AreaRectangle));

        if(!area.id&&!area.upleft_latitude&&!area.upleft_longitude&&!area.lowright_latitude&&!area.lowright_longitude)break;//ID�����ϵ�γ�ȡ����ȡ����µ�γ�ȡ�����ȫ��Ϊ0
        if(area.id==0xFFFFFFFF&&area.upleft_latitude==0xFFFFFFFF&&area.upleft_longitude==0xFFFFFFFF&&area.lowright_latitude==0xFFFFFFFF&&area.lowright_longitude==0xFFFFFFFF)break;//ȫ��ΪFF�����˳�
                
        for(i=0;i<total;i++)
        {
            u32 ID=ntohl(*(u32*)pbody);
            pbody+=4;
            if(ID==area.id)break;
        }
        pbody=msg->body;
        pbody++;

        if(i==total)               //��IDƥ����
        {
            memcpy(p_msg_buf_new,p_msg_buf,sizeof(JTT808t_AreaRectangle));
            p_msg_buf_new+=sizeof(JTT808t_AreaRectangle);
        }

        p_msg_buf+=sizeof(JTT808t_AreaRectangle);
    }

    W25X_EraseSector(RECTANGULAR_AREAR_ADDR/4096);  //����ҳ
    W25X_Write_NoCheck(jtt808_global_buf_new,RECTANGULAR_AREAR_ADDR,4096);    //д������            

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
}


void JTT808_ArearectangleCheck(void)
{
    JTT808t_AreaRectangle area={0};
    u32 addr=RECTANGULAR_AREAR_ADDR;

    while(addr-RECTANGULAR_AREAR_ADDR<4096)
    {
        u8 is_in_tim=0;        //�Ƿ���ʱ�����
        u8 is_in_area=0;       //�Ƿ���Χ���ڲ�
        
        W25X_Read((u8*)&area,addr,sizeof(JTT808t_AreaRectangle));
        addr+=sizeof(JTT808t_AreaRectangle);

        if(!area.id)break;      //IDΪ0
        if(area.id==0xFFFFFFFF&&area.upleft_latitude==0xFFFFFFFF&&area.upleft_longitude==0xFFFFFFFF&&area.lowright_latitude==0xFFFFFFFF&&area.lowright_longitude==0xFFFFFFFF)break;//ȫ��ΪFF�����˳�
               

        /********************�ж��Ƿ���ʱ�����************************/
        if(area.area_property.time)     //�������ʱ�� ,�ж��Ƿ���ʱ�����
        {
            u8 start_tim[7]={0},end_tim[7]={0},gps_tim[7]={0};
            u8 i,start_len=0,end_len=0;          //ǰ���м����ֽ�Ϊ0
            
            memcpy(start_tim,area.start_time,  6);
            memcpy(end_tim,  area.end_time,    6);
            memcpy(gps_tim,  gps_location.bj_tim.bcd_tim,6);

            for(i=0;i<6;i++)
            {
                if(start_tim[i]==0x00)start_len++;
                if(start_tim[i])break;
            }
            for(i=0;i<6;i++)
            {
                if(end_tim[i]==0x00)end_len++;
                if(end_tim[i])break;
            }

            i=start_len<=end_len?start_len:end_len; //�ҳ�ǰ��Ϊ0��С����

            memcpy(start_tim,&start_tim[i],6-i);    //�ҳ���Чʱ��
            memcpy(end_tim,  &end_tim[i],  6-i);
            memcpy(gps_tim,  &gps_tim[i],  6-i);

            start_tim[6-i]=0;
            end_tim[6-i]=0;
            gps_tim[6-i]=0;

            if(strcmp((const char*)gps_tim,(const char*)start_tim)>0&&strcmp((const char*)gps_tim,(const char*)end_tim)<0)is_in_tim=1; //������ʼʱ�䣬С�ڽ���ʱ��
            else is_in_tim=0; 
        }
        else                         //���������ʱ�� ������Զ��ʱ�����
        {
            is_in_tim=1;
        }
        if(is_in_tim==0)continue;        //�������ʱ����� �����к�������


        /********************�ж��Ƿ���Χ����***********************/
        if(gps_location.latitude  <= area.upleft_latitude    &&    //��ǰ��γ��С�����ϵ�γ��
           gps_location.latitude  >= area.lowright_latitude  &&    //��ǰ��γ�ȴ������µ�γ��
           gps_location.longitude <= area.lowright_longitude &&    //��ǰ�㾭��С�����µ㾭��
           gps_location.longitude >= area.upleft_longitude)        //��ǰ�㾭�ȴ������ϵ㾭��   //��Χ���ڲ�
        {
            is_in_area=1;
        }
        else                                                                                    //��Χ���ⲿ
        {
            is_in_area=0;
        }


        /********************����Χ�������жϱ���***************************/
        if(area.area_property.in2driver&&is_in_area==1)   //�����򱨾�����ʻԱ
        {
//			static u32 TTS_Area=0;

//            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//            {
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);

//                TTS_Area=TimerSecond;
//				strcat(TTS,"���ѽ����������");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//                API_Play_TTS(TTS);
//            }
        }
        if(area.area_property.in2platform&&is_in_area==1) //�����򱨾���ƽ̨
        {
            JTT808_Location_Alarm.across_area=1;
        }
        if(area.area_property.out2driver&&is_in_area==0)  //�����򱨾�����ʻԱ
        {
//			static u32 TTS_Area=0;

//            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//            {
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);

//                TTS_Area=TimerSecond;
//				strcat(TTS,"���ѳ���������");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//                API_Play_TTS(TTS);
//            }
        }
        if(area.area_property.out2platform&&is_in_area==0)//�����򱨾���ƽ̨
        {
            JTT808_Location_Alarm.across_area=1;
        }

        if(area.area_property.doorcontrol&&is_in_area==1) //��ֹ����
        {
            JTT808_Forbidden_opendoor();
        }
        if(area.area_property.in2community&&is_in_area==1)//������ر�GPRSͨ��
        {
            JTT808_Gprs_Disconnect();
        }
        if(area.area_property.in2GNSSdate&&is_in_area==1) //������ɼ���ϸGNSS����
        {
            JTT808_GNSS_Sample();
        }


        /******************�ж��Ƿ���************************/
        if(area.area_property.limit_speed&&is_in_area==1)  //��������ٶ�
        {
            if((area.top_speed)&&((gps_location.speed/10)>area.top_speed))       //����
            {
//				static u32 TTS_Area=0;

				JTT808_Location_Alarm.speeding=1;          //���ٱ���

//	            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//	            {
//					char TTS[50]={0},tmp[10]={0};
//					u32 id=htonl(area.id);

//	                TTS_Area=TimerSecond;
//					strcat(TTS,"�����ھ�������");
//					strncpy(tmp,(char*)&id,4);
//					strcat(TTS,tmp);
//					strcat(TTS,"�ڳ���");
//	                API_Play_TTS(TTS);
//	            } 
            }
        }  
    }
}


void JTT808_Areapolygonset(JTT808t_Msg* msg)            //0x8604	���ö��������
{
    u8 *pbody=msg->body;
    u8 *p_msg_buf=jtt808_global_buf;
    u8 *p_msg_buf_new=jtt808_global_buf_new;
    JTT808t_AreaPolygonSet area={0};

	memset(jtt808_global_buf,0x00,sizeof(jtt808_global_buf));
	memset(jtt808_global_buf_new,0x00,sizeof(jtt808_global_buf_new));

    W25X_Read(jtt808_global_buf,POLYGON_AREAR_ADDR,4096);              //��������ԭ������
    

    while(p_msg_buf-jtt808_global_buf<4096)                            //ȡ�����в�ͬID������
    {
        u32 ID=ntohl(*(u32*)pbody);

        memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_AreaPolygonSet));
         
        if(!area.id&&!area.peak_cnt)break;                  //ID������������Ϊ0
        if(area.id==0xffffffff&&area.peak_cnt==0xffff)break;//ȫΪff  
        
        if(ID==area.id)
        {
            p_msg_buf+=sizeof(JTT808t_AreaPolygonSet);
            p_msg_buf+=area.peak_cnt*8;
            continue;
        }

        memcpy(p_msg_buf_new,p_msg_buf,sizeof(JTT808t_AreaPolygonSet));
        p_msg_buf_new+=sizeof(JTT808t_AreaPolygonSet);
        p_msg_buf+=sizeof(JTT808t_AreaPolygonSet);

        memcpy(p_msg_buf_new,p_msg_buf,area.peak_cnt*8);
        p_msg_buf_new+=area.peak_cnt*8;
        p_msg_buf+=area.peak_cnt*8;
    }

    area.id=ntohl(*(u32*)pbody);                //����ID
    pbody+=4;
    memcpy((char*)&area.area_property,pbody,2); //��������
    *(u16*)&area.area_property=ntohs(*(u16*)&area.area_property);
    pbody+=2;

    if(area.area_property.time)
    {
        memcpy(area.start_time,pbody,6);        //��ʼʱ��
        pbody+=6;
        memcpy(area.end_time,pbody,6);          //����ʱ��
        pbody+=6;
    }
    else
    {
        memset(area.start_time,0x00,6);
        memset(area.end_time,0x00,6);
    }

    if(area.area_property.limit_speed)
    {                                                  
        area.top_speed=ntohs(*(u16*)pbody);     //����ٶ�
        pbody+=2;
        area.speeding_time=*pbody++;            //����ʱ��
    }
    else
    {
        memset((char*)&area.top_speed,0x00,2);
        memset((char*)&area.speeding_time,0x00,1);
    }
    area.peak_cnt=ntohs(*(u16*)pbody);          //��������
    pbody+=2;

    if(area.peak_cnt<3)
    {
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
        return;
    }

    memcpy(p_msg_buf_new,(char*)&area,sizeof(JTT808t_AreaPolygonSet));
    p_msg_buf_new+=sizeof(JTT808t_AreaPolygonSet);
    p_msg_buf+=sizeof(JTT808t_AreaPolygonSet);

    while(area.peak_cnt--)
    {
        JTT808t_AreaPolygonItem item={0};

        item.latitude=ntohl(*(u32*)pbody);
        pbody+=4;
        item.longitude=ntohl(*(u32*)pbody);
        pbody+=4;

        memcpy(p_msg_buf_new,(char*)&item,sizeof(JTT808t_AreaPolygonItem));
        p_msg_buf_new+=sizeof(JTT808t_AreaPolygonItem);
    }

    if(msg->head.property.size!=(pbody-(u8*)msg->body))
    {
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
        return;
    }

    W25X_EraseSector(POLYGON_AREAR_ADDR/4096);  //����ҳ
    W25X_Write_NoCheck(jtt808_global_buf_new,POLYGON_AREAR_ADDR,4096);
}


void JTT808_Areapolygondel(JTT808t_Msg* msg)            //0x8605	ɾ�����������
{
    u8 *pbody=msg->body;
    u8 total=*pbody++;
    u8 *p_msg_buf=jtt808_global_buf;
    u8 *p_msg_buf_new=jtt808_global_buf_new;
    JTT808t_AreaPolygonSet area={0};

	memset(jtt808_global_buf,0x00,sizeof(jtt808_global_buf));
	memset(jtt808_global_buf_new,0x00,sizeof(jtt808_global_buf_new));

    if(msg->head.property.size!=(1+total*4))
    {
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
        return;
    }

    if(total==0)
    {
        W25X_EraseSector(POLYGON_AREAR_ADDR/4096);  //����ҳ
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
        return;
    }

    W25X_Read(jtt808_global_buf,POLYGON_AREAR_ADDR,4096);              //��������ԭ������

    while(p_msg_buf-jtt808_global_buf<4096)
    {
        int i=0;
        memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_AreaPolygonSet));
         
        if(!area.id&&!area.peak_cnt)break;                  //ID������������Ϊ0
        if(area.id==0xffffffff&&area.peak_cnt==0xffff)break; //ȫΪff

        for(i=0;i<total;i++)
        {
            u32 ID=ntohl(*(u32*)pbody);
            pbody+=4;
            if(ID==area.id)break;
        }
        pbody=msg->body;
        pbody++;

        if(i==total)                                        //��ƥ����
        {
            memcpy(p_msg_buf_new,(char*)&area,sizeof(JTT808t_AreaPolygonSet));
            p_msg_buf_new+=sizeof(JTT808t_AreaPolygonSet);
            p_msg_buf+=sizeof(JTT808t_AreaPolygonSet);
        
            memcpy(p_msg_buf_new,p_msg_buf,area.peak_cnt*8);
            p_msg_buf_new+=area.peak_cnt*8;
            p_msg_buf+=area.peak_cnt*8;
        }
        else
        {
            p_msg_buf+=sizeof(JTT808t_AreaPolygonSet);
            p_msg_buf+=area.peak_cnt*8;
        }

    }
    W25X_EraseSector(POLYGON_AREAR_ADDR/4096);  //����ҳ
    W25X_Write_NoCheck(jtt808_global_buf_new,POLYGON_AREAR_ADDR,4096);

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
}


void JTT808_AreapolygonCheck(void)
{
    JTT808t_AreaPolygonSet area={0};
    u32 addr=POLYGON_AREAR_ADDR;

    while(addr-POLYGON_AREAR_ADDR<4096)
    {
        u8 is_in_tim=0;        //�Ƿ���ʱ�����
        u8 is_in_area=0;       //�Ƿ���Χ���ڲ�
        POINT poly[50]={0},q={0};
        int i=0;
        
        W25X_Read((u8*)&area,addr,sizeof(JTT808t_AreaPolygonSet));
        addr+=sizeof(JTT808t_AreaPolygonSet);

        if(!area.id||area.peak_cnt<3)break;                  //ID������������Ϊ0
        if(area.id==0xffffffff&&area.peak_cnt==0xffff)break; //ȫΪff


        /********************�ж��Ƿ���ʱ�����************************/
        if(area.area_property.time)     //�������ʱ�� ,�ж��Ƿ���ʱ�����
        {
            u8 start_tim[7]={0},end_tim[7]={0},gps_tim[7]={0};
            u8 i,start_len=0,end_len=0;          //ǰ���м����ֽ�Ϊ0
            
            memcpy(start_tim,area.start_time,  6);
            memcpy(end_tim,  area.end_time,    6);
            memcpy(gps_tim,  gps_location.bj_tim.bcd_tim,6);

            for(i=0;i<6;i++)
            {
                if(start_tim[i]==0x00)start_len++;
                if(start_tim[i])break;
            }
            for(i=0;i<6;i++)
            {
                if(end_tim[i]==0x00)end_len++;
                if(end_tim[i])break;
            }

            i=start_len<=end_len?start_len:end_len; //�ҳ�ǰ��Ϊ0��С����

            memcpy(start_tim,&start_tim[i],6-i);    //�ҳ���Чʱ��
            memcpy(end_tim,  &end_tim[i],  6-i);
            memcpy(gps_tim,  &gps_tim[i],  6-i);

            start_tim[6-i]=0;
            end_tim[6-i]=0;
            gps_tim[6-i]=0;

            if(strcmp((const char*)gps_tim,(const char*)start_tim)>0&&strcmp((const char*)gps_tim,(const char*)end_tim)<0)is_in_tim=1; //������ʼʱ�䣬С�ڽ���ʱ��
            else is_in_tim=0; 
        }
        else                         //���������ʱ�� ������Զ��ʱ�����
        {
            is_in_tim=1;
        }
        if(is_in_tim==0)             //�������ʱ����� �����к�������
        {
            addr+=area.peak_cnt*8;
            continue;    
        }


        /********************�ж��Ƿ���Χ����***********************/
        q.x=gps_location.latitude;
        q.y=gps_location.longitude;      //�Ե�ǰ�㸳ֵ

        for(i=0;i<area.peak_cnt;i++)
        {
            u8 buf[8]={0};

            W25X_Read(buf,addr,8);
            addr+=8;

            poly[i].x=*(u32*)&buf[0];
            poly[i].y=*(u32*)&buf[4];
        }                               //�Զ��㸳ֵ
        
        if(is_inside(poly,area.peak_cnt,q))               //��Χ���ڲ�
        {
            is_in_area=1;
        }
        else                                              //��Χ���ⲿ
        {
            is_in_area=0;
        }


        /********************����Χ�������жϱ���***************************/
        if(area.area_property.in2driver&&is_in_area==1)   //�����򱨾�����ʻԱ
        {
//			static u32 TTS_Area=0;

//            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//            {
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);

//                TTS_Area=TimerSecond;
//				strcat(TTS,"���ѽ�����������");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//                API_Play_TTS(TTS);
//            } 
        }
        if(area.area_property.in2platform&&is_in_area==1) //�����򱨾���ƽ̨
        {
            JTT808_Location_Alarm.across_area=1;
        }
        if(area.area_property.out2driver&&is_in_area==0)  //�����򱨾�����ʻԱ
        {
//			static u32 TTS_Area=0;

//            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//            {
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);

//                TTS_Area=TimerSecond;
//				strcat(TTS,"���ѳ����������");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//                API_Play_TTS(TTS);
//            } 
        }
        if(area.area_property.out2platform&&is_in_area==0)//�����򱨾���ƽ̨
        {
            JTT808_Location_Alarm.across_area=1;
        }

        if(area.area_property.doorcontrol&&is_in_area==1) //��ֹ����
        {
            JTT808_Forbidden_opendoor();
        }
        if(area.area_property.in2community&&is_in_area==1)//������ر�GPRSͨ��
        {
            JTT808_Gprs_Disconnect();
        }
        if(area.area_property.in2GNSSdate&&is_in_area==1) //������ɼ���ϸGNSS����
        {
            JTT808_GNSS_Sample();
        }


        /******************�ж��Ƿ���************************/
        if(area.area_property.limit_speed&&is_in_area==1)  //��������ٶ�
        {
            if((area.top_speed)&&((gps_location.speed/10)>area.top_speed))       //����
            {
//				static u32 TTS_Area=0;

				JTT808_Location_Alarm.speeding=1;          //���ٱ���

//	            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//	            {
//					char TTS[50]={0},tmp[10]={0};
//					u32 id=htonl(area.id);

//	                TTS_Area=TimerSecond;
//					strcat(TTS,"�����ڶ��������");
//					strncpy(tmp,(char*)&id,4);
//					strcat(TTS,tmp);
//					strcat(TTS,"�ڳ���");
//	                API_Play_TTS(TTS);
//	            }  
            }
        }
    }
}


void JTT808_Routeset(JTT808t_Msg* msg)                  //0x8606	����·��
{
    u8 *pbody=msg->body;
    u8 *p_msg_buf=jtt808_global_buf;
    u8 *p_msg_buf_new=jtt808_global_buf_new;
    JTT808t_RouteSet area={0};

	memset(jtt808_global_buf,0x00,sizeof(jtt808_global_buf));
	memset(jtt808_global_buf_new,0x00,sizeof(jtt808_global_buf_new));

    W25X_Read(jtt808_global_buf,ROUTE_AREAR_ADDR,4096);              //��������ԭ������

    while(p_msg_buf-jtt808_global_buf<4096)
    {
        u32 ID=ntohl(*(u32*)pbody);

        memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_RouteSet));
         
        if(!area.id&&!area.turn_point_cnt)break;                  //ID������������Ϊ0
        if(area.id==0xffffffff&&area.turn_point_cnt==0xffff)break; //ȫΪff  
        
        if(ID==area.id)
        {
            p_msg_buf+=sizeof(JTT808t_RouteSet);
            p_msg_buf+=area.turn_point_cnt*sizeof(JTT808t_RouteTurnPoint);
            continue;
        }

        memcpy(p_msg_buf_new,p_msg_buf,sizeof(JTT808t_RouteSet));
        p_msg_buf_new+=sizeof(JTT808t_RouteSet);
        p_msg_buf+=sizeof(JTT808t_RouteSet);

        memcpy(p_msg_buf_new,p_msg_buf,area.turn_point_cnt*sizeof(JTT808t_RouteTurnPoint));
        p_msg_buf_new+=area.turn_point_cnt*sizeof(JTT808t_RouteTurnPoint);
        p_msg_buf+=area.turn_point_cnt*sizeof(JTT808t_RouteTurnPoint);    
    }

    area.id=ntohl(*(u32*)pbody);                //����ID
    pbody+=4;
    memcpy((char*)&area.property,pbody,2);      //��������
    *(u16*)&area.property=ntohs(*(u16*)&area.property);
    pbody+=2;

    if(area.property.depend_time)
    {
        memcpy(area.start_time,pbody,6);        //��ʼʱ��
        pbody+=6;
        memcpy(area.end_time,pbody,6);          //����ʱ��
        pbody+=6;
    }
    else
    {
        memset(area.start_time,0x00,6);
        memset(area.end_time,0x00,6);
    }

    area.turn_point_cnt=ntohs(*(u16*)pbody);    //�յ�����
    pbody+=2;

    if(area.turn_point_cnt<2)
    {
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
        return;
    }

    memcpy(p_msg_buf_new,(char*)&area,sizeof(JTT808t_RouteSet));
    p_msg_buf_new+=sizeof(JTT808t_RouteSet);
    p_msg_buf+=sizeof(JTT808t_RouteSet);

    while(area.turn_point_cnt--)
    {
        JTT808t_RouteTurnPoint item={0};

        item.turn_point_id=ntohl(*(u32*)pbody);
        pbody+=4;
        item.section_id=ntohl(*(u32*)pbody);
        pbody+=4;
        item.turn_point_latitude=ntohl(*(u32*)pbody);
        pbody+=4;
        item.turn_point_longitude=ntohl(*(u32*)pbody);
        pbody+=4;
        item.section_width=*pbody++;
        *(u8*)&item.section_property=*pbody++;

        if(item.section_property.drive_time)
        {
            item.section_length_threshold=ntohs(*(u16*)pbody);
            pbody+=2;
            item.section_inadequate_threshold=ntohs(*(u16*)pbody);
            pbody+=2;
        }
        else
        {
            memset((char*)&item.section_length_threshold,0x00,2);
            memset((char*)&item.section_inadequate_threshold,0x00,2);
        }

        if(item.section_property.limit_speed)
        {
            item.section_top_speed=ntohs(*(u16*)pbody);
            pbody+=2;
            item.section_speeding_time=*pbody++;
        }
        else
        {
            memset((char*)&item.section_top_speed,0x00,2);
            memset((char*)&item.section_speeding_time,0x00,1);
        }

		//����903��ĿҪ����ӣ��ǲ���Э��
		{
			memcpy(item.start_time,pbody,6);        //��ʼʱ��
	        pbody+=6;
	        memcpy(item.end_time,pbody,6);          //����ʱ��
	        pbody+=6;
		}

        if(!item.section_width)
        {
            JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
            return;
        }

        memcpy(p_msg_buf_new,(char*)&item,sizeof(JTT808t_RouteTurnPoint));
        p_msg_buf_new+=sizeof(JTT808t_RouteTurnPoint);
    }

    if(msg->head.property.size!=(pbody-(u8*)msg->body))
    {
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
        return;
    }

    W25X_EraseSector(ROUTE_AREAR_ADDR/4096);  //����ҳ
    W25X_Write_NoCheck(jtt808_global_buf_new,ROUTE_AREAR_ADDR,4096);
}


void JTT808_Routedel(JTT808t_Msg* msg)                  //0x8607	ɾ��·��
{
    u8 *pbody=msg->body;
    u8 total=*pbody++;
    u8 *p_msg_buf=jtt808_global_buf;
    u8 *p_msg_buf_new=jtt808_global_buf_new;
    JTT808t_RouteSet area={0};

	memset(jtt808_global_buf,0x00,sizeof(jtt808_global_buf));
	memset(jtt808_global_buf_new,0x00,sizeof(jtt808_global_buf_new));

    if(msg->head.property.size!=(1+total*4))
    {
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //��Ϣ����
        return;
    }

    if(total==0)
    {
        W25X_EraseSector(ROUTE_AREAR_ADDR/4096);  //����ҳ
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
        return;
    }

    W25X_Read(jtt808_global_buf,ROUTE_AREAR_ADDR,4096);              //��������ԭ������

    while(p_msg_buf-jtt808_global_buf<4096)
    {
        int i=0;

        memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_RouteSet));
         
        if(!area.id&&!area.turn_point_cnt)break;                  //ID���յ�������Ϊ0
        if(area.id==0xffffffff&&area.turn_point_cnt==0xffff)break; //ȫΪff

        for(i=0;i<total;i++)
        {
            u32 ID=ntohl(*(u32*)pbody);
            pbody+=4;
            if(ID==area.id)break;
        }
        pbody=msg->body;
        pbody++;

        if(i==total)                                        //��ƥ����
        {
            memcpy(p_msg_buf_new,(char*)&area,sizeof(JTT808t_RouteSet));
            p_msg_buf_new+=sizeof(JTT808t_RouteSet);
            p_msg_buf+=sizeof(JTT808t_RouteSet);
        
            memcpy(p_msg_buf_new,p_msg_buf,area.turn_point_cnt*sizeof(JTT808t_RouteTurnPoint));
            p_msg_buf_new+=area.turn_point_cnt*sizeof(JTT808t_RouteTurnPoint);
            p_msg_buf+=area.turn_point_cnt*sizeof(JTT808t_RouteTurnPoint);
        }
        else
        {
            p_msg_buf+=sizeof(JTT808t_RouteSet);
            p_msg_buf+=area.turn_point_cnt*sizeof(JTT808t_RouteTurnPoint);
        }

    }
    W25X_EraseSector(ROUTE_AREAR_ADDR/4096);  //����ҳ
    W25X_Write_NoCheck(jtt808_global_buf_new,ROUTE_AREAR_ADDR,4096);

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //�����ն�ͨ��Ӧ��
}


void JTT808_RouteCheck(void)
{
    JTT808t_RouteSet area={0};
    JTT808t_RouteTurnPoint in_item={0};                    //��·���ڵĹյ�
    static u32 start_sec=0;                                //·�ο�ʼʱ��
    static JTT808t_RouteTurnPoint last_item={0};           //��һ��·��

    u32 addr=ROUTE_AREAR_ADDR;


    while(addr-ROUTE_AREAR_ADDR<4096)
    {
        u8 is_in_tim=0;        //�Ƿ���ʱ�����
        u8 is_in_area=0;       //�Ƿ���Χ���ڲ�
        
        
        W25X_Read((u8*)&area,addr,sizeof(JTT808t_RouteSet));
        addr+=sizeof(JTT808t_RouteSet);

        if(!area.id||area.turn_point_cnt<2)break;                  //ID������������Ϊ0
        if(area.id==0xffffffff&&area.turn_point_cnt==0xffff)break; //ȫΪff


        /********************�ж��Ƿ���ʱ�����************************/
        if(area.property.depend_time)                              //�������ʱ�� ,�ж��Ƿ���ʱ�����
        {
            u8 start_tim[7]={0},end_tim[7]={0},gps_tim[7]={0};
            u8 i,start_len=0,end_len=0;          //ǰ���м����ֽ�Ϊ0
            
            memcpy(start_tim,area.start_time,  6);
            memcpy(end_tim,  area.end_time,    6);
            memcpy(gps_tim,  gps_location.bj_tim.bcd_tim,6);

            for(i=0;i<6;i++)
            {
                if(start_tim[i]==0x00)start_len++;
                if(start_tim[i])break;
            }
            for(i=0;i<6;i++)
            {
                if(end_tim[i]==0x00)end_len++;
                if(end_tim[i])break;
            }

            i=start_len<=end_len?start_len:end_len; //�ҳ�ǰ��Ϊ0��С����

            memcpy(start_tim,&start_tim[i],6-i);    //�ҳ���Чʱ��
            memcpy(end_tim,  &end_tim[i],  6-i);
            memcpy(gps_tim,  &gps_tim[i],  6-i);

            start_tim[6-i]=0;
            end_tim[6-i]=0;
            gps_tim[6-i]=0;

            if(strcmp((const char*)gps_tim,(const char*)start_tim)>0&&strcmp((const char*)gps_tim,(const char*)end_tim)<0)is_in_tim=1; //������ʼʱ�䣬С�ڽ���ʱ��
            else is_in_tim=0;    
        }
        else                         //���������ʱ�� ������Զ��ʱ�����
        {
            is_in_tim=1;
        }
        if(is_in_tim==0)             //�������ʱ����� �����к�������
        {
            addr+=area.turn_point_cnt*sizeof(JTT808t_RouteTurnPoint);
            continue;    
        }


        /********************�ж��Ƿ���Χ����***********************/
        while(--area.turn_point_cnt)
        {
            JTT808t_RouteTurnPoint item1={0},item2={0};

            W25X_Read((u8*)&item1,addr,sizeof(JTT808t_RouteTurnPoint));
            addr+=sizeof(JTT808t_RouteTurnPoint);
            W25X_Read((u8*)&item2,addr,sizeof(JTT808t_RouteTurnPoint));

            if(is_in_line(&item1,&item2))
            {
                is_in_area=1;
                memcpy((char*)&in_item,(char*)&item1,sizeof(JTT808t_RouteTurnPoint));
                addr+=area.turn_point_cnt*sizeof(JTT808t_RouteTurnPoint);
                break;
            }
        }
        if(!is_in_area)addr+=sizeof(JTT808t_RouteTurnPoint);


        /********************����Χ�������жϱ���***************************/
        if(is_in_area)              //��·����
        {
            if((in_item.turn_point_id!=last_item.turn_point_id)||(in_item.section_id!=last_item.section_id))   //��ǰ·�κ���һ�ε�·�β���ͬ
            {
                 if((last_item.section_property.drive_time)&&(TimerSecond-start_sec<last_item.section_inadequate_threshold))     //��ʻʱ�䲻��
                 {
//				 	static u32 TTS_Area=0;

                    JTT808_Location_Alarm.section_drive_time_err=1; 
//					if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//		            {
//						char TTS[50]={0},tmp[10]={0};
//						u32 id=htonl(area.id);

//		                TTS_Area=TimerSecond;
//						strcat(TTS,"����·��");
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,tmp);
//						strcat(TTS,"��");
//						id=htonl(last_item.section_id);
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,"·������ʻʱ�䲻��");
//		                API_Play_TTS(TTS);
//		            } 
                 }
                 start_sec=TimerSecond;                                                    //�Ե�ǰ·�ο�ʼʱ�丳ֵ
                 memcpy((char*)&last_item,(char*)&in_item,sizeof(JTT808t_RouteTurnPoint)); //����һ��·�θ�ֵ
            }

            if((in_item.section_property.drive_time)&&(TimerSecond-start_sec>in_item.section_length_threshold))              //��ʻʱ�����
            {
//				static u32 TTS_Area=0;

                JTT808_Location_Alarm.section_drive_time_err=1;
//				if(timerSecondSub(TimerSecond,TTS_Area)>=60)
	            {
//					char TTS[50]={0},tmp[10]={0};
//					u32 id=htonl(area.id);

//	                TTS_Area=TimerSecond;
//					strcat(TTS,"����·��");
//					strncpy(tmp,(char*)&id,4);
//					strcat(TTS,tmp);
//					strcat(TTS,"��");
//					id=htonl(in_item.section_id);
//					strncpy(tmp,(char*)&id,4);
//					strcat(TTS,"·������ʻʱ�����");
//	                API_Play_TTS(TTS);
	            } 
            }

            if(in_item.section_property.limit_speed)            //�������
            {
                if((in_item.section_top_speed)&&((gps_location.speed/10)>in_item.section_top_speed))       //����
                {
//					static u32 TTS_Area=0;

					JTT808_Location_Alarm.speeding=1;          //���ٱ���

//		            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
		            {
//						char TTS[50]={0},tmp[10]={0};
//						u32 id=htonl(area.id);

//		                TTS_Area=TimerSecond;
//						strcat(TTS,"������·��");
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,tmp);
//						strcat(TTS,"��");
//						id=htonl(in_item.section_id);
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,"·���ڳ���");
//		                API_Play_TTS(TTS);
		            }   
                }
            }

			//��������903��ĿҪ����ӣ��ǲ���Э��
//			{
//				u8 start_tim[7]={0},end_tim[7]={0},gps_tim[7]={0};
//				u8 i,start_len=0,end_len=0;          //ǰ���м����ֽ�Ϊ0
//				
//				memcpy(start_tim,in_item.start_time,  6);
//				memcpy(end_tim,  in_item.end_time,    6);
//				memcpy(gps_tim,  gps_location.bj_tim.bcd_tim,6);
//				
//				for(i=0;i<6;i++)
//				{
//				    if(start_tim[i]==0x00)start_len++;
//				    if(start_tim[i])break;
//				}
//				for(i=0;i<6;i++)
//				{
//				    if(end_tim[i]==0x00)end_len++;
//				    if(end_tim[i])break;
//				}
//				
//				i=start_len<=end_len?start_len:end_len; //�ҳ�ǰ��Ϊ0��С����
//				
//				memcpy(start_tim,&start_tim[i],6-i);    //�ҳ���Чʱ��
//				memcpy(end_tim,  &end_tim[i],  6-i);
//				memcpy(gps_tim,  &gps_tim[i],  6-i);
//				
//				start_tim[6-i]=0;
//				end_tim[6-i]=0;
//				gps_tim[6-i]=0;
				
				
//				if(strcmp((const char*)gps_tim,(const char*)start_tim)<0)//С����ʼʱ��  ��ǰ����ڵ�
//				{
//					static u32 TTS_Area=0;

//		            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//		            {
//						char TTS[50]={0},tmp[10]={0};
//						u32 id=htonl(area.id);

//		                TTS_Area=TimerSecond;
//						strcat(TTS,"������ǰ����·��");
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,"��");
//						id=htonl(in_item.section_id);
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,"·��");
//						strcat(TTS,tmp);
//		                API_Play_TTS(TTS);
//		            }   
//				}
//				if(strcmp((const char*)gps_tim,(const char*)end_tim)>0)  //���ڽ���ʱ��  ����ʱ�仹δ����ڵ�
//				{
//					static u32 TTS_Area=0;

//		            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//		            {
//						char TTS[50]={0},tmp[10]={0};
//						u32 id=htonl(area.id);

//		                TTS_Area=TimerSecond;
//						strcat(TTS,"���ѳ�ʱδ��·��");
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,"��");
//						id=htonl(in_item.section_id);
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,"·��");
//						strcat(TTS,tmp);
//		                API_Play_TTS(TTS);
//		            }   
//				}
//				if(strcmp((const char*)gps_tim,(const char*)start_tim)>0&&strcmp((const char*)gps_tim,(const char*)end_tim)<0)//������ʼʱ�䣬С�ڽ���ʱ�� ��������
//				{
//				}
//			}
        }



        if(area.property.in2driver&&is_in_area==1)   //�����򱨾�����ʻԱ
        {
//			static u32 TTS_Area=0;
//			
//			if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//			{
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);
//				
//			    TTS_Area=TimerSecond;
//				strcat(TTS,"���ѽ���·��");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//			    API_Play_TTS(TTS);
//			} 
        }
        if(area.property.in2platform&&is_in_area==1) //�����򱨾���ƽ̨
        {
            JTT808_Location_Alarm.across_route=1;
        }
        if(area.property.out2driver&&is_in_area==0)  //�����򱨾�����ʻԱ
        {
//			static u32 TTS_Area=0;
//			
//			if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//			{
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);
//				
//			    TTS_Area=TimerSecond;
//				strcat(TTS,"���ѳ�·��");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//			    API_Play_TTS(TTS);
//			}
        }
        if(area.property.out2platform&&is_in_area==0)//�����򱨾���ƽ̨
        {
            JTT808_Location_Alarm.across_route=1;
        }
        if(is_in_area==0)
        {
            JTT808_Location_Alarm.route_out=1;       //ƫ��·��
        }
    }
}



void JTT808_Area_Alarm_check(void)
{
    if(!JTT808_Location_State.is_location)return;  //δ��λ�˳�
    if(!JTT808_Location_State.acc)return;		   //ACC�ر��˳�

    JTT808_AreacycleCheck();
    JTT808_ArearectangleCheck();
    JTT808_AreapolygonCheck();
    JTT808_RouteCheck();
}



