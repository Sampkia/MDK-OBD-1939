#ifndef __UPDATE_H
#define __UPDATE_H

#include "stm32f10x.h"

#define FLASH_UPPARAM_ADDRESS 		0x0803F800		//Flash 127ҳ

typedef enum
{
    UPDATA_BEGIN=0x10,   //��ʼ����    
    UPDATA_ALLOW ,       //����ɹ�
    UPDATA_RUNFIAL,	 	 //����ʧ��
		UPDATA_FAIL,		 //����ʧ��
		UPDATA_VER_FAI,		 //�汾����
		UPDATA_NULL,         //������
		UPDATA_SUCCEED,		 //�����ɹ�
} UPDATA_INFO_TYPE;


typedef __packed struct
{
		u32 inital;				  //��ʼ����־
		u32 updata_sta;			  //��������
		u32 up_mode;              //������ʽ   0:�������·�   1:�����·�   2��USB/SD
		u32 app_size;             //������usb/sd����   
		u8 phone_no[20];          //���Żظ��绰���� �����ڶ����·�

		u8 APN[40];               //APN
		u8 APN_user[40];          //user
		u8 APN_pwd[40];           //pwd

		u8 ip[68];				  //FTP������IP
		u8 port[8];				  //FTP�������˿�
		u8 ftp_user[40];		  //FTP�������û���
		u8 ftp_pwd[40];			  //FTP����������

		u8 file_path[80];		  //�ļ�·��
}UPDATE_PARAM;


extern uint8_t Update_Flag;
extern uint16_t Timer_count;

uint8_t Update_Process(char *path);
void Read_Update_File(void);
void fatfs_test(void);
void USB_SD_Status_Switch(void);
void Set_Update_para(void);

#endif
