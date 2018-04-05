#include "lm4f120h5qr.h"
#include "delay.h"

#define LED_RED   (1U<<1)
#define LED_BLUE  (1U<<2)
#define LED_GREEN (1U<<3)

unsigned fact(unsigned n);

int main() {
    unsigned volatile x;
    
    x =fact(0U);
    x = 2U + 3U *fact(1U);
    (void)fact(5U);
  
    SYSCTL_RCGCGPIO_R |= (1U<<5); //Turn off Port F clock gating
    SYSCTL_GPIOHBCTL_R |= (1U<<5); //enable AHB for GPIO
    GPIO_PORTF_AHB_DIR_R |= (LED_RED|LED_BLUE|LED_GREEN); //set pin to output
    GPIO_PORTF_AHB_DEN_R |= (LED_RED|LED_BLUE|LED_GREEN); //set output type to digital output
    
    GPIO_PORTF_AHB_DATA_BITS_R[LED_BLUE]= LED_BLUE;//turn the blue LED on
    while (1) {
        //GPIO_PORTF_DATA_R |= LED_RED; //set GPIO data to turn on the LED
        //*((unsigned long volatile *)(0x40025000+(LED_RED<<2))) = LED_RED;
        //*(GPIO_PORTF_DATA_BITS_R + LED_RED) = LED_RED;
        GPIO_PORTF_AHB_DATA_BITS_R[LED_RED] = LED_RED;
        
      
        delay(1000000);
        
        GPIO_PORTF_AHB_DATA_BITS_R[LED_RED] = 0; //set GPIO data to turn off the LED
        
        delay(500000);
    }
    
    return 0;
}

unsigned fact(unsigned n){
    //0! = 1
    //n! = n*(n-1)! for n >0
  
    if (n == 0U) {
        return 1U;
    }
    else {
        return n*fact(n-1U);
    }
}

