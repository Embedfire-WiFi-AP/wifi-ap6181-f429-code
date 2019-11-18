/**
******************************************************************************
* @file    ov2640_Init.c
* @author  fire
* @version V1.0
* @date    2015-xx-xx
* @brief   OV2640摄像头驱动
******************************************************************************
* @attention
*
* 实验平台:秉火  STM32 F429 开发板
* 论坛    :http://www.chuxue123.com
* 淘宝    :http://firestm32.taobao.com
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "ov2640_Init.h"
#include "stm32f4xx.h"
#include "camera_data_queue.h"
#include "wifi_base_config.h"

uint32_t frame_counter = 0;
uint32_t line_counter = 0;
uint32_t vs_counter = 0;
uint32_t err_counter = 0;

uint32_t dcmi_start_counter = 0;
uint32_t dma_start_counter = 0;
uint32_t dma_complete_counter = 0;

//uint32_t img_real_len=0;

uint32_t  XferTransferNumber=0;
uint32_t 	XferCount = 0;
uint32_t 	XferSize = 0;
uint32_t 	pBuffPtr = 0;
uint8_t 	DCMI_State;
uint32_t	DMA2_Stream1_State;

extern CircularBuffer cam_circular_buff;

/** @addtogroup DCMI_Camera
* @{
*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
void OV2640_HW_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    /*** Configures the DCMI GPIOs to interface with the OV2640 camera module ***/
    /* Enable DCMI GPIOs clocks */
    RCC_AHB1PeriphClockCmd(DCMI_PWDN_GPIO_CLK|DCMI_VSYNC_GPIO_CLK | DCMI_HSYNC_GPIO_CLK | DCMI_PIXCLK_GPIO_CLK|
                           DCMI_D0_GPIO_CLK| DCMI_D1_GPIO_CLK| DCMI_D2_GPIO_CLK| DCMI_D3_GPIO_CLK|
                               DCMI_D4_GPIO_CLK| DCMI_D5_GPIO_CLK| DCMI_D6_GPIO_CLK| DCMI_D7_GPIO_CLK, ENABLE);

    /*摄像头信号线*/
    GPIO_InitStructure.GPIO_Pin = DCMI_VSYNC_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
    GPIO_Init(DCMI_VSYNC_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DCMI_VSYNC_GPIO_PORT, DCMI_VSYNC_PINSOURCE, DCMI_VSYNC_AF);

    GPIO_InitStructure.GPIO_Pin = DCMI_HSYNC_GPIO_PIN ;
    GPIO_Init(DCMI_HSYNC_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DCMI_HSYNC_GPIO_PORT, DCMI_HSYNC_PINSOURCE, DCMI_HSYNC_AF);

    GPIO_InitStructure.GPIO_Pin = DCMI_PIXCLK_GPIO_PIN ;
    GPIO_Init(DCMI_PIXCLK_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DCMI_PIXCLK_GPIO_PORT, DCMI_PIXCLK_PINSOURCE, DCMI_PIXCLK_AF);

    GPIO_InitStructure.GPIO_Pin = DCMI_PWDN_GPIO_PIN ;
    GPIO_Init(DCMI_PWDN_GPIO_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(DCMI_PWDN_GPIO_PORT,DCMI_PWDN_GPIO_PIN);
    //	  GPIO_PinAFConfig(DCMI_PWDN_GPIO_PORT, DCMI_HSYNC_PINSOURCE, DCMI_HSYNC_AF);

    GPIO_InitStructure.GPIO_Pin = DCMI_D0_GPIO_PIN ;
    GPIO_Init(DCMI_D0_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DCMI_D0_GPIO_PORT, DCMI_D0_PINSOURCE, DCMI_D0_AF);

    GPIO_InitStructure.GPIO_Pin = DCMI_D1_GPIO_PIN ;
    GPIO_Init(DCMI_D1_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DCMI_D1_GPIO_PORT, DCMI_D1_PINSOURCE, DCMI_D1_AF);

    GPIO_InitStructure.GPIO_Pin = DCMI_D2_GPIO_PIN ;
    GPIO_Init(DCMI_D2_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DCMI_D2_GPIO_PORT, DCMI_D2_PINSOURCE, DCMI_D2_AF);

    GPIO_InitStructure.GPIO_Pin = DCMI_D3_GPIO_PIN ;
    GPIO_Init(DCMI_D3_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DCMI_D3_GPIO_PORT, DCMI_D3_PINSOURCE, DCMI_D3_AF);

    GPIO_InitStructure.GPIO_Pin = DCMI_D4_GPIO_PIN ;
    GPIO_Init(DCMI_D4_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DCMI_D4_GPIO_PORT, DCMI_D4_PINSOURCE, DCMI_D4_AF);

    GPIO_InitStructure.GPIO_Pin = DCMI_D5_GPIO_PIN ;
    GPIO_Init(DCMI_D5_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DCMI_D5_GPIO_PORT, DCMI_D5_PINSOURCE, DCMI_D5_AF);

    GPIO_InitStructure.GPIO_Pin = DCMI_D6_GPIO_PIN ;
    GPIO_Init(DCMI_D6_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DCMI_D6_GPIO_PORT, DCMI_D6_PINSOURCE, DCMI_D6_AF);

    GPIO_InitStructure.GPIO_Pin = DCMI_D7_GPIO_PIN ;
    GPIO_Init(DCMI_D7_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(DCMI_D7_GPIO_PORT, DCMI_D7_PINSOURCE, DCMI_D7_AF);

}


void DMA_DCMIConfiguration(uint32_t *BufferSRC, uint32_t BufferSize)
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC | DMA_IT_TE | DMA_IT_HT,DISABLE);
	DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_HTIF1);

	DMA_Cmd(DMA2_Stream1, DISABLE);
	//memset(&DMA_InitStructure, 0, sizeof(DMA_InitTypeDef));

	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&(DCMI->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)BufferSRC;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;    //..... DMA_MemoryDataSize_HalfWord
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);

	DMA_ITConfig(DMA2_Stream1, /*DMA_IT_TC|*/DMA_IT_TE/*|DMA_IT_HT*/, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;//DMA2_Stream1_IRQn_Priority  TBD----- by Ken
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_Cmd(DMA2_Stream1, ENABLE);

}

