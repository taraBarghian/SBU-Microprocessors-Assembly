#include "stm32f4xx.h"
#define mask(x) (1UL << (x))

void delay(uint16_t amt);
void MAX_write(int16_t data ) ;
void MAX_init(void);
void USART2_init(void);
void GPIO_init(void); 
void SPI1_init(void);
uint8_t recieve(void);
void assert(void);
void disassert(void);
void write_operation(int16_t data);
char USART2_receive(void);

char x, y;

void delay(uint16_t amt){
	volatile uint16_t i;
	for(i = 0; i < amt; i++){}
}

void UART_Transmit(char digit){
	USART2->DR = digit;		
	while(!READ_BIT(USART2->SR, USART_SR_TC)){}	
}

int main()
{
	
	SPI1_init();
	MAX_init();
	USART2_init ();
	
		while(1){
			USART2_receive();
			UART_Transmit(x);
			//int temp2 = x - '0';
			int x1 = x - '0';
			int y1 = y - '0';
			
			MAX_write(0x0200 + x);
			MAX_write(0x0100 + y);
			
	}
}




void MAX_init(void){
	MAX_write(0xC01); //TURN ON MODULE
	MAX_write(0xA0F); //MAX INTENSITY
	MAX_write(0xB01);	 //2 DIGITS
	MAX_write(0x9FF); //B DECODING
}


void GPIO_init(void) 
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; 
	// PA5, PA7 for SPI1 MOSI and SCLK 
	GPIOA->MODER &= ~((3 << 10) | (3 << 14)); 
	GPIOA->MODER |= ((2 << 10) | (2 << 14)); // AF MODE
	GPIOA->AFR[0] &= ~0xF0F00000; 
	GPIOA->AFR[0] |= 0x50500000; 
	// PA4 as GPIO output for SPI slave select 
	GPIOA->MODER &= ~(3 << 8); 
	GPIOA->MODER |= (1 << 8); 	
}

void SPI1_init(void) 
{
	GPIO_init();
	SPI1->CR1 = 0;
	SPI1->CR1 = 0x31C; // 9600 Baud rate, 8-bit // LSBFIRST 
	SPI1->CR2 = 0;
	SPI1->CR1 |= SPI_CR1_SPE; 	
}


void assert(void){
	GPIOA->BSRR = 0x00100000; //ACTIVE LOW ASSERT
}
void disassert(void){
	GPIOA->BSRR = 0x00000010; 
}

void write_operation(int16_t data){
	SPI1->DR = data >> 8 ;  //  0....0(D11-D8) 
	while (!(SPI1->SR & SPI_SR_TXE)) {} 
	SPI1->DR = data & 0xFF;  // write command  
	while (SPI1->SR & SPI_SR_BSY) {} 
}


void MAX_write(int16_t data) {
	
	while (!(SPI1->SR & SPI_SR_TXE)) {} 
	assert();	
	write_operation(data);	
	disassert();
		
}


void USART2_init (void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 	/* enable GPIOA clock */
	RCC->APB1ENR |= 0x20000; 							/* enable USART2 clock */
	/* Configure PA2 for USART2_TX */
	GPIOA->OSPEEDR |= 0x20;
	GPIOA->OSPEEDR |= 0x80;
	GPIOA->AFR[0] &= ~0x0FF00;
	GPIOA->AFR[0] |= 0x07700; /* alt7 for USART2 */
	GPIOA->MODER  &= ~0x00F0;
	GPIOA->MODER  |= 0x00A0; /* enable alternate function for PA2,PA3*/
	USART2->BRR = 0x0683; /* 9600 baud @ 16 MHz */
	USART2->CR1 |= 0x0008; /* enable Tx, 8-bit data */
	USART2->CR1 |= 0x0004; /* enable Rx, 8-bit data */
	//USART2->CR1 |= USART_CR1_TCIE;
	USART2->CR2 |= (2 << 12); /* 1 stop bit */
	USART2->CR3 = 0x0000; /* no flow control */
	USART2->CR1 |= 0x2000; /* enable USART2 */

}

char USART2_receive(){
	while (!READ_BIT(USART2->SR, USART_SR_RXNE)){}
	x = (uint8_t)(USART2->DR);
	while (!READ_BIT(USART2->SR, USART_SR_RXNE)){}
	y = (uint8_t)(USART2->DR);
	return 't';
}
