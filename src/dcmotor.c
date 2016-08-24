//header files
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "stm32f3_discovery.h"
#include "stm32f303xc.h"
#include "common.h"




void TIMER_DELAY(uint16_t value)
{
  RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;	  // Enable Clock APB1(36Mhz), value is 0x10
  TIM6->ARR = value;      		  // delay in ms
  TIM6->PSC = 36000-1;  		  // APB1 = 36Mhz so eqn :  CLK_CNT = (36000000)/36000,i.e. CLK_CNT = 1Khz
  TIM6->CCR1 = value/4;			  // duty cycle: 25%
  TIM6->CR1 = 0x01;         		  // Enable TIMER 
  while(!((TIM6->SR & 0x01) == 0x01));    // Check for Update Event
  TIM6->CR1 &= ~(0x01);                   // Disable timer
  TIM6->SR &= ~(0x01);                    // Reset the update event
}




void configure_DCmotor(void)
{

//initialize the LEDs
BSP_LED_Init(3);

//enable GPIOC clock
RCC->AHBENR 	|= RCC_AHBENR_GPIOCEN;  

//Configure PC4(TX) pin
GPIOC->MODER 	|= 0X00000100;          //GPIO Mode: output, IN1 of L293D 
GPIOC->OTYPER 	|= 0X00000010;          //Output Type: Open Drain
GPIOC->OSPEEDR 	|= 0X00000300;          //GPIO Speed: 50MHz    
GPIOC->PUPDR 	|= 0X00000100;          //PULL UP
//GPIOC->AFR [0] 	|= 0X00070000;          //AF7: Alternate function register
	

//Configure PC5(RX) pin
GPIOC->MODER   |= 0X00000400;          //GPIO Mode:output, IN2 of L293D 
GPIOC->OTYPER  |= 0X00000020;          //Output Type: Open Drain
GPIOC->OSPEEDR |= 0X00000C00;          //GPIO Speed: 50MHz    
GPIOC->PUPDR   |= 0X00000400;          //PULL UP 
//GPIOC->AFR [0] |= 0X00700000;        //AF7: Alternate function register

}



void CmdDCmotor_antiClock(int mode){
	
        //uint32_t val;
        //int rc;

	configure_DCmotor();

	if(mode != CMD_INTERACTIVE) {
    	 return;
  	}

        while(1){

        GPIOC->ODR	|= 0X00000010;	//anti-clockwise
        BSP_LED_On(3);
        TIMER_DELAY(1000);
	GPIOC->ODR	|= 0X00000000;
	BSP_LED_Off(3);	
        TIMER_DELAY(1000);
        }
}
ADD_CMD("dcmotora", CmdDCmotor_antiClock, "		Run the DC Motor anti-clockwise");



void CmdDCmotor_Clock(int mode){
	
        //uint32_t val;
        //int rc;

	configure_DCmotor();

	if(mode != CMD_INTERACTIVE) {
    	 return;
  	}

        while(1){
        GPIOC->ODR	|= 0X00000000;	
        TIMER_DELAY(1000);
	GPIOC->ODR	|= 0X00000020;	//clockwise
	TIMER_DELAY(1000);
        }
}
ADD_CMD("dcmotorc", CmdDCmotor_Clock, "		Run the DC Motor clockwise");
