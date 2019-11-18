/**
******************************************************************************
* @file    ov2640_SCCB.c
* @author  fire
* @version V1.0
* @date    2015-xx-xx
* @brief   OV2640摄像头SCCB驱动
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
#include "ov2640_reg.h"
#include "camera_data_queue.h"
#include "wifi_base_config.h"

#include "./delay/core_delay.h"  

/**
* @brief  Initializes the OV2640 CAMERA component.
* @param  DeviceAddr: Device address on communication Bus.
* @param  resolution: Camera resolution
* @retval None
*/
int32_t ov2640_SCCB_Init(ImageFormat_TypeDef ImageFormat)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef  I2C_InitStruct;
    uint16_t index;
    /****** Configures the I2C2 used for OV2640 camera module configuration *****/
    /* I2C2 clock enable */
    RCC_APB1PeriphClockCmd(CAMERA_I2C_CLK, ENABLE);

    /* GPIOB clock enable */
    RCC_AHB1PeriphClockCmd(CAMERA_I2C_SCL_GPIO_CLK|CAMERA_I2C_SDA_GPIO_CLK, ENABLE);

    /* Connect I2C2 pins to AF4 */
    GPIO_PinAFConfig(CAMERA_I2C_SCL_GPIO_PORT, CAMERA_I2C_SCL_SOURCE, CAMERA_I2C_SCL_AF);

    /* Configure I2C2 GPIOs */
    GPIO_InitStructure.GPIO_Pin = CAMERA_I2C_SCL_PIN ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(CAMERA_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(CAMERA_I2C_SCL_GPIO_PORT, CAMERA_I2C_SCL_SOURCE, CAMERA_I2C_SCL_AF);

    GPIO_InitStructure.GPIO_Pin = CAMERA_I2C_SDA_PIN ;
    GPIO_Init(CAMERA_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
    GPIO_PinAFConfig(CAMERA_I2C_SDA_GPIO_PORT, CAMERA_I2C_SDA_SOURCE, CAMERA_I2C_SDA_AF);

    /* Configure I2C2 */
    /* I2C DeInit */
    I2C_DeInit(CAMERA_I2C);

    /* Enable the I2C peripheral */
    I2C_Cmd(CAMERA_I2C, ENABLE);

    /* Set the I2C structure parameters */
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0xFE;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_ClockSpeed = 40000;

    /* Initialize the I2C peripheral w/ selected parameters */
    I2C_Init(CAMERA_I2C, &I2C_InitStruct);

    /* Prepare the camera to be configured */
    OV2640_Reset();
    CPU_TS_Tmr_Delay_MS(200);

    index = ov2640_ReadID();

    if (index != OV2640_ID)
        return kGeneralErr;

    OV2640_JPEGConfig(ImageFormat);

    return kNoErr;
}
/**
* @brief  Read the OV2640 Camera identity.
* @param  DeviceAddr: Device address on communication Bus.
* @retval the OV2640 ID
*/
uint16_t ov2640_ReadID(void)
{
    /* Initialize I2C */
    //CAMERA_IO_Init();

    /* Prepare the sensor to read the Camera ID */
    OV2640_WriteReg(OV2640_DSP_RA_DLMT, 0x01);

    /* Get the camera ID */
    return (OV2640_ReadReg(OV2640_SENSOR_PIDH));
}

/**
* @brief  Resets the OV2640 camera.
* @param  None
* @retval None
*/
void OV2640_Reset(void)
{
    OV2640_WriteReg(OV2640_DSP_RA_DLMT, 0x01);
    OV2640_WriteReg(OV2640_SENSOR_COM7, 0x80);
}

/**
* @brief  Configures the OV2640 camera in JPEG mode.
* @param  JPEGImageSize: JPEG image size
* @retval None
*/
void OV2640_JPEGConfig(ImageFormat_TypeDef ImageFormat)
{
    uint32_t i;

    OV2640_Reset();
    CPU_TS_Tmr_Delay_MS(200);
    /* Initialize OV2640 */
    for(i=0; i<(sizeof(OV2640_JPEG_INIT)/2); i++)
    {
        OV2640_WriteReg(OV2640_JPEG_INIT[i][0], OV2640_JPEG_INIT[i][1]);
    }

    /* Set to output YUV422 */
    for(i=0; i<(sizeof(OV2640_YUV422)/2); i++)
    {
        OV2640_WriteReg(OV2640_YUV422[i][0], OV2640_YUV422[i][1]);
    }

    OV2640_WriteReg(0xff, 0x01);
    OV2640_WriteReg(0x15, 0x00);

    /* Set to output JPEG */
    for(i=0; i<(sizeof(OV2640_JPEG)/2); i++)
    {
        OV2640_WriteReg(OV2640_JPEG[i][0], OV2640_JPEG[i][1]);
    }

    CPU_TS_Tmr_Delay_MS(200);

    switch(ImageFormat)
    {
     case JPEG_160x120:
        {
            for(i=0; i<(sizeof(OV2640_160x120_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_160x120_JPEG[i][0], OV2640_160x120_JPEG[i][1]);
            }
            break;
        }
     case JPEG_176x144:
        {
            for(i=0; i<(sizeof(OV2640_176x144_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_176x144_JPEG[i][0], OV2640_176x144_JPEG[i][1]);
            }
            break;
        }
     case JPEG_320x240:
        {
            for(i=0; i<(sizeof(OV2640_320x240_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_320x240_JPEG[i][0], OV2640_320x240_JPEG[i][1]);
            }
            break;
        }
     case JPEG_352x288:
        {
            for(i=0; i<(sizeof(OV2640_352x288_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_352x288_JPEG[i][0], OV2640_352x288_JPEG[i][1]);
            }
            break;
        }
     case JPEG_640x480:
        {
            for(i=0; i<(sizeof(OV2640_640x480_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_640x480_JPEG[i][0], OV2640_640x480_JPEG[i][1]);
            }
            break;
        }

     case JPEG_800x600:
        {
            for(i=0; i<(sizeof(OV2640_800x600_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_800x600_JPEG[i][0], OV2640_800x600_JPEG[i][1]);
            }
            break;
        }
     case JPEG_1024x768:
        {
            for(i=0; i<(sizeof(OV2640_1024x768_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_1024x768_JPEG[i][0], OV2640_1024x768_JPEG[i][1]);
            }
            break;
        }
     default:
        {
            for(i=0; i<(sizeof(OV2640_160x120_JPEG)/2); i++)
            {
                OV2640_WriteReg(OV2640_160x120_JPEG[i][0], OV2640_160x120_JPEG[i][1]);
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
void OV2640_LightMode(uint8_t mode)
{
    switch(mode)
    {

     case 0:     //Auto
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0xc7, 0x00); //AWB on
        break;

     case 1:     //Sunny
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0xc7, 0x40); //AWB off
        OV2640_WriteReg(0xcc, 0x5e);
        OV2640_WriteReg(0xcd, 0x41);
        OV2640_WriteReg(0xce, 0x54);

        break;

     case 2:    //Cloudy
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0xc7, 0x40); //AWB off
        OV2640_WriteReg(0xcc, 0x65);
        OV2640_WriteReg(0xcd, 0x41);
        OV2640_WriteReg(0xce, 0x4f);
        break;

     case 3:   //Office
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0xc7, 0x40); //AWB off
        OV2640_WriteReg(0xcc, 0x52);
        OV2640_WriteReg(0xcd, 0x41);
        OV2640_WriteReg(0xce, 0x66);
        break;

     case 4:   //Home
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0xc7, 0x40); //AWB off
        OV2640_WriteReg(0xcc, 0x42);
        OV2640_WriteReg(0xcd, 0x3f);
        OV2640_WriteReg(0xce, 0x71);
        break;

    }
}

/**
* @brief  特殊效果
* @param  参数用于选择光线模式
*         0x00 Antique   复古
*         0x01 Bluish    偏蓝
*         0x02 Greenish  偏绿
*         0x03 Reddish   偏红
*         0x04 B&W       黑白
*         0x05 Negative  反相
*         0x06 B&W negative  黑白反相
*         0x07 Normal     正常

* @retval None
*/
void OV2640_SpecialEffects(uint8_t mode)
{
    switch(mode)
    {
     case 0:
        // Antique
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0x7c, 0x00);
        OV2640_WriteReg(0x7d, 0x18);
        OV2640_WriteReg(0x7c, 0x05);
        OV2640_WriteReg(0x7d, 0x40);
        OV2640_WriteReg(0x7d, 0xa6);
        break;

     case 1:
        //Bluish
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0x7c, 0x00);
        OV2640_WriteReg(0x7d, 0x18);
        OV2640_WriteReg(0x7c,0x05);
        OV2640_WriteReg(0x7d, 0xa0);
        OV2640_WriteReg(0x7d, 0x40);

        break;

     case 2:
        //Greenish
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0x7c, 0x00);
        OV2640_WriteReg(0x7d, 0x18);
        OV2640_WriteReg(0x7c, 0x05);
        OV2640_WriteReg(0x7d, 0x40);
        OV2640_WriteReg(0x7d, 0x40);
        break;

     case 3:
        // Reddish
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0x7c, 0x00);
        OV2640_WriteReg(0x7d, 0x18);
        OV2640_WriteReg(0x7c, 0x05);
        OV2640_WriteReg(0x7d, 0x40);
        OV2640_WriteReg(0x7d, 0xc0);
        break;

     case 4:
        // B&W
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0x7c, 0x00);
        OV2640_WriteReg(0x7d, 0x18);
        OV2640_WriteReg(0x7c, 0x05);
        OV2640_WriteReg(0x7d, 0x80);
        OV2640_WriteReg(0x7d, 0x80);
        break;

     case 5:
        //Negative
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0x7c, 0x00);
        OV2640_WriteReg(0x7d, 0x40);
        OV2640_WriteReg(0x7c, 0x05);
        OV2640_WriteReg(0x7d, 0x80);
        OV2640_WriteReg(0x7d, 0x80);

        break;

     case 6:
        //B&W negative
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0x7c, 0x00);
        OV2640_WriteReg(0x7d, 0x58);
        OV2640_WriteReg(0x7c, 0x05);
        OV2640_WriteReg(0x7d, 0x80);
        OV2640_WriteReg(0x7d, 0x80);

        break;

     case 7:
        //Normal
        OV2640_WriteReg(0xff, 0x00);
        OV2640_WriteReg(0x7c, 0x00);
        OV2640_WriteReg(0x7d, 0x00);
        OV2640_WriteReg(0x7c, 0x05);
        OV2640_WriteReg(0x7d, 0x80);
        OV2640_WriteReg(0x7d, 0x80);

        break;

    }
}

