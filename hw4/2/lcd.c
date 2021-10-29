#include "lcd.h"
#include "stm32f4xx.h"
#include "string.h" 


#define MASK(x)  (1UL << (x))

#define RS MASK(12)
#define RW MASK(13)
#define EN MASK(14)


volatile int delay_c = 25000;
volatile int  row = 100;   //to keep pressed pin of port b
volatile int  col = 200;	 //to keep pressed pin of port c
volatile int  result = 0;	 
volatile int  number1 = 0;
volatile char pressed = '#';
volatile char last_op = '#';



//------------------------------------------- definitions
void convert_to_number(void);
void lcd_putchar(unsigned char data);
void calc ( void );
void create_number (void) ;


static void delay(volatile uint32_t count) {
	 volatile uint32_t i = 0;
	 for(i = 0; i < count * delay_c; i++);
}



//------------------------------------------- lcd 

void init_lcd_port( void )
	{
	 SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN);
   SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN);
	
   GPIOA->MODER = 0x55555555;
   GPIOB->MODER = 0x55555555;
		
	 CLEAR_BIT(GPIOB->ODR,RW);
   CLEAR_BIT(GPIOB->ODR,EN);
	 CLEAR_BIT(GPIOB->ODR,RS);
	}
	
void lcd_send_command(unsigned char command)
{
	  delay(2);//small delay is necessary to obtain correct operation
	
  //your code begins here
	 GPIOB -> ODR &= ~RW; // RW = 0
	 GPIOB -> ODR &= ~RS; // RS = 0
	 GPIOA -> ODR = command;
	 GPIOB -> ODR |= EN; //EN = 1
   delay(1);
	 GPIOB -> ODR &= ~EN; // neg edge EN = 0
   //****RW = GND for now***
	  
  //Dont forget to apply a negative edge signal at the end
}


void lcd_putchar(unsigned char data)
{
	 delay(1);

	 GPIOB -> ODR &= ~RW;
	 GPIOB -> ODR |= RS;
   GPIOA -> ODR = data;
	 
	 //create EN falling edge
	 GPIOB -> ODR |= EN;
	 delay(1);
	 GPIOB -> ODR &= ~ EN;
}

void lcd_init(void)
{
	 delay(1);
	
  //function set to display information in 2 rows in 8-bit mode
	 lcd_send_command(0x38);
	 //turn on the lcd and cusor
	 lcd_send_command(0x0E);
	 //clear 
	 lcd_send_command(0x01);
	 //enable entry mode
	 lcd_send_command(0x06);
	 //cursor at start of line 1
	 lcd_send_command(0x80);
	 
	 delay(1); 
}


void lcd_puts(char *str)
{
	delay(1);

	uint32_t l = strlen(str);
	for( int i = 0; i<l ; i++)
	{
		lcd_putchar(str[i]);
	}
	
}


void lcd_put_int (int num)
{
	volatile int n = num ;
	volatile int reverse = 0,rem = 0;
	while( n > 0 )
	{
		 rem=n%10;      
     reverse=reverse*10+rem;    
     n/=10;   
	}
	
	while ( reverse > 0 )
	{
		//char c = ;
		lcd_putchar( 0x30 + (reverse%10) );
		reverse /=10;
	}

}


void lcd_gotoxy(unsigned char x, unsigned char y)
{
	 delay(1);
  //HAL_Delay(1);
  //your code begins here
  //your code ends here 

}

void lcd_clear(void)
{
	 delay(1);
  //clear the lcd and move the cursor to the appropriate place
	lcd_send_command(0x01);
	lcd_send_command(0x80);
	delay(1);
}
 

//------------------------------------------- interrupt  

void EXTI0_IRQHandler (void)
{
	
	EXTI->PR |= (MASK(0));
	NVIC_ClearPendingIRQ(EXTI0_IRQn);

	if(GPIOC->IDR & MASK(0))
	{
		row = 0;
		GPIOC->IDR &= MASK(0);		

		for(int i = 4 ; i < 8 ; i++)
		{
			GPIOC->ODR &= MASK(0);
			GPIOC->ODR |= MASK(i);
			
			if(GPIOC->IDR & 0x000F)
				{
					col = i;		
					break;
				}
		}
	}
}


void EXTI1_IRQHandler (void)
{
	EXTI->PR |= MASK(1);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	
	if(GPIOC->IDR & MASK(1))
	{
		row = 1;
		GPIOC->IDR &= MASK(1);		

		for(int i = 4 ; i < 8 ; i++)
		{
			GPIOC->ODR &= MASK(1);
			GPIOC->ODR |= MASK(i);
			
			if(GPIOC->IDR & 0x000F)
				{
					col = i;		
					break;
				}
		}
	}
	
	
}


void EXTI2_IRQHandler (void)
{
	EXTI->PR |= MASK(2);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	
	if(GPIOC->IDR & MASK(2))
	{
		row = 2;
		GPIOC->IDR &= MASK(2);		

		for(int i = 4 ; i < 8 ; i++)
		{
			GPIOC->ODR &= MASK(2);
			GPIOC->ODR |= MASK(i);
			
			if(GPIOC->IDR & 0x000F)
				{
					col = i;		
					break;
				}
		}
	}
	
	
}

void EXTI3_IRQHandler (void)
{
	EXTI->PR |= MASK(3);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	
	if(GPIOC->IDR & MASK(3))
	{
		row = 3;
		GPIOC->IDR &= MASK(3);		

		for(int i = 4 ; i < 8 ; i++)
		{
			GPIOC->ODR &= MASK(3);
			GPIOC->ODR |= MASK(i);
			
			if(GPIOC->IDR & 0x000F)
				{
					col = i;		
					break;
				}
		}
	}
	
}






