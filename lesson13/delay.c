#include "delay.h"

void delay(int volatile iter) {
    while (iter>0) {
            --iter;
        }
}