/**
* @brief  初始化摄像头，初始化mcu的dcmi接口，以及摄像头，但是这里还没开始捕获图像
* @param
* @param
*/
int32_t open_camera(uint32_t *BufferSRC, uint32_t BufferSize)
{
	DCMI_InitTypeDef DCMI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//DMA2
	OV2640_HW_Init();

	DCMI_DeInit();
	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_Low;
	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Rising;
	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_Low;

	DCMI_Init(&DCMI_InitStructure);
//	DMA_DCMIConfiguration(BufferSRC, BufferSize);
  HAL_DCMI_Start_DMA((uint32_t)BufferSRC, BufferSize);

	DCMI_ITConfig(DCMI_IT_FRAME /*| DCMI_IT_OVF | DCMI_IT_ERR |DCMI_IT_VSYNC | DCMI_IT_LINE*/, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;//DCMI_IRQn_Priority;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	if (ov2640_SCCB_Init(JPEG_IMAGE_FORMAT) != kNoErr)
        return  kGeneralErr;

	DCMI_JPEGCmd(ENABLE); //for ov2640

	start_capture_img();

	DCMI_Cmd(ENABLE); //数据开关

	return kNoErr;
}

/**
* @brief  关闭摄像头
* @param
* @param
*/
int close_camera()
{
	DCMI_Cmd(DISABLE);

	DMA_Cmd(DMA2_Stream1, DISABLE);

	DCMI_DeInit();

	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, DISABLE);

	return 0;
}

/**
* @brief  开始捕获图像，如果是连续获取模式，图片会源源不断的发往img_send_thread线程，等待发送出去
* @param
* @param
*/
void start_capture_img()
{
	DCMI_CaptureCmd(ENABLE);
}

/**
* @brief  停止捕获图像
* @param
* @param
*/
void stop_capture_img()
{
	DCMI_CaptureCmd(DISABLE);
}




void DCMI_Start(void)
{
	camera_data *data_p;
	
	/*获取写缓冲区指针，准备写入新数据*/
	data_p = cbWrite(&cam_circular_buff);
	
	if (data_p != NULL)	//若缓冲队列未满，开始传输
	{		
		dma_start_counter++;
		
		/*配置DMA传输*/
		HAL_DCMI_Start_DMA((uint32_t )data_p->head, CAMERA_QUEUE_DATA_LEN);
		DMA_Cmd(DMA2_Stream1, ENABLE);			//重新传输	
	}
	
	 dcmi_start_counter ++;
		
	DCMI_CaptureCmd(ENABLE);
}

