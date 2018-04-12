#include <stdint.h>
#include "lm4f120h5qr.h"
#include "delay.h"

#define LED_RED   (1U<<1)
#define LED_BLUE  (1U<<2)
#define LED_GREEN (1U<<3)


typedef struct Point{
    uint16_t x;
    uint8_t y;
} Point;

struct Point pa, pb;


Point p1, p2;

uint8_t u8a, u8b;
uint16_t u16c, u16d;
uint32_t u32e, u32f;

int8_t s8;
int16_t s16;
int32_t s32;


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

    p1.x = sizeof(Point);
    p1.y = p1.x - 3U;
  
    u8a = sizeof(u8a);
    u16c = sizeof(uint16_t);
    u32e = sizeof(uint32_t);
    
    u8a = 0xA1U;
    u16c = 0xc1c2U;
    u32e = 0xe1e2e3e4U;
    
    u8b = u8a;
    u16d = u16c;
    u32f = u32e;
    
    u16c = 40000U;
    u16d = 30000U;
    u32e = (uint32_t)u16c+u16d;
    
    u16c = 100U;
    s32 = 10 - (int16_t)u16c;
    
    if ((int32_t)u32e > -1) { //always false w/o cast
        u8a = 1U;
    }
    else {
        u8a = 0U;
    }
    
    u8a = 0xffU;
    if ((uint8_t)~u8a ==0x00U) {
        u8b = 1U;
    }
    
  
    SYSCTL_RCGCGPIO_R |= (1U<<5); //Turn off Port F clock gating
    SYSCTL_GPIOHBCTL_R |= (1U<<5); //enable AHB for GPIO
    GPIO_PORTF_AHB_DIR_R |= (LED_RED|LED_BLUE|LED_GREEN); //set pin to output
    GPIO_PORTF_AHB_DEN_R |= (LED_RED|LED_BLUE|LED_GREEN); //set output type to digital output
    
    GPIO_PORTF_AHB_DATA_BITS_R[LED_BLUE]= LED_BLUE;//turn the blue LED on
    while (1) {
        int x = 1000000;
        int y = 1000000/2;
    
        int *p = new_swap(&x, &y);
      

        GPIO_PORTF_AHB_DATA_BITS_R[LED_RED] = LED_RED;
        
        delay(p[0]);
        
        GPIO_PORTF_AHB_DATA_BITS_R[LED_RED] = 0; //set GPIO data to turn off the LED
        
        delay(p[1]);
    }
    
    //return 0;
}

