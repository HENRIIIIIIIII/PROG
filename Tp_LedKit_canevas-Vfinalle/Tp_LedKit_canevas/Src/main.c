/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * Canevas pour le TP microcontr?leur LedKit 
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
#include "select_mode.h"


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
void SystemClock_Config(void);
void GestMode(int *mod);		// Pour modifé le mod
int GestDuration(int byV_Time, int mod);				// Pour les blink
void Leds_off(void);														// Pour etaindre les leds
void Leds_on(void); 															// Pour allumé les leds
void chenillard_GD(int byV_Time);
void chenillard_DG(int byV_Time);
void blink_led(int byV_Time);




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
	int mod = 1;
	int mod_P = 1;
	
	//Chenillard
	int byV_Time = 200;
	int byV_Time_P  = 200;
	
	
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
	
	//selon donn?e : durant 4 sec ? l'init, chenillard D0>D3 250ms durant 4 s
	for(i=0 ; i<4 ; i++)
	{
		HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET);
		delay_ms(250);
		HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_RESET);
		delay_ms(250);
		HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_RESET);
		delay_ms(250);
		HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_RESET);
		delay_ms(250);
		HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET);
	}

	//affichage initial
	
	//Init LCD
	lcd_init();
	lcd_clearLine(2);
  lcd_gotoxy(1, 2);
  printf_lcd("Duree : --");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
	 	GestMode(&mod);
		byV_Time = GestDuration(byV_Time,mod);
		
	  // Verifier s'il y un changement de mode ou de temps
    if ((mod != mod_P) || (byV_Time != byV_Time_P))
    {
			/************* Si nous passons aux modes 3 ou 4 **********/
			if ((mod_P == 5 && (mod == 4 || mod == 3)) || (mod_P == 3 && mod == 4))
      {
        if (byV_Time > 800)
        {
          byV_Time = 800;
        }
        else if (byV_Time < 200)
        {
          byV_Time = 200;
        }
        else if (byV_Time % 50 != 0)
        {
          byV_Time = (byV_Time / 50) * 50;
        }
      }
		  /*************** Si nous passons au mode 5 ************/
      else if (mod_P == 4 && mod == 5)
      {
        if (byV_Time < 100)
        {
          byV_Time = 100;
        }
        else if (byV_Time % 100 != 0)
        {
          byV_Time = (byV_Time / 100) * 100;
        }
      }
      mod_P = mod;
      byV_Time_P = byV_Time;
	  }
		
		HAL_GPIO_WritePin(GPIOB, TEST_Pin, GPIO_PIN_SET);
		// On nettoie le LCD
		lcd_clearLine(2);
    lcd_gotoxy(1, 2);
		if(mod < 3)
		{
			printf_lcd("Duree : --");
		}
		else
		{
			printf_lcd("Duree : %d [ms]",byV_Time);
		}
		// mise à jour du LCD
		lcd_clearLine(1);
    lcd_gotoxy(0, 1);
		HAL_GPIO_WritePin(GPIOB, TEST_Pin, GPIO_PIN_RESET);			
		
		/*** GESTION LCD EN FONCTION DU MODE ***/
		 
		
		switch(mod)
		{
			case 1:  // mode 1
				printf_lcd("Mode 1 : OFF");
				Leds_off();
			break;
			
			case 2 : // mode 2
				printf_lcd("Mode 2 : ON ");
				Leds_on();
			break;
			
			case 3 : // mode 3
				printf_lcd("Mode 3 : CHENI G>D");
				chenillard_GD(byV_Time);
			break;
			
			case 4 : // mode 4
				printf_lcd("Mode 4 : CHENI G<D");
				chenillard_DG(byV_Time);
			break;
			
			case 5 : // mode 5
			 	printf_lcd("Mode 5 : CLI_LED");
				blink_led(byV_Time);
			break;
		}
		
		
		//HAL_GPIO_WritePin(GPIOB, TEST_Pin, GPIO_PIN_SET);  
		//*** DELAI GLOBAL DANS LA BOUCLE (LA SEULE ATTENTE) ***
		delay_ms(MAIN_DELAY);   
		//HAL_GPIO_WritePin(GPIOB, TEST_Pin, GPIO_PIN_RESET); 
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
