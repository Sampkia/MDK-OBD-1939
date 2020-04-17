#include "update.h"
#include "include.h"

uint8_t Update_Flag;			//������־λ
uint32_t File_Size;
uint16_t Timer_count = 0;
UPDATE_PARAM up_param = {0};
/*--------------------------------------------
��������Update_Process
����:path, ���ص��̷�
���:��
���������³�����
--------------------------------------------*/
uint8_t Update_Process(char *path)
{
		const TCHAR Update_file[] = "UPDATE.BIN";
		TCHAR filename[20] = {0};
		FATFS fs_0, fs_1;
		FIL file;
		FILINFO fileinfo;
		DIR dir;
		uint32_t br;

		uint8_t i;
		uint32_t page_offst = 0;
		uint8_t buff[512]={0};
		
		if(f_mount(0, &fs_0) != FR_OK)
//		if(f_mount(&fs_0, path, 0) != FR_OK)
		{
				return 0;
		}
		
		if(f_opendir (&dir, path)!=FR_OK)
		{
				Update_Flag=2;
				f_mount(0, NULL);
//				f_mount(NULL, path, 0);
				return 0;
		}
		
		do
		{
			//read
			if(f_readdir(&dir, &fileinfo)!=FR_OK)
			{
//					f_closedir(&dir);	
//					f_mount(NULL, path, 0);
					f_mount(0, NULL);
					Update_Flag=2;
					return 0;
			}
			
			if(strstr(fileinfo.fname, Update_file))					   				//�������ļ�
			{
					memset(filename,0x00,20);
					strcat(filename, path);
					strcat(filename, Update_file);
					if(f_open(&file, filename, FA_READ) != FR_OK)		   	//�������ļ�
					{
							f_close(&file);
//							f_closedir (&dir);
//							f_mount(NULL ,path, 0);
							f_mount(0, NULL);
							Update_Flag=2;
							return 0;
					}
					
					for(i=0; i<50; i++)							  //�����ⲿflash
					{
//							W25X_EraseSector(i);
					}
					
					do												  //��ȡ�����ļ����ݣ�ֱ����ȡ���
					{
							memset(buff,0x00,512);
							if(f_read(&file, (void*)buff, 512, &br)!=FR_OK) 	//��ȡ512�ֽڵ�����
							{
									f_close(&file);
//									f_closedir (&dir);
//									f_mount(NULL, path, 0);
									f_mount(0, NULL);
									
									Update_Flag=2;
									return 0;
							}
							
//							W25X_Write_NoCheck(buff, page_offst*512, br); 			//дFlash
							page_offst++;
					}while(br);
					
					Update_Flag=1;
					File_Size = fileinfo.fsize;
					f_close(&file);
					break;
			}
		}while(fileinfo.fname[0]);
		
//		f_closedir (&dir);
		f_mount(0, NULL);
//		f_mount(NULL, path, 0);
		
		if(Update_Flag!=1)
		{
			Update_Flag=2;
			f_mount(0, NULL);
//			f_mount(NULL,path,0);
			return 0;
		}
		else 	//Update_Flag == 1
		{
			if(f_mount(4, &fs_1) != FR_OK)
//				if(f_mount(&fs_1, "4:", 0) != FR_OK)
				{
						return 0;
				}
				if(f_unlink("4:/update.bin") == FR_OK)		//ɾ�������ļ�
				{
					f_mount(4, NULL);
//					f_mount(NULL,"4:",0);
					ioUart.putChars(1, (unsigned char *)"Remove the update file success!!\r\n", 35);
					Set_Update_para();		//����IAP���������������
					return 1;
				}
//				f_mount(NULL,"4:",0);
				f_mount(4, NULL);
		}
		return 0;
}

/*--------------------------------------------
��������save_update_param
����:��
���:��
����������IAP�������
--------------------------------------------*/
void save_update_param(void)
{
//	WriteFlash(FLASH_UPPARAM_ADDRESS, (uint8_t*)&flash_para, sizeof(flash_para));
}

/*--------------------------------------------
��������Set_Update_para
����:��
���:��
����������IAP�������������������
--------------------------------------------*/
void Set_Update_para(void)
{
	if(Update_Flag == 1)
	{
			memset(&up_param, 0x00, sizeof(up_param));
			up_param.inital = 0x01010101;
			up_param.updata_sta = UPDATA_BEGIN;
			up_param.up_mode = 2;
			up_param.app_size = File_Size;
			save_update_param();
//			NVIC_SystemReset();	
	}
}

/*--------------------------------------------
��������Read_Update_File
����:��
���:��
��������ȡ���µĳ������ⲿFlash���ȽϽϲ���ʹ��
--------------------------------------------*/
void Read_Update_File(void)
{
		uint8_t buff[512];
		uint32_t address = 0;
		uint32_t size = File_Size;
		static uint8_t read_flag = 0;

		FATFS fs_2;
		FIL fdst;
		UINT bw;
	
		if(Update_Flag == 1 && !read_flag)
		{
				if(f_mount(0, &fs_2) != FR_OK)
//				if(f_mount(&fs_2, "5:", 0) != FR_OK)
				{
					return;
				}
			
				if(f_open(&fdst, "5:update.bin", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
				{
					f_mount(0, NULL);
//					f_mount(NULL, "5:", 0);
					return;
				}
				while(size)
				{
						if(size >= 512)
						{
//							W25X_Read(buff, address, 512);
							size -= 512;
							if(f_write(&fdst, buff, size, &bw) != FR_OK)
							{
								f_close(&fdst);
								f_mount(0, NULL);
//								f_mount(NULL, "5:", 0);
								ioUart.putChars(1, (unsigned char *)"Update program failed \r\n", 25);
							}
						}
						else
						{
//							W25X_Read(buff, address, size);
							
							if(f_write(&fdst, buff, size, &bw) != FR_OK)
							{
								f_close(&fdst);
								f_mount(0, NULL);
//								f_mount(NULL, "5:", 0);
								ioUart.putChars(1, (unsigned char *)"Update program failed \r\n", 25);
							}
							size = 0; 
							f_close(&fdst);
							f_mount(0, NULL);
//							f_mount(NULL, "5:", 0);
							ioUart.putChars(1, (unsigned char *)"Update program success \r\n", 30);
						}
//						ioUart.putChars(1, buff, 512);
//						ioUart.putChars(1, (unsigned char *)"\r\n", 2);
						address += 512;
				}
				read_flag = 1;
		}
}


/*--------------------------------------------
��������fatfs_test
����:��
���:��
�������ļ�ϵͳ���Գ���
--------------------------------------------*/
void fatfs_test(void)
{
	FATFS fs_0, fs_1, fs_3;
	FIL fsrc, fdst;
	UINT br, bw;
	
	const TCHAR read_buf[50] = {0};
	const TCHAR write_buf[20] = {"Hello, world"};
	
//	if(f_mount(&fs_0, "0:", 0) != FR_OK)
	if(f_mount(0, &fs_0) != FR_OK)
	{
			return;
	}
	
	if(f_mount(1, &fs_1) != FR_OK)
	{
			return;
	}
	
	
	if(f_open(&fsrc, "0:read.txt", FA_OPEN_EXISTING | FA_READ) != FR_OK)
	{
		return;
	}
	if(f_open(&fdst, "1:write.txt", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
	{
		return;
	}
	
	f_read(&fsrc, (void*)read_buf, 50, &br);
	ioUart.putChars(1, (unsigned char *)read_buf, 50);
	ioUart.putChars(1, (unsigned char *)"\r\n", 2);
	
	f_write(&fdst, write_buf, 13, &bw);
	ioUart.putChars(1, (unsigned char *)write_buf, sizeof(write_buf));
	ioUart.putChars(1, (unsigned char *)"\r\n", 2);
	
	f_close(&fsrc);
  f_close(&fdst);
	
	f_mount(0, NULL);
	f_mount(1, NULL);
//	f_mount(NULL, "0:", 0);
//  f_mount(NULL, "1:", 0);
	
//	if(f_mount(&fs_3, "2:", 0) != FR_OK)
	if(f_mount(2, &fs_3) != FR_OK)
	{
			return;
	}
//	if(f_unlink("2:/read.txt") == FR_OK)
	{
		ioUart.putChars(1, (unsigned char *)"Remove the update file success!!\r\n", 35);
	}
}


/*--------------------------------------------
��������USB_SD_Status_Switch
����:��
���:��
������USB�豸״̬��⣬�л�
--------------------------------------------*/
void USB_SD_Status_Switch(void)
{
		static uint32_t second_count = 0;
		
		if(bDeviceState == UNCONNECTED)
		{
			SD_Store_Process();
			Update_Process("0:");
			if(Update_Flag == 1)
			{
					Set_Update_para();
//				Read_Update_File();
			}
		}
		else
		{
			if(!second_count)
			{
				second_count = TimerSecond;
			}
			if(USB_STATUS_REG & 0x01)
			{
				//���ڶ�
			}
			
			if(USB_STATUS_REG & 0x02)
			{
				//����д
			}
			
			if(timerSecondSub(TimerSecond, second_count) >= 2)
			{
					if(USB_STATUS_REG & 0x10)
					{
							//����״̬
							second_count = TimerSecond;
					}
					if(timerSecondSub(TimerSecond, second_count) >= 5)		//5����û�����ݴ���
					{
							second_count = 0;
							bDeviceState = UNCONNECTED;
					}
					USB_STATUS_REG = 0;
			}
		}
}

