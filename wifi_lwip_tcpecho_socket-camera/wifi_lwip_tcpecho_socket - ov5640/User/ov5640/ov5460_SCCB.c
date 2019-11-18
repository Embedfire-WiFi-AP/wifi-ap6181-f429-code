/**
******************************************************************************
* @file    OV5640_SCCB.c
* @author  fire
* @version V1.0
* @date    2015-xx-xx
* @brief   OV5640摄像头SCCB驱动
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
#include "OV5640_Init.h"
#include "bsp_ov5640.h"
#include "OV5640_reg.h"
#include "camera_data_queue.h"
#include "ov5640_AF.h"
#include "Debug.h"
#include "wifi_base_config.h"

#define ov5640_SCCB_log(M, ...) custom_log("ov5640_SCCB", M, ##__VA_ARGS__)


/**
* @brief  Initializes the OV5640 CAMERA component.
* @param  DeviceAddr: Device address on communication Bus.
* @param  resolution: Camera resolution
* @retval None
*/
OV5640_IDTypeDef OV5640_Camera_ID;
int32_t OV5640_SCCB_Init(ImageFormat_TypeDef ImageFormat)
{
	I2C_InitTypeDef  I2C_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
		/****** 配置I2C，使用I2C与摄像头的SCCB接口通讯*****/
		/* 使能I2C时钟 */
		RCC_APB1PeriphClockCmd(CAMERA_I2C_CLK, ENABLE);
		/* 使能I2C使用的GPIO时钟 */
		RCC_AHB1PeriphClockCmd(CAMERA_I2C_SCL_GPIO_CLK|CAMERA_I2C_SDA_GPIO_CLK, ENABLE);
		/* 配置引脚源 */
		GPIO_PinAFConfig(CAMERA_I2C_SCL_GPIO_PORT, CAMERA_I2C_SCL_SOURCE, CAMERA_I2C_SCL_AF);
		GPIO_PinAFConfig(CAMERA_I2C_SDA_GPIO_PORT, CAMERA_I2C_SDA_SOURCE, CAMERA_I2C_SDA_AF);

		/* 初始化GPIO */
		GPIO_InitStructure.GPIO_Pin = CAMERA_I2C_SCL_PIN ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(CAMERA_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
		GPIO_PinAFConfig(CAMERA_I2C_SCL_GPIO_PORT, CAMERA_I2C_SCL_SOURCE, CAMERA_I2C_SCL_AF);

		GPIO_InitStructure.GPIO_Pin = CAMERA_I2C_SDA_PIN ;
		GPIO_Init(CAMERA_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

		/*初始化I2C模式 */
		I2C_DeInit(CAMERA_I2C); 

		I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStruct.I2C_OwnAddress1 = 0xFE;
		I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStruct.I2C_ClockSpeed = 400000;

		/* 写入配置 */
		I2C_Init(CAMERA_I2C, &I2C_InitStruct);

		/* 使能I2C */
		I2C_Cmd(CAMERA_I2C, ENABLE);

		Delay(50);
    
		OV5640_Camera_ID.PIDH = OV5640_ReadReg(OV5640_SENSOR_PIDH);
		OV5640_Camera_ID.PIDL = OV5640_ReadReg(OV5640_SENSOR_PIDL);
		CAMERA_DEBUG("%x %x",OV5640_Camera_ID.PIDH ,OV5640_Camera_ID.PIDL);
    
    if (OV5640_Camera_ID.PIDH != 0x56)
        return kGeneralErr;
    
    ov5640_SCCB_log("OV5640_ID:0x%x\n",OV5640_Camera_ID.PIDH);
    
    OV5640_JPEGConfig(ImageFormat);
    ov5640_SCCB_log("OV5640_ImageFormat:%x\n",ImageFormat);
    OV5640_AUTO_FOCUS();
    return kNoErr;
}
/**
* @brief  Read the OV5640 Camera identity.
* @param  DeviceAddr: Device address on communication Bus.
* @retval the OV5640 ID
*/
//uint16_t OV5640_ReadID(void)
//{
//    uint16_t sensor_ID=0;
//    /*读取寄存芯片ID*/
//    sensor_ID=OV5640_ReadReg(OV5640_SENSOR_PIDH);
//    /* Get the camera ID */
//    return (sensor_ID);
//}

/**
* @brief  Resets the OV5640 camera.
* @param  None
* @retval None
*/
//void OV5640_Reset(void)
//{
//	/*OV5640软件复位*/
//  OV5640_WriteReg(0x3008, 0x80);
//}

/**
* @brief  Configures the OV5640 camera in JPEG mode.
* @param  JPEGImageSize: JPEG image size
* @retval None
*/
uint8_t sensor_reg[sizeof(OV5640_JPEG_INIT)/4]={0};
//uint32_t sensor_cnt=0;
void OV5640_JPEGConfig(ImageFormat_TypeDef ImageFormat)
{
    uint32_t i;

    OV5640_Reset();
    mico_thread_msleep(200);
    /* Initialize OV5640 */
    for(i=0; i<(sizeof(OV5640_JPEG_INIT)/4); i++)
    {
        OV5640_WriteReg(OV5640_JPEG_INIT[i][0], OV5640_JPEG_INIT[i][1]);
    }
//    for(i=0; i<(sizeof(OV5640_JPEG_INIT)/4); i++)
//    {
//        OV5640_WriteReg(OV5640_JPEG_INIT[i][0], OV5640_JPEG_INIT[i][1]);
//    }
//    for(i=0; i<(sizeof(OV5640_JPEG_INIT)/4); i++)
//    {
//        OV5640_WriteReg(OV5640_JPEG_INIT[i][0], OV5640_JPEG_INIT[i][1]);
//    }
//    for(i=0; i<(sizeof(OV5640_JPEG_INIT)/4); i++)
//    {
//        OV5640_WriteReg(OV5640_JPEG_INIT[i][0], OV5640_JPEG_INIT[i][1]);
//    }
//    for(i=0; i<(sizeof(OV5640_JPEG_INIT)/4); i++)
//    {
//        OV5640_WriteReg(OV5640_JPEG_INIT[i][0], OV5640_JPEG_INIT[i][1]);
//    }

//    for(i=0; i<(sizeof(OV5640_JPEG_INIT)/4); i++)
//    {
//        sensor_reg[i]=OV5640_ReadReg(OV5640_JPEG_INIT[i][0]); 
//        if(OV5640_JPEG_INIT[i][1] != sensor_reg[i])        
//        ov5640_SCCB_log("sensor_reg[%d]:%x-%x\n",i,OV5640_JPEG_INIT[i][1],sensor_reg[i]);
//    }
    
    mico_thread_msleep(10);

    switch(ImageFormat)
    {
     case JPEG_160x120:
        {
            for(i=0; i<(sizeof(OV5640_160x120_JPEG)/4); i++)
            {
                OV5640_WriteReg(OV5640_160x120_JPEG[i][0], OV5640_160x120_JPEG[i][1]);
            }
            break;
        }
     case JPEG_320x240:
        {
            for(i=0; i<(sizeof(OV5640_320x240_JPEG)/4); i++)
            {
                OV5640_WriteReg(OV5640_320x240_JPEG[i][0], OV5640_320x240_JPEG[i][1]);
            }
            break;
        }
     case JPEG_640x480:
        {
            for(i=0; i<(sizeof(OV5640_640x480_JPEG)/4); i++)
            {
                OV5640_WriteReg(OV5640_640x480_JPEG[i][0], OV5640_640x480_JPEG[i][1]);
            }
            break;            
        }

     case JPEG_800x600:
        {
            for(i=0; i<(sizeof(OV5640_800x600_JPEG)/4); i++)
            {
                OV5640_WriteReg(OV5640_800x600_JPEG[i][0], OV5640_800x600_JPEG[i][1]);
            }
            break;
        }
     case JPEG_1024x768:
        {
            for(i=0; i<(sizeof(OV5640_1024x768_JPEG)/4); i++)
            {
                OV5640_WriteReg(OV5640_1024x768_JPEG[i][0], OV5640_1024x768_JPEG[i][1]);
            }
            break;
        }
		case JPEG_1280x960:
        {
            for(i=0; i<(sizeof(OV5640_1280x960_JPEG)/4); i++)
            {
                    OV5640_WriteReg(OV5640_1280x960_JPEG[i][0], OV5640_1280x960_JPEG[i][1]);
            }
            break;
        }
		case JPEG_1600x1200:
        {
            for(i=0; i<(sizeof(OV5640_1600x1200_JPEG)/4); i++)
            {
                    OV5640_WriteReg(OV5640_1600x1200_JPEG[i][0], OV5640_1600x1200_JPEG[i][1]);
            }
            break;
        }
        case JPEG_2048x1536:
        {
            for(i=0; i<(sizeof(OV5640_2048x1536_JPEG)/4); i++)
            {
                    OV5640_WriteReg(OV5640_2048x1536_JPEG[i][0], OV5640_2048x1536_JPEG[i][1]);
            }
            break;
        }
        case JPEG_2320x1740:
        {
            for(i=0; i<(sizeof(OV5640_2320x1740_JPEG)/4); i++)
            {
                    OV5640_WriteReg(OV5640_2320x1740_JPEG[i][0], OV5640_2320x1740_JPEG[i][1]);
            }
            break;
        }
        case JPEG_2592x1944:
        {
            for(i=0; i<(sizeof(OV5640_2592x1944_JPEG)/4); i++)
            {
                    OV5640_WriteReg(OV5640_2592x1944_JPEG[i][0], OV5640_2592x1944_JPEG[i][1]);
            }
            break;
        }
     default:
        {
            for(i=0; i<(sizeof(OV5640_320x240_JPEG)/4); i++)
            {
                OV5640_WriteReg(OV5640_320x240_JPEG[i][0], OV5640_320x240_JPEG[i][1]);
            }
            break;
        }
    }
}

/**
* @brief  配置光线模式
* @param  参数用于选择光线模式
*         0x00 Auto     自动
*         0x01 Sunny    光照
*         0x02 Cloudy   阴天
*         0x03 Office   办公室
*         0x04 Home     家里

* @retval None
*/
void OV5640_LightMode(uint8_t mode)
{
    switch(mode)
    {

     case 0:     //Auto 自动
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x3406, 0x00);
        OV5640_WriteReg(0x3400, 0x04);
        OV5640_WriteReg(0x3401, 0x00);
        OV5640_WriteReg(0x3402, 0x04);
        OV5640_WriteReg(0x3403, 0x00);
        OV5640_WriteReg(0x3404, 0x04);
        OV5640_WriteReg(0x3405, 0x00);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // lanuch group 3
        break;

     case 1:     //Sunny 日光
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x3406, 0x01);
        OV5640_WriteReg(0x3400, 0x06);
        OV5640_WriteReg(0x3401, 0x1c);
        OV5640_WriteReg(0x3402, 0x04);
        OV5640_WriteReg(0x3403, 0x00);
        OV5640_WriteReg(0x3404, 0x04);
        OV5640_WriteReg(0x3405, 0xf3);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // lanuch group 3
        break;

     case 2:    //Office 办公室
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x3406, 0x01);
        OV5640_WriteReg(0x3400, 0x05);
        OV5640_WriteReg(0x3401, 0x48);
        OV5640_WriteReg(0x3402, 0x04);
        OV5640_WriteReg(0x3403, 0x00);
        OV5640_WriteReg(0x3404, 0x07);
        OV5640_WriteReg(0x3405, 0xcf);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // lanuch group 3
        break;

     case 3:   //Cloudy 阴天
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x3406, 0x01);
        OV5640_WriteReg(0x3400, 0x06);
        OV5640_WriteReg(0x3401, 0x48);
        OV5640_WriteReg(0x3402, 0x04);
        OV5640_WriteReg(0x3403, 0x00);
        OV5640_WriteReg(0x3404, 0x04);
        OV5640_WriteReg(0x3405, 0xd3);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // lanuch group 3
        break;

     case 4:   //Home 室内
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x3406, 0x01);
        OV5640_WriteReg(0x3400, 0x04);
        OV5640_WriteReg(0x3401, 0x10);
        OV5640_WriteReg(0x3402, 0x04);
        OV5640_WriteReg(0x3403, 0x00);
        OV5640_WriteReg(0x3404, 0x08);
        OV5640_WriteReg(0x3405, 0x40);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // lanuch group 3
        break;
    }
}

