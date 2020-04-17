#include "include.h"



void JTT808_Areacycleset(JTT808t_Msg* msg)              //0x8600	设置圆形区域
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
                area.id=ntohl(*(u32*)pbody);                //区域ID
                pbody+=4;
                memcpy((char*)&area.area_property,pbody,2); //区域属性
                *(u16*)&area.area_property=ntohs(*(u16*)&area.area_property);
                pbody+=2;
                area.latitude=ntohl(*(u32*)pbody);          //中心点纬度
                pbody+=4;
                area.longitude=ntohl(*(u32*)pbody);         //中心点经度
                pbody+=4;
                area.radius=ntohl(*(u32*)pbody);            //半径
                pbody+=4;
                if(area.area_property.time)
                {
                    memcpy(area.start_time,pbody,6);        //起始时间
                    pbody+=6;
                    memcpy(area.end_time,pbody,6);          //结束时间
                    pbody+=6;
                }
                else
                {
                    memset(area.start_time,0x00,6);
                    memset(area.end_time,0x00,6);
                }

                if(area.area_property.limit_speed)
                {                                                  
                    area.top_speed=ntohs(*(u16*)pbody);     //最高速度
                    pbody+=2;
                    area.speeding_time=*pbody++;            //持续时间
                }
                else
                {
                    memset((char*)&area.top_speed,0x00,2);
                    memset((char*)&area.speeding_time,0x00,1);
                }
                memcpy(p_msg_buf_new,(char*)&area,sizeof(JTT808t_AreaCycle));
                p_msg_buf_new+=sizeof(JTT808t_AreaCycle);

                if(!area.radius||(p_msg_buf_new-jtt808_global_buf_new)>=4096)     //接收数据出错
                {
                    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
                    return;
                }
            }

            if(msg->head.property.size!=(pbody-(u8*)msg->body))
            {
                JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
                return;
            }

            W25X_EraseSector(CYCLE_AREAR_ADDR/4096);  //擦除页
            W25X_Write_NoCheck(jtt808_global_buf_new,CYCLE_AREAR_ADDR,4096);       //写入区域            
        break;

        case 1:
        case 2:
            W25X_Read(jtt808_global_buf,CYCLE_AREAR_ADDR,4096);              //读出所有原有区域
            while((p_msg_buf-jtt808_global_buf)<=4096)                       //找出所有不匹配的区域
            {
                int i=0;
                memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_AreaCycle));

                if(!area.id&&!area.latitude&&!area.longitude&&!area.radius)break;//ID、中心点纬度、经度、半径全部为0
                if(area.id==0xFFFFFFFF&&area.latitude==0xFFFFFFFF&&area.longitude==0xFFFFFFFF&&area.radius==0xFFFFFFFF)break;//全部为FF，则退出
                
                for(i=0;i<total;i++)
                {
                    JTT808t_AreaCycle area_new={0};

                    area_new.id=ntohl(*(u32*)pbody);                //区域ID
                    pbody+=4;
                    memcpy((char*)&area_new.area_property,pbody,2); //区域属性
                    *(u16*)&area_new.area_property=ntohs(*(u16*)&area_new.area_property);
                    pbody+=2;
                    area_new.latitude=ntohl(*(u32*)pbody);          //中心点纬度
                    pbody+=4;
                    area_new.longitude=ntohl(*(u32*)pbody);         //中心点经度
                    pbody+=4;
                    area_new.radius=ntohl(*(u32*)pbody);            //半径
                    pbody+=4;

                    if(area_new.area_property.time)
                    {
                        memcpy(area_new.start_time,pbody,6);        //起始时间
                        pbody+=6;
                        memcpy(area_new.end_time,pbody,6);          //结束时间
                        pbody+=6;
                    }
                    if(area_new.area_property.limit_speed)
                    {                                                  
                        area_new.top_speed=ntohs(*(u16*)pbody);     //最高速度
                        pbody+=2;
                        area_new.speeding_time=*pbody++;            //持续时间
                    }

                    if(!area_new.radius)
                    {
                        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
                        return;
                    } 
                    if(area.id==area_new.id)break;   
                }
                pbody=msg->body;
                pbody+=2;

                if(i==total)                                           //无匹配项
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
                area.id=ntohl(*(u32*)pbody);                //区域ID
                pbody+=4;
                memcpy((char*)&area.area_property,pbody,2); //区域属性
                *(u16*)&area.area_property=ntohs(*(u16*)&area.area_property);
                pbody+=2;
                area.latitude=ntohl(*(u32*)pbody);          //中心点纬度
                pbody+=4;
                area.longitude=ntohl(*(u32*)pbody);         //中心点经度
                pbody+=4;
                area.radius=ntohl(*(u32*)pbody);            //半径
                pbody+=4;
                if(area.area_property.time)
                {
                    memcpy(area.start_time,pbody,6);        //起始时间
                    pbody+=6;
                    memcpy(area.end_time,pbody,6);          //结束时间
                    pbody+=6;
                }
                else
                {
                    memset(area.start_time,0x00,6);
                    memset(area.end_time,0x00,6);
                }

                if(area.area_property.limit_speed)
                {                                                  
                    area.top_speed=ntohs(*(u16*)pbody);     //最高速度
                    pbody+=2;
                    area.speeding_time=*pbody++;            //持续时间
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
                    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_FAIL); //失败
                    return;
                }
            }
            
            if(msg->head.property.size!=(pbody-(u8*)msg->body))
            {
                JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
                return;
            }

            W25X_EraseSector(CYCLE_AREAR_ADDR/4096);  //擦除页
            W25X_Write_NoCheck(jtt808_global_buf_new,CYCLE_AREAR_ADDR,4096);      //写入区域            
        break;

        default:
            JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
            return;
    }
    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //返回终端通用应答
}


