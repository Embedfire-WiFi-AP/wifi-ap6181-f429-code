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
 * @brief app_main
 *
 */
void app_main( void )
{
		SAMPLE_PRT("任务已经开始执行");
		/*配置wifi lwip信息*/
		Config_WIFI_LwIP_Info();

		tcpecho_init();
}
