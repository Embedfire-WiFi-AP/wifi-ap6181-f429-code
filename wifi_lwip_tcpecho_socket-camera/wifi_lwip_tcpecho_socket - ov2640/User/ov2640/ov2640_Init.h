#ifndef __OV2640_INIT_H
#define	__OV2640_INIT_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
//#include "./systick/bsp_SysTick.h"
//#include "mico.h"


/* Exported constants --------------------------------------------------------*/
#define FSMC_LCD_ADDRESS      LCD_FRAME_BUFFER

/* Image Sizes enumeration */
typedef enum
{
    BMP_QQVGA             =   0x00,	    /* BMP Image QQVGA 160x120 Size */
    BMP_QVGA              =   0x01,      /* BMP Image VGA 800*480 Size */
    JPEG_160x120          =   0x02,	    /* JPEG Image 160x120 Size */
    JPEG_176x144          =   0x03,	    /* JPEG Image 176x144 Size */
    JPEG_320x240          =   0x04,	    /* JPEG Image 320x240 Size */
    JPEG_352x288          =   0x05,	    /* JPEG Image 352x288 Size */
    JPEG_640x480          =   0x06,
    JPEG_800x600          =   0x07,     /* JPEG Image 800x600 Size */
    JPEG_1024x768         =   0x08     	/* JPEG Image 1024x768 Size */
}ImageFormat_TypeDef;

//#define JPEG_IMAGE_FORMAT  JPEG_160x120
#define JPEG_IMAGE_FORMAT  JPEG_320x240
//#define JPEG_IMAGE_FORMAT  JPEG_640x480
//#define JPEG_IMAGE_FORMAT  JPEG_800x600
//#define JPEG_IMAGE_FORMAT  JPEG_1024x768


/*CAMERA interface */
//IIC SCCB
#define CAMERA_I2C                          I2C1
#define CAMERA_I2C_CLK                      RCC_APB1Periph_I2C1
//B6
#define CAMERA_I2C_SCL_PIN                  GPIO_Pin_6
#define CAMERA_I2C_SCL_GPIO_PORT            GPIOB
#define CAMERA_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define CAMERA_I2C_SCL_SOURCE               GPIO_PinSource6
#define CAMERA_I2C_SCL_AF                   GPIO_AF_I2C1
//B7
#define CAMERA_I2C_SDA_PIN                  GPIO_Pin_7
#define CAMERA_I2C_SDA_GPIO_PORT            GPIOB
#define CAMERA_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define CAMERA_I2C_SDA_SOURCE               GPIO_PinSource7
#define CAMERA_I2C_SDA_AF                   GPIO_AF_I2C1

//VSYNC HSYNC PIXCLK
//I5
#define DCMI_VSYNC_GPIO_PORT        	    GPIOI
#define DCMI_VSYNC_GPIO_CLK         	    RCC_AHB1Periph_GPIOI
#define DCMI_VSYNC_GPIO_PIN         	    GPIO_Pin_5
#define DCMI_VSYNC_PINSOURCE        	    GPIO_PinSource5
#define DCMI_VSYNC_AF			            GPIO_AF_DCMI
//A4
#define DCMI_HSYNC_GPIO_PORT        	    GPIOA
#define DCMI_HSYNC_GPIO_CLK         	    RCC_AHB1Periph_GPIOA
#define DCMI_HSYNC_GPIO_PIN         	    GPIO_Pin_4
#define DCMI_HSYNC_PINSOURCE        	    GPIO_PinSource4
#define DCMI_HSYNC_AF			            GPIO_AF_DCMI
//A6
#define DCMI_PIXCLK_GPIO_PORT        	    GPIOA
#define DCMI_PIXCLK_GPIO_CLK         	    RCC_AHB1Periph_GPIOA
#define DCMI_PIXCLK_GPIO_PIN         	    GPIO_Pin_6
#define DCMI_PIXCLK_PINSOURCE        	    GPIO_PinSource6
#define DCMI_PIXCLK_AF			            GPIO_AF_DCMI
//G3
#define DCMI_PWDN_GPIO_PORT        	        GPIOG
#define DCMI_PWDN_GPIO_CLK         	        RCC_AHB1Periph_GPIOG
#define DCMI_PWDN_GPIO_PIN         	        GPIO_Pin_3
//#define DCMI_PWDN_PINSOURCE        	    GPIO_PinSource3
//#define DCMI_PWDN_AF			            GPIO_AF_DCMI