void JTT808_Areacycledel(JTT808t_Msg* msg)              //0x8601	删除圆形区域
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
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
        return;
    }

    if(total==0)
    {
        W25X_EraseSector(CYCLE_AREAR_ADDR/4096);  //擦除页
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //返回终端通用应答
        return;
    }

    W25X_Read(jtt808_global_buf,CYCLE_AREAR_ADDR,4096);              //读出所有原有区域

    while(p_msg_buf-jtt808_global_buf<4096)                          //去掉所有ID匹配项
    {
        int i=0;

        memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_AreaCycle));

        if(!area.id&&!area.latitude&&!area.longitude&&!area.radius)break;//ID、中心点纬度、经度、半径全部为0
        if(area.id==0xFFFFFFFF&&area.latitude==0xFFFFFFFF&&area.longitude==0xFFFFFFFF&&area.radius==0xFFFFFFFF)break;//全部为FF，则退出
        
        for(i=0;i<total;i++)
        {
            u32 ID=ntohl(*(u32*)pbody);
            pbody+=4;
            if(ID==area.id)break;
        }
        pbody=msg->body;
        pbody++;

        if(i==total)               //无ID匹配项
        {
            memcpy(p_msg_buf_new,p_msg_buf,sizeof(JTT808t_AreaCycle));
            p_msg_buf_new+=sizeof(JTT808t_AreaCycle);
        }

        p_msg_buf+=sizeof(JTT808t_AreaCycle);
    }

    W25X_EraseSector(CYCLE_AREAR_ADDR/4096);  //擦除页
    W25X_Write_NoCheck(jtt808_global_buf_new,CYCLE_AREAR_ADDR,4096);     //写入区域            

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //返回终端通用应答
}


void JTT808_AreacycleCheck(void)                         //圆形区域扫描   必须要GPS有定位才扫描
{
    JTT808t_AreaCycle area={0};
    u32 addr=CYCLE_AREAR_ADDR;

    while(addr-CYCLE_AREAR_ADDR<4096)
    {
        u8 is_in_tim=0;        //是否在时间段内
        u8 is_in_area=0;       //是否在围栏内部
        
        W25X_Read((u8*)&area,addr,sizeof(JTT808t_AreaCycle));
        addr+=sizeof(JTT808t_AreaCycle);

        if(!area.id||!area.radius)break;//ID、中心点纬度、经度、半径全部为0
        if(area.id==0xFFFFFFFF&&area.latitude==0xFFFFFFFF&&area.longitude==0xFFFFFFFF&&area.radius==0xFFFFFFFF)break;//全部为FF，则退出
    

        /********************判断是否在时间段内************************/
        if(area.area_property.time)     //如果依据时间 ,判断是否在时间段内
        {
            u8 start_tim[7]={0},end_tim[7]={0},gps_tim[7]={0};
            u8 i,start_len=0,end_len=0;          //前面有几个字节为0
            
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

            i=start_len<=end_len?start_len:end_len; //找出前面为0最小长度

            memcpy(start_tim,&start_tim[i],6-i);    //找出有效时间
            memcpy(end_tim,  &end_tim[i],  6-i);
            memcpy(gps_tim,  &gps_tim[i],  6-i);

            start_tim[6-i]=0;
            end_tim[6-i]=0;
            gps_tim[6-i]=0;

            if(strcmp((const char*)gps_tim,(const char*)start_tim)>0&&strcmp((const char*)gps_tim,(const char*)end_tim)<0)is_in_tim=1; //大于起始时间，小于结束时间
            else is_in_tim=0; 
        }
        else                         //如果不依据时间 ，则永远在时间段内
        {
            is_in_tim=1;
        }
        if(is_in_tim==0)continue;        //如果不在时间段内 不进行后续操作


        /********************判断是否在围栏内***********************/
        if(two_point_distance(gps_location.latitude, gps_location.latitude_type,
                              gps_location.longitude,gps_location.longitude_type,
                              area.latitude, area.area_property.latitude_type,
                              area.longitude,area.area_property.longitude_type)   <=area.radius)//在围栏内部
        {
            is_in_area=1;
        }
        else                                                                                    //在围栏外部
        {
            is_in_area=0;
        }


        /********************依据围栏属性判断报警***************************/
        if(area.area_property.in2driver&&is_in_area==1)   //进区域报警给驾驶员
        {
//            static u32 TTS_Area=0;

//            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//            {
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);

//                TTS_Area=TimerSecond;
//				strcat(TTS,"您已进入圆形区域");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//                API_Play_TTS(TTS);
//            }
        }
        if(area.area_property.in2platform&&is_in_area==1) //进区域报警给平台
        {
            JTT808_Location_Alarm.across_area=1;
        }

        if(area.area_property.out2driver&&is_in_area==0)  //出区域报警给驾驶员
        {
//		    static u32 TTS_Area=0;

//            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//            {
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);				

//                TTS_Area=TimerSecond;
//				strcat(TTS,"您已出圆形区域");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//                API_Play_TTS(TTS);
//            }
        }
        if(area.area_property.out2platform&&is_in_area==0)//出区域报警给平台
        {
            JTT808_Location_Alarm.across_area=1;
        }

        if(area.area_property.doorcontrol&&is_in_area==1) //禁止开门
        {
            JTT808_Forbidden_opendoor();
        }
        if(area.area_property.in2community&&is_in_area==1)//进区域关闭GPRS通信
        {
            JTT808_Gprs_Disconnect();
        }
        if(area.area_property.in2GNSSdate&&is_in_area==1) //进区域采集详细GNSS数据
        {
            JTT808_GNSS_Sample();
        }

        /******************判断是否超速************************/
        if(area.area_property.limit_speed&&is_in_area==1)  //如果依据速度
        {
            if((area.top_speed)&&((gps_location.speed/10)>area.top_speed))       //超速
            {
//                static u32 TTS_Area=0;

                JTT808_Location_Alarm.speeding=1;          //超速报警

//	            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//	            {
//									char TTS[50]={0},tmp[10]={0};
//									u32 id=htonl(area.id);

//	                TTS_Area=TimerSecond;
//									strcat(TTS,"您已在圆形区域");
//									strncpy(tmp,(char*)&id,4);
//									strcat(TTS,tmp);
//									strcat(TTS,"内超速");
//	                API_Play_TTS(TTS);
//	            }
            }
        }
    }
}


void JTT808_Arearectangleset(JTT808t_Msg* msg)          //0x8602	设置矩形区域
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
                area.id=ntohl(*(u32*)pbody);                //区域ID
                pbody+=4;
                memcpy((char*)&area.area_property,pbody,2); //区域属性
                *(u16*)&area.area_property=ntohs(*(u16*)&area.area_property);
                pbody+=2;
                area.upleft_latitude=ntohl(*(u32*)pbody);   //左上点纬度
                pbody+=4;
                area.upleft_longitude=ntohl(*(u32*)pbody);  //左上点经度
                pbody+=4;
                area.lowright_latitude=ntohl(*(u32*)pbody); //右下点纬度
                pbody+=4;
                area.lowright_longitude=ntohl(*(u32*)pbody);//右下点经度
                pbody+=4;
                if(area.area_property.time)
                {
                    memcpy(area.start_time,pbody,6);        //起始时间
                    pbody+=6;
                    memcpy(area.end_time,pbody,6);          //结束时间
                    pbody+=6;
                }
                else
                {
                    memset(area.start_time,0x00,6);
                    memset(area.end_time,0x00,6);
                }

                if(area.area_property.limit_speed)
                {                                                  
                    area.top_speed=ntohs(*(u16*)pbody);     //最高速度
                    pbody+=2;
                    area.speeding_time=*pbody++;            //持续时间
                }
                else
                {
                    memset((char*)&area.top_speed,0x00,2);
                    memset((char*)&area.speeding_time,0x00,1);
                }

                memcpy(p_msg_buf_new,(char*)&area,sizeof(JTT808t_AreaRectangle));
                p_msg_buf_new+=sizeof(JTT808t_AreaRectangle);

                if((!area.id&&!area.upleft_latitude&&!area.upleft_longitude&&!area.lowright_latitude&&!area.lowright_longitude)||(p_msg_buf_new-jtt808_global_buf_new)>=4096)     //接收数据出错
                {
                    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
                    return;
                }
            }

            if(msg->head.property.size!=(pbody-(u8*)msg->body))
            {
                JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
                return;
            }

            W25X_EraseSector(RECTANGULAR_AREAR_ADDR/4096);  //擦除页
            W25X_Write_NoCheck(jtt808_global_buf_new,RECTANGULAR_AREAR_ADDR,4096);   //写入区域          
        break;

        case 1:
        case 2:
            W25X_Read(jtt808_global_buf,RECTANGULAR_AREAR_ADDR,4096);        //读出所有原有区域
            while(p_msg_buf-jtt808_global_buf<4096)                          //找出所有不匹配的区域
            {
                int i=0;
                memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_AreaRectangle));

                if(!area.id&&!area.upleft_latitude&&!area.upleft_longitude&&!area.lowright_latitude&&!area.lowright_longitude)break;//ID、左上点纬度、经度、右下点纬度、经度全部为0
                if(area.id==0xFFFFFFFF&&area.upleft_latitude==0xFFFFFFFF&&area.upleft_longitude==0xFFFFFFFF&&area.lowright_latitude==0xFFFFFFFF&&area.lowright_longitude==0xFFFFFFFF)break;//全部为FF，则退出
                
                for(i=0;i<total;i++)
                {
                    JTT808t_AreaRectangle area_new={0};

                    area_new.id=ntohl(*(u32*)pbody);                //区域ID
                    pbody+=4;
                    memcpy((char*)&area_new.area_property,pbody,2); //区域属性
                    *(u16*)&area_new.area_property=ntohs(*(u16*)&area_new.area_property);
                    pbody+=2;
                    area_new.upleft_latitude=ntohl(*(u32*)pbody);   //左上点纬度
                    pbody+=4;
                    area_new.upleft_longitude=ntohl(*(u32*)pbody);  //左上点经度
                    pbody+=4;
                    area_new.lowright_latitude=ntohl(*(u32*)pbody); //右下点纬度
                    pbody+=4;
                    area_new.lowright_longitude=ntohl(*(u32*)pbody);//右下点经度
                    pbody+=4;

                    if(area_new.area_property.time)
                    {
                        memcpy(area_new.start_time,pbody,6);        //起始时间
                        pbody+=6;
                        memcpy(area_new.end_time,pbody,6);          //结束时间
                        pbody+=6;
                    }
                    if(area_new.area_property.limit_speed)
                    {                                                  
                        area_new.top_speed=ntohs(*(u16*)pbody);     //最高速度
                        pbody+=2;
                        area_new.speeding_time=*pbody++;            //持续时间
                    } 

                    if(!area_new.id&&!area_new.upleft_latitude&&!area_new.upleft_longitude&&!area_new.lowright_latitude&&!area_new.lowright_longitude)
                    {
                        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
                        return;
                    }

                    if(area.id==area_new.id)break;   
                }
                pbody=msg->body;
                pbody+=2;

                if(i==total)                                           //无匹配项
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
                area.id=ntohl(*(u32*)pbody);                //区域ID
                pbody+=4;
                memcpy((char*)&area.area_property,pbody,2); //区域属性
                *(u16*)&area.area_property=ntohs(*(u16*)&area.area_property);
                pbody+=2;
                area.upleft_latitude=ntohl(*(u32*)pbody);   //左上点纬度
                pbody+=4;
                area.upleft_longitude=ntohl(*(u32*)pbody);  //左上点经度
                pbody+=4;
                area.lowright_latitude=ntohl(*(u32*)pbody); //右下点纬度
                pbody+=4;
                area.lowright_longitude=ntohl(*(u32*)pbody);//右下点经度
                pbody+=4;
                if(area.area_property.time)
                {
                    memcpy(area.start_time,pbody,6);        //起始时间
                    pbody+=6;
                    memcpy(area.end_time,pbody,6);          //结束时间
                    pbody+=6;
                }
                else
                {
                    memset(area.start_time,0x00,6);
                    memset(area.end_time,0x00,6);
                }

                if(area.area_property.limit_speed)
                {                                                  
                    area.top_speed=ntohs(*(u16*)pbody);     //最高速度
                    pbody+=2;
                    area.speeding_time=*pbody++;            //持续时间
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
                    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_FAIL); //失败
                    return;
                }
            }

            if(msg->head.property.size!=(pbody-(u8*)msg->body))
            {
                JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
                return;
            }

            W25X_EraseSector(RECTANGULAR_AREAR_ADDR/4096);  //擦除页
            W25X_Write_NoCheck(jtt808_global_buf_new,RECTANGULAR_AREAR_ADDR,4096);      //写入区域             
        break;

        default:
            JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
            return;
    }
    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //返回终端通用应答
}


void JTT808_Arearectangledel(JTT808t_Msg* msg)          //0x8603	删除矩形区域
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
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
        return;
    }

    if(total==0)
    {
        W25X_EraseSector(RECTANGULAR_AREAR_ADDR/4096);  //擦除页
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //返回终端通用应答
        return;
    }

    W25X_Read(jtt808_global_buf,RECTANGULAR_AREAR_ADDR,4096);              //读出所有原有区域

    while(p_msg_buf-jtt808_global_buf<4096)                                //去掉所有ID匹配项
    {
        int i=0;

        memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_AreaRectangle));

        if(!area.id&&!area.upleft_latitude&&!area.upleft_longitude&&!area.lowright_latitude&&!area.lowright_longitude)break;//ID、左上点纬度、经度、右下点纬度、经度全部为0
        if(area.id==0xFFFFFFFF&&area.upleft_latitude==0xFFFFFFFF&&area.upleft_longitude==0xFFFFFFFF&&area.lowright_latitude==0xFFFFFFFF&&area.lowright_longitude==0xFFFFFFFF)break;//全部为FF，则退出
                
        for(i=0;i<total;i++)
        {
            u32 ID=ntohl(*(u32*)pbody);
            pbody+=4;
            if(ID==area.id)break;
        }
        pbody=msg->body;
        pbody++;

        if(i==total)               //无ID匹配项
        {
            memcpy(p_msg_buf_new,p_msg_buf,sizeof(JTT808t_AreaRectangle));
            p_msg_buf_new+=sizeof(JTT808t_AreaRectangle);
        }

        p_msg_buf+=sizeof(JTT808t_AreaRectangle);
    }

    W25X_EraseSector(RECTANGULAR_AREAR_ADDR/4096);  //擦除页
    W25X_Write_NoCheck(jtt808_global_buf_new,RECTANGULAR_AREAR_ADDR,4096);    //写入区域            

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //返回终端通用应答
}


void JTT808_ArearectangleCheck(void)
{
    JTT808t_AreaRectangle area={0};
    u32 addr=RECTANGULAR_AREAR_ADDR;

    while(addr-RECTANGULAR_AREAR_ADDR<4096)
    {
        u8 is_in_tim=0;        //是否在时间段内
        u8 is_in_area=0;       //是否在围栏内部
        
        W25X_Read((u8*)&area,addr,sizeof(JTT808t_AreaRectangle));
        addr+=sizeof(JTT808t_AreaRectangle);

        if(!area.id)break;      //ID为0
        if(area.id==0xFFFFFFFF&&area.upleft_latitude==0xFFFFFFFF&&area.upleft_longitude==0xFFFFFFFF&&area.lowright_latitude==0xFFFFFFFF&&area.lowright_longitude==0xFFFFFFFF)break;//全部为FF，则退出
               

        /********************判断是否在时间段内************************/
        if(area.area_property.time)     //如果依据时间 ,判断是否在时间段内
        {
            u8 start_tim[7]={0},end_tim[7]={0},gps_tim[7]={0};
            u8 i,start_len=0,end_len=0;          //前面有几个字节为0
            
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

            i=start_len<=end_len?start_len:end_len; //找出前面为0最小长度

            memcpy(start_tim,&start_tim[i],6-i);    //找出有效时间
            memcpy(end_tim,  &end_tim[i],  6-i);
            memcpy(gps_tim,  &gps_tim[i],  6-i);

            start_tim[6-i]=0;
            end_tim[6-i]=0;
            gps_tim[6-i]=0;

            if(strcmp((const char*)gps_tim,(const char*)start_tim)>0&&strcmp((const char*)gps_tim,(const char*)end_tim)<0)is_in_tim=1; //大于起始时间，小于结束时间
            else is_in_tim=0; 
        }
        else                         //如果不依据时间 ，则永远在时间段内
        {
            is_in_tim=1;
        }
        if(is_in_tim==0)continue;        //如果不在时间段内 不进行后续操作


        /********************判断是否在围栏内***********************/
        if(gps_location.latitude  <= area.upleft_latitude    &&    //当前点纬度小于左上点纬度
           gps_location.latitude  >= area.lowright_latitude  &&    //当前点纬度大于右下点纬度
           gps_location.longitude <= area.lowright_longitude &&    //当前点经度小于右下点经度
           gps_location.longitude >= area.upleft_longitude)        //当前点经度大于左上点经度   //在围栏内部
        {
            is_in_area=1;
        }
        else                                                                                    //在围栏外部
        {
            is_in_area=0;
        }


        /********************依据围栏属性判断报警***************************/
        if(area.area_property.in2driver&&is_in_area==1)   //进区域报警给驾驶员
        {
//			static u32 TTS_Area=0;

//            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//            {
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);

//                TTS_Area=TimerSecond;
//				strcat(TTS,"您已进入矩形区域");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//                API_Play_TTS(TTS);
//            }
        }
        if(area.area_property.in2platform&&is_in_area==1) //进区域报警给平台
        {
            JTT808_Location_Alarm.across_area=1;
        }
        if(area.area_property.out2driver&&is_in_area==0)  //出区域报警给驾驶员
        {
//			static u32 TTS_Area=0;

//            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//            {
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);

//                TTS_Area=TimerSecond;
//				strcat(TTS,"您已出矩形区域");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//                API_Play_TTS(TTS);
//            }
        }
        if(area.area_property.out2platform&&is_in_area==0)//出区域报警给平台
        {
            JTT808_Location_Alarm.across_area=1;
        }

        if(area.area_property.doorcontrol&&is_in_area==1) //禁止开门
        {
            JTT808_Forbidden_opendoor();
        }
        if(area.area_property.in2community&&is_in_area==1)//进区域关闭GPRS通信
        {
            JTT808_Gprs_Disconnect();
        }
        if(area.area_property.in2GNSSdate&&is_in_area==1) //进区域采集详细GNSS数据
        {
            JTT808_GNSS_Sample();
        }


        /******************判断是否超速************************/
        if(area.area_property.limit_speed&&is_in_area==1)  //如果依据速度
        {
            if((area.top_speed)&&((gps_location.speed/10)>area.top_speed))       //超速
            {
//				static u32 TTS_Area=0;

				JTT808_Location_Alarm.speeding=1;          //超速报警

//	            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//	            {
//					char TTS[50]={0},tmp[10]={0};
//					u32 id=htonl(area.id);

//	                TTS_Area=TimerSecond;
//					strcat(TTS,"您已在矩形区域");
//					strncpy(tmp,(char*)&id,4);
//					strcat(TTS,tmp);
//					strcat(TTS,"内超速");
//	                API_Play_TTS(TTS);
//	            } 
            }
        }  
    }
}


