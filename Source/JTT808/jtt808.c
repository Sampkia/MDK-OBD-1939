#include "include.h"


char JTT808_Flag_Authentication=0;  //��Ȩ
char dirver_login_status=0;         //��ʻԱ��¼״̬�� 0 ��δ�忨   1:�Ѳ忨
JTT808t_DialBack Dial={0};          //�绰�ز�������
RSA_type secrt_key={0};


static u8 jtt808_msg_body[JTT808_MSG_MAX_BODY_SIZE]={0};

u8 jtt808_global_buf[4096]={0};
u8 jtt808_global_buf_new[4096]={0};



JTT808t_Msg jtt808_msg={0};


/******************************************************************************
 * JTT808_MsgEscape - ��Ϣת��/��ԭ
 *
 * Input: flag, 1 for ת��, 0 for ת�廹ԭ; src; size
 * Output: dst
 * Returns: ת����size
 ******************************************************************************/
int JTT808_MsgEscape(char flag,const u8* src,u8* dst,int size)
{
    int escape_size = size;
    
    if(flag)                                     //1 ת��
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
    else                                         //0 ��ԭ
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
 * JTT808_Check - ���У��
 *
 * Input: src, size
 * Output:
 * Returns: У����
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
 * JTT808_MsgEncode - ��Ϣ���ݱ���
 *
 * Input: src
 * Output: dst
 * Returns: ����������ܴ�С
 * ��ע��src�е�body�Ѿ�����С��ת����headδ����ת����
 * --------------------
 *******************************************************************************/
u32 JTT808_MsgEncode(const JTT808t_Msg *src, u8 *dst)
{
    u8 *tmp=jtt808_global_buf_new,*msg_buf=tmp;
    JTT808t_MsgHead head={0};
    int len=src->head.property.packet?sizeof(JTT808t_MsgHead):(sizeof(JTT808t_MsgHead)-4);  //��Ϣͷ��С


    /*******�Խ��յ���Ϣͷ��С��ת���������ھֲ�����head��*************/
    head.id=htons(*(u16*)&src->head.id);                                    //ת����ϢID
    *(u16*)&head.property=htons(*(u16*)&src->head.property);                //ת����Ϣ������
    memcpy(head.mobile_no,src->head.mobile_no,sizeof(src->head.mobile_no)); //�����ն��ֻ���
    head.serial_no=htons(*(u16*)&src->head.serial_no);                      //ת����Ϣ��ˮ��
    head.package.cnt=htons(*(u16*)&src->head.package.cnt);                  //ת���ְ���Ϣ����
    head.package.num=htons(*(u16*)&src->head.package.num);                  //ת���ְ���Ϣ���
    /*******************************************************************/


    /*����Ϣ�ṹ�������Ϣͷ����Ϣ�����tmp�ﲢ����У��*/
    memcpy(tmp,(char*)&head,len);                            //����С��ת�����head����tmp��
    tmp+=len;
    memcpy(tmp,src->body,src->head.property.size);           //�����յ�����Ϣ�����tmp��
    tmp+=src->head.property.size;
    len+=src->head.property.size;
    memset(tmp,JTT808_Check(msg_buf,len),sizeof(src->checksum)); //У�鲢��У��λ����tmp��
    len+=sizeof(src->checksum);
    /***************************************************/

    
    *dst++=JTT808_MSG_TAG;                  //��ֵ��ʶλ
    len=JTT808_MsgEscape(1,msg_buf,dst,len);//��Ϣͷ����Ϣ�塢У��λת�岢������Ŀ����
    *(dst+len)=JTT808_MSG_TAG;              //��ֵ��ʶλ
    return (len+2);                         //����ת�������ݳ���
}


/******************************************************************************
 * JTT808_MsgDecode - ��Ϣ���ݽ���
 *
 * Input: src, src_size
 * Output: dst
 *���صĽṹ����ֻ����Ϣͷ����Ϣ�������ݣ���Ϣͷ��ת������Ϣ��δת��
 ******************************************************************************/
JTT808t_Err JTT808_MsgDecode(const u8 *src, u32 src_size, JTT808t_Msg *dst)
{
    u8 *tmp=jtt808_global_buf_new;
    JTT808t_MsgHead head={0};
    int len;
    int head_len=sizeof(JTT808t_MsgHead);
    char* p_body=(char*)dst->body;
    
    src++;
    
    if((len=JTT808_MsgEscape(0,src,tmp,src_size-2)) == 0)  //����Ϣͷ����Ϣ�塢У��λ ת�廹ԭ��������tmp��  ����ת�����Ϣͷ����Ϣ���У����ĳ���
		{
				return JTT808_ERR;
		}
    if(tmp[len-1]!=JTT808_Check(tmp,len-1))      //���㲢�ж�У��    ��Ϣͷ����Ϣ�塢У��λ��len���ֽ�   tmp[len-1]ΪԭʼУ����
    {
        printf("У�����!����Ϊ:%02X,ʵ��Ϊ:%02X\r\n",JTT808_Check(tmp,len-1),tmp[len-1]);
        return JTT808_ERR;
    }
    /*���е�����tmp�д������Ϣͷ����Ϣ�塢У��λ��Ϣ*/
    
    memcpy((char*)&head,tmp,sizeof(JTT808t_MsgHead));  //���������tmp��ǰ����Ϣͷ���ȵ����ݸ�ֵ���ֲ�����head��
    head.id=ntohs(*(u16*)&head.id);                    //����ϢID���д�С��ת��
    *(u16*)&head.property=ntohs(*(u16*)&head.property);//����Ϣ�����Խ��д�С��ת��
    head.serial_no=ntohs(*(u16*)&head.serial_no);      //����Ϣ��ˮ�Ž��д�С��ת��
    head.package.cnt=ntohs(*(u16*)&head.package.cnt);  //�Էְ���Ϣ�������д�С��ת��
    head.package.num=ntohs(*(u16*)&head.package.num);  //�Էְ���Ϣ��Ž��д�С��ת��
    if(!head.property.packet)                          //������Ϣ���������¶Էְ���Ϣλ��ֵ
    {
        memset((char*)&head.package,0x00,sizeof(head.package));
        head_len-=4;
    }
	
	if((head_len+head.property.size+1)!=len)
	{
		printf("ԭʼ���ݳ��ȴ��󣡼���Ϊ:%02X,ʵ��Ϊ:%02X",head_len+head.property.size+3,len+2);
		return JTT808_ERR;
	}

    tmp+=head_len;


    memcpy((char*)&dst->head,(char*)&head,sizeof(JTT808t_MsgHead)); //����С��ת����ľֲ�����head��ֵ��Ŀ��ṹ�������Ϣͷ��

//	if(head.property.encrypt==1)									//�����Ϣ���Ѽ��ܣ������
//	{
//		int i=0;

//		decrypt(tmp,head.property.size,secrt_key.key,secrt_key.key_len);

//		printf("\r\n��������Ϊ:");
//		for(i=0;i<head.property.size;i++)
//		{
//			printf("%02X ",tmp[i]);
//		}
//		printf("\r\n");
//	}

    while(head.property.size--)                                     //�����յ�����Ϣ�帳ֵ��Ŀ��ṹ�����Ϣ���С�
    {
        *p_body++=*tmp++;
    }

    return JTT808_OK;               //���صĽṹ����ֻ����Ϣͷ����Ϣ�������ݣ���Ϣͷ��ת������Ϣ��δת��
}


/******************************************************************************
 * JTT808_MsgPacket - ��Ϣ�������
 *
 * Input:  cmd       ��ϢID 
 *         body_src  ��Ϣ��ָ��
 *         body_size ��Ϣ�峤��
 *         package   ��Ϣ�ְ�����
 *		   prio      0:������Ϣ   1��������Ϣ				
 * Output:
 * Returns:
 ******************************************************************************/
JTT808t_Err JTT808_MsgPacket(u16 cmd, const void * body_src, u32 body_size, const JTT808t_MsgPackage package, u8 prio)
{
    JTT808t_Msg* msg=&jtt808_msg;
		u8* msg_buf=jtt808_global_buf;
    int msg_buf_len=0;
		static u16 serial_no = 0;

    if(body_size&&!body_src)         //���Ȳ�Ϊ0���������ݵ�ַ�ǿ�ָ��
    {
        return JTT808_ERR;
    }
    if(package.num&&!package.cnt)    //�зְ���ţ����Ƿְ�����Ϊ0
    {
        return JTT808_ERR;
    }


    /**********************������ϢID����Ϣ�塢�ְ����Զ�msg��ֵ************************************/
    msg->head_tag = JTT808_MSG_TAG;     //��ʶλ
		msg->end_tag = JTT808_MSG_TAG;      //��ʶλ
    msg->head.id=cmd;                   //��ϢID
    msg->head.property.size=body_size;  //��Ϣ�峤��
//		if((cmd!=JTT808_CMD_TERMINAL_REG)&&(cmd!=JTT808_CMD_TERMINAL_AUTHENTICATION)&&(cmd!=JTT808_CMD_TERMINAL_BEAT)&&(secrt_key.use_key))
//				msg->head.property.encrypt=1;	   //��ǰʹ�ü��ܣ����ҵ�ǰ��ϢID��Ϊע�ᡢ��Ȩʱ������Ϣ����м���
//		else 
				msg->head.property.encrypt=0;  //�������ԣ���0 	
//		if((cmd!=JTT808_CMD_TERMINAL_REG)&&(cmd!=JTT808_CMD_TERMINAL_AUTHENTICATION)&&(cmd!=JTT808_CMD_TERMINAL_BEAT)&&(!msg->head.property.encrypt))
//			return JTT808_ERR;     //��ǰ��ϢID��Ϊע���Ȩ������û��ʹ�ü��� ���˳�    
    if(package.cnt!=0||package.num!=0)  //��Ϣ�ְ�����
    {
        msg->head.property.packet=1;
    }
    else
    {
        msg->head.property.packet=0;
    }
    getBCDFromStr(msg->head.mobile_no,(char*)JTT808_Terminal_Param.local_tel);   //�ն��ֻ���
    msg->head.serial_no=serial_no++;                                             //��Ϣ��ˮ��
    memcpy((char*)&msg->head.package,(char*)&package,sizeof(JTT808t_MsgPackage));//��Ϣ�ְ�����
		memcpy(msg->body,body_src,body_size);
	
		//��Ϣ��
//		if(msg->head.property.encrypt==1)											
//		{
//		int i=0;
//		char* pbody=msg->body;

//		printf("\r\nԭʼ����Ϊ:");
//		for(i=0;i<body_size;i++)
//		{
//			printf("%02X ",pbody[i]);
//		}
//		printf("\r\n");

//		encrypt(msg->body,body_size,secrt_key.key,secrt_key.key_len);
//		}												                                                        
    /**********************************************************************************************/


    msg_buf_len=JTT808_MsgEncode(msg, msg_buf);              //���ṹ��msg�����������,��������ݵĳ���

	  JTT808_TXD(msg_buf,msg_buf_len);

	return JTT808_OK;
}



 /******************************************************************************
 * JTT808_Process - ����Э�鴦���߳�
 * --------------------
 * 16-jul-2013, ������ modify: �޸�״̬��
 * 09-jul-2013, ������ written
 * --------------------
 ******************************************************************************/
void JTT808_Process(void)
{
		static JTT808_RX_BUF jtt808_rx_buf={0};

    JTT808_AlarmStateCheck();      //����״̬���
    JTT808_InitiativeCmd();        //������Ϣ����������ע�ᡢ��Ȩ����λ���ٵȡ�

    if(JTT808_get_msg(&jtt808_rx_buf)==1)
    {
        JTT808_msg_prase(&jtt808_rx_buf);
    }
}


void JTT808_init(void)
{
    u16 inital=BKP_ReadBackupRegister(BKP_DR2);

    jtt808_param_init();

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��
    PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ������� 
    if(inital!=0x5050)
    {
        BKP_WriteBackupRegister(BKP_DR2, 0X5050);

        BKP_WriteBackupRegister(BKP_DR3, LOACTION_START_ADDR>>16);  
        BKP_WriteBackupRegister(BKP_DR4, LOACTION_START_ADDR&0xFFFF);       //�ϵ���������ַ

        BKP_WriteBackupRegister(BKP_DR5, LOACTION_START_ADDR>>16);
        BKP_WriteBackupRegister(BKP_DR6, LOACTION_START_ADDR&0xFFFF);       //�ϵ�����д��ַ
    }
    LOACTION_READ_ADDR=BKP_ReadBackupRegister(BKP_DR3)<<16|BKP_ReadBackupRegister(BKP_DR4);   //�ϵ���������ַ
    LOACTION_WRITE_ADDR=BKP_ReadBackupRegister(BKP_DR5)<<16|BKP_ReadBackupRegister(BKP_DR6);  //�ϵ�����д��ַ

	jtt808_msg.body=jtt808_msg_body;
}