/**
* @brief  Configures the OV2640 camera brightness.
* @param  Brightness: Brightness value, where Brightness can be:
*         0x40 for Brightness +2,
*         0x30 for Brightness +1,
*         0x20 for Brightness 0,
*         0x10 for Brightness -1,
*         0x00 for Brightness -2,
* @retval None
*/
void OV2640_BrightnessConfig(uint8_t Brightness)
{
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0x7c, 0x00);
    OV2640_WriteReg(0x7d, 0x04);
    OV2640_WriteReg(0x7c, 0x09);
    OV2640_WriteReg(0x7d, Brightness);
    OV2640_WriteReg(0x7d, 0x00);
}

/**
* @brief  Configures the OV2640 camera Black and white mode.
* @param  BlackWhite: BlackWhite value, where BlackWhite can be:
*         0x18 for B&W,
*         0x40 for Negative,
*         0x58 for B&W negative,
*         0x00 for Normal,
* @retval None
*/
void OV2640_BandWConfig(uint8_t BlackWhite)
{
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0x7c, 0x00);
    OV2640_WriteReg(0x7d, BlackWhite);
    OV2640_WriteReg(0x7c, 0x05);
    OV2640_WriteReg(0x7d, 0x80);
    OV2640_WriteReg(0x7d, 0x80);
}