void JTT808_Areapolygonset(JTT808t_Msg* msg)            //0x8604	设置多边形区域
{
    u8 *pbody=msg->body;
    u8 *p_msg_buf=jtt808_global_buf;
    u8 *p_msg_buf_new=jtt808_global_buf_new;
    JTT808t_AreaPolygonSet area={0};

	memset(jtt808_global_buf,0x00,sizeof(jtt808_global_buf));
	memset(jtt808_global_buf_new,0x00,sizeof(jtt808_global_buf_new));

    W25X_Read(jtt808_global_buf,POLYGON_AREAR_ADDR,4096);              //读出所有原有区域
    

    while(p_msg_buf-jtt808_global_buf<4096)                            //取出所有不同ID的区域
    {
        u32 ID=ntohl(*(u32*)pbody);

        memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_AreaPolygonSet));
         
        if(!area.id&&!area.peak_cnt)break;                  //ID、顶点总数均为0
        if(area.id==0xffffffff&&area.peak_cnt==0xffff)break;//全为ff  
        
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

    area.id=ntohl(*(u32*)pbody);                //区域ID
    pbody+=4;
    memcpy((char*)&area.area_property,pbody,2); //区域属性
    *(u16*)&area.area_property=ntohs(*(u16*)&area.area_property);
    pbody+=2;

    if(area.area_property.time)
    {
        memcpy(area.start_time,pbody,6);        //起始时间
        pbody+=6;
        memcpy(area.end_time,pbody,6);          //结束时间
        pbody+=6;
    }
    else
    {
        memset(area.start_time,0x00,6);
        memset(area.end_time,0x00,6);
    }

    if(area.area_property.limit_speed)
    {                                                  
        area.top_speed=ntohs(*(u16*)pbody);     //最高速度
        pbody+=2;
        area.speeding_time=*pbody++;            //持续时间
    }
    else
    {
        memset((char*)&area.top_speed,0x00,2);
        memset((char*)&area.speeding_time,0x00,1);
    }
    area.peak_cnt=ntohs(*(u16*)pbody);          //定点总数
    pbody+=2;

    if(area.peak_cnt<3)
    {
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
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
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
        return;
    }

    W25X_EraseSector(POLYGON_AREAR_ADDR/4096);  //擦除页
    W25X_Write_NoCheck(jtt808_global_buf_new,POLYGON_AREAR_ADDR,4096);
}


void JTT808_Areapolygondel(JTT808t_Msg* msg)            //0x8605	删除多边形区域
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
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
        return;
    }

    if(total==0)
    {
        W25X_EraseSector(POLYGON_AREAR_ADDR/4096);  //擦除页
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //返回终端通用应答
        return;
    }

    W25X_Read(jtt808_global_buf,POLYGON_AREAR_ADDR,4096);              //读出所有原有区域

    while(p_msg_buf-jtt808_global_buf<4096)
    {
        int i=0;
        memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_AreaPolygonSet));
         
        if(!area.id&&!area.peak_cnt)break;                  //ID、顶点总数均为0
        if(area.id==0xffffffff&&area.peak_cnt==0xffff)break; //全为ff

        for(i=0;i<total;i++)
        {
            u32 ID=ntohl(*(u32*)pbody);
            pbody+=4;
            if(ID==area.id)break;
        }
        pbody=msg->body;
        pbody++;

        if(i==total)                                        //无匹配项
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
    W25X_EraseSector(POLYGON_AREAR_ADDR/4096);  //擦除页
    W25X_Write_NoCheck(jtt808_global_buf_new,POLYGON_AREAR_ADDR,4096);

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //返回终端通用应答
}


