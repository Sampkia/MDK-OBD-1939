/**
  ******************************************************************************
  * @file    mass_mal.c
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    29-June-2012
  * @brief   Medium Access Layer interface
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "mass_mal.h"
#include "Peripheral/w25x.h"
#include "include.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t Mass_Memory_Size[2];
uint32_t Mass_Block_Size[2];
uint32_t Mass_Block_Count[2];
__IO uint32_t Status = 0;


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Init(uint8_t lun)
{
  uint16_t status = MAL_OK;
  switch (lun)
  {
    case 0: 
			W25X_Init();
//			status = SD_Init();
      break;
		
    case 1:
      break;
		
    default:
      return MAL_FAIL;
  }
  return status;
}
/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_Write(uint8_t lun,
                   uint32_t Memory_Offset, 
                   uint32_t *Writebuff, 
                   uint16_t Transfer_Length)
{
  switch (lun)
  {
  case 0:
//		W25X_Write((uint8_t*)Writebuff, Memory_Offset, Transfer_Length);
			switch(Flash_ID)
			{
				case W25Q32:
					W25X_Write_NoCheck((uint8_t*)Writebuff, Memory_Offset+W25Q32_OFFSET, Transfer_Length);
//						W25X_Write((uint8_t*)Writebuff, Memory_Offset, Transfer_Length);
					break;
				case W25Q16:
					W25X_Write_NoCheck((uint8_t*)Writebuff, Memory_Offset+W25Q16_OFFSET, Transfer_Length);
//						W25X_Write((uint8_t*)Writebuff, Memory_Offset, Transfer_Length);
					break;
			}
			
			
//			SD_Write((uint8_t *)Writebuff, Memory_Offset, Transfer_Length);
    break;
	
    case 1:
			
      break;
		
  default:
    return MAL_FAIL;
  }
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
uint16_t MAL_Read(uint8_t lun,
                  uint32_t Memory_Offset,
                  uint32_t *Readbuff,
                  uint16_t Transfer_Length)
{
  switch (lun)
  {
		//Æ«ÒÆ2M,
    case 0:
//			W25X_Read((u8*)Readbuff, Memory_Offset, Transfer_Length);
			switch(Flash_ID)
			{
				case W25Q32:
					W25X_Read((u8*)Readbuff, Memory_Offset+W25Q32_OFFSET, Transfer_Length);
//					W25X_Read((u8*)Readbuff, Memory_Offset, Transfer_Length);
					break;
				
				case W25Q16:
					W25X_Read((u8*)Readbuff, Memory_Offset+W25Q16_OFFSET, Transfer_Length);
//					W25X_Read((u8*)Readbuff, Memory_Offset+W25Q16_OFFSET, Transfer_Length);
					break;
			}
			
			
//			SD_Read((uint8_t *)Readbuff, Memory_Offset, Transfer_Length);
      break;
    case 1:
			
			break;
    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t MAL_GetStatus (uint8_t lun)
{
  switch(lun)
  {
  case 0:
		Flash_ID = W25X_ReadId();
		switch(Flash_ID)
		{
			case W25Q64:
				Mass_Block_Size[0] = 4096;
				Mass_Block_Count[0] = 2048;
				Mass_Memory_Size[0]=Mass_Block_Count[0]  * Mass_Block_Size[0];
				return MAL_OK;
			
			case W25Q32:
				Mass_Block_Size[0] = 4096;
				Mass_Block_Count[0] = 1024/2;
				Mass_Memory_Size[0]=Mass_Block_Count[0]  * Mass_Block_Size[0];
				return MAL_OK;
			
			case W25Q16:
				Mass_Block_Size[0] = 4096;
				Mass_Block_Count[0] = 512/2;
				Mass_Memory_Size[0]=Mass_Block_Count[0]  * Mass_Block_Size[0];
				return MAL_OK;
			
			default:
				return MAL_FAIL;
		}
		
		/*
		//SD¿¨
		
    Mass_Block_Size[0] = 512;
    Mass_Memory_Size[0] = (long long)SD_GetSectorCount()*512;
    Mass_Block_Count[0] = Mass_Memory_Size[0] / Mass_Block_Size[0];
		return MAL_OK;
		*/
  case 1:
		return MAL_OK;
  default:
    return MAL_FAIL;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

