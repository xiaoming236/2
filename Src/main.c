/* Core/Src/main.c */
/* Created by: xiaoming236
 * Created on: 2025-01-13
 * Description: ?????,???????????
 */

#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "app_main.h"
#include "sensors.h"
#include "display.h"
#include "wifi.h"
#include "control.h"

/* Private variables */
SensorData_t sensorData = {0};
static uint32_t lastUpdateTime = 0;

void SystemClock_Config(void);

int main(void)
{
    /* MCU Configuration */
    HAL_Init();
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_ADC1_Init();
    MX_SPI2_Init();
    MX_USART1_UART_Init();

    /* Initialize application modules */
    Sensors_Init();
    Display_Init();
    WIFI_Init();
    Control_Init();

    /* Show boot screen */
    Display_ShowBootScreen();
    HAL_Delay(2000);
    Display_ShowMainScreen();

    /* Infinite loop */
    while (1)
    {
        /* Read sensor data */
        if (Sensors_ReadAll(&sensorData) == HAL_OK)
        {
            /* Check alarm conditions */
            if (Check_Alarm_Condition(&sensorData))
            {
                Control_SetAlarm(1);
                Control_SetFan(255);
                Control_SetWater(1);
                sensorData.alarm_status = 1;
            }
            else
            {
                Control_SetAlarm(0);
                Control_SetFan(0);
                Control_SetWater(0);
                sensorData.alarm_status = 0;
            }

            /* Update display every 500ms */
            if (HAL_GetTick() - lastUpdateTime >= 500)
            {
                Display_UpdateData(&sensorData);
                lastUpdateTime = HAL_GetTick();
            }

            /* Send data to cloud */
            WIFI_SendData(&sensorData);
        }
        
        HAL_Delay(100);
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number */
}
#endif /* USE_FULL_ASSERT */