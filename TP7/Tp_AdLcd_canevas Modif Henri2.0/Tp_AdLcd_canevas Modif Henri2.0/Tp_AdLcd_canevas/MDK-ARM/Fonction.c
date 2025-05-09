/* USER CODE BEGIN Includes */

#include "17400.h"
#include "stm32delays.h"
#include "stm32driverlcd.h"

//Tableau pour stocker les sw
#define TEMPS_CALIBRATION 500
#define REGLAGE_VREF 5

int Sw_Mem[2];
int Btn0 = 0;
int Btn2 = 1;
int Val_Entré = 0;  // Added missing variable
int ValVref_mV = 0; // Added missing variable

// ... existing code ...

//Fonction de calibration
int Calibration(int ValVref_mV)  // Changed function name to match the other file
{
    //Après que on appui pour plus que 500ms, on peut commencer la calibration
    printf_lcd("R11 au maximum");
    lcd_gotoxy(1, 2);

    while (Val_Entré != 0)  // Added proper condition
    {
        // Si on appui sur le bouton 0, on diminue la valeur de la référence
        if (Btn0 != 0)
        {
            ValVref_mV = ValVref_mV - REGLAGE_VREF;
        }

        // Si on appui sur le bouton 2, on augmente la valeur de la référence
        if (Btn2 != 0)
        {
            ValVref_mV = ValVref_mV + REGLAGE_VREF;
        }
    }

    return ValVref_mV;
}