#include "lm4f120h5qr.h"
#include "delay.h"

#define LED_RED   (1U<<1)
#define LED_BLUE  (1U<<2)
#define LED_GREEN (1U<<3)

//unsigned fact(unsigned n);
void swap(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

int *new_swap(int *x, int *y) {
    int static tmp[2];
    tmp[0] = *x;
    tmp[1] = *y;
    *x = tmp[1];
    *y = tmp[0];
    return tmp;
}

int main() {
    /*
    unsigned volatile x;
    
    x =fact(0U);
    x = 2U + 3U *fact(1U);
    (void)fact(7U);
    */ 
    
    
  
    
  
    SYSCTL_RCGCGPIO_R |= (1U<<5); //Turn off Port F clock gating
    SYSCTL_GPIOHBCTL_R |= (1U<<5); //enable AHB for GPIO
    GPIO_PORTF_AHB_DIR_R |= (LED_RED|LED_BLUE|LED_GREEN); //set pin to output
    GPIO_PORTF_AHB_DEN_R |= (LED_RED|LED_BLUE|LED_GREEN); //set output type to digital output
    
    GPIO_PORTF_AHB_DATA_BITS_R[LED_BLUE]= LED_BLUE;//turn the blue LED on
    while (1) {
        int x = 1000000;
        int y = 1000000/2;
    
        int *p = new_swap(&x, &y);
      
        //GPIO_PORTF_DATA_R |= LED_RED; //set GPIO data to turn on the LED
        //*((unsigned long volatile *)(0x40025000+(LED_RED<<2))) = LED_RED;
        //*(GPIO_PORTF_DATA_BITS_R + LED_RED) = LED_RED;
        GPIO_PORTF_AHB_DATA_BITS_R[LED_RED] = LED_RED;
        
        delay(p[0]);
        
        GPIO_PORTF_AHB_DATA_BITS_R[LED_RED] = 0; //set GPIO data to turn off the LED
        
        delay(p[1]);
    }
    
    //return 0;
}

/*
unsigned fact(unsigned n){
    //0! = 1
    //n! = n*(n-1)! for n >0
    unsigned foo[6];
    foo[n] = n;
  
    if (n == 0U) {
        return 1U;
    }
    else {
        return n*fact(n-1U);
    }
}
*/
