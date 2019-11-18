//#include "wifi_base_config.h"
//#include "appliance.h"
//#include "stm32f4xx.h"
///* FreeRTOS头文件 */
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "tcpecho.h"

//#define PRINTF printf



///**
// * @brief app_main
// *
// */
//void app_main( void )
//{
//		SAMPLE_PRT("任务已经开始执行");
//		/*配置wifi lwip信息*/
//		Config_WIFI_LwIP_Info();

//		tcpecho_init();
//}



#include "wifi_base_config.h"
#include "appliance.h"
#include "stm32f4xx.h"
#include "debug.h"
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "tcpecho.h"
#include "bsp_sdram.h"
#include "ov2640_Init.h"
				
#include "camera_data_queue.h"
#include "tcp_server.h"
#include "stdlib.h"
#include "string.h"

				
				
/**
 * @brief app_main
 *
 */
#define fire_demo_log(M, ...) custom_log("WIFI", M, ##__VA_ARGS__)


void app_main( void )
{
		host_thread_type_t    wwd_thread;
		camera_data * cambuf;
		int32_t err = kNoErr;

		/*配置wifi lwip信息*/
		Config_WIFI_LwIP_Info();

		err = camera_queue_init();
		cambuf = cbWrite(&cam_circular_buff);
		err = open_camera((uint32_t *)cambuf->head, CAMERA_QUEUE_DATA_LEN);
		SDRAM_Init();//初始化外部sdram
	
//		tcpecho_init();
		host_rtos_create_thread( &wwd_thread, (void *)tcp_server_thread, "TCP_server", NULL,4096, 1);


    while(1)
    {

    }

}




void app_main_( void )
{
		host_thread_type_t    wwd_thread;
		camera_data * cambuf;
		int32_t err = kNoErr;

		/*配置wifi lwip信息*/
		Config_WIFI_LwIP_Info();
		SDRAM_Init();//初始化外部sdram
	
    err = camera_queue_init();
    cambuf = cbWrite(&cam_circular_buff);
    err = open_camera((uint32_t *)cambuf->head, CAMERA_QUEUE_DATA_LEN);

		host_rtos_create_thread( &wwd_thread, (void *)tcp_server_thread, "TCP_server", NULL,4096, 1);
		
    while(1)
    {
//				host_rtos_delay_milliseconds(10);
//				fire_demo_log("camera capture frame counter： frame_count = %d", frame_counter);
//				fire_demo_log("DMA capture frame counter：dma_start_counter= %d", dma_start_counter);

//				if(frame_counter>100)
//				{
//						frame_counter=0;
//						dma_start_counter=0;
//				}

    }

}




