#ifndef __UPDATE_H
#define __UPDATE_H

#include "stm32f10x.h"

#define FLASH_UPPARAM_ADDRESS 		0x0803F800		//Flash 127页

typedef enum
{
    UPDATA_BEGIN=0x10,   //开始升级    
    UPDATA_ALLOW ,       //允许成功
    UPDATA_RUNFIAL,	 	 //运行失败
		UPDATA_FAIL,		 //升级失败
		UPDATA_VER_FAI,		 //版本错误
		UPDATA_NULL,         //无升级
		UPDATA_SUCCEED,		 //升级成功
} UPDATA_INFO_TYPE;


typedef __packed struct
{
		u32 inital;				  //初始化标志
		u32 updata_sta;			  //升级进度
		u32 up_mode;              //升级方式   0:服务器下发   1:短信下发   2：USB/SD
		u32 app_size;             //适用于usb/sd升级   
		u8 phone_no[20];          //短信回复电话号码 适用于短信下发

		u8 APN[40];               //APN
		u8 APN_user[40];          //user
		u8 APN_pwd[40];           //pwd

		u8 ip[68];				  //FTP服务器IP
		u8 port[8];				  //FTP服务器端口
		u8 ftp_user[40];		  //FTP服务器用户名
		u8 ftp_pwd[40];			  //FTP服务器密码

		u8 file_path[80];		  //文件路径
}UPDATE_PARAM;


extern uint8_t Update_Flag;
extern uint16_t Timer_count;

uint8_t Update_Process(char *path);
void Read_Update_File(void);
void fatfs_test(void);
void USB_SD_Status_Switch(void);
void Set_Update_para(void);

#endif
