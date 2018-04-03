#include "delay.h"

void delay(int iter) {
    int volatile counter = 0;
    while (counter<iter) {
            counter++;
        }
}