/**
* @brief  特殊效果
* @param  参数用于选择光线模式
* @retval None
*/
void OV5640_SpecialEffects(uint8_t mode)
{
    switch(mode)
    {

     case 0:     //Normal (off) 正常
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x5580, 0x06);
        OV5640_WriteReg(0x5583, 0x40); // sat U
        OV5640_WriteReg(0x5584, 0x10); // sat V
        OV5640_WriteReg(0x5003, 0x08);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // launch group 3
        break;

     case 1:     //Blueish (cool light) 冷色
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x5580, 0x1e);
        OV5640_WriteReg(0x5583, 0xa0);
        OV5640_WriteReg(0x5584, 0x40);
        OV5640_WriteReg(0x5003, 0x08);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // launch group 3
        break;

     case 2:    //Redish (warm) 暖色
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x5580, 0x1e);
        OV5640_WriteReg(0x5583, 0x80);
        OV5640_WriteReg(0x5584, 0xc0);
        OV5640_WriteReg(0x5003, 0x08);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // launch group 3
        break;

     case 3:   //Black and white 黑白
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x5580, 0x1e);
        OV5640_WriteReg(0x5583, 0x80);
        OV5640_WriteReg(0x5584, 0x80);
        OV5640_WriteReg(0x5003, 0x08);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // launch group 3
        break;

     case 4:   //Sepia 泛黄        
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x5580, 0x1e);
        OV5640_WriteReg(0x5583, 0x40);
        OV5640_WriteReg(0x5584, 0xa0);
        OV5640_WriteReg(0x5003, 0x08);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // launch group 3
        break;
     
    case 5:   //Negative 反色     
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x5580, 0x40);
        OV5640_WriteReg(0x5003, 0x08);
        OV5640_WriteReg(0x5583, 0x40); // sat U
        OV5640_WriteReg(0x5584, 0x10); // sat V
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // launch group 3
        break;

    case 6:   //Greenish 偏绿    
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x5580, 0x1e);
        OV5640_WriteReg(0x5583, 0x60);
        OV5640_WriteReg(0x5584, 0x60);
        OV5640_WriteReg(0x5003, 0x08);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // launch group 3
        break;

    case 7:   //Overexposure 过曝      
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x5580, 0x1e);
        OV5640_WriteReg(0x5583, 0xf0);
        OV5640_WriteReg(0x5584, 0xf0);
        OV5640_WriteReg(0x5003, 0x08);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // launch group 3
        break;

    case 8:   //Solarize 正负片叠加     
        OV5640_WriteReg(0x3212, 0x03); // start group 3
        OV5640_WriteReg(0x5580, 0x06);
        OV5640_WriteReg(0x5583, 0x40); // sat U
        OV5640_WriteReg(0x5584, 0x10); // sat V
        OV5640_WriteReg(0x5003, 0x09);
        OV5640_WriteReg(0x3212, 0x13); // end group 3
        OV5640_WriteReg(0x3212, 0xa3); // launch group 3
        break;
    }
}

