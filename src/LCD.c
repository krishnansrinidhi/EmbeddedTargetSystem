#include <stdio.h>
#include <string.h>
#include "stm32f3xx_hal.h"
#include "stm32f3_discovery.h"
#include "common.h"

uint32_t dispAttr;
char *character;
char *string;
void LCD_backlight ()
{
//GPIO Configuration
    __GPIOB_CLK_ENABLE();
    
    GPIO_InitTypeDef gpio_init_struct; // GPIO Initialisation structure
    
    gpio_init_struct.Pin = GPIO_PIN_8;
    
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP; // Push pull configuration
    
    gpio_init_struct.Pull = GPIO_PULLDOWN; // Pull down configuration
    
    gpio_init_struct.Speed = GPIO_SPEED_LOW; // Low speed configuration
    
    HAL_GPIO_Init(GPIOB,&gpio_init_struct); 
    
}

/*LCD initialization*/
void init_LCD()
{
     
    fetch_uint32_arg(&dispAttr);
    //LCD_backlight ();
    gpio_pin_init (0);
    GPIOD -> ODR = 0x00000000;
    HAL_Delay(45);
    GPIOD -> ODR = 0x00000120;
    HAL_Delay(10); 
    switch (dispAttr)
    {
        case 0:
            // display off
            GPIOD -> ODR = 0x00000820;
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
            break;
        case 1:
            //display on, cursor off
            GPIOD -> ODR = 0x00000C20;
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
            break;
        case 2:
            //display on, cursor on
            GPIOD -> ODR = 0x00000E20;
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
            break;
        case 3:
            //display on, cursor on flashing
            GPIOD -> ODR = 0x00000F20;
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
            break;
    }
}
ADD_CMD("initLcd",init_LCD,"0 display off, 1 display on, cuGPIO_PIN_4or off, 2 display on, cuGPIO_PIN_4or on, 3 display on, cuGPIO_PIN_4or on flashing")

/*LCD add character*/
void putc_LCD(int mode)
{
    if(mode!=CMD_INTERACTIVE) return;
    fetch_string_arg(&character);
    gpio_pin_init (0);
    //function set
    GPIOD -> ODR = 0x00003820;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
    //Entry mode set
    GPIOD -> ODR = 0x00000620;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
    //Display characteracter
    GPIOD -> ODR = 0x00000030 | (uint32_t)*character << 8;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
    printf("%s\n",character);
}
ADD_CMD("putc",putc_LCD,"send a single letter characteracter to the screen")

/*LCD add string*/
void puts_LCD(int mode)
{
    if(mode!=CMD_INTERACTIVE) return;
    fetch_string_arg(&string);
    uint8_t characterCount = 1;
    while(*string != 0)
    {
        if (characterCount > 16)
        {
            gpio_pin_init (0);
            GPIOD -> ODR = 0x0000C020;
            HAL_Delay(10);
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
        }
        gpio_pin_init (0);
        //function configuration
        GPIOD -> ODR = 0x00003820;
        HAL_Delay(10);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
        //Entry mode configuration
        GPIOD -> ODR = 0x00000620;
        HAL_Delay(10);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
        GPIOD -> ODR = 0x00000030 | (uint32_t)*string << 8;
        HAL_Delay(10);
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
        string = string + 0x00000001;
        characterCount ++;
    }
}
ADD_CMD("puts",puts_LCD,"Send a String to the LCD screen")

/*LCD Home*/
void LCD_home(int mode)
{
    gpio_pin_init (0);
    GPIOD -> ODR = 0x00000220;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
}
ADD_CMD("home",LCD_home,"lcd home")

/*LCD Clear screen*/
void LCD_Clear_Screen(int mode)
{
    if(mode!=CMD_INTERACTIVE) return;
    gpio_pin_init (0);
    GPIOD -> ODR = 0x00000120;
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
}
ADD_CMD("clrscr",LCD_Clear_Screen,"clear the LCD screen")

void backlight_On(int mode)
{
    if(mode!=CMD_INTERACTIVE) return;
	LCD_backlight ();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 1);
    
}
ADD_CMD("backlighton",backlight_On,"clear the LCD screen")

void backlight_Off(int mode)
{
    if(mode!=CMD_INTERACTIVE) return;
	LCD_backlight ();
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, 0);
}
ADD_CMD("backlightoff",backlight_Off,"clear the LCD screen")