void DCMI_Stop(void)
{	
	camera_data *data_p;
	
	/*关闭dma*/
    DMA_Cmd(DMA2_Stream1,DISABLE);
	while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}

//    DCMI_CaptureCmd(DISABLE);	
		

	/*获取正在操作的写指针*/	
	data_p = cbWriteUsing(&cam_circular_buff);
		
	/*计算dma传输的数据个数，用于减少jpeg搜索文件尾的时间*/	
	if (data_p != NULL)	
	{
		data_p->img_dma_len =0; //复位	
		
		if(CAMERA_QUEUE_DATA_LEN>65536*4)	//双dma buff
		{
			data_p->img_dma_len = (XferSize - DMA_GetCurrDataCounter(DMA2_Stream1))*4; //最后一个包
			
			if(dma_complete_counter>=2)
				data_p->img_dma_len += ((dma_complete_counter-1)*XferSize)*4 ;		//	dma_complete_counter个大小为XferSize的包
		}
		else	//单dma buf
			data_p->img_dma_len = (CAMERA_QUEUE_DATA_LEN/4 - DMA_GetCurrDataCounter(DMA2_Stream1))*4;
	}
	
	/*写入缓冲区完毕*/
	cbWriteFinish(&cam_circular_buff);
}



void DMA2_Stream1_IRQHandler(void)
{
      /* Transfer Complete Interrupt management ***********************************/
  if (DMA_GetFlagStatus(DMA2_Stream1,DMA_FLAG_TCIF1)==SET)
  {
//    if(DMA_GetFlagStatus(DMA2_Stream1, DMA_IT_TC) != RESET)
    {
      if(((DMA2_Stream1->CR) & (uint32_t)(DMA_SxCR_DBM)) != 0)
      {

        /* Clear the transfer complete flag */
        DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);

        /* Current memory buffer used is Memory 1 */
        if((DMA2_Stream1->CR & DMA_SxCR_CT) == 0)
        {
          /* Transfer complete Callback for memory0 */
            DCMI_DMAConvCplt();
        }
        /* Current memory buffer used is Memory 0 */
        else if((DMA2_Stream1->CR & DMA_SxCR_CT) != 0) 
        {		

          /* Transfer complete Callback for memory0 */
            DCMI_DMAConvCplt();
        }
      }
      /* Disable the transfer complete interrupt if the DMA mode is not CIRCULAR */
      else
      {
        if(((DMA2_Stream1->CR) & (uint32_t)(DMA_SxCR_DBM)) == 0)
        {
          /* Disable the transfer complete interrupt */
          DMA_ITConfig(DMA2_Stream1, DMA_IT_TC,DISABLE);
        }
        /* Clear the transfer complete flag */
        DMA_ClearFlag(DMA2_Stream1,DMA_FLAG_TCIF1);
        /* Update error code */

        /* Change the DMA state */
        DMA2_Stream1_State = HAL_DMA_STATE_READY_MEM0;
 
        /* Transfer complete callback */
        DCMI_DMAConvCplt();
      }
    }
  }
}


void DCMI_IRQHandler(void)
{
	if(DCMI_GetITStatus(DCMI_IT_FRAME) == SET )
	{
			DCMI_ClearITPendingBit(DCMI_IT_FRAME);
			frame_counter ++;
			//1.停止DCMI传输
			DCMI_Stop();
			//2.根据缓冲区使用情况决定是否开启dma
			DCMI_Start();
			dma_complete_counter=0;
	}

    if(DCMI_GetITStatus(DCMI_IT_LINE) == SET )
	{
        DCMI_ClearITPendingBit(DCMI_IT_LINE);
        line_counter ++;
	}

    if(DCMI_GetITStatus(DCMI_IT_VSYNC) == SET )
	{
        DCMI_ClearITPendingBit(DCMI_IT_VSYNC);
        vs_counter ++;
	}

    if(DCMI_GetITStatus(DCMI_IT_ERR) == SET )
	{
        err_counter ++;
	}

}







/**
  * @brief  Sets the DMA Transfer parameter.
  * @param  hdma:       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream.
  * @param  SrcAddress: The source memory Buffer address
  * @param  DstAddress: The destination memory Buffer address
  * @param  DataLength: The length of data to be transferred from source to destination
  * @retval HAL status
  */