//数据信号线
//H9
#define DCMI_D0_GPIO_PORT        	        GPIOH
#define DCMI_D0_GPIO_CLK         	        RCC_AHB1Periph_GPIOH
#define DCMI_D0_GPIO_PIN         	        GPIO_Pin_9
#define DCMI_D0_PINSOURCE        	        GPIO_PinSource9
#define DCMI_D0_AF			                GPIO_AF_DCMI
//H10
#define DCMI_D1_GPIO_PORT        	        GPIOH
#define DCMI_D1_GPIO_CLK         	        RCC_AHB1Periph_GPIOH
#define DCMI_D1_GPIO_PIN         	        GPIO_Pin_10
#define DCMI_D1_PINSOURCE        	        GPIO_PinSource10
#define DCMI_D1_AF			                GPIO_AF_DCMI
//H11
#define DCMI_D2_GPIO_PORT        	        GPIOH
#define DCMI_D2_GPIO_CLK         	        RCC_AHB1Periph_GPIOH
#define DCMI_D2_GPIO_PIN         	        GPIO_Pin_11
#define DCMI_D2_PINSOURCE        	        GPIO_PinSource11
#define DCMI_D2_AF			                GPIO_AF_DCMI
//H12
#define DCMI_D3_GPIO_PORT        	        GPIOH
#define DCMI_D3_GPIO_CLK         	        RCC_AHB1Periph_GPIOH
#define DCMI_D3_GPIO_PIN         	        GPIO_Pin_12
#define DCMI_D3_PINSOURCE        	        GPIO_PinSource12
#define DCMI_D3_AF			                GPIO_AF_DCMI
//H14
#define DCMI_D4_GPIO_PORT        	        GPIOH
#define DCMI_D4_GPIO_CLK         	        RCC_AHB1Periph_GPIOH
#define DCMI_D4_GPIO_PIN         	        GPIO_Pin_14
#define DCMI_D4_PINSOURCE        	        GPIO_PinSource14
#define DCMI_D4_AF			                GPIO_AF_DCMI
//D3
#define DCMI_D5_GPIO_PORT        	        GPIOD
#define DCMI_D5_GPIO_CLK         	        RCC_AHB1Periph_GPIOD
#define DCMI_D5_GPIO_PIN         	        GPIO_Pin_3
#define DCMI_D5_PINSOURCE        	        GPIO_PinSource3
#define DCMI_D5_AF			                GPIO_AF_DCMI
//I6
#define DCMI_D6_GPIO_PORT        	        GPIOI
#define DCMI_D6_GPIO_CLK         	        RCC_AHB1Periph_GPIOI
#define DCMI_D6_GPIO_PIN         	        GPIO_Pin_6
#define DCMI_D6_PINSOURCE        	        GPIO_PinSource6
#define DCMI_D6_AF			                GPIO_AF_DCMI
//I7
#define DCMI_D7_GPIO_PORT        	        GPIOI
#define DCMI_D7_GPIO_CLK         	        RCC_AHB1Periph_GPIOI
#define DCMI_D7_GPIO_PIN         	        GPIO_Pin_7
#define DCMI_D7_PINSOURCE        	        GPIO_PinSource7
#define DCMI_D7_AF			                GPIO_AF_DCMI


/*debug*/

#define CAMERA_DEBUG_ON             1
#define CAMERA_DEBUG_ARRAY_ON       1
#define CAMERA_DEBUG_FUNC_ON        1


// Log define
#define CAMERA_INFO(fmt,arg...)           printf("<<-CAMERA-INFO->> "fmt"\n",##arg)
#define CAMERA_ERROR(fmt,arg...)          printf("<<-CAMERA-ERROR->> "fmt"\n",##arg)
#define CAMERA_DEBUG(fmt,arg...)          do{\
if(CAMERA_DEBUG_ON)\
    printf("<<-CAMERA-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)

#define CAMERA_DEBUG_ARRAY(array, num)    do{\
                                           int32_t i;\
                                               uint8_t* a = array;\
                                                   if(CAMERA_DEBUG_ARRAY_ON)\
                                                       {\
                                                           printf("<<-CAMERA-DEBUG-ARRAY->>\n");\
                                                               for (i = 0; i < (num); i++)\
                                                                   {\
                                                                       printf("%02x   ", (a)[i]);\
                                                                           if ((i + 1 ) %10 == 0)\
                                                                               {\
                                                                                   printf("\n");\
                                                                               }\
                                                                   }\
                                                                       printf("\n");\
                                                       }\
                                       }while(0)