/**
* @brief  Configures the OV2640 camera color effects.
* @param  value1: Color effects value1
* @param  value2: Color effects value2
*         where value1 and value2 can be:
*         value1 = 0x40, value2 = 0xa6 for Antique,
*         value1 = 0xa0, value2 = 0x40 for Bluish,
*         value1 = 0x40, value2 = 0x40 for Greenish,
*         value1 = 0x40, value2 = 0xc0 for Reddish,
* @retval None
*/
void OV2640_ColorEffectsConfig(uint8_t value1, uint8_t value2)
{
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0x7c, 0x00);
    OV2640_WriteReg(0x7d, 0x18);
    OV2640_WriteReg(0x7c, 0x05);
    OV2640_WriteReg(0x7d, value1);
    OV2640_WriteReg(0x7d, value2);
}

/**
* @brief  Configures the OV2640 camera contrast.
* @param  value1: Contrast value1
* @param  value2: Contrast value2
*         where value1 and value2 can be:
*         value1 = 0x28, value2 = 0x0c for Contrast +2,
*         value1 = 0x24, value2 = 0x16 for Contrast +1,
*         value1 = 0x20, value2 = 0x20 for Contrast 0,
*         value1 = 0x1c, value2 = 0x2a for Contrast -1,
*         value1 = 0x18, value2 = 0x34 for Contrast -2,
* @retval None
*/
void OV2640_ContrastConfig(uint8_t value1, uint8_t value2)
{
    OV2640_WriteReg(0xff, 0x00);
    OV2640_WriteReg(0x7c, 0x00);
    OV2640_WriteReg(0x7d, 0x04);
    OV2640_WriteReg(0x7c, 0x07);
    OV2640_WriteReg(0x7d, 0x20);
    OV2640_WriteReg(0x7d, value1);
    OV2640_WriteReg(0x7d, value2);
    OV2640_WriteReg(0x7d, 0x06);
}

/**
* @brief  Writes a byte at a specific Camera register
* @param  Addr: OV2640 register address.
* @param  Data: Data to be written to the specific register
* @retval 0x00 if write operation is OK.
*       0xFF if timeout condition occured (device not connected or bus error).
*/
uint8_t OV2640_WriteReg(uint16_t Addr, uint8_t Data)
{
    uint32_t timeout = DCMI_TIMEOUT_MAX;

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
    I2C_Send7bitAddress(CAMERA_I2C, OV2640_DEVICE_WRITE_ADDRESS, I2C_Direction_Transmitter);

    /* Test on CAMERA_I2C EV6 and clear it */
    timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        if ((timeout--) == 0) return 0xFF;
    }

    /* Send CAMERA_I2C location address LSB */
    I2C_SendData(CAMERA_I2C, (uint8_t)(Addr));

    /* Test on CAMERA_I2C EV8 and clear it */
    timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        if ((timeout--) == 0) return 0xFF;
    }

    /* Send Data */
    I2C_SendData(CAMERA_I2C, Data);

    /* Test on CAMERA_I2C EV8 and clear it */
    timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        if ((timeout--) == 0) return 0xFF;
    }

    /* Send CAMERA_I2C STOP Condition */
    I2C_GenerateSTOP(CAMERA_I2C, ENABLE);

    /* If operation is OK, return 0 */
    return 0;
}

/**
* @brief  Reads a byte from a specific Camera register
* @param  Addr: OV2640 register address.
* @retval data read from the specific register or 0xFF if timeout condition
*         occured.
*/
uint8_t OV2640_ReadReg(uint16_t Addr)
{
    uint32_t timeout = DCMI_TIMEOUT_MAX;
    uint8_t Data = 0;

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
    I2C_Send7bitAddress(CAMERA_I2C, OV2640_DEVICE_READ_ADDRESS, I2C_Direction_Transmitter);

    /* Test on CAMERA_I2C EV6 and clear it */
    timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        if ((timeout--) == 0) return 0xFF;
    }

    /* Send CAMERA_I2C location address LSB */
    I2C_SendData(CAMERA_I2C, (uint8_t)(Addr));

    /* Test on CAMERA_I2C EV8 and clear it */
    timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        if ((timeout--) == 0) return 0xFF;
    }

    /* Clear AF flag if arised */
    CAMERA_I2C->SR1 |= (uint16_t)0x0400;

    /* Generate the Start Condition */
    I2C_GenerateSTART(CAMERA_I2C, ENABLE);

    /* Test on CAMERA_I2C EV6 and clear it */
    timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
        /* If the timeout delay is exeeded, exit with error code */
        if ((timeout--) == 0) return 0xFF;
    }

    /* Send DCMI selcted device slave Address for write */
    I2C_Send7bitAddress(CAMERA_I2C, OV2640_DEVICE_READ_ADDRESS, I2C_Direction_Receiver);

    /* Test on CAMERA_I2C EV6 and clear it */
    timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        if ((timeout--) == 0) return 0xFF;
    }

    /* Prepare an NACK for the next data received */
    I2C_AcknowledgeConfig(CAMERA_I2C, DISABLE);

    /* Test on CAMERA_I2C EV7 and clear it */
    timeout = DCMI_TIMEOUT_MAX; /* Initialize timeout value */
    while(!I2C_CheckEvent(CAMERA_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
        /* If the timeout delay is exeeded, exit with error code */
        if ((timeout--) == 0) return 0xFF;
    }

    /* Prepare Stop after receiving data */
    I2C_GenerateSTOP(CAMERA_I2C, ENABLE);

    /* Receive the Data */
    Data = I2C_ReceiveData(CAMERA_I2C);

    /* return the read data */
    return Data;
}