void JTT808_AreapolygonCheck(void)
{
    JTT808t_AreaPolygonSet area={0};
    u32 addr=POLYGON_AREAR_ADDR;

    while(addr-POLYGON_AREAR_ADDR<4096)
    {
        u8 is_in_tim=0;        //是否在时间段内
        u8 is_in_area=0;       //是否在围栏内部
        POINT poly[50]={0},q={0};
        int i=0;
        
        W25X_Read((u8*)&area,addr,sizeof(JTT808t_AreaPolygonSet));
        addr+=sizeof(JTT808t_AreaPolygonSet);

        if(!area.id||area.peak_cnt<3)break;                  //ID、顶点总数均为0
        if(area.id==0xffffffff&&area.peak_cnt==0xffff)break; //全为ff


        /********************判断是否在时间段内************************/
        if(area.area_property.time)     //如果依据时间 ,判断是否在时间段内
        {
            u8 start_tim[7]={0},end_tim[7]={0},gps_tim[7]={0};
            u8 i,start_len=0,end_len=0;          //前面有几个字节为0
            
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

            i=start_len<=end_len?start_len:end_len; //找出前面为0最小长度

            memcpy(start_tim,&start_tim[i],6-i);    //找出有效时间
            memcpy(end_tim,  &end_tim[i],  6-i);
            memcpy(gps_tim,  &gps_tim[i],  6-i);

            start_tim[6-i]=0;
            end_tim[6-i]=0;
            gps_tim[6-i]=0;

            if(strcmp((const char*)gps_tim,(const char*)start_tim)>0&&strcmp((const char*)gps_tim,(const char*)end_tim)<0)is_in_tim=1; //大于起始时间，小于结束时间
            else is_in_tim=0; 
        }
        else                         //如果不依据时间 ，则永远在时间段内
        {
            is_in_tim=1;
        }
        if(is_in_tim==0)             //如果不在时间段内 不进行后续操作
        {
            addr+=area.peak_cnt*8;
            continue;    
        }


        /********************判断是否在围栏内***********************/
        q.x=gps_location.latitude;
        q.y=gps_location.longitude;      //对当前点赋值

        for(i=0;i<area.peak_cnt;i++)
        {
            u8 buf[8]={0};

            W25X_Read(buf,addr,8);
            addr+=8;

            poly[i].x=*(u32*)&buf[0];
            poly[i].y=*(u32*)&buf[4];
        }                               //对顶点赋值
        
        if(is_inside(poly,area.peak_cnt,q))               //在围栏内部
        {
            is_in_area=1;
        }
        else                                              //在围栏外部
        {
            is_in_area=0;
        }


        /********************依据围栏属性判断报警***************************/
        if(area.area_property.in2driver&&is_in_area==1)   //进区域报警给驾驶员
        {
//			static u32 TTS_Area=0;

//            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//            {
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);

//                TTS_Area=TimerSecond;
//				strcat(TTS,"您已进入多边形区域");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//                API_Play_TTS(TTS);
//            } 
        }
        if(area.area_property.in2platform&&is_in_area==1) //进区域报警给平台
        {
            JTT808_Location_Alarm.across_area=1;
        }
        if(area.area_property.out2driver&&is_in_area==0)  //出区域报警给驾驶员
        {
//			static u32 TTS_Area=0;

//            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//            {
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);

//                TTS_Area=TimerSecond;
//				strcat(TTS,"您已出多边形区域");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//                API_Play_TTS(TTS);
//            } 
        }
        if(area.area_property.out2platform&&is_in_area==0)//出区域报警给平台
        {
            JTT808_Location_Alarm.across_area=1;
        }

        if(area.area_property.doorcontrol&&is_in_area==1) //禁止开门
        {
            JTT808_Forbidden_opendoor();
        }
        if(area.area_property.in2community&&is_in_area==1)//进区域关闭GPRS通信
        {
            JTT808_Gprs_Disconnect();
        }
        if(area.area_property.in2GNSSdate&&is_in_area==1) //进区域采集详细GNSS数据
        {
            JTT808_GNSS_Sample();
        }


        /******************判断是否超速************************/
        if(area.area_property.limit_speed&&is_in_area==1)  //如果依据速度
        {
            if((area.top_speed)&&((gps_location.speed/10)>area.top_speed))       //超速
            {
//				static u32 TTS_Area=0;

				JTT808_Location_Alarm.speeding=1;          //超速报警

//	            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//	            {
//					char TTS[50]={0},tmp[10]={0};
//					u32 id=htonl(area.id);

//	                TTS_Area=TimerSecond;
//					strcat(TTS,"您已在多边形区域");
//					strncpy(tmp,(char*)&id,4);
//					strcat(TTS,tmp);
//					strcat(TTS,"内超速");
//	                API_Play_TTS(TTS);
//	            }  
            }
        }
    }
}


