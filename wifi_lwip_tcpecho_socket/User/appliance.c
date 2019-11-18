#include "wifi_base_config.h"
#include "appliance.h"
#include "stm32f4xx.h"
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "tcpecho.h"

#define PRINTF printf

/**
 * @brief 现象及实验说明
 *
 */
void ExperimentalDescription()
{
	PRINTF("本例程演示电脑客户端发送数据到服开发板务器端\n\n");
	
	PRINTF("开发板为服务器，电脑为客户端\n\n");

	PRINTF("先打开服务端再打开客户端，Network ready IP: 对应的就是IP地址\n\n");	
}
/**
 * @brief app_main
 *
 */
void app_main( void )
{
	
		/*配置wifi lwip信息*/
		Config_WIFI_LwIP_Info();
	
		ExperimentalDescription();
		tcpecho_init();
}
