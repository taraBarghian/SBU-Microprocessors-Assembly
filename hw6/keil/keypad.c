#include "keypad.h"
/* 
PC0-PC3 are columns and pulled up EXTI pins with falling edge trigger detection
PC4-PC7 are rows and output pins
*/



char key_cal(uint8_t r, uint8_t c){
	if(r == 0){
		switch(c) {
			case 0: return '7';
			case 1: return '8';
			case 2: return '9';
			case 3: return '/';
		}
	}
	else if(r == 1){
		switch(c) {
			case 0: return '4';
			case 1: return '5';
			case 2: return '6';
			case 3: return 'X';
		}
	}
	else if(r == 2){
		switch(c) {
			case 0: return '1';
			case 1: return '2';
			case 2: return '3';
			case 3: return '-';
		}
	}
	else{  // r == 3
		switch(c) {
			case 0: return 'C';
			case 1: return '0';
			case 2: return '=';
			case 3: return '+';
		}
	}
	return 'N';
}

char get_key_dpressed (uint8_t c)
{
	char key = 'N'; // default value for "no key pressed"

	for(uint8_t r = 0; r < ROW_SIZE; r++)
	{
		
		// write 1 to one of the rows
		HAL_GPIO_WritePin(GPIOC, ROW_GPIO_PIN[r], GPIO_PIN_SET);
		for(volatile int dad =0 ; dad<=100 ; dad++ ){}
			
		// if the col is high
		if(HAL_GPIO_ReadPin(GPIOC, COL_GPIO_PIN[c]) == GPIO_PIN_SET)
		{
			// Debounce
			for(volatile int dad =0 ; dad<=100 ; dad++ ){}
			// is button really pressed?
			if(HAL_GPIO_ReadPin(GPIOC, COL_GPIO_PIN[c]) == GPIO_PIN_SET)
			{
				// return the key
				key = key_cal(r, c);
				break;
			}
		}			
	}
	//GPIOB->ODR |= (1<<9);
	for(uint8_t j = 0; j < ROW_SIZE; j++){
		HAL_GPIO_WritePin(GPIOC, ROW_GPIO_PIN[j], GPIO_PIN_RESET);
	}
	for(volatile int dad =0 ; dad<=100 ; dad++ ){}
	while(HAL_GPIO_ReadPin(GPIOC, COL_GPIO_PIN[c]) == GPIO_PIN_RESET)
		for(volatile int dad =0 ; dad<=100 ; dad++ ){}
	return key;
}