void JTT808_Routeset(JTT808t_Msg* msg)                  //0x8606	设置路线
{
    u8 *pbody=msg->body;
    u8 *p_msg_buf=jtt808_global_buf;
    u8 *p_msg_buf_new=jtt808_global_buf_new;
    JTT808t_RouteSet area={0};

	memset(jtt808_global_buf,0x00,sizeof(jtt808_global_buf));
	memset(jtt808_global_buf_new,0x00,sizeof(jtt808_global_buf_new));

    W25X_Read(jtt808_global_buf,ROUTE_AREAR_ADDR,4096);              //读出所有原有区域

    while(p_msg_buf-jtt808_global_buf<4096)
    {
        u32 ID=ntohl(*(u32*)pbody);

        memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_RouteSet));
         
        if(!area.id&&!area.turn_point_cnt)break;                  //ID、顶点总数均为0
        if(area.id==0xffffffff&&area.turn_point_cnt==0xffff)break; //全为ff  
        
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

    area.id=ntohl(*(u32*)pbody);                //区域ID
    pbody+=4;
    memcpy((char*)&area.property,pbody,2);      //区域属性
    *(u16*)&area.property=ntohs(*(u16*)&area.property);
    pbody+=2;

    if(area.property.depend_time)
    {
        memcpy(area.start_time,pbody,6);        //起始时间
        pbody+=6;
        memcpy(area.end_time,pbody,6);          //结束时间
        pbody+=6;
    }
    else
    {
        memset(area.start_time,0x00,6);
        memset(area.end_time,0x00,6);
    }

    area.turn_point_cnt=ntohs(*(u16*)pbody);    //拐点总数
    pbody+=2;

    if(area.turn_point_cnt<2)
    {
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
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

		//依据903项目要求添加，非部标协议
		{
			memcpy(item.start_time,pbody,6);        //起始时间
	        pbody+=6;
	        memcpy(item.end_time,pbody,6);          //结束时间
	        pbody+=6;
		}

        if(!item.section_width)
        {
            JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
            return;
        }

        memcpy(p_msg_buf_new,(char*)&item,sizeof(JTT808t_RouteTurnPoint));
        p_msg_buf_new+=sizeof(JTT808t_RouteTurnPoint);
    }

    if(msg->head.property.size!=(pbody-(u8*)msg->body))
    {
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
        return;
    }

    W25X_EraseSector(ROUTE_AREAR_ADDR/4096);  //擦除页
    W25X_Write_NoCheck(jtt808_global_buf_new,ROUTE_AREAR_ADDR,4096);
}


void JTT808_Routedel(JTT808t_Msg* msg)                  //0x8607	删除路线
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
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_ERR); //消息有误
        return;
    }

    if(total==0)
    {
        W25X_EraseSector(ROUTE_AREAR_ADDR/4096);  //擦除页
        JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //返回终端通用应答
        return;
    }

    W25X_Read(jtt808_global_buf,ROUTE_AREAR_ADDR,4096);              //读出所有原有区域

    while(p_msg_buf-jtt808_global_buf<4096)
    {
        int i=0;

        memcpy((char*)&area,p_msg_buf,sizeof(JTT808t_RouteSet));
         
        if(!area.id&&!area.turn_point_cnt)break;                  //ID、拐点总数均为0
        if(area.id==0xffffffff&&area.turn_point_cnt==0xffff)break; //全为ff

        for(i=0;i<total;i++)
        {
            u32 ID=ntohl(*(u32*)pbody);
            pbody+=4;
            if(ID==area.id)break;
        }
        pbody=msg->body;
        pbody++;

        if(i==total)                                        //无匹配项
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
    W25X_EraseSector(ROUTE_AREAR_ADDR/4096);  //擦除页
    W25X_Write_NoCheck(jtt808_global_buf_new,ROUTE_AREAR_ADDR,4096);

    JTT808_terminal_act(msg->head.serial_no,msg->head.id,JTT808_ACT_RET_OK); //返回终端通用应答
}


void JTT808_RouteCheck(void)
{
    JTT808t_RouteSet area={0};
    JTT808t_RouteTurnPoint in_item={0};                    //在路段内的拐点
    static u32 start_sec=0;                                //路段开始时间
    static JTT808t_RouteTurnPoint last_item={0};           //上一个路段

    u32 addr=ROUTE_AREAR_ADDR;


    while(addr-ROUTE_AREAR_ADDR<4096)
    {
        u8 is_in_tim=0;        //是否在时间段内
        u8 is_in_area=0;       //是否在围栏内部
        
        
        W25X_Read((u8*)&area,addr,sizeof(JTT808t_RouteSet));
        addr+=sizeof(JTT808t_RouteSet);

        if(!area.id||area.turn_point_cnt<2)break;                  //ID、顶点总数均为0
        if(area.id==0xffffffff&&area.turn_point_cnt==0xffff)break; //全为ff


        /********************判断是否在时间段内************************/
        if(area.property.depend_time)                              //如果依据时间 ,判断是否在时间段内
        {
            u8 start_tim[7]={0},end_tim[7]={0},gps_tim[7]={0};
            u8 i,start_len=0,end_len=0;          //前面有几个字节为0
            
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

            i=start_len<=end_len?start_len:end_len; //找出前面为0最小长度

            memcpy(start_tim,&start_tim[i],6-i);    //找出有效时间
            memcpy(end_tim,  &end_tim[i],  6-i);
            memcpy(gps_tim,  &gps_tim[i],  6-i);

            start_tim[6-i]=0;
            end_tim[6-i]=0;
            gps_tim[6-i]=0;

            if(strcmp((const char*)gps_tim,(const char*)start_tim)>0&&strcmp((const char*)gps_tim,(const char*)end_tim)<0)is_in_tim=1; //大于起始时间，小于结束时间
            else is_in_tim=0;    
        }
        else                         //如果不依据时间 ，则永远在时间段内
        {
            is_in_tim=1;
        }
        if(is_in_tim==0)             //如果不在时间段内 不进行后续操作
        {
            addr+=area.turn_point_cnt*sizeof(JTT808t_RouteTurnPoint);
            continue;    
        }


        /********************判断是否在围栏内***********************/
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


        /********************依据围栏属性判断报警***************************/
        if(is_in_area)              //在路线内
        {
            if((in_item.turn_point_id!=last_item.turn_point_id)||(in_item.section_id!=last_item.section_id))   //当前路段和上一次的路段不相同
            {
                 if((last_item.section_property.drive_time)&&(TimerSecond-start_sec<last_item.section_inadequate_threshold))     //行驶时间不足
                 {
//				 	static u32 TTS_Area=0;

                    JTT808_Location_Alarm.section_drive_time_err=1; 
//					if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//		            {
//						char TTS[50]={0},tmp[10]={0};
//						u32 id=htonl(area.id);

//		                TTS_Area=TimerSecond;
//						strcat(TTS,"您在路线");
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,tmp);
//						strcat(TTS,"的");
//						id=htonl(last_item.section_id);
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,"路段内行驶时间不足");
//		                API_Play_TTS(TTS);
//		            } 
                 }
                 start_sec=TimerSecond;                                                    //对当前路段开始时间赋值
                 memcpy((char*)&last_item,(char*)&in_item,sizeof(JTT808t_RouteTurnPoint)); //对上一个路段赋值
            }

            if((in_item.section_property.drive_time)&&(TimerSecond-start_sec>in_item.section_length_threshold))              //行驶时间过长
            {
//				static u32 TTS_Area=0;

                JTT808_Location_Alarm.section_drive_time_err=1;
//				if(timerSecondSub(TimerSecond,TTS_Area)>=60)
	            {
//					char TTS[50]={0},tmp[10]={0};
//					u32 id=htonl(area.id);

//	                TTS_Area=TimerSecond;
//					strcat(TTS,"您在路线");
//					strncpy(tmp,(char*)&id,4);
//					strcat(TTS,tmp);
//					strcat(TTS,"的");
//					id=htonl(in_item.section_id);
//					strncpy(tmp,(char*)&id,4);
//					strcat(TTS,"路段内行驶时间过长");
//	                API_Play_TTS(TTS);
	            } 
            }

            if(in_item.section_property.limit_speed)            //如果限速
            {
                if((in_item.section_top_speed)&&((gps_location.speed/10)>in_item.section_top_speed))       //超速
                {
//					static u32 TTS_Area=0;

					JTT808_Location_Alarm.speeding=1;          //超速报警

//		            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
		            {
//						char TTS[50]={0},tmp[10]={0};
//						u32 id=htonl(area.id);

//		                TTS_Area=TimerSecond;
//						strcat(TTS,"您已在路线");
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,tmp);
//						strcat(TTS,"的");
//						id=htonl(in_item.section_id);
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,"路段内超速");
//		                API_Play_TTS(TTS);
		            }   
                }
            }

			//下面依据903项目要求添加，非部标协议
