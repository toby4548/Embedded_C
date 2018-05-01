#include <stdint.h>
#include "TM4C123GH6PM.h"
#include "bsp.h"
//#include "intrinsics.h"

int main() {
    
    SYSCTL->RCGC2 |= (1U<<5); //Turn off Port F clock gating
    SYSCTL->GPIOHBCTL |= (1U<<5); //enable AHB for GPIO
    GPIOF_AHB->DIR |= (LED_RED|LED_BLUE|LED_GREEN); //set pin to output
    GPIOF_AHB->DEN |= (LED_RED|LED_BLUE|LED_GREEN); //set output type to digital output
    
    SysTick->LOAD = SYS_CLOCK_HZ/2U - 1U;
    SysTick->VAL = 0U;
    SysTick->CTRL = (1U<<2) | (1U<<1) | (1U);
    
    SysTick_Handler();
    
    __enable_irq();
    //GPIOF_HS->DATA_Bits[LED_BLUE]= 0;//turn the blue LED on
    while (1) {
        GPIOF_AHB->DATA_Bits[LED_GREEN] = LED_GREEN;
        GPIOF_AHB->DATA_Bits[LED_GREEN] = 0U;

        //__disable_irq();
        //GPIOF_AHB->DATA |= LED_GREEN;
        //__enable_irq();

        //__disable_irq();
        //GPIOF_AHB->DATA &= ~LED_GREEN;
        //__enable_irq();
    }
    
    //return 0;
}

