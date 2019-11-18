#ifndef __OV5640_INIT_H
#define	__OV5640_INIT_H
#include "bsp_ov5640.h"
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

void start_capture_img(void);

void OV5640_JPEGConfig(ImageFormat_TypeDef ImageFormat);

void DCMI_DMAConvCplt(void);
int32_t HAL_DCMI_Start_DMA(uint32_t pData, uint32_t Length);
int32_t DCMI_DMA_MultiBufferStart_IT(uint32_t SrcAddress, uint32_t DstAddress, uint32_t SecondMemAddress, uint32_t DataLength);


#endif /* __DCMI_OV5640_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

