    /* FILE     : USART LAB
   PROJECT      : CNTR8000
   programmer   : BASHORUN ANEEZ
   FIRST VERSION: 2016-06-16
   DESCRIPTION  : This program configures  the USART1 for transmitting and receiving 8 bit data 
                : On the stm32f3 discovery board. the values transmitted is displayed on the serial 
                : the terminal through the simulated serial port over USB. 
                : Also file name is added to the makefile for compilation 
*/
    // neccessary include libaries
    #include "stm32f3_discovery.h"
    #include <stdio.h>
    #include <string.h>
    #include <stdint.h>
    #include "common.h"

/*FUNCTION         : configUsart
  DESCRIPTION      : This function initializes and configures the
                   : the GPIO PORT C pin 4 and pin 5 for transmiting and receiving
  PARAMETERS       : No Parameter passed
  Returns          : Nothing returned from the function
*/
  void configUsart(void)
  {
    //enable GPIO Port C Clock
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    //Configuration for GPIO Port c Pin 4
    GPIOC->MODER |= 0X00000200;   // Configure pin4 as alternate function
    GPIOC->OTYPER |= 0X00000010;  // configure port output type register
    GPIOC->OSPEEDR |= 0X00000300; // set the pin speed as high speed 50Mhz
    GPIOC->PUPDR |= 0x00000100;   // No pull up or pull down
    GPIOC->AFR[0] |= 0x00070000;  // USART1 Alternate Function mapping 

    //Configuration for GPIO Port c Pin 5
    GPIOC->MODER |= 0X00000800;   // Configure pin4 as alternate function
    GPIOC->OTYPER |= 0X00000020;  // configure port output type register
    GPIOC->OSPEEDR |= 0X00000C00; // set the pin speed as high speed 50Mhz
    GPIOC->PUPDR |= (0x00000400); // No pull up or pull down
    GPIOC->AFR[0] |= 0x00700000;  // USART1 Alternate Function mapping 

    //initialize Usart
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;       //Enable Usart1 clock
    USART1->BRR  =  72000000/9600; // Configure BRR by deviding the bus clock with the baud rate
    USART1->CR1 &= ~(0x00008000);  // Oversampling mode = 16
    USART1->CR1 &= ~(0x00009000);  // Word length = 8 bits
    USART1->CR1 &= ~(0x00000400);  // No parity
    USART1->CR1 |= (0x00000008);   // Transmitter enable
    USART1->CR1 |=  0x00000004;    // Receiver enable
    USART1->CR1 |=  0x00000001;    // USART enable
    USART1->CR2 &= ~(0x00002000 | 0x00001000); // one stop bit
  }

/*FUNCTION         : sendValue
  DESCRIPTION      : This function receives a value through the serial monitor saves
                   : it in the transmit data register for transmission
  PARAMETERS       : No Parameter passed
  Returns          : mode
*/
  void sendValue(int mode)
  {  
    configUsart();               //calls function to configure usart 
    if(mode != CMD_INTERACTIVE)  
    {
     return;
   }
   uint32_t tx;
   int rc;
   rc = fetch_uint32_arg(&tx);  //receives value to be through terminal
   if(rc)
   {
    printf("Error! Enter transmit value\n"); //prints error if no value is entered
    return;
  }
USART1->TDR = tx ; 			  //save value in transmit data register
printf("%d\n transmited\n",USART1->TDR ); //prints value in TDR
while((USART1->ISR & USART_ISR_TC) == 0); //wait to ensure all data is completely transmitted
return;                                   // return when data transmission is done
}
ADD_CMD ("sendValue", sendValue," print data"); //Adds command to send value in terminal

/*FUNCTION         : recValue
  DESCRIPTION      : This function receives transmitted value and saves it in the Receive data register
                   : and prints the received value on the monitor
  PARAMETERS       : No Parameter passed
  Returns          : int mode
*/
  void recValue(int mode)
  {  
    BSP_LED_Init(4);
    if(mode != CMD_INTERACTIVE)
    {
     return;
   }
   while((USART1->ISR & USART_ISR_RXNE) == 0); //wait until data is ready to be received
   printf("data received is %i\n",(USART1->RDR & 0x1FF)); // Prints received value
   if((USART1->RDR & 0x1FF) == 50)
	{
	BSP_LED_On(4);
	BSP_LED_On(5);
	BSP_LED_On(6);
	}
   else if((USART1->RDR & 0x1FF) == 51)
	{
	BSP_LED_Off(4);
	BSP_LED_Off(5);
	BSP_LED_Off(6);
	}
	
 }
ADD_CMD ("recValue", recValue," print data"); //Adds command to receive value in terminal