static void DMA_SetConfig(uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
//  /* Clear DBM bit */
//  DMA2_Stream1->CR &= (uint32_t)(~DMA_SxCR_DBM);
	
  /* Configure DMA Stream data length */
  DMA2_Stream1->NDTR = DataLength;

  /* Peripheral to Memory */
    /* Configure DMA Stream source address */
  DMA2_Stream1->PAR = SrcAddress;
    
    /* Configure DMA Stream destination address */
  DMA2_Stream1->M0AR = DstAddress;
}

int32_t HAL_DCMI_Start_DMA(uint32_t pData, uint32_t Length)
{  
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    /* Initialize the second memory address */
    uint32_t SecondMemAddress = 0;

	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC | DMA_IT_TE | DMA_IT_HT,DISABLE);
	DMA_ClearFlag(DMA2_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_TEIF1 | DMA_FLAG_HTIF1 | DMA_FLAG_DMEIF1 | DMA_FLAG_HTIF1);

	DMA_Cmd(DMA2_Stream1, DISABLE);
	
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;   
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;//DMA_FIFOMode_Disable
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);

//	DMA_ITConfig(DMA2_Stream1, /*DMA_IT_TC|*/DMA_IT_TE/*|DMA_IT_HT*/, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;//DMA2_Stream1_IRQn_Priority  TBD----- by Ken
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

//  /* Process Locked */
//  __HAL_LOCK(hdcmi);

//  /* Lock the DCMI peripheral state */
//  hdcmi->State = HAL_DCMI_STATE_BUSY;

//  /* Check the parameters */
//  assert_param(IS_DCMI_CAPTURE_MODE(DCMI_Mode));

//  /* Configure the DCMI Mode */
//  hdcmi->Instance->CR &= ~(DCMI_CR_CM);
//  hdcmi->Instance->CR |=  (uint32_t)(DCMI_Mode);

  /* Set the DMA memory0 conversion complete callback */
//  hdcmi->DMA_Handle->XferCpltCallback = DCMI_DMAConvCplt;

//  /* Set the DMA error callback */
//  hdcmi->DMA_Handle->XferErrorCallback = DCMI_DMAError;

  if(Length/4 <= 0xFFFF)
  {
    /* Enable the DMA Stream */
     /* Disable the peripheral */
		DCMI_CaptureCmd(DISABLE);

  /* Configure the source, destination address and the data length */
  DMA_SetConfig((uint32_t)&DCMI->DR, (uint32_t)pData, Length/4);

  /* Enable all interrupts */
	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);
		
  }
  else /* DCMI_DOUBLE_BUFFER Mode */
  {
    /* Set the DMA memory1 conversion complete callback */
//    hdcmi->DMA_Handle->XferM1CpltCallback = DCMI_DMAConvCplt; 

    /* Initialize transfer parameters */
    XferCount = 1;
    XferSize = Length/4;
    pBuffPtr = pData;

    /* Get the number of buffer */
    while(XferSize > 0xFFFF)
    {
      XferSize  = XferSize/2;
      XferCount = XferCount*2;
    }
		DMA_DoubleBufferModeCmd(DMA2_Stream1,ENABLE);//双缓冲模式开启
    /* Update DCMI counter  and transfer number*/
    XferCount = (XferCount - 2);
    XferTransferNumber = XferCount;

    /* Update second memory address */
    SecondMemAddress = (uint32_t)(pData + (4*XferSize));

    /* Start DMA multi buffer transfer */
    DCMI_DMA_MultiBufferStart_IT((uint32_t)&DCMI->DR, (uint32_t)pData, SecondMemAddress,XferSize);
  }
	
//	DMA_Cmd(DMA2_Stream1, ENABLE);
	
   /* Enable the Peripheral */
		DCMI_CaptureCmd(ENABLE);

  /* Return function status */
  return kNoErr;
}

/**
  * @brief  Change the memory0 or memory1 address on the fly.
  * @param  hdma:       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream.  
  * @param  Address:    The new address
  * @param  memory:     the memory to be changed, This parameter can be one of 
  *                     the following values:
  *                      MEMORY0 /
  *                      MEMORY1
  * @note   The MEMORY0 address can be changed only when the current transfer use
  *         MEMORY1 and the MEMORY1 address can be changed only when the current 
  *         transfer use MEMORY0.
  * @retval HAL status
  */
