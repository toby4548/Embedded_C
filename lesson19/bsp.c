/* Board Support Package */
#include "TM4C123GH6PM.h"
#include "bsp.h"

__attribute__((naked)) void assert_failed(char const *file, int line) {
    /*TBD: damage control */
    NVIC_SystemReset(); /*reset system*/
}

void SysTick_Handler(void) {
  
    GPIOF_AHB->DATA_Bits[LED_BLUE] ^= LED_BLUE; //set GPIO data to turn off the LED
}
