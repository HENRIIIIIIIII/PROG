/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "17400.h"
#include "stm32delays.h"
#include "stm32driverlcd.h"


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//typedef enum {Intialisation = 1, Execution = 2, Attente = 3}State;
//typedef enum {OneDigi = 1, TwoDigi = 2, ThreeDigi = 3, FourDigi = 4}NbrDigi;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t Etat = Intialisation;;
uint8_t Fin_Init = 0;
uint8_t Calibration = 0;
uint8_t Calibration_1 = 0;
uint8_t One = 0;
uint8_t Two = 0;
uint8_t Three = 0;
uint8_t Four = 0;
uint16_t Unite;
uint16_t Dixaines;
uint16_t Centaines;
uint16_t Millier;
uint16_t ValLimit1;
uint16_t ValLimit2;
uint16_t valVref_mV = V_ALIM;
uint16_t Count = 0;
uint16_t Compteur = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// ----------------------------------------------------------------
// Conversion tension de LSB en mV. Utilise la variable globale "valVref_mV"
// 0 -> 0 mV
// 4095 -> 3300 mV
uint16_t ConvAdcMilliVolt(uint16_t nLsb)
{
	
	nLsb = (nLsb*80566)/100000;
	//nLsb = (nLsb*8)/10;
	
	return(nLsb);
}

// ----------------------------------------------------------------
// Conversion de mV en chaine de caractères (ou structure) exprimée 
// en V sur nDigits digits. Prévu pour tension entre 0 et 3,3V (=> u_mV entre 0 et 3300).
// La sortie contiendra donc unité + évt point décimal, dixièmes, centièmes, millièmes,
//  selon le nb de digits demandés.
void ConvMilliVoltVolt(uint16_t u_mV, uint8_t nDigits, S_nbrReel* X)
{	
	// *** A COMPLETER ! ***
	Unite = u_mV/1000;
	
	ValLimit1 = u_mV - (Unite*1000);
	Dixaines = ValLimit1/100;
	
	ValLimit2 = ValLimit1 - (Dixaines*100);
	Centaines = ValLimit2/10;
	
	Millier = ValLimit2 - (Centaines*10);
	
	
	switch(nDigits)
	{
		case 1 :
			if(Dixaines >= 5)
			{
				Unite = Unite + 1;
			}
			X->unite = Unite;
			
			break;
		
		case 2 :
			if(Centaines >= 5)
			{
				if(Dixaines == 9)
				{
					Dixaines = 0;
					Unite = Unite + 1;
				}
				else
				{
					Dixaines = Dixaines + 1;
				}
			}
			X->unite = Unite;
			X->dixieme = Dixaines;
			break;
		
		case 3 :
			if(Millier >= 5)
			{
				if(Centaines == 9)
				{
					Centaines = 0;
					Dixaines = Dixaines + 1;
				}
				else
				{
					Centaines = Centaines + 1;
				}
			}
			X->unite = Unite;
			X->dixieme = Dixaines;
			X->centieme = Centaines;
			
			break;
		
		case 4 :
			X->unite = Unite;
			X->dixieme = Dixaines;
			X->centieme = Centaines;
			X->millieme = Millier;
			
			break;
		
		default :
		break;
	}
	
}

// ----------------------------------------------------------------
// *** ADC ***

// Lecture d'un canal AD par polling
// Il faut auparavant avoir configuré la/les pin(s) concernée(s) en entrée analogique
uint16_t Adc_read(uint8_t chNr)
{
	  uint16_t ValRetour;
		hadc.Instance->CHSELR = ADC_CHSELR_CHANNEL(chNr);  
	
		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
		ValRetour = HAL_ADC_GetValue(&hadc);
		HAL_ADC_Stop(&hadc);
		return(ValRetour);
}

// ----------------------------------------------------------------


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	//variables
	// *** A COMPLETER ! ***
	uint8_t Btn0;
	uint8_t Btn0_Old;
	uint8_t Btn1;
	uint8_t Btn1_Old;
	uint8_t Btn2;
	uint8_t Btn2_Old;
	uint8_t Btn3;
	uint8_t Btn3_Old;
	uint8_t Count_Btn0 = 0;
	uint8_t Count_Digit = 0;
	uint16_t ValBrute = 0;
	uint16_t ValMiliVolt = 0;
	S_nbrReel ValVolt;

	S_nbrReel structure;
	//init	
	// *** A COMPLETER ! ***
	
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
  MX_ADC_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim6);

	Btn0_Old = HAL_GPIO_ReadPin(BTN0_PORT, BTN0_BIT);
	Btn1_Old = HAL_GPIO_ReadPin(BTN1_PORT, BTN1_BIT);
	Btn2_Old = HAL_GPIO_ReadPin(BTN2_PORT, BTN2_BIT);
	Btn3_Old = HAL_GPIO_ReadPin(BTN3_PORT, BTN3_BIT);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		/*Allumer les LEDs LED0 a LED3*/
		HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_RESET);
		/*Affichage du message d'initialisation sur le LCD*/
		if(Calibration == 0)
		{
			HAL_ADCEx_Calibration_Start(&hadc);
			lcd_init();
			lcd_gotoxy(1,1);
			printf_lcd("TP7 AdLcd <2025>");
			lcd_gotoxy(1,2);
			printf_lcd("KGR HMT");
			lcd_bl_on();
		}
		Calibration = 1;
		// For mesurments Pin PA1 ON
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
		// ------------Switch de calibration------------ //
		lcd_gotoxy(1,1);
		printf_lcd("R11 au maximum !");
		lcd_gotoxy(1, 2);
		
		// Si on appui sur le bouton 0, on diminue la valeur de la référence
		if((Btn1 != Btn0_Old) && (Btn1 == 0)) 
		{
			valVref_mV = valVref_mV - REGLAGE_VREF;
		}
		
		// Si on appui sur le bouton 2, on augmente la valeur de la référence
		if((Btn2 != Btn2_Old) && (Btn2 == 0)) 
		{
			valVref_mV = valVref_mV + REGLAGE_VREF;
		}
		
		// Update display with proper formatting
		lcd_clearLine(2);
		lcd_gotoxy(1, 2);
		printf_lcd("Vref: %d mV", valVref_mV);
		
		// For mesurments Pin PA1 OFF
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
		
		// Update old button states at the end of main loop
		Btn0_Old = Btn0;
		Btn1_Old = Btn1;
		Btn2_Old = Btn2;
		Btn3_Old = Btn3;
		Etat = Attente;
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	
	if(Fin_Init == 0)
	{
		if(Count < COUNT_MAX)
		{
			Count ++;
			//Etat = Intialisation;
		}	
		else
		{
			Fin_Init = 1;
			Count = 0;
			
			Etat = Execution;
		}
	}		
	if(Fin_Init == 1)
	{
		if(Compteur < COMPTEUR_MAX)
		{
		  Compteur ++;
		}	
		else
		{
		  Compteur = 0;
			Etat = Execution;
		}
	}
}
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
