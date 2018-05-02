#include <stdint.h>
#include "bsp.h"
#include "miros.h"

uint32_t stack_blinky1[40];
OSThread blinky1;

void main_blinky1() {
    while (1) {
        BSP_ledGreenOn();
        BSP_delay(BSP_TICKS_PER_SEC / 4U);
		BSP_ledGreenOff();
        BSP_delay(BSP_TICKS_PER_SEC * 3U / 4U);
    }
	
}

uint32_t stack_blinky2[40];
OSThread blinky2;

void main_blinky2() {
    while (1) {
        BSP_ledBlueOn();
        BSP_delay(BSP_TICKS_PER_SEC / 2U);
		BSP_ledBlueOff();
        BSP_delay(BSP_TICKS_PER_SEC / 3U);
    }
	
}


/* background code: sequential with blocking version */
int main() {
  
    BSP_init();
    OS_init();
    
    OSThread_start(&blinky1,
                   &main_blinky1,
                   stack_blinky1, sizeof(stack_blinky1));
    
    OSThread_start(&blinky2,
                   &main_blinky2,
                   stack_blinky2, sizeof(stack_blinky2));
    
    while(1) {
    }
    
    //return 0;
}

