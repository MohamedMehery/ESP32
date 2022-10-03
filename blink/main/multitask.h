#ifndef _MT_H_
#define _MT_H_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

#define STACK_SIZE 4096
#define BLINK_GPIO GPIO_NUM_2
const char *TAG = "XXXXXXXXXXXXXX";

 uint8_t s_led_state = 0;
extern uint8_t s_led_state;

void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);

}


// Task to be created.
void vToggle_led_core0( void * pvParameters )
{
  for( ;; )
  {
      // Task code goes here.
	  blink_led();
	  ESP_LOGI(TAG, " CORE-0 toggle led!\n");
	  vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Function that creates a task.
void vOtherFunction( void )
{
// uint8_t ucParameterToPass;
TaskHandle_t xHandle = NULL;

	  // Create the task, storing the handle.  Note that the passed parameter ucParameterToPass
	  // must exist for the lifetime of the task, so in this case is declared static.  If it was just an
	  // an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
	  // the new task attempts to access it.
		xTaskCreatePinnedToCore( vToggle_led_core0, "vToggle_led_core0", STACK_SIZE, NULL , 3, NULL, 0 );
//	  configASSERT( xHandle );

	  // Use the handle to delete the task.
//	  if( xHandle != NULL )
//	  {
//		 vTaskDelete( xHandle );
//	  }
}

// Task to be created.
void vprintlog( void * pvParameters )
{
  for( ;; )
  {
      // Task code goes here.
	  ESP_LOGI(TAG, " CORE-1 Turning the LED %s!\n", s_led_state == true ? "ON" : "OFF");
	  vTaskDelay(1000 / portTICK_PERIOD_MS);
	  s_led_state = !s_led_state;

  }
}

// Function that creates a task.
void v1therFunction( void )
{
// uint8_t ucParameterToPass;
TaskHandle_t xHandle = NULL;

  // Create the task, storing the handle.  Note that the passed parameter ucParameterToPass
  // must exist for the lifetime of the task, so in this case is declared static.  If it was just an
  // an automatic stack variable it might no longer exist, or at least have been corrupted, by the time
  // the new task attempts to access it.
	xTaskCreatePinnedToCore( vprintlog, "vprintlog", STACK_SIZE, NULL , 3, &xHandle, 1 );
//  configASSERT( xHandle );

  // Use the handle to delete the task.
//  if( xHandle != NULL )
//  {
//     vTaskDelete( xHandle );
//  }
}

#endif