void init_extri( void)
{
	
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN);
	
// c0-3 for push bottun interupt
	SYSCFG->EXTICR[0]|=SYSCFG_EXTICR1_EXTI0_PC;
	SYSCFG->EXTICR[0]|=SYSCFG_EXTICR1_EXTI1_PC;	
	SYSCFG->EXTICR[0]|=SYSCFG_EXTICR1_EXTI2_PC;
	SYSCFG->EXTICR[0]|=SYSCFG_EXTICR1_EXTI3_PC;	
	
	
	EXTI->IMR |= (MASK(0));
	EXTI->IMR |= (MASK(1));
	EXTI->IMR |= (MASK(2));
	EXTI->IMR |= (MASK(3));
	
	EXTI-> RTSR |= (MASK(0));
	EXTI-> RTSR |= (MASK(1));
	EXTI-> RTSR |= (MASK(2));
	EXTI-> RTSR |= (MASK(3));
	
	
	__enable_irq();
	NVIC_SetPriority(EXTI0_IRQn,0);
	NVIC_ClearPendingIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);
	
	NVIC_SetPriority(EXTI1_IRQn,1);
	NVIC_ClearPendingIRQ(EXTI1_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
	
	
	NVIC_SetPriority(EXTI2_IRQn,2);
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI2_IRQn);
	
	
	NVIC_SetPriority(EXTI3_IRQn,3);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);
	


}

//------------------------------------------- keypad  
void init_keypad() {
	
	 SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN);
	 GPIOC->MODER = 0x00005500;
}



void detect_key( void)
{
	switch(row){														
				case 0: 
						switch(col){
							case 4: 
															
								lcd_putchar('7'); 
								pressed = '7';
								create_number();
								row = 100 ;	col = 200; 
								break;
					
							case 5:  
															
								lcd_putchar('8'); 
								pressed = '8';
								create_number();
								row = 100 ;	col = 200; 
								break;
					
							case 6:  
																
								lcd_putchar('9'); 
								pressed = '9';
								create_number();
								row = 100 ;	col = 200; 
								break;
					
							case 7:  
															
								lcd_putchar('/'); 
								pressed = '/';
								calc();
								last_op ='/';
								row = 100 ;	col = 200; 
								break;
						
						}
				break;
				case 1: 
					switch(col){
						case 4:  
							
							lcd_putchar('4'); 
							pressed = '4';
							create_number();
							row = 100 ;	col = 200; 
							break;
												
						case 5:  
														
							lcd_putchar('5'); 
							pressed = '5';
							create_number();
							row = 100 ;	col = 200; 
							break;
					
						case 6:  
														
							lcd_putchar('6'); 
							pressed = '6';
							create_number();
							row = 100 ;	col = 200; 
							break;
					
						case 7:  
														
							lcd_putchar('*'); 
							pressed = '*';
							calc();
							last_op ='*';
							row = 100 ;	col = 200; 
							break;
						
					}	
				break;
				case 2:
					switch(col){
						case 4:  
														
							lcd_putchar('1'); 
							pressed = '1';
							create_number();
							row = 100 ;	col = 200; 
							break;
					
						case 5:  
														
							lcd_putchar('2'); 
							pressed = '2';
							create_number();
							row = 100 ;	col = 200; 
							break;
					
						case 6:  
														
							lcd_putchar('3'); 
							pressed = '3';
							create_number();
							row = 100 ;	col = 200; 
							break;
					
						case 7:  
														
							lcd_putchar('-'); 
							pressed = '-';
							calc();
							last_op ='-';
							row = 100 ;	col = 200; 
							break;
						
					}
					break;
				case 3:
					switch(col){
						
						case 4:  // reset everything if ON pressed;
							lcd_clear(); 
							result=0;
							number1=0; 
							row = 100 ;	col = 200; 
							break; 
						
						case 5:  
							
							lcd_putchar('0'); 
							pressed = '0';
							create_number();
							row = 100 ;	col = 200; 
							break;
						
						case 6:  
							//see res
							lcd_putchar('='); 
							pressed='='; 
							calc(); 
							last_op='#';
							row = 100 ;	col = 200; 
							break;
						
						
						case 7:  
							
							lcd_putchar('+'); 
							pressed = '+';
							calc();
							last_op ='+';
							row = 100 ;	col = 200; 
							break;
						
					}
					break;
			
			}
				delay(1);
			  GPIOC->ODR |= 0x00F0;	 //clean everything just like first
				delay(1);
}




void create_number (void) 
{
	if (pressed <= '9' && pressed >= '0')
	{
		number1 = (number1*10) + ( pressed - '0');
	}
}

void calc ( void )
{
	
	if ( last_op == '+')
	{
		result += number1 ;
		number1=0;
	}
	else if ( last_op == '-')
	{
		result -= number1 ;
		number1=0;
	}
	else if ( last_op == '/')
	{
		result /= number1 ;
		number1=0;
	}
	else if ( last_op == '*')
	{
		result *= number1 ;
		number1=0;
	}
	else if ( last_op == '#')
	{
		result = number1 ;
		number1=0;
	}
	
	if (pressed == '=' )
	{
		lcd_put_int(result);
		delay(25);
		number1=0;
		result=0;
		last_op='#';
	}
	

}



void init (void)
{
	init_lcd_port();
	lcd_init();
	init_keypad();
	init_extri();
	
}


int main(void) {
	init();
	
	GPIOC->ODR &= 0x0000;
	GPIOC->ODR |= 0xF0;
	
//
//lcd_puts( "Hello!" );
//delay(25);
//lcd_clear();
//lcd_puts( "Press On To Start" );
//
	
	  while(1) {
			
			detect_key();

    }
	
	
	
}