/**
  * @brief  写一字节数据到OV5640寄存器
  * @param  Addr: OV5640 的寄存器地址
  * @param  Data: 要写入的数据
  * @retval 返回0表示写入正常，0xFF表示错误
  */
//uint8_t OV5640_WriteReg(uint16_t Addr, uint8_t Data)
//{
//	uint32_t timeout = DCMI_TIMEOUT_MAX;
//  
//	/* Generate the Start Condition */
//	I2C_GenerateSTART(CAMERA_I2C, ENABLE);

//	/* Test on CAMERA_I2C EV5 and clear it */
//	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
//	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_MODE_SELECT))
//	{
//		/* If the timeout delay is exeeded, exit with error code */
//		if ((timeout--) == 0) return 0xFF;
//	}
//   
//	/* Send DCMI selcted device slave Address for write */
//	I2C_Send7bitAddress(CAMERA_I2C, OV5640_DEVICE_ADDRESS, I2C_Direction_Transmitter);
// 
//	/* Test on CAMERA_I2C EV6 and clear it */
//	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
//	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//	{
//		/* If the timeout delay is exeeded, exit with error code */
//		if ((timeout--) == 0) return 0xFF;
//	}
// 
//	/* Send CAMERA_I2C location address MSB */
//	I2C_SendData(CAMERA_I2C, (uint8_t)( (Addr >> 8) & 0xFF) );

