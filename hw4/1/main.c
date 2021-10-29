#include "stm32f4xx.h" 
#include <stdbool.h>

#define DELAY_COUNT    ( 250000 )   //delay count
#define MASK(x)  (1UL << (x) )

#define set_moder_B 0x0

#define N0 0X3F
#define N1 0X06
#define N2 0X5B
#define N3 0X4F
#define N4 0X66
#define N5 0X6D
#define N6 0X7D
#define N7 0X07
#define N8 0X7F
#define N9 0X6F

#define letter_N 0x37
#define letter_F 0x71


volatile int i = 0;
volatile int j = 0;

//array containing all the hex codes of 7 seg
uint32_t hex_codes[] = {N0, N1, N2, N3, N4, N5, N6, N7, N8, N9}; 
/*******************************************************************************/
void btn2_handeler(void);
void btn1_handeler(void);
bool checkBTN1 ( void );
bool checkBTN2 ( void );
bool check_time_BTN1 (void);
bool check_time_BTN2 (void);
bool wait_until_sth_pressed (void);
void start_app(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
static void init_ports( void );
/*******************************************************************************/

static void delay( )
{
    volatile uint32_t c = 0;
    for( c=0; c<=DELAY_COUNT; c++ ){
			
		}
}

//------------------------------------------- init functions 

static void init( void )
{
	//Enable the AHB clock GPIO ports A,B,C and SYSCFG
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
  SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
	
	
	
	//set all Port A0..15,B0..15 as output 
  GPIOA->MODER = 0X55555555;
  GPIOB->MODER = 0X55555555;
	GPIOC->MODER = 0X00;
	
	//Show On at first
	GPIOA->ODR &= 0XFFFF0000;
	GPIOB->ODR &= 0XFFFF0000;
	GPIOA->ODR |= N0;
	GPIOB->ODR |= letter_N;
}


static void init_ports( void )
{
	
	// c0-1 for push bottun interupt
	SYSCFG->EXTICR[0]|=SYSCFG_EXTICR1_EXTI0_PC;
	SYSCFG->EXTICR[0]|=SYSCFG_EXTICR1_EXTI1_PC;	
	
	EXTI->IMR |= (MASK(0));
	EXTI->IMR |= (MASK(1));
	
	EXTI-> RTSR |= (MASK(0));
	EXTI-> RTSR |= (MASK(1));
	
	
	__enable_irq();
	NVIC_SetPriority(EXTI0_IRQn,0);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);
	
	NVIC_SetPriority(EXTI1_IRQn,0);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	
}




//------------------------------------------- counting functions 

bool checkBTN1 ( void )
{
	if(GPIOC->IDR & MASK(0))
	{
		return true;
	}
	return false;
}

bool checkBTN2 ( void )
{
	if(GPIOC->IDR & MASK(1))
	{
		return true;
	}
	return false;

}



bool check_time_BTN1 (void)
{
	bool flg = true;
	volatile uint32_t cn = 0;
	volatile uint32_t _3_sec = 6*DELAY_COUNT ; // 3 sec = 6 * 0.5 sec :))
	
  for( cn=0; cn<= _3_sec ; cn++ )
	{		
		if( !(checkBTN1()) )
		{
			flg = false;
			break;
		}
	}
	if(flg)
	{
		GPIOB->BSRR = MASK(9); 
		delay();
	}
	GPIOB->BSRR = MASK(25); 	
	return flg;
}


bool check_time_BTN2 (void)
{
	bool flg = true;
	volatile uint32_t cn = 0;
	volatile uint32_t _3_sec = 6*DELAY_COUNT ; // 3 sec = 6 * 0.5 sec :))
	
  for( cn=0; cn<= _3_sec ; cn++ )
	{
		if( !(checkBTN2()) )
		{
			flg = false;
			break;
		}
	
	}
		if(flg)
	{
		GPIOB->BSRR = MASK(9); 
		delay();
	}
	GPIOB->BSRR = MASK(25); 	
	return flg;
}





//return true if btn1 pressed , false if btn 2
bool wait_until_sth_pressed (void)
{
	
	GPIOB->BSRR = 0xFFFF0000;
	GPIOA->BSRR = 0xFFFF0000;
	GPIOB->BSRR = hex_codes[j];			
	GPIOA->BSRR = hex_codes[i];
	
	delay();
		
	volatile int cn = 0;
	for( cn=0; cn<=DELAY_COUNT; cn++ )
	{

		if( GPIOC->IDR & MASK(0) )
			return true;
		if( GPIOC->IDR & MASK(1) )
			return false;
		if(cn == DELAY_COUNT )
			cn=0;
	}
	return false; //never happen! just to make sure it will be compile.
}




void btn1_handeler ( void )
{		
	
			//do incremental counting
			for(; i < 10; i++)
			{	
				GPIOA->BSRR = 0xFFFF0000;
				GPIOA->BSRR = hex_codes[i];
				for(; j < 10; j++)
				{
				GPIOB->BSRR = 0xFFFF0000;
				GPIOB->BSRR = hex_codes[j];
					
				// if btn 2 pressed 	
				if (checkBTN2())
				{
					bool chck = check_time_BTN2();
					//if pressed 3sec
					if(chck)
						{
							GPIOA->BSRR = 0xFFFF0000;
							GPIOA->BSRR = hex_codes[0];
							GPIOB->BSRR = 0xFFFF0000;
							GPIOB->BSRR = letter_F;
							while(1);
						}
					else
							btn2_handeler();
				}
				

					//if pressed 3sec
					if(check_time_BTN1())
						{
							bool flg = wait_until_sth_pressed();
							if(flg)
								btn1_handeler();
							else
								btn2_handeler();
						}

				delay();
			 }
				j=0;
				if( i == 9)
				{
					i=-1;			
				}
			}
    

}



void btn2_handeler ( void )
{

			for(; i > -1; i--)
			{			
			GPIOA->BSRR = 0xFFFF0000;
			GPIOA->BSRR = hex_codes[i];
			
			for(; j > -1; j--)
				{
				GPIOB->BSRR = 0xFFFF0000;
				GPIOB->BSRR = hex_codes[j];
					
	
				// if btn 1 pressed 	
				if (checkBTN1())
				{
					//if pressed 3sec
					bool chck = check_time_BTN1();
					if(chck)
						{
							bool flg = wait_until_sth_pressed();
							if(flg)
								btn1_handeler();
							else
								btn2_handeler();
						}
					else
							btn1_handeler();
					
				}
					
				

					//if pressed 3sec
					if(check_time_BTN2())
						{
							GPIOA->BSRR = 0xFFFF0000;
							GPIOA->BSRR = hex_codes[0];
							GPIOB->BSRR = 0xFFFF0000;
							GPIOB->BSRR = letter_F;
							while(1);
						}
						
				delay();
				}
				j=9;
				if( i == 0 )
				{
					// to count again
					i=10;			
				}
						
			}
	
}



void EXTI0_IRQHandler(void)
{
	EXTI->PR |= (MASK(0));
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	
	if(checkBTN1())
	{
		btn1_handeler();
	}
}

void EXTI1_IRQHandler(void)
{
	EXTI->PR |= (MASK(1));
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	
	if(checkBTN2())
		btn2_handeler();	
}

int main(void){

  init();
	init_ports();
  //Endless loop
  while(1){}
}
