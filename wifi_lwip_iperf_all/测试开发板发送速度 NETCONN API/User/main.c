
#include "wwd_network.h"
#include "wwd_management.h"
#include "wwd_wifi.h"
#include "wwd_debug.h"
#include "wwd_assert.h"
#include "platform/wwd_platform_interface.h"
#include "RTOS/wwd_rtos_interface.h"
#include "wwd_buffer_interface.h"

#include "./led/bsp_led.h" 
#include "wifi_base_config.h"

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
/* 开发板硬件bsp头文件 */
#include "./led/bsp_led.h"
#include "./usart/bsp_debug_usart.h"
#include "./key/bsp_key.h"

#include "platform_init.h"

/** @endcond */

/*****************************************************
 *               静态函数声明
 ******************************************************/

static void tcpip_init_done( void * arg );
static void startup_thread( void *arg );

/******************************************************
 *               变量定义
 ******************************************************/

static TaskHandle_t  startup_thread_handle;
void BSP_Init();


/**
 * 主函数
 */

int main( void )
{
		BSP_Init();

    /*创建一个初始线程 */									
		BaseType_t xReturn = pdPASS;
		xReturn = xTaskCreate((TaskFunction_t )startup_thread,  /* 任务入口函数 */
													(const char*    )"app_thread",/* 任务名字 */
													(uint16_t       )APP_THREAD_STACKSIZE/sizeof( portSTACK_TYPE ),  /* 任务栈大小 */
													(void*          )NULL,/* 任务入口函数参数 */
													(UBaseType_t    )DEFAULT_THREAD_PRIO, /* 任务的优先级 */
													(TaskHandle_t*  )&startup_thread_handle);/* 任务控制块指针 */ 
		 /* 启动任务调度 */           
		if(pdPASS == xReturn)
			vTaskStartScheduler();   /* 启动任务，开启调度 */
		else
			return -1;  
    /* 除非vTaskStartScheduler中出现错误，否则永远不要到这里 */
    WPRINT_APP_ERROR(("Main() function returned - error" ));
    return 0;
}



/**
 *  初始线程功能-启动LwIP并调用app_main
 * 该函数使用tcpip_init函数启动LwIP，然后等待信号量，
 * 直到LwIP通过调用回调@ref tcpip_init_done指示其已
 * 启动。完成后，将调用应用程序的@ref app_main函数。
 * @param arg :  未使用-符合线程功能原型所需
 */

static void startup_thread( void *arg )
{
    SemaphoreHandle_t lwip_done_sema;
    UNUSED_PARAMETER( arg);

   // WPRINT_APP_INFO( ( "\nPlatform " PLATFORM " initialised\n" ) );
    WPRINT_APP_INFO( ( "Started FreeRTOS" FreeRTOS_VERSION "\n" ) );
    WPRINT_APP_INFO( ( "Starting LwIP " LwIP_VERSION "\n" ) );

    /* 创建信号量以在LwIP完成初始化时发出信号 */
    lwip_done_sema = xSemaphoreCreateCounting( 1, 0 );
    if ( lwip_done_sema == NULL )	
    {
        /*无法创建信号量 */
        WPRINT_APP_ERROR(("Could not create LwIP init semaphore"));
        return;
    }

    /*初始化LwIP，提供回调函数和回调信号量 */
    tcpip_init( tcpip_init_done, (void*) &lwip_done_sema );
    xSemaphoreTake( lwip_done_sema, portMAX_DELAY );
    vQueueDelete( lwip_done_sema );

    /* 运行主应用程序功能 */
    app_main( );

    /* 清理此启动线程*/
    vTaskDelete( startup_thread_handle );
}



/**
 *  LwIP初始化完成回调
 * @param arg : the handle for the semaphore to post (cast to a void pointer)
 */

static void tcpip_init_done( void * arg )
{
    SemaphoreHandle_t * lwip_done_sema = (SemaphoreHandle_t *) arg;
    xSemaphoreGive( *lwip_done_sema );
}


static void BSP_Init(void)
{	

	platform_init_mcu_infrastructure();
	
	/* LED 初始化 */
	LED_GPIO_Config();

	/* 串口初始化	*/
	Debug_USART_Config();
	  
}