//			{
//				u8 start_tim[7]={0},end_tim[7]={0},gps_tim[7]={0};
//				u8 i,start_len=0,end_len=0;          //前面有几个字节为0
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
//				i=start_len<=end_len?start_len:end_len; //找出前面为0最小长度
//				
//				memcpy(start_tim,&start_tim[i],6-i);    //找出有效时间
//				memcpy(end_tim,  &end_tim[i],  6-i);
//				memcpy(gps_tim,  &gps_tim[i],  6-i);
//				
//				start_tim[6-i]=0;
//				end_tim[6-i]=0;
//				gps_tim[6-i]=0;
				
				
//				if(strcmp((const char*)gps_tim,(const char*)start_tim)<0)//小于起始时间  提前到达节点
//				{
//					static u32 TTS_Area=0;

//		            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//		            {
//						char TTS[50]={0},tmp[10]={0};
//						u32 id=htonl(area.id);

//		                TTS_Area=TimerSecond;
//						strcat(TTS,"您已提前到达路线");
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,"的");
//						id=htonl(in_item.section_id);
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,"路段");
//						strcat(TTS,tmp);
//		                API_Play_TTS(TTS);
//		            }   
//				}
//				if(strcmp((const char*)gps_tim,(const char*)end_tim)>0)  //大于结束时间  超过时间还未到达节点
//				{
//					static u32 TTS_Area=0;

//		            if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//		            {
//						char TTS[50]={0},tmp[10]={0};
//						u32 id=htonl(area.id);

//		                TTS_Area=TimerSecond;
//						strcat(TTS,"您已超时未出路线");
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,"的");
//						id=htonl(in_item.section_id);
//						strncpy(tmp,(char*)&id,4);
//						strcat(TTS,"路段");
//						strcat(TTS,tmp);
//		                API_Play_TTS(TTS);
//		            }   
//				}
//				if(strcmp((const char*)gps_tim,(const char*)start_tim)>0&&strcmp((const char*)gps_tim,(const char*)end_tim)<0)//大于起始时间，小于结束时间 任务正常
//				{
//				}
//			}
        }



        if(area.property.in2driver&&is_in_area==1)   //进区域报警给驾驶员
        {
//			static u32 TTS_Area=0;
//			
//			if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//			{
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);
//				
//			    TTS_Area=TimerSecond;
//				strcat(TTS,"您已进入路线");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//			    API_Play_TTS(TTS);
//			} 
        }
        if(area.property.in2platform&&is_in_area==1) //进区域报警给平台
        {
            JTT808_Location_Alarm.across_route=1;
        }
        if(area.property.out2driver&&is_in_area==0)  //出区域报警给驾驶员
        {
//			static u32 TTS_Area=0;
//			
//			if(timerSecondSub(TimerSecond,TTS_Area)>=60)
//			{
//				char TTS[50]={0},tmp[10]={0};
//				u32 id=htonl(area.id);
//				
//			    TTS_Area=TimerSecond;
//				strcat(TTS,"您已出路线");
//				strncpy(tmp,(char*)&id,4);
//				strcat(TTS,tmp);
//			    API_Play_TTS(TTS);
//			}
        }
        if(area.property.out2platform&&is_in_area==0)//出区域报警给平台
        {
            JTT808_Location_Alarm.across_route=1;
        }
        if(is_in_area==0)
        {
            JTT808_Location_Alarm.route_out=1;       //偏离路线
        }
    }
}



void JTT808_Area_Alarm_check(void)
{
    if(!JTT808_Location_State.is_location)return;  //未定位退出
    if(!JTT808_Location_State.acc)return;		   //ACC关闭退出

    JTT808_AreacycleCheck();
    JTT808_ArearectangleCheck();
    JTT808_AreapolygonCheck();
    JTT808_RouteCheck();
}



