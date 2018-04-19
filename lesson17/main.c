#include <stdint.h>
#include "tm4c_cmsis.h"
#include "bsp.h"
#include "intrinsics.h"

int main() {
    
    SYSCTL->RCGC2 |= (1U<<5); //Turn off Port F clock gating
    SYSCTL->GPIOHSCTL |= (1U<<5); //enable AHB for GPIO
    GPIOF_HS->DIR |= (LED_RED|LED_BLUE|LED_GREEN); //set pin to output
    GPIOF_HS->DEN |= (LED_RED|LED_BLUE|LED_GREEN); //set output type to digital output
    
    SysTick->LOAD = SYS_CLOCK_HZ/2U - 1U;
    SysTick->VAL = 0U;
    SysTick->CTRL = (1U<<2) | (1U<<1) | (1U);
    
    __enable_interrupt();
    //GPIOF_HS->DATA_Bits[LED_BLUE]= 0;//turn the blue LED on
    while (1) {      
    }
    
    //return 0;
}

