#include "include.h"


char JTT808_Flag_Authentication=0;  //鉴权
char dirver_login_status=0;         //驾驶员登录状态： 0 ：未插卡   1:已插卡
JTT808t_DialBack Dial={0};          //电话回拨、监听
RSA_type secrt_key={0};


static u8 jtt808_msg_body[JTT808_MSG_MAX_BODY_SIZE]={0};

u8 jtt808_global_buf[4096]={0};
u8 jtt808_global_buf_new[4096]={0};



JTT808t_Msg jtt808_msg={0};


/******************************************************************************
 * JTT808_MsgEscape - 消息转义/还原
 *
 * Input: flag, 1 for 转义, 0 for 转义还原; src; size
 * Output: dst
 * Returns: 转义后的size
 ******************************************************************************/
int JTT808_MsgEscape(char flag,const u8* src,u8* dst,int size)
{
    int escape_size = size;
    
    if(flag)                                     //1 转义
    {
        while(size--)
        {
            if(*src==JTT808_MSG_TAG)             //0x7E-->0x7D 0x02
            {
                *dst++=JTT808_MSG_ESCAPE_TAG;
                *dst++=0x02;
                escape_size++;
            }
            else if(*src==JTT808_MSG_ESCAPE_TAG) //0x7D-->0x7D 0x01
            {
                *dst++=JTT808_MSG_ESCAPE_TAG;
                *dst++=0x01;
                escape_size++;
            }
            else
            {
                *dst++=*src;
            }
            src++;
        }
    }
    else                                         //0 还原
    {
        while(size--)
        {
            if(*src==JTT808_MSG_ESCAPE_TAG)      
            {
                src++;
                if(*src==0x01)                   //0x7D 0x01-->0x7D
                {
                    *dst++=JTT808_MSG_ESCAPE_TAG;
                }
                else if(*src==0x02)              //0x7D 0x02-->0x7E
                {
                    *dst++=JTT808_MSG_TAG;
                }
								else
								{
										return 0;
								}
                escape_size--;
								size--;
            }
            else
            {
                *dst++=*src;
            }
            src++;
        }
    }
    return escape_size;
}



/******************************************************************************
 * JTT808_Check - 异或校验
 *
 * Input: src, size
 * Output:
 * Returns: 校验码
 ******************************************************************************/
__INLINE u8 JTT808_Check(const u8 * src, u32 size)
{
    u8 checksum = 0;

    while(size--)
    {
        checksum ^= *src++;
    }
    return checksum;
}



/**************************************************************
 * JTT808_MsgEncode - 信息数据编码
 *
 * Input: src
 * Output: dst
 * Returns: 编码后数据总大小
 * 备注：src中的body已经过大小端转换，head未经过转换。
 * --------------------
 *******************************************************************************/
u32 JTT808_MsgEncode(const JTT808t_Msg *src, u8 *dst)
{
    u8 *tmp=jtt808_global_buf_new,*msg_buf=tmp;
    JTT808t_MsgHead head={0};
    int len=src->head.property.packet?sizeof(JTT808t_MsgHead):(sizeof(JTT808t_MsgHead)-4);  //消息头大小


    /*******对接收的消息头大小端转换，保存在局部变量head里*************/
    head.id=htons(*(u16*)&src->head.id);                                    //转换消息ID
    *(u16*)&head.property=htons(*(u16*)&src->head.property);                //转换消息体属性
    memcpy(head.mobile_no,src->head.mobile_no,sizeof(src->head.mobile_no)); //复制终端手机号
    head.serial_no=htons(*(u16*)&src->head.serial_no);                      //转换消息流水号
    head.package.cnt=htons(*(u16*)&src->head.package.cnt);                  //转换分包消息总数
    head.package.num=htons(*(u16*)&src->head.package.num);                  //转换分包消息序号
    /*******************************************************************/


    /*将消息结构体里的消息头、消息体放在tmp里并计算校验*/
    memcpy(tmp,(char*)&head,len);                            //将大小端转换后的head放在tmp里
    tmp+=len;
    memcpy(tmp,src->body,src->head.property.size);           //将接收到的消息体放在tmp里
    tmp+=src->head.property.size;
    len+=src->head.property.size;
    memset(tmp,JTT808_Check(msg_buf,len),sizeof(src->checksum)); //校验并将校验位放在tmp里
    len+=sizeof(src->checksum);
    /***************************************************/

    
    *dst++=JTT808_MSG_TAG;                  //赋值标识位
    len=JTT808_MsgEscape(1,msg_buf,dst,len);//消息头、消息体、校验位转义并拷贝到目标里
    *(dst+len)=JTT808_MSG_TAG;              //赋值标识位
    return (len+2);                         //返回转义后的数据长度
}


/******************************************************************************
 * JTT808_MsgDecode - 信息数据解码
 *
 * Input: src, src_size
 * Output: dst
 *返回的结构体中只有消息头、消息体有数据，消息头已转换，消息体未转换
 ******************************************************************************/
JTT808t_Err JTT808_MsgDecode(const u8 *src, u32 src_size, JTT808t_Msg *dst)
{
    u8 *tmp=jtt808_global_buf_new;
    JTT808t_MsgHead head={0};
    int len;
    int head_len=sizeof(JTT808t_MsgHead);
    char* p_body=(char*)dst->body;
    
    src++;
    
    if((len=JTT808_MsgEscape(0,src,tmp,src_size-2)) == 0)  //对消息头、消息体、校验位 转义还原并将放在tmp中  返回转义后消息头、消息体和校验码的长度
		{
				return JTT808_ERR;
		}
    if(tmp[len-1]!=JTT808_Check(tmp,len-1))      //计算并判断校验    消息头、消息体、校验位工len个字节   tmp[len-1]为原始校验码
    {
        printf("校验错误!计算为:%02X,实际为:%02X\r\n",JTT808_Check(tmp,len-1),tmp[len-1]);
        return JTT808_ERR;
    }
    /*运行到这里tmp中存放这消息头、消息体、校验位信息*/
    
    memcpy((char*)&head,tmp,sizeof(JTT808t_MsgHead));  //将大端数据tmp里前面消息头长度的数据赋值到局部变量head中
    head.id=ntohs(*(u16*)&head.id);                    //对消息ID进行大小端转换
    *(u16*)&head.property=ntohs(*(u16*)&head.property);//对消息体属性进行大小端转换
    head.serial_no=ntohs(*(u16*)&head.serial_no);      //对消息流水号进行大小端转换
    head.package.cnt=ntohs(*(u16*)&head.package.cnt);  //对分包消息总数进行大小端转换
    head.package.num=ntohs(*(u16*)&head.package.num);  //对分包消息序号进行大小端转换
    if(!head.property.packet)                          //依据消息体属性重新对分包消息位赋值
    {
        memset((char*)&head.package,0x00,sizeof(head.package));
        head_len-=4;
    }
	
	if((head_len+head.property.size+1)!=len)
	{
		printf("原始数据长度错误！计算为:%02X,实际为:%02X",head_len+head.property.size+3,len+2);
		return JTT808_ERR;
	}

    tmp+=head_len;


    memcpy((char*)&dst->head,(char*)&head,sizeof(JTT808t_MsgHead)); //将大小端转换后的局部变量head赋值到目标结构体里的消息头中

//	if(head.property.encrypt==1)									//如果消息体已加密，则解密
//	{
//		int i=0;

//		decrypt(tmp,head.property.size,secrt_key.key,secrt_key.key_len);

//		printf("\r\n解密数据为:");
//		for(i=0;i<head.property.size;i++)
//		{
//			printf("%02X ",tmp[i]);
//		}
//		printf("\r\n");
//	}

    while(head.property.size--)                                     //将接收到的消息体赋值到目标结构体的消息体中。
    {
        *p_body++=*tmp++;
    }

    return JTT808_OK;               //返回的结构体中只有消息头、消息体有数据，消息头已转换，消息体未转换
}


/******************************************************************************
 * JTT808_MsgPacket - 消息打包发送
 *
 * Input:  cmd       消息ID 
 *         body_src  消息体指针
 *         body_size 消息体长度
 *         package   消息分包属性
 *		   prio      0:被发消息   1：主发消息				
 * Output:
 * Returns:
 ******************************************************************************/
