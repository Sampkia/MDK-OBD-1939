#include "Config_Parameter.h"
#include "include.h"

static uint8_t Config_Flag = 0;

/*--------------------------------------
函数名：Config_Parameter
输入：无
输出：无
描述：通过Config.txt文件设置目标板参数
--------------------------------------*/
void Config_Parameter(void)
{
		if(!Config_Flag)
		{
				FATFS fs_1;
				FIL file;
				UINT br;
				
				uint32_t sum_lenth, lenth;
				char *pChar1, *pChar2;
			
				Config_Flag = 1;
			
				if(f_mount(&fs_1, "0:", 0) != FR_OK)
				{
					return;
				}
				
				if(f_open(&file, "0:Config.txt", FA_OPEN_ALWAYS | FA_READ) != FR_OK)
				{
						f_mount(NULL, "0:", 0);
						return;
				}
				
				sum_lenth=f_size(&file);
				if(sum_lenth > 4096)
				{
						sum_lenth = 4096;
				}
				
				if(f_read(&file, (void*)jtt808_global_buf, sum_lenth, &br) != FR_OK)
				{
						f_close(&file);
						f_mount(NULL, "1:", 0);
						return;
				}
				
				if(strstr((const char*)jtt808_global_buf, "终端ID号"))
				{
						pChar1 = strStrfN((const char*)jtt808_global_buf, ":", 1);
						pChar2 = strStrfN((const char*)jtt808_global_buf, "\r\n", 1);
					
						if((pChar1)&&(pChar2))
						{ 
								pChar1++;
								lenth=pChar2-pChar1;
							
							  if(lenth)
								{
										memcpy((char *)JTT808_Terminal_Param.terminalID, pChar1, lenth);
                    jtt808_save_para();
								}
						}
				}
				
				if(strstr((const char*)jtt808_global_buf, "IP地址"))
				{
						pChar1 = strStrfN((const char*)jtt808_global_buf, ":", 2);
						pChar2 = strStrfN((const char*)jtt808_global_buf, "\r\n", 2);
					
						if((pChar1)&&(pChar2))
						{ 
								pChar1++;
								lenth=pChar2-pChar1;
							
							  if(lenth)
								{
										memcpy((char *)JTT808_Terminal_Param.main_server_domain, pChar1, lenth);
								}
						}
				}
				
				if(strstr((const char*)jtt808_global_buf, "备用IP"))
				{
						pChar1 = strStrfN((const char*)jtt808_global_buf, ":", 3);
						pChar2 = strStrfN((const char*)jtt808_global_buf, "\r\n", 3);
					
						if((pChar1)&&(pChar2))
						{ 
								pChar1++;
								lenth=pChar2-pChar1;
							
							  if(lenth)
								{
										memcpy((char *)JTT808_Terminal_Param.backup_server_domain, pChar1, lenth);
								}
						}
				}
				
				if(strstr((const char*)jtt808_global_buf, "端口号"))
				{
						pChar1 = strStrfN((const char*)jtt808_global_buf, ":", 4);
						pChar2 = strStrfN((const char*)jtt808_global_buf, "\r\n", 4);
					
						if((pChar1)&&(pChar2))
						{ 
								pChar1++;
								lenth=pChar2-pChar1;
							
							  if(lenth)
								{
										memcpy((char *)JTT808_Terminal_Param.server_tcp_port, pChar1, lenth);
								}
						}
				}
				
				if(strstr((const char*)jtt808_global_buf, "APN"))
				{
						pChar1 = strStrfN((const char*)jtt808_global_buf, ":", 5);
						pChar2 = strStrfN((const char*)jtt808_global_buf, "\r\n", 5);
					
						if((pChar1)&&(pChar2))
						{ 
								pChar1++;
								lenth=pChar2-pChar1;
							
							  if(lenth)
								{
										memcpy((char *)JTT808_Terminal_Param.main_server_apn, pChar1, lenth);
								}
						}
				}
				
				if(strstr((const char*)jtt808_global_buf, "用户名"))
				{
						pChar1 = strStrfN((const char*)jtt808_global_buf, ":", 6);
						pChar2 = strStrfN((const char*)jtt808_global_buf, "\r\n", 6);
					
						if((pChar1)&&(pChar2))
						{ 
								pChar1++;
								lenth=pChar2-pChar1;
							
							  if(lenth)
								{
										memcpy((char *)JTT808_Terminal_Param.main_server_user, pChar1, lenth);
								}
						}
				}
				
				if(strstr((const char*)jtt808_global_buf, "密码"))
				{
						pChar1 = strStrfN((const char*)jtt808_global_buf, ":", 7);
						pChar2 = strStrfN((const char*)jtt808_global_buf, "\r\n", 7);
					
						if((pChar1)&&(pChar2))
						{ 
								pChar1++;
								lenth=pChar2-pChar1;
							
							  if(lenth)
								{
										memcpy((char *)JTT808_Terminal_Param.main_server_psw, pChar1, lenth);
								}
						}
				}
				
				if(strstr((const char*)jtt808_global_buf, "中心SIM卡号"))
				{
						pChar1 = strStrfN((const char*)jtt808_global_buf, ":", 8);
						pChar2 = strStrfN((const char*)jtt808_global_buf, "\r\n", 8);
					
						if((pChar1)&&(pChar2))
						{ 
								pChar1++;
								lenth=pChar2-pChar1;
							
							  if(lenth)
								{
										memcpy((char *)JTT808_Terminal_Param.monitor_platform_tel, pChar1, lenth);
								}
						}
				}
				
				if(strstr((const char*)jtt808_global_buf, "车速系数"))
				{
						pChar1 = strStrfN((const char*)jtt808_global_buf, ":", 9);
						pChar2 = strStrfN((const char*)jtt808_global_buf, "\r\n", 9);
					
						if((pChar1)&&(pChar2))
						{ 
								pChar1++;
								lenth=pChar2-pChar1;
							
							  if(lenth)
								{
										memcpy((char *)JTT808_Terminal_Param.wheel_coefficient, pChar1, lenth);
								}
						}
				}
				
				if(strstr((const char*)jtt808_global_buf, "SIM卡号"))
				{
						pChar1 = strStrfN((const char*)jtt808_global_buf, ":", 10);
						pChar2 = strStrfN((const char*)jtt808_global_buf, "\r\n", 10);
					
						if((pChar1)&&(pChar2))
						{ 
								pChar1++;
								lenth=pChar2-pChar1;
							
							  if(lenth)
								{
										memcpy((char *)JTT808_Terminal_Param.local_tel, pChar1, lenth);
								}
						}
				}
				
				if(strstr((const char*)jtt808_global_buf, "车牌号"))
				{
						pChar1 = strStrfN((const char*)jtt808_global_buf, ":", 11);
						pChar2 = strStrfN((const char*)jtt808_global_buf, "\r\n", 11);
					
						if((pChar1)&&(pChar2))
						{ 
								pChar1++;
								lenth=pChar2-pChar1;
								
							  if(lenth)
								{
										memcpy((char *)JTT808_Terminal_Param.lic_plate, pChar1, lenth);
								}
						}
				}
				
				if(strstr((const char*)jtt808_global_buf, "车牌颜色"))
				{
						pChar1 = strStrfN((const char*)jtt808_global_buf, ":", 12);
						pChar2 = strStrfN((const char*)jtt808_global_buf, "\r\n", 12);
					
						if((pChar1)&&(pChar2))
						{ 
								pChar1++;
								lenth=pChar2-pChar1;
								
							  if(lenth == 1)
								{
										JTT808_Terminal_Param.lic_plate_color = pChar1[0] - 0x30;
								}
						}
				}
				
				jtt808_save_para();
		}
}

