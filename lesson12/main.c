#include <stdint.h>
#include "tm4c_cmsis.h"
#include "delay.h"

#define LED_RED   (1U<<1)
#define LED_BLUE  (1U<<2)
#define LED_GREEN (1U<<3)


typedef struct {
    uint16_t x;
    uint8_t y;
} Point;

Point p1, p2;

typedef struct {
    Point top_left;
    Point bottom_right;
} Windows;

typedef struct {
    Point corners[3];
} Triangle;

Windows w, w2;
Triangle t;

int *new_swap(int *x, int *y) {
    int static tmp[2];
    tmp[0] = *x;
    tmp[1] = *y;
    *x = tmp[1];
    *y = tmp[0];
    return tmp;
}

int main() {
    Point *pp;
    Windows *wp;

    p1.x = sizeof(Point);
    p1.y = p1.x - 3U;
    
    w.top_left.x = 1U;
    w.bottom_right.y = 2U;
    
    t.corners[0].x = 1U;
    t.corners[2].y = 2U;
  
    p2 = p1;
    w2 = w;
    
    pp = &p1;
    wp = &w2;
    
    (*pp).x = 1U;
    (*wp).top_left = *pp;
    
    pp->x = 1U;
    wp->top_left = *pp;
    
    
    SYSCTL->RCGC2 |= (1U<<5); //Turn off Port F clock gating
    SYSCTL->GPIOHSCTL |= (1U<<5); //enable AHB for GPIO
    GPIOF_HS->DIR |= (LED_RED|LED_BLUE|LED_GREEN); //set pin to output
    GPIOF_HS->DEN |= (LED_RED|LED_BLUE|LED_GREEN); //set output type to digital output
    
    GPIOF_HS->DATA_Bits[LED_BLUE]= LED_BLUE;//turn the blue LED on
    while (1) {
        int x = 1000000;
        int y = 1000000/2;
    
        int *p = new_swap(&x, &y);
      

        GPIOF_HS->DATA_Bits[LED_RED] = LED_RED;
        
        delay(p[0]);
        
        GPIOF_HS->DATA_Bits[LED_RED] = 0; //set GPIO data to turn off the LED
        
        delay(p[1]);
    }
    
    //return 0;
}