static void DMA_ChangeMemory(uint32_t Address, HAL_DMA_MemoryTypeDef memory)
{
  if(memory == MEMORY0)
  {
    /* change the memory0 address */
    DMA2_Stream1->M0AR = Address;
  }
  else
  {

    /* change the memory1 address */
    DMA2_Stream1->M1AR = Address;
  }

}

/* Private functions ---------------------------------------------------------*/
/** @defgroup DCMI_Private_Functions DCMI Private Functions
  * @{
  */
  /**
  * @brief  DMA conversion complete callback. 
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *                the configuration information for the specified DMA module.
  * @retval None
  */
void DCMI_DMAConvCplt(void)
{
  uint32_t tmp = 0;
 
  DCMI_State= HAL_DCMI_STATE_READY; 
	

  if(XferCount != 0)
  {
    /* Update memory 0 address location */
    tmp = ((DMA2_Stream1->CR) & DMA_SxCR_CT);
    if(((XferCount % 2) == 0) && (tmp != 0))
    {
      tmp = DMA2_Stream1->M0AR;
      DMA_ChangeMemory((tmp + (8*XferSize)), MEMORY0);
      XferCount--;
			dma_complete_counter++;

    }
    /* Update memory 1 address location */
    else if((DMA2_Stream1->CR & DMA_SxCR_CT) == 0)
    {
      tmp = DMA2_Stream1->M1AR;
      DMA_ChangeMemory((tmp + (8*XferSize)), MEMORY1);
      XferCount--;
			dma_complete_counter++;

    }
  }
  /* Update memory 0 address location */
  else if((DMA2_Stream1->CR & DMA_SxCR_CT) != 0)
  {
    DMA2_Stream1->M0AR = pBuffPtr;
  }
  /* Update memory 1 address location */
  else if((DMA2_Stream1->CR & DMA_SxCR_CT) == 0)
  {
    tmp = pBuffPtr;
    DMA2_Stream1->M1AR = (tmp + (4*XferSize));
    XferCount = XferTransferNumber;
  }

  if(DCMI_GetFlagStatus(DCMI_FLAG_FRAMERI) != RESET)
  {
    /* FRAME Callback */
//    HAL_DCMI_FrameEventCallback(hdcmi);
  }
}

/**
  * @brief  Starts the multi_buffer DMA Transfer with interrupt enabled.
  * @param  hdma:       pointer to a DMA_HandleTypeDef structure that contains
  *                     the configuration information for the specified DMA Stream.  
  * @param  SrcAddress: The source memory Buffer address
  * @param  DstAddress: The destination memory Buffer address
  * @param  SecondMemAddress: The second memory Buffer address in case of multi buffer Transfer  
  * @param  DataLength: The length of data to be transferred from source to destination
  * @retval HAL status
  */
int32_t DCMI_DMA_MultiBufferStart_IT(uint32_t SrcAddress, uint32_t DstAddress, uint32_t SecondMemAddress, uint32_t DataLength)
{

  /* Current memory buffer used is Memory 0 */
  if((DMA2_Stream1->CR & DMA_SxCR_CT) == 0)
  {
    DMA2_Stream1_State = HAL_DMA_STATE_BUSY_MEM0;
  }
  /* Current memory buffer used is Memory 1 */
  else if((DMA2_Stream1->CR & DMA_SxCR_CT) != 0)
  {
    DMA2_Stream1_State = HAL_DMA_STATE_BUSY_MEM1;
  }
	
	//清零，强制重新使用地址0开始接收数据
	DMA2_Stream1->CR &= ~(DMA_SxCR_CT);

  /* Check the parameters */
  assert_param(IS_DMA_BUFFER_SIZE(DataLength));

  /* Disable the peripheral */
  DMA_Cmd(DMA2_Stream1, DISABLE);  

  /* Enable the Double buffer mode */
  DMA2_Stream1->CR |= (uint32_t)DMA_SxCR_DBM;

  /* Configure DMA Stream destination address */
  DMA2_Stream1->M1AR = SecondMemAddress;

  /* Configure the source, destination address and the data length */
  DMA_SetConfig(SrcAddress, DstAddress, DataLength); 
	
  /* Enable the transfer complete interrupt */
  /* Enable the Half transfer interrupt */
  /* Enable the transfer Error interrupt */
  /* Enable the fifo Error interrupt */
  /* Enable the direct mode Error interrupt */
	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE);
  /* Enable the peripheral */
  DMA_Cmd(DMA2_Stream1, ENABLE);

  return kNoErr; 
}