//	/* Test on CAMERA_I2C EV8 and clear it */
//	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
//	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//	{
//		/* If the timeout delay is exeeded, exit with error code */
//		if ((timeout--) == 0) return 0xFF;
//	}
//	//--------------------------------------------------------
//	/* Send I2C1 location address LSB */
//	I2C_SendData( CAMERA_I2C, (uint8_t)(Addr & 0xFF) );
//	/* Test on I2C1 EV8 and clear it */
//	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
//	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//	{
//		/* If the timeout delay is exeeded, exit with error code */
//		if ((timeout--) == 0) return 0xFF;
//	} 
//  

//	//--------------------------------------------------------

//	/* Send Data */
//	I2C_SendData(CAMERA_I2C, Data);    

//	/* Test on CAMERA_I2C EV8 and clear it */
//	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
//	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//	{
//		/* If the timeout delay is exeeded, exit with error code */
//		if ((timeout--) == 0) return 0xFF;
//	}  
// 
//	/* Send I2C1 STOP Condition */
//	I2C_GenerateSTOP(CAMERA_I2C, ENABLE);
//  
//	/* If operation is OK, return 0 */
//	return 0;
//}
/**
  * @brief  从OV5640寄存器中读取一个字节的数据
  * @param  Addr: 寄存器地址
  * @retval 返回读取得的数据
  */