JTT808t_Err JTT808_MsgPacket(u16 cmd, const void * body_src, u32 body_size, const JTT808t_MsgPackage package, u8 prio)
{
    JTT808t_Msg* msg=&jtt808_msg;
		u8* msg_buf=jtt808_global_buf;
    int msg_buf_len=0;
		static u16 serial_no = 0;

    if(body_size&&!body_src)         //长度不为0，但是数据地址是空指针
    {
        return JTT808_ERR;
    }
    if(package.num&&!package.cnt)    //有分包序号，但是分包总数为0
    {
        return JTT808_ERR;
    }


    /**********************依据消息ID、消息体、分包属性对msg赋值************************************/
    msg->head_tag = JTT808_MSG_TAG;     //标识位
		msg->end_tag = JTT808_MSG_TAG;      //标识位
    msg->head.id=cmd;                   //消息ID
    msg->head.property.size=body_size;  //消息体长度
//		if((cmd!=JTT808_CMD_TERMINAL_REG)&&(cmd!=JTT808_CMD_TERMINAL_AUTHENTICATION)&&(cmd!=JTT808_CMD_TERMINAL_BEAT)&&(secrt_key.use_key))
//				msg->head.property.encrypt=1;	   //当前使用加密，并且当前消息ID不为注册、鉴权时，对消息体进行加密
//		else 
				msg->head.property.encrypt=0;  //加密属性，置0 	
//		if((cmd!=JTT808_CMD_TERMINAL_REG)&&(cmd!=JTT808_CMD_TERMINAL_AUTHENTICATION)&&(cmd!=JTT808_CMD_TERMINAL_BEAT)&&(!msg->head.property.encrypt))
//			return JTT808_ERR;     //当前消息ID不为注册鉴权，并且没有使用加密 则退出    
    if(package.cnt!=0||package.num!=0)  //消息分包属性
    {
        msg->head.property.packet=1;
    }
    else
    {
        msg->head.property.packet=0;
    }
    getBCDFromStr(msg->head.mobile_no,(char*)JTT808_Terminal_Param.local_tel);   //终端手机号
    msg->head.serial_no=serial_no++;                                             //消息流水号
    memcpy((char*)&msg->head.package,(char*)&package,sizeof(JTT808t_MsgPackage));//消息分包属性
		memcpy(msg->body,body_src,body_size);
	
		//消息体
//		if(msg->head.property.encrypt==1)											
//		{
//		int i=0;
//		char* pbody=msg->body;

//		printf("\r\n原始数据为:");
//		for(i=0;i<body_size;i++)
//		{
//			printf("%02X ",pbody[i]);
//		}
//		printf("\r\n");

//		encrypt(msg->body,body_size,secrt_key.key,secrt_key.key_len);
//		}												                                                        
    /**********************************************************************************************/


    msg_buf_len=JTT808_MsgEncode(msg, msg_buf);              //将结构体msg打包到数组中,并获得数据的长度

	  JTT808_TXD(msg_buf,msg_buf_len);

	return JTT808_OK;
}



 /******************************************************************************
 * JTT808_Process - 部标协议处理线程
 * --------------------
 * 16-jul-2013, 梁广文 modify: 修改状态机
 * 09-jul-2013, 梁广文 written
 * --------------------
 ******************************************************************************/
void JTT808_Process(void)
{
		static JTT808_RX_BUF jtt808_rx_buf={0};

    JTT808_AlarmStateCheck();      //报警状态检测
    JTT808_InitiativeCmd();        //主发消息，如心跳、注册、鉴权、定位跟踪等。

    if(JTT808_get_msg(&jtt808_rx_buf)==1)
    {
        JTT808_msg_prase(&jtt808_rx_buf);
    }
}


void JTT808_init(void)
{
    u16 inital=BKP_ReadBackupRegister(BKP_DR2);

    jtt808_param_init();

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟
    PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
    if(inital!=0x5050)
    {
        BKP_WriteBackupRegister(BKP_DR2, 0X5050);

        BKP_WriteBackupRegister(BKP_DR3, LOACTION_START_ADDR>>16);  
        BKP_WriteBackupRegister(BKP_DR4, LOACTION_START_ADDR&0xFFFF);       //断点续传读地址

        BKP_WriteBackupRegister(BKP_DR5, LOACTION_START_ADDR>>16);
        BKP_WriteBackupRegister(BKP_DR6, LOACTION_START_ADDR&0xFFFF);       //断点续传写地址
    }
    LOACTION_READ_ADDR=BKP_ReadBackupRegister(BKP_DR3)<<16|BKP_ReadBackupRegister(BKP_DR4);   //断点续传读地址
    LOACTION_WRITE_ADDR=BKP_ReadBackupRegister(BKP_DR5)<<16|BKP_ReadBackupRegister(BKP_DR6);  //断点续传写地址

	jtt808_msg.body=jtt808_msg_body;
}




