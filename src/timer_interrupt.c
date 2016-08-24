#include <stdio.h>
#include <stdint.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#include "stm32f303xc.h"
#include "common.h"


void TIMER_DELAY(uint16_t value)
{
        
        
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;	//Enable Clock, value is 0x10
        TIM6->ARR = value;       		// delay in ms
	TIM6->PSC = 36000-1;  			// APB1 = 36Mhz so eqn :  CLK_CNT = (36000000)/36000,i.e. CLK_CNT = 1Khz
	TIM6->CR1 = 0x01;         		// ENaBle TIMER 
	while(!((TIM6->SR & 0x01) == 0x01));    // Check for Update Event
        printf("Disabling timer 6...\n");
	TIM6->CR1 &= ~(0x01);                   // Disable timer
	TIM6->SR &= ~(0x01);                    // Reset the update event
}



void timer3_interrupt(int mode)
{

  if(mode != CMD_INTERACTIVE) 
   {
     return;
   }

 
  //BSP_LED_Init(3);


  // TIM3 clock enable 
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 


  // delay = 0.5 = (PSC+1)*ARR/FCLK = 60,000*600/72,000,000 
  TIM3->PSC = 599; 		// Set pre-scaler to 600 (PSC + 1) 
  TIM3->ARR = 60000; 		// Auto reload value 600000 
  TIM3->CR1 = TIM_CR1_CEN; 	// Enable timer 
  TIM3->DIER |= 1 << 0; 	// enable interrupt 
  NVIC->ISER[0] |= 1 << 29; 	// enable TIM3 interrupt in NVIC

  //initialize led 3
  RCC ->AHBENR |= RCC_AHBENR_GPIOEEN;	//enable clock
  GPIOE ->MODER |= 0x00040000;		//Output
  GPIOE ->OTYPER &= ~(0x00000200); 	//Push pull
  GPIOE ->OSPEEDR |=  0x000C0000; 	//Max. speed
  GPIOE ->PUPDR &= ~(0x000C0000); 	//No pull up and pull down


  if(TIM3->SR & TIM_SR_UIF)     // if UIF flag is set
	
  {
    TIM3->SR &= ~TIM_SR_UIF;    // clear UIF flag
    //BSP_LED_On(3);
    GPIOE ->BSRRL = 0x0200; 		//on LD3
  }

}

ADD_CMD("timerint", timer3_interrupt, "		execute timer 3 interrupt");