//uint8_t OV5640_ReadReg(uint16_t Addr)
//{
//	uint32_t timeout = DCMI_TIMEOUT_MAX;
//	uint8_t Data = 0;

//	/* Generate the Start Condition */
//	I2C_GenerateSTART(CAMERA_I2C, ENABLE);

//	/* Test on CAMERA_I2C EV5 and clear it */
//	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
//	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_MODE_SELECT))
//	{
//		/* If the timeout delay is exeeded, exit with error code */
//		if ((timeout--) == 0) return 0xFF;
//	} 
//  
//	/* Send DCMI selcted device slave Address for write */
//	I2C_Send7bitAddress(CAMERA_I2C, OV5640_DEVICE_ADDRESS, I2C_Direction_Transmitter);
// 
//	/* Test on I2C1 EV6 and clear it */
//	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
//	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//	{
//		/* If the timeout delay is exeeded, exit with error code */
//		if ((timeout--) == 0) return 0xFF;
//	} 

//	/* Send I2C1 location address MSB */
//	I2C_SendData( CAMERA_I2C, (uint8_t)((Addr>>8) & 0xFF) );

//	/* Test on I2C1 EV8 and clear it */
//	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
//	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//	{
//		/* If the timeout delay is exeeded, exit with error code */
//		if ((timeout--) == 0) return 0xFF;
//	} 
//  
//	/* Clear AF flag if arised */
//	CAMERA_I2C->SR1 |= (uint16_t)0x0400; 
//  
////--------------------------------------------------------
//	/* Send I2C1 location address LSB */
//	I2C_SendData( CAMERA_I2C, (uint8_t)(Addr & 0xFF) );
//	/* Test on I2C1 EV8 and clear it */
//	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
//	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//	{
//		/* If the timeout delay is exeeded, exit with error code */
//		if ((timeout--) == 0) return 0xFF;
//	} 
//  
//	/* Clear AF flag if arised */
//	CAMERA_I2C->SR1 |= (uint16_t)0x0400;
//	//--------------------------------------------------------

//	/* Generate the Start Condition */
//	I2C_GenerateSTART(CAMERA_I2C, ENABLE);
//  
//	/* Test on I2C1 EV6 and clear it */
//	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
//	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_MODE_SELECT))
//	{
//		/* If the timeout delay is exeeded, exit with error code */
//		if ((timeout--) == 0) return 0xFF;
//	} 
//  
//	/* Send DCMI selcted device slave Address for write */
//	I2C_Send7bitAddress(CAMERA_I2C, OV5640_DEVICE_ADDRESS, I2C_Direction_Receiver);
//   
//	/* Test on I2C1 EV6 and clear it */
//	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
//	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
//	{
//		/* If the timeout delay is exeeded, exit with error code */
//		if ((timeout--) == 0) return 0xFF;
//	}  
// 
//	/* Prepare an NACK for the next data received */
//	I2C_AcknowledgeConfig(CAMERA_I2C, DISABLE);
//	
//	/* Test on I2C1 EV7 and clear it */
//	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
//	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
//	{
//		/* If the timeout delay is exeeded, exit with error code */
//		if ((timeout--) == 0) return 0xFF;
//	}   
//    
//	/* Prepare Stop after receiving data */
//	I2C_GenerateSTOP(CAMERA_I2C, ENABLE);

//	/* Receive the Data */
//	Data = I2C_ReceiveData(CAMERA_I2C);

//	/* return the read data */
//	return Data;
//}






/**

  * @brief  将固件写入到OV5640片内MCU
  * @param  Addr: OV5640 的MCU基地址0x8000
  * @param  Data: 要写入的数据
  * @retval 返回0表示写入正常，0xFF表示错误
  */
uint8_t OV5640_WriteFW(uint8_t *pBuffer ,uint16_t BufferSize)
{
	uint32_t timeout = DCMI_TIMEOUT_MAX;
    uint16_t Addr=0x8000;
    
	/* Generate the Start Condition */
	I2C_GenerateSTART(CAMERA_I2C, ENABLE);

	/* Test on CAMERA_I2C EV5 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
	}
   
	/* Send DCMI selcted device slave Address for write */
	I2C_Send7bitAddress(CAMERA_I2C, OV5640_DEVICE_ADDRESS, I2C_Direction_Transmitter);
 
	/* Test on CAMERA_I2C EV6 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
	}
 
	/* Send CAMERA_I2C location address MSB */
	I2C_SendData(CAMERA_I2C, (uint8_t)( (Addr >> 8) & 0xFF) );

	/* Test on CAMERA_I2C EV8 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
	}
	//--------------------------------------------------------
	/* Send I2C1 location address LSB */
	I2C_SendData( CAMERA_I2C, (uint8_t)(Addr & 0xFF) );
	/* Test on I2C1 EV8 and clear it */
	timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
	while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		/* If the timeout delay is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
	} 
  
	//--------------------------------------------------------
/* While there is data to be written */
  while(BufferSize--)  
  {
    /* Send Data */
    I2C_SendData(CAMERA_I2C, *pBuffer);  
      
    /* Point to the next byte to be written */
    pBuffer++; 
    /* Test on CAMERA_I2C EV8 and clear it */
    timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        if ((timeout--) == 0) return 0xFF;
    }  
   }
	/* Send I2C1 STOP Condition */
	I2C_GenerateSTOP(CAMERA_I2C, ENABLE);
  
	/* If operation is OK, return 0 */
	return 0;
}
