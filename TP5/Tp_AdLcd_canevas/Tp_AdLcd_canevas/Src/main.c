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
typedef enum {Intialisation = 1, Execution = 2, Attente = 3, Calibration = 4}State; //Edit Henri
typedef enum {OneDigi = 1, TwoDigi = 2, ThreeDigi = 3, FourDigi = 4}NbrDigi; 
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t Etat;
uint8_t Fin_Init = 0;
uint16_t valVref_mV = V_ALIM;	//valeur par d�faut � l�initialisation
char nbr_digit[4]; 
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
// Conversion de mV en chaine de caract�res (ou structure) exprim�e 
// en V sur nDigits digits. Pr�vu pour tension entre 0 et 3,3V (=> u_mV entre 0 et 3300).
// La sortie contiendra donc unit� + �vt point d�cimal, dixi�mes, centi�mes, milli�mes,
//  selon le nb de digits demand�s.
//void ConvMilliVoltVolt(uint16_t u_mV, uint8_t nDigits, char* str_V /* *** OU STRUCTURE *** */)
//{	
//	// *** A COMPLETER ! ***
//	switch(nDigits)
//	{
//		case 1 :
//			//str_V-> = u_mV/1000;
//		
//			break;
//		
//		case 2 :
//			//str_V-> = u_mV/100;
//		
//			break;
//		
//		case 3 :
//			//str_V-> = u_mV/10;
//		
//			break;
//		
//		case 4 :
//			//str_V-> = u_mV;
//		
//			break;
//		
//		default :
//		break;
//	}
//	
//}

// ----------------------------------------------------------------
// *** ADC ***

