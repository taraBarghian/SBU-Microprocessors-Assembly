#include <stm32f4xx.h>
#define MASK(x) (1UL << (x))

void EXTI_init(void);
void GPIO_init(void);
void UART_Transmit(char digit);
void delay(uint16_t amt);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void USART2_init (void);
void USART2_receive(void);
void change_state(void);
char itoc(int d);


volatile char address, data, command;
 
void delay(uint16_t amt){
 volatile uint16_t i;
 for(i = 0; i < amt; i++){}
}


char itoc(int d){
	return (char)(d + '0');
	
}


void UART_Transmit(char digit){
 USART2->DR = digit;
 while(!READ_BIT(USART2->SR, USART_SR_TC)){}
}


void EXTI0_IRQHandler(void){
	
	EXTI-> PR |= MASK(0);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);

	if(GPIOC->IDR & MASK(0)){			
		GPIOB->ODR ^= MASK(0);
		
		GPIOB->ODR |= MASK(5);
	  UART_Transmit('4');
	  GPIOB->ODR &= ~MASK(5);
		
	
	}

		
	

}

void EXTI1_IRQHandler(void){
	
	EXTI-> PR |= MASK(0);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);

	if( GPIOC->IDR & MASK(1)){
		GPIOB->ODR ^= MASK(1);
		
		GPIOB->ODR |= MASK(5);
	  UART_Transmit('5');
	  GPIOB->ODR &= ~MASK(5);
		

		}		

}


void EXTI2_IRQHandler(void){
	
	EXTI-> PR |= MASK(0);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);

	if( GPIOC->IDR & MASK(2)){
		GPIOB->ODR ^= MASK(2);
		
		GPIOB->ODR |= MASK(5);
	  UART_Transmit('6');
	  GPIOB->ODR &= ~MASK(5);


	}
	

}

void EXTI3_IRQHandler(void){
	
	EXTI-> PR |= MASK(0);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);

	if(GPIOC->IDR & MASK(3)){
		GPIOB->ODR ^= MASK(3);
		
		GPIOB->ODR |= MASK(5);
	  UART_Transmit('7');
	  GPIOB->ODR &= ~MASK(5);
		

	}
	

}





void EXTI_init(void){
	// IRQ0
	SYSCFG-> EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PC;
	EXTI->IMR |= (MASK(0));
	EXTI->RTSR |= (MASK(0));
	__enable_irq();
	NVIC_SetPriority(EXTI0_IRQn,0);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);

	// IRQ1
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PC;	
	EXTI->IMR |= (MASK(1));												
	EXTI->RTSR |= (MASK(1));
	NVIC_SetPriority(EXTI1_IRQn,0);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	
	
	// IRQ2
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PC;	
	EXTI->IMR |= (MASK(2));												
	EXTI->RTSR |= (MASK(2));
	NVIC_SetPriority(EXTI2_IRQn,0);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	
	
	// IRQ3
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PC;	
	EXTI->IMR |= (MASK(3));												
	EXTI->RTSR |= (MASK(3));
	NVIC_SetPriority(EXTI3_IRQn,3);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
}




void GPIO_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
}


void USART2_init (void) { 
 RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  
 RCC->APB1ENR |= 0x20000;       
 // Configure PA2 for USART2_TX 
 GPIOA->OSPEEDR |= 0x20; 
 GPIOA->OSPEEDR |= 0x80; 
 GPIOA->AFR[0] &= ~0x0FF00; 
 GPIOA->AFR[0] |= 0x07700;
 GPIOA->MODER  &= ~0x00F0; 
 GPIOA->MODER  |= 0x00A0; //enable alternate function for PA2,PA3
 USART2->BRR = 0x0683; // 9600 baud @ 16 MHz  
 USART2->CR1 |= 0x0008; // enable Tx, 8-bit data  
 USART2->CR1 |= 0x0004; // enable Rx, 8-bit data 
 //USART2->CR1 |= USART_CR1_TCIE; 
 USART2->CR2 |= (2 << 12); // 2 stop bits 
 USART2->CR3 = 0;  // flow cntrl
	
 USART2->CR1 |= 0x2000; 
 
} 
 
void USART2_receive(){ 
 while (!READ_BIT(USART2->SR, USART_SR_RXNE)){} 
		address = (uint8_t)(USART2->DR); 
 while (!READ_BIT(USART2->SR, USART_SR_RXNE)){} 
		command = (uint8_t)(USART2->DR); 
 if (command == '1'){	 
	while (!READ_BIT(USART2->SR, USART_SR_RXNE)){} 
		data = (uint8_t)(USART2->DR); 
	}
 //else
	

}




void change_state(){
	int value = data - '0';
	GPIOB->ODR ^= MASK(value);
	data = '9';
	
}

int main(){

		
		RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;	
		RCC-> APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	  
    GPIOB->MODER = 0x555;
		EXTI_init();
	
		USART2_init();
		GPIOB->ODR = 0;
	  GPIOB->ODR &= ~MASK(5);
    while(1) {
			
			USART2_receive();
			change_state();
			
			
    }
	
}
