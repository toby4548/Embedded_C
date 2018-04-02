#include "lm4f120h5qr.h"

#define LED_RED   (1U<<1)
#define LED_BLUE  (1U<<2)
#define LED_GREEN (1U<<3)

int main() {
   
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
        
      
        int volatile counter[2] = {0,0};
        while (counter[0]<1000000) {
            counter[0]++;
        }
        
        GPIO_PORTF_AHB_DATA_BITS_R[LED_RED] = 0; //set GPIO data to turn off the LED
        //counter = 0;
        while (*(counter+1)<1000000) {
            counter[1]++;
        } 
    }
    
    return 0;
}
