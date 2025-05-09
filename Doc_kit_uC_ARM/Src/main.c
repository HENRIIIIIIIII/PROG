/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * Canevas pour le TP microcontrôleur LedKit 
  * Cible : Kit ARM Cortex M0, carte 17400
  * 
  * Auteur /date : SCA / 18.12.2020
  * 
  * Etudiant(s) :  
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "17400.h"
#include "stm32delays.h"
#include "stm32driverlcd.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */




/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAIN_DELAY    25



/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

 
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */




/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

	//variables locales
	uint8_t i;
	
	// what state is the machine 
	uint8_t mod = 1;
	
	//Chenillard
	uint32_t byV_Time = 50;
	
	//hold the sitches values
	char byT_Sw[4] = {0,0,0,0};
	//past values
	char byT_SwP[4] = {0,0,0,0};
	
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

	//Init LCD
	lcd_init();
	printf_lcd("TP LedKit 2025");
	lcd_gotoxy(1,2);
	printf_lcd("Luis & Henri");
	lcd_bl_on();
	
	//selon donnée : durant 4 sec à l'init, chenillard D0>D3 250ms durant 4 s
	for(i=0 ; i<10 ; i++)
	{
		HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET);
		delay_ms(50);
		HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_RESET);
		delay_ms(50);
		HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_RESET);
		delay_ms(50);
		HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_RESET);
		delay_ms(50);
		HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET);
	}

	//affichage initial
	
	//Init LCD
	lcd_init();
	lcd_clearLine(2);
  lcd_gotoxy(1, 2);
  printf_lcd("Duree : -");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		//save paste state of switch
		byT_SwP[0] = byT_Sw[0];
		byT_SwP[1] = byT_Sw[1];
		byT_SwP[2] = byT_Sw[2];
		byT_SwP[3] = byT_Sw[3];

		// Reading the switches
		byT_Sw[0] = HAL_GPIO_ReadPin(BTN0_GPIO_Port, BTN0_BIT);
		byT_Sw[1] = HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_BIT);
		byT_Sw[2] = HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_BIT);
		byT_Sw[3] = HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_BIT);
		
		
		// Test if user whant to change mod
		if(((byT_Sw[0] ==0) && (byT_SwP[0] == 1)) && (mod > 1))
		{
			mod--;
			//Reset des time
			byV_Time = 100;
		}
		
		if(((byT_Sw[1] == 0) && (byT_SwP[1] == 1)) && (mod < 5))
		{
			mod++;
			//Reset des time
			byV_Time = 100;
		}
		
		//Chenillard speed changer
		if((mod == 3) || (mod == 4))
		{
			if((byT_Sw[2] != byT_SwP[2]) && (byV_Time > 200))
			{
				byV_Time = byV_Time - 50;
			}
			if((byT_Sw[3] != byT_SwP[3]) && (byV_Time < 800))
			{
				byV_Time = byV_Time + 50;
			}
		}
		else if(mod == 5)
		{
			if((byT_Sw[2] != byT_SwP[2]) && (byV_Time > 100))
			{
				byV_Time = byV_Time - 100;
			}
			if((byT_Sw[3] != byT_SwP[3]) && (byV_Time < 1000))
			{
				byV_Time = byV_Time + 100;
			}
		}
			
		//*** GESTION BOUTONS ET AFFICHAGE ***
		
		//*** GESTION LEDS EN FONCTION DU MODE ***
		switch(mod)
		{
				
			case 1:
				lcd_gotoxy(0,1);
				printf_lcd("Mod :1");
				lcd_clearLine(2);
				lcd_gotoxy(1, 2);
				printf_lcd("Duree : -");
				//LEDS OFF
				HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET);
		
				HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET);
		
				HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET);
		
				HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET);
			break;
			
			case 2:
				lcd_gotoxy(0,1);
				printf_lcd("Mod :2");
				lcd_clearLine(2);
				lcd_gotoxy(1, 2);
				printf_lcd("Duree : -");
				//LEDS ON
				HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET);
		
				HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_RESET);
		
				HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_RESET);
		
				HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_RESET);
		
			break;
			
			case 3:
				lcd_gotoxy(0,1);
				printf_lcd("Mod :3");
				lcd_clearLine(2);
				lcd_gotoxy(1, 2);
				printf_lcd("Duree : byV_Time ");
				//lcd_gotoxy(0,2);
				//printf_lcd("Duree: byV_Time");
				//Chenillard
							
				HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET);
				delay_ms(byV_Time);
				HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_RESET);
				delay_ms(byV_Time);
				HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_RESET);
			  delay_ms(byV_Time);
				
			  HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_RESET);
				delay_ms(byV_Time);
				HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET);
				delay_ms(byV_Time);
		
		
			break;
			
			case 4:
				lcd_gotoxy(0,1);
				printf_lcd("Mod :4");
				lcd_clearLine(2);
				lcd_gotoxy(1, 2);
				printf_lcd("Duree : byV_Time ");
			
			
				//printf_lcd("Duree: byV_Time");
				//Chenillard
			
				//printf_lcd("Duree: <");
				HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET);
				delay_ms(byV_Time);
				HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET);
				delay_ms(byV_Time);
				HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET);
				delay_ms(byV_Time);
				HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET);
				delay_ms(byV_Time);
				HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET);
					
		
			break;
			
			// Mode 5 la féte
			case 5:
				
				lcd_gotoxy(0,1);
				printf_lcd("Mod :5");
				lcd_clearLine(2);
				lcd_gotoxy(1, 2);
				printf_lcd("Duree : byV_Time ");
			
				//Clignotement
		  	HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET);
				delay_ms(byV_Time);
			
				HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET);		
				
			break;
		}
		
		switch (byV_Time)
		{
			case 1:
				
			lcd_clearLine(2);
			printf_lcd("Duree : byV_Time ");
			
			break;
			
			case 2:
				
			lcd_clearLine(2);
			printf_lcd("Duree : byV_Time ");
			
			break;
		}
		
		//*** DELAI GLOBAL DANS LA BOUCLE (LA SEULE ATTENTE) ***
		delay_ms(MAIN_DELAY);
  }
  /* USER CODE END 3 */
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
