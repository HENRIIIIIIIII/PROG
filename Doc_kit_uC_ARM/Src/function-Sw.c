/* USER CODE BEGIN PFP */
void ReadSwitches(char* current, char* previous);
/* USER CODE END PFP */

// ... existing code ...

/* USER CODE BEGIN 0 */
void ReadSwitches(char* current, char* previous) {
    // Save previous state
    for(int i = 0; i < 4; i++) {
        previous[i] = current[i];
    }

    // Read new state
    current[0] = HAL_GPIO_ReadPin(BTN0_GPIO_Port, BTN0_BIT);
    current[1] = HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_BIT);
    current[2] = HAL_GPIO_ReadPin(BTN2_GPIO_Port, BTN2_BIT);
    current[3] = HAL_GPIO_ReadPin(BTN3_GPIO_Port, BTN3_BIT);
}
/* USER CODE END 0 */

// ... existing code ...

/* USER CODE BEGIN 3 */
        // Replace the switch reading code with function call
        ReadSwitches(byT_Sw, byT_SwP);
/* USER CODE END 3 */