#define CAMERA_DEBUG_FUNC()               do{\
                                           if(CAMERA_DEBUG_FUNC_ON)\
                                               printf("<<-CAMERA-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


                                           /* Exported constants --------------------------------------------------------*/

                                           /* Use this define to set the maximum delay timeout for the I2C DCMI_OV2640_SingleRandomWrite()
                                           and DCMI_OV2640_SingleRandomRead() operations. Exeeding this timeout delay,
                                           the read/write functions will be aborted and return error code (0xFF).
                                           The period of the delay will depend on the system operating frequency. The following
                                           value has been set for system running at 168 MHz. */
#define DCMI_TIMEOUT_MAX               10000

#define OV2640_DEVICE_WRITE_ADDRESS    0x60
#define OV2640_DEVICE_READ_ADDRESS     0x61

                                           /* OV2640 Registers definition when DSP bank selected (0xFF = 0x00) */
#define OV2640_DSP_R_BYPASS     0x05
#define OV2640_DSP_Qs           0x44
#define OV2640_DSP_CTRL         0x50
#define OV2640_DSP_HSIZE1       0x51
#define OV2640_DSP_VSIZE1       0x52
#define OV2640_DSP_XOFFL        0x53
#define OV2640_DSP_YOFFL        0x54
#define OV2640_DSP_VHYX         0x55
#define OV2640_DSP_DPRP         0x56
#define OV2640_DSP_TEST         0x57
#define OV2640_DSP_ZMOW         0x5A
#define OV2640_DSP_ZMOH         0x5B
#define OV2640_DSP_ZMHH         0x5C
#define OV2640_DSP_BPADDR       0x7C
#define OV2640_DSP_BPDATA       0x7D
#define OV2640_DSP_CTRL2        0x86
#define OV2640_DSP_CTRL3        0x87
#define OV2640_DSP_SIZEL        0x8C
#define OV2640_DSP_HSIZE2       0xC0
#define OV2640_DSP_VSIZE2       0xC1
#define OV2640_DSP_CTRL0        0xC2
#define OV2640_DSP_CTRL1        0xC3
#define OV2640_DSP_R_DVP_SP     0xD3
#define OV2640_DSP_IMAGE_MODE   0xDA
#define OV2640_DSP_RESET        0xE0
#define OV2640_DSP_MS_SP        0xF0
#define OV2640_DSP_SS_ID        0x7F
#define OV2640_DSP_SS_CTRL      0xF8
#define OV2640_DSP_MC_BIST      0xF9
#define OV2640_DSP_MC_AL        0xFA
#define OV2640_DSP_MC_AH        0xFB
#define OV2640_DSP_MC_D         0xFC
#define OV2640_DSP_P_STATUS     0xFE
#define OV2640_DSP_RA_DLMT      0xFF

                                           /* OV2640 Registers definition when sensor bank selected (0xFF = 0x01) */
#define OV2640_SENSOR_GAIN       0x00
#define OV2640_SENSOR_COM1       0x03
#define OV2640_SENSOR_REG04      0x04
#define OV2640_SENSOR_REG08      0x08
#define OV2640_SENSOR_COM2       0x09
#define OV2640_SENSOR_PIDH       0x0A
#define OV2640_SENSOR_PIDL       0x0B
#define OV2640_SENSOR_COM3       0x0C
#define OV2640_SENSOR_COM4       0x0D
#define OV2640_SENSOR_AEC        0x10
#define OV2640_SENSOR_CLKRC      0x11
#define OV2640_SENSOR_COM7       0x12
#define OV2640_SENSOR_COM8       0x13
#define OV2640_SENSOR_COM9       0x14
#define OV2640_SENSOR_COM10      0x15
#define OV2640_SENSOR_HREFST     0x17
#define OV2640_SENSOR_HREFEND    0x18
#define OV2640_SENSOR_VSTART     0x19
#define OV2640_SENSOR_VEND       0x1A
#define OV2640_SENSOR_MIDH       0x1C
#define OV2640_SENSOR_MIDL       0x1D
#define OV2640_SENSOR_AEW        0x24
#define OV2640_SENSOR_AEB        0x25
#define OV2640_SENSOR_W          0x26
#define OV2640_SENSOR_REG2A      0x2A
#define OV2640_SENSOR_FRARL      0x2B
#define OV2640_SENSOR_ADDVSL     0x2D
#define OV2640_SENSOR_ADDVHS     0x2E
#define OV2640_SENSOR_YAVG       0x2F
#define OV2640_SENSOR_REG32      0x32
#define OV2640_SENSOR_ARCOM2     0x34
#define OV2640_SENSOR_REG45      0x45
#define OV2640_SENSOR_FLL        0x46
#define OV2640_SENSOR_FLH        0x47
#define OV2640_SENSOR_COM19      0x48
#define OV2640_SENSOR_ZOOMS      0x49
#define OV2640_SENSOR_COM22      0x4B
#define OV2640_SENSOR_COM25      0x4E
#define OV2640_SENSOR_BD50       0x4F
#define OV2640_SENSOR_BD60       0x50
#define OV2640_SENSOR_REG5D      0x5D
#define OV2640_SENSOR_REG5E      0x5E
#define OV2640_SENSOR_REG5F      0x5F
#define OV2640_SENSOR_REG60      0x60
#define OV2640_SENSOR_HISTO_LOW  0x61
#define OV2640_SENSOR_HISTO_HIGH 0x62

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void start_capture_img(void);
void stop_capture_img(void);
int32_t open_camera(uint32_t *BufferSRC, uint32_t BufferSize);
void DMA_DCMIConfiguration(uint32_t *BufferSRC, uint32_t BufferSize);
int close_camera(void);
void camera_button_event(int event);
int switch_jpeg_size(void);
int send_img_local(uint8_t *buff, int size);
void DCMI_Start(void);
void DCMI_Stop(void);


void OV2640_HW_Init(void);
void OV2640_Reset(void);
void OV2640_Init(void);
int32_t ov2640_SCCB_Init(ImageFormat_TypeDef ImageFormat);
void OV2640_QQVGAConfig(void);
void OV2640_QVGAConfig(void);
void OV2640_JPEGConfig(ImageFormat_TypeDef ImageFormat);
void OV2640_BrightnessConfig(uint8_t Brightness);
void OV2640_ContrastConfig(uint8_t value1, uint8_t value2);
void OV2640_BandWConfig(uint8_t BlackWhite);
void OV2640_ColorEffectsConfig(uint8_t value1, uint8_t value2);
uint8_t OV2640_WriteReg(uint16_t Addr, uint8_t Data);
uint8_t OV2640_ReadReg(uint16_t Addr);
uint16_t ov2640_ReadID(void);

void OV2640_DMA_Buffer1_Config(void);
void OV2640_DMA_Buffer2_Config(void);
void OV2640_DMA_Buffer3_Config(void);

void OV2640_DMA_Config(uint32_t DMA_Memory0BaseAddr,uint16_t DMA_BufferSize);
void OV2640_SpecialEffects(uint8_t mode);
void OV2640_LightMode(uint8_t mode);

extern uint32_t frame_counter;
extern uint32_t dma_start_counter;



void DCMI_DMAConvCplt(void);
int32_t HAL_DCMI_Start_DMA(uint32_t pData, uint32_t Length);
int32_t DCMI_DMA_MultiBufferStart_IT(uint32_t SrcAddress, uint32_t DstAddress, uint32_t SecondMemAddress, uint32_t DataLength);
void DCMI_DMA_Init(uint32_t DMA_Memory0BaseAddr,uint32_t DMA_Memory1BaseAddr,uint16_t DMA_BufferSize);
/** 
  * @brief  HAL DMA State structures definition
  */
typedef enum
{
  HAL_DMA_STATE_RESET             = 0x00,  /*!< DMA not yet initialized or disabled */
  HAL_DMA_STATE_READY             = 0x01,  /*!< DMA initialized and ready for use   */
  HAL_DMA_STATE_READY_MEM0        = 0x11,  /*!< DMA Mem0 process success            */
  HAL_DMA_STATE_READY_MEM1        = 0x21,  /*!< DMA Mem1 process success            */
  HAL_DMA_STATE_READY_HALF_MEM0   = 0x31,  /*!< DMA Mem0 Half process success       */
  HAL_DMA_STATE_READY_HALF_MEM1   = 0x41,  /*!< DMA Mem1 Half process success       */
  HAL_DMA_STATE_BUSY              = 0x02,  /*!< DMA process is ongoing              */
  HAL_DMA_STATE_BUSY_MEM0         = 0x12,  /*!< DMA Mem0 process is ongoing         */
  HAL_DMA_STATE_BUSY_MEM1         = 0x22,  /*!< DMA Mem1 process is ongoing         */
  HAL_DMA_STATE_TIMEOUT           = 0x03,  /*!< DMA timeout state                   */
  HAL_DMA_STATE_ERROR             = 0x04,  /*!< DMA error state                     */
}HAL_DMA_StateTypeDef;
/** @defgroup DCMI_Exported_Types DCMI Exported Types
  * @{
  */
/** 
  * @brief  HAL DCMI State structures definition
  */ 
typedef enum
{
  HAL_DCMI_STATE_RESET             = 0x00,  /*!< DCMI not yet initialized or disabled  */
  HAL_DCMI_STATE_READY             = 0x01,  /*!< DCMI initialized and ready for use    */
  HAL_DCMI_STATE_BUSY              = 0x02,  /*!< DCMI internal processing is ongoing   */
  HAL_DCMI_STATE_TIMEOUT           = 0x03,  /*!< DCMI timeout state                    */
  HAL_DCMI_STATE_ERROR             = 0x04   /*!< DCMI error state                      */
}HAL_DCMI_StateTypeDef;

/** 
  * @brief  HAL DMA Memory definition  
  */ 
typedef enum
{
  MEMORY0      = 0x00,    /*!< Memory 0     */
  MEMORY1      = 0x01,    /*!< Memory 1     */

}HAL_DMA_MemoryTypeDef;


#endif /* __DCMI_OV2640_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

