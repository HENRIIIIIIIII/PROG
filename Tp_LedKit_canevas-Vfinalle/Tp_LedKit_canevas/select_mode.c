#include "select_mode.h"
#include "gpio.h"
#include "17400.h"
#include "stm32delays.h"
/******************************************************
									Gestion des modes
******************************************************/
void GestMode(int *mod) 
{
	static int Sw2P = 1; // état precedent de S2
  static int Sw3P = 1; // état precedent de S3
  // Lire l'état actuel des boutons
  int Sw2 = HAL_GPIO_ReadPin(BTN0_GPIO_Port, BTN0_BIT); // Touche S2 (decrementation)
  int Sw3 = HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_BIT); // Touche S3 (incrementation)
  // Gestion des changements de mode avec detection de flanc descendant (appui)
  if ((Sw2 == 0) && (Sw2P == 1) && (*mod > 1)) 
  {
		*mod -= 1;
	}
  if ((Sw3 == 0) && (Sw3P == 1) && (*mod < 5)) 
  {
		*mod += 1;
	}
  // Mise à jour des états precedents
  Sw2P = Sw2;
  Sw3P = Sw3;

}

/********************************************************
								Gestion de la durée mode 3, 4 et 5
********************************************************/
int GestDuration(int byV_Time,int mod)
{
	static int Sw4 = 1;
	static int Sw4P = 1;
	static int Sw5 = 1;
	static int Sw5P = 1;
	
  Sw4P = Sw4;
  Sw5P = Sw5;
  // Reading the switches
  Sw4 = HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_BIT);
  Sw5 = HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_BIT);
	/************* Speed changer **************/
	if ((Sw4 != Sw4P) && (Sw4 == 0) && (mod >= 3) && (Sw4 != Sw5))
  {
    if (mod == 5)
    {
      if (byV_Time <= 100)
      {
        byV_Time = 1000;
      }
      else
      {
        byV_Time -= 100;
      }
    }
    else // Mode 3 et 4
    {
      if (byV_Time <= 200)
      {
        byV_Time = 800;
      }
      else
      {
        byV_Time -= 50;
      }
    }
   }
  // Gestion du bouton S5 (augmentation de la durée)
  if ((Sw5 != Sw5P) && (Sw5 == 0) && (mod >= 3) && (Sw4 != Sw5))
  {
		if (mod == 5)
    {
			if (byV_Time >= 1000)
      {
        byV_Time = 100;
      }
      else
      {
        byV_Time += 100;
      }
    }
    else // Mode 3 et 4
    {
			if (byV_Time >= 800)
      {
        byV_Time = 200;
      }
      else
      {
        byV_Time += 50;
      }
    }
  }
  return byV_Time;
}

/******************************************************
								Mode 1 : LEDs OFF
******************************************************/
void Leds_off(void) 
{
 //HAL_GPIO_WritePin(GPIOC,LED0_Pin|LED1_Pin | LED2_Pin | LED3_Pin,GPIO_PIN_SET);
	GPIOC -> BSRR = (LED0_Pin|LED1_Pin | LED2_Pin | LED3_Pin);
	
}

/******************************************************
								Mode 2 : LEDs ON
******************************************************/
void Leds_on(void) 
{
 // HAL_GPIO_WritePin(GPIOC,LED0_Pin|LED1_Pin | LED2_Pin | LED3_Pin,GPIO_PIN_RESET);
	
	GPIOC->BSRR = (LED0_Pin | LED1_Pin | LED2_Pin | LED3_Pin) << 16;
}

/********************************************************
Mode 3 : Chenillard de gauche à droite
********************************************************/
void chenillard_GD(int byV_Time) 
{
	static uint32_t lastToggleTime = 0;  // Temps du dernier changement d'état
  static int currentLed = 0;           // LED courante (0 à 3)
  uint32_t currentTime = HAL_GetTick(); // Temps actuel
  // V?rifier si le temps ecoulé dépasse la durée spicifiée
  if ((currentTime - lastToggleTime) >= byV_Time) 
	{
		lastToggleTime = currentTime;  // Met à jour le dernier temps
    // Turn all LEDs off
    HAL_GPIO_WritePin(GPIOC, LED0_Pin | LED1_Pin | LED2_Pin | LED3_Pin, GPIO_PIN_SET);
    // Turn on only the current LED
    switch(currentLed) 
		{
			case 0: HAL_GPIO_WritePin(GPIOC, LED0_Pin, GPIO_PIN_RESET); break;
      case 1: HAL_GPIO_WritePin(GPIOC, LED1_Pin, GPIO_PIN_RESET); break;
      case 2: HAL_GPIO_WritePin(GPIOC, LED2_Pin, GPIO_PIN_RESET); break;
      case 3: HAL_GPIO_WritePin(GPIOC, LED3_Pin, GPIO_PIN_RESET); break;
    }
    // Move to next LED
    currentLed = (currentLed + 1) % 4;
  }
}

/***********************************************************
Mode 4 : Chenillard de droite à gauche
***********************************************************/
void chenillard_DG(int byV_Time) 
{
	static uint32_t lastToggleTime = 0;
	static int currentLed = 3;  // Start from rightmost LED
	uint32_t currentTime = HAL_GetTick();
	if ((currentTime - lastToggleTime) >= byV_Time) 
	{
		lastToggleTime = currentTime;
		// Turn all LEDs off
		HAL_GPIO_WritePin(GPIOC, LED0_Pin | LED1_Pin | LED2_Pin | LED3_Pin, GPIO_PIN_SET);
		// Turn on only the current LED
		switch(currentLed) 
		{
			case 3: HAL_GPIO_WritePin(GPIOC, LED3_Pin, GPIO_PIN_RESET); break;
			case 2: HAL_GPIO_WritePin(GPIOC, LED2_Pin, GPIO_PIN_RESET); break;
			case 1: HAL_GPIO_WritePin(GPIOC, LED1_Pin, GPIO_PIN_RESET); break;
			case 0: HAL_GPIO_WritePin(GPIOC, LED0_Pin, GPIO_PIN_RESET); break;
		}
		// Move to next LED (going right to left)
		currentLed = (currentLed - 1);
		if (currentLed < 0) currentLed = 3;
	}
}
/***********************************************************
 Mode 5 : Clignotement de toutes les LEDs
***********************************************************/
void blink_led(int byV_Time) 
{
	static uint32_t lastToggleTime = 0; // Stocke le dernier temps de changement
  static int ledState = 0; // ?tat actuel des LEDs (0 = ?teint, 1 = allum?)
  uint32_t currentTime = HAL_GetTick(); // Temps actuel
  // V?rifier si le temps ?coul? d?passe la moiti? de la p?riode
  if ((currentTime - lastToggleTime) >= (byV_Time / 2)) 
	{
		lastToggleTime = currentTime; // Mise ? jour du dernier temps de bascule
		// Basculer l'état des LEDs
		ledState = !ledState;
		HAL_GPIO_WritePin(GPIOC, LED0_Pin | LED1_Pin | LED2_Pin | LED3_Pin,ledState ? GPIO_PIN_SET : GPIO_PIN_RESET);
  }
}