// Lecture d'un canal AD par polling
// Il faut auparavant avoir configur� la/les pin(s) concern�e(s) en entr�e analogique
uint16_t Adc_read(uint8_t chNr)
{
		hadc.Instance->CHSELR = (uint32_t)chNr;  
	
		HAL_ADC_Start(&hadc);
		HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
		HAL_ADC_GetValue(&hadc);
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
	uint8_t Btn0_Old = 0;
	uint8_t Btn1;
	uint8_t Btn1_Old = 0;
	uint8_t Btn2;
	uint8_t Btn2_Old = 0;
	uint8_t Count_Btn0 = 0;
	uint8_t Count_Digit = 1;
	uint16_t ValBrute = 0;
	uint16_t ValMiliVolt = 0;
	uint16_t ValVolt = 0;
	
	//Henri
	//Tableau pour stocker les sw
	int Btn0_past = 0;
	int Val_Entre = 0;
	#define TEMPS_CALIBRATION 500
	#define REGLAGE_VREF 5
	
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
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		// ---------Henri--------- 
		// memorisation des Sw pass�
		 Btn0_past = Btn0;

		 // Conteur pour rentr� dans le menu de calibration
		 if ((Btn0 != 0) && (Btn0_past != 0)) 		// reading the switch is not correct
		 {
				 // Incrementation du compteur
				 Val_Entre++;

				 // Si le compteur est sup�rieur � 500, on appelle la fonction de calibration
				 if (Val_Entre > TEMPS_CALIBRATION)
				 {
						 Etat = Calibration; //Mise de la switch en mode de calibration
				 }
		 }
		 else if (Btn0 == 0)
		 {
				 // R�initialisation du compteur
				 Val_Entre = 0;
		 }
		//
		 
		// *** A COMPLETER ! ***
		switch(Etat)
		{
			case Intialisation :
				/*Allumer les LEDs LED0 a LED3*/
				HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET);
				HAL_ADCEx_Calibration_Start(&hadc);
				/*Affichage du message d'initialisation sur le LCD*/
				lcd_init();
				printf_lcd("TP7 AdLcd <2025>");
				lcd_gotoxy(1,2);
				printf_lcd("KGR HMT");
				lcd_bl_on();
				
				break;
			
			case Execution :
			
				ValBrute = Adc_read(0); //METTRE LE CANAL A UTILISER
				ValMiliVolt = ConvAdcMilliVolt(ValBrute);
			
				Btn0 = HAL_GPIO_ReadPin(BTN0_PORT, BTN0_BIT);
				Btn1 = HAL_GPIO_ReadPin(BTN1_PORT, BTN1_BIT);
			  Btn2 = HAL_GPIO_ReadPin(BTN2_PORT, BTN2_BIT);
				
				if((Btn1 != Btn1_Old)&&(Btn1_Old == 0))
				{
					Count_Digit --;
					if(Count_Digit < 1)
					{
						Count_Digit = 0;
					}
				}
			
				if((Btn2 != Btn2_Old)&&(Btn2_Old == 0))
				{
					Count_Digit ++;
					if(Count_Digit > 4)
					{
						Count_Digit = 4;
					}
				}
				
				switch(Count_Digit)
				{
					case OneDigi :
						if((Btn0 != Btn0_Old)&&(Btn0_Old == 0)) 
						{
							Count_Btn0 ++;
							if(Count_Btn0 == 2)
							{
								HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET );	// Eteindre la LED 0
								HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET ); 	// Eteindre la LED 1
								HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET ); 	// Eteindre la LED 2
								HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET ); 	// Eteindre la LED 3
						
								lcd_gotoxy(1,2);
								printf_lcd("");
								Count_Btn0 = 0;
							}
							else
							{
								HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET );	// allumer la LED 0
								printf_lcd("AI0 : %4d / %4d", ValBrute, ValMiliVolt);
								lcd_gotoxy(1,2);
								printf_lcd("%d", ValVolt);
							}
						}
							break;
					
					case TwoDigi :
						if((Btn0 != Btn0_Old)&&(Btn0_Old == 0)) 
						{
							Count_Btn0 ++;
							if(Count_Btn0 == 2)
							{
								HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET );		// Eteindre la LED 0
								HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET ); 		// Eteindre la LED 1
								HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET ); 		// Eteindre la LED 2
								HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET );	 	// Eteindre la LED 3
						
								lcd_gotoxy(1,2);
								printf_lcd("");
								Count_Btn0 = 0;
							}
							else
							{
								HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET );	// allumer la LED 0
								HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_RESET );	// allumer la LED 1
								printf_lcd("AI0 : %4d / %4d", ValBrute, ValMiliVolt);
								lcd_gotoxy(1,2);
								printf_lcd("%3d", ValVolt);
							}
						}
						break;
					
					case ThreeDigi :
						if((Btn0 != Btn0_Old)&&(Btn0_Old == 0)) 
						{
							Count_Btn0 ++;
							if(Count_Btn0 == 2)
							{
								HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET );		// Eteindre la LED 0
								HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET ); 		// Eteindre la LED 1
								HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET ); 		// Eteindre la LED 2
								HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET ); 		// Eteindre la LED 3
						
								lcd_gotoxy(1,2);
								printf_lcd("");
								Count_Btn0 = 0;
							}
							else
							{
								HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET );	// allumer la LED 0
								HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_RESET );	// allumer la LED 1
								HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_RESET );	// allumer la LED 2
								printf_lcd("AI0 : %4d / %4d", ValBrute, ValMiliVolt);
								lcd_gotoxy(1,2);
								printf_lcd("%4d", ValVolt);
							}
						}
						break;
							
					case FourDigi :
						if((Btn0 != Btn0_Old)&&(Btn0_Old == 0)) 
						{
							Count_Btn0 ++;
							if(Count_Btn0 == 2)
							{
								HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET );	// Eteindre la LED 0
								HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET ); 	// Eteindre la LED 1
								HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET ); 	// Eteindre la LED 2
								HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET ); 	// Eteindre la LED 3
						
								lcd_gotoxy(1,2);
								printf_lcd("");
								Count_Btn0 = 0;
							}
							else
							{
								HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET );	// allumer la LED 0
								HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_RESET );	// allumer la LED 1
								HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_RESET );	// allumer la LED 2
								HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_RESET );	// allumer la LED 3
								printf_lcd("AI0 : %4d / %4d", ValBrute, ValMiliVolt);
								lcd_gotoxy(1,2);
								printf_lcd("%5d", ValVolt);
							}
						}
						break;
						
					
					default :
						break;
				}	
				
				
				if((Btn0 != Btn0_Old)&&(Btn0_Old == 0)) 
				{
					Count_Btn0 ++;
					if(Count_Btn0 == 2)
					{
						HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_SET );	// Eteindre la LED 0
						HAL_GPIO_WritePin(LED1_PORT, LED1_BIT, GPIO_PIN_SET ); // Eteindre la LED 1
						HAL_GPIO_WritePin(LED2_PORT, LED2_BIT, GPIO_PIN_SET ); // Eteindre la LED 2
						HAL_GPIO_WritePin(LED3_PORT, LED3_BIT, GPIO_PIN_SET ); // Eteindre la LED 3
						
						lcd_gotoxy(1,2);
						printf_lcd("");
						Count_Btn0 = 0;
					}
					else
					{
						HAL_GPIO_WritePin(LED0_PORT, LED0_BIT, GPIO_PIN_RESET );	// allumer la LED 0
						printf_lcd("AI0 : %4d / %4d", ValBrute, ValMiliVolt);
						lcd_gotoxy(1,2);
						printf_lcd("%d");
					}
				}
				
				//ConvMilliVoltVolt(ValMiliVolt, Count_Digit);
				break;
			
			case Attente :
				
				break;
			
			default :
				
				break;
			
		Btn0_Old = Btn0_Old;
		Btn1_Old = Btn1_Old;
		Btn2_Old = Btn2_Old;
		
		Etat = Attente;
			
			// ------------Switch de calibration------------  //
			case Calibration :
						lcd_init();
						// Rest du conteur
						Val_Entre = 0;
						//Apr�s que on appui pour plus que 500ms, on peut commencer la calibration
						printf_lcd("R11 au maximum");
						lcd_gotoxy(1, 2);

						while (Val_Entre != 0)  // Added proper condition
						{
								// Si on appui sur le bouton 0, on diminue la valeur de la r�f�rence
								if (Btn0 != 0)
								{
										valVref_mV = valVref_mV - REGLAGE_VREF;
									// Add in that we can see the value change on display and get the back light on
								}

								// Si on appui sur le bouton 2, on augmente la valeur de la r�f�rence
								if (Btn2 != 0)
								{
										valVref_mV = valVref_mV + REGLAGE_VREF;
								}
								lcd_gotoxy(1, 2);
								printf_lcd("%d", valVref_mV);
						
						}

						
						break;
		}
		
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
	uint16_t Count;
	uint16_t Compteur;
	
	HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
	
	if(Fin_Init == 0)
	{
		if(Count < COUNT_MAX)
		{
			Count ++;
			Compteur ++;
		}	
		else
		{
			Compteur = 0;
			Fin_Init = 1;
			Etat = 2;
		}
	}		
	else
	{
		if(Compteur < COMPTEUR_MAX)
		{
			Compteur = 0;
			Etat = 2;
		}	
		else
		{
		  Compteur ++;
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
