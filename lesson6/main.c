#include "lm4f120h5qr.h"

int main() {
    SYSCTL_RCGCGPIO_R = 0x20U; //Turn off Port F clock gating
    GPIO_PORTF_DIR_R = 0x0EU; //set pin to output
    GPIO_PORTF_DEN_R = 0x0EU; //set output type to digital output
    
    while (1) {
        int volatile counter = 0;
        while (counter<1000000) {
            counter++;
        }
        GPIO_PORTF_DATA_R = 0x02U; //set GPIO data to turn on the LED
        
        counter = 0;
        while (counter<1000000) {
            counter++;
        }
        GPIO_PORTF_DATA_R = 0x00U; //set GPIO data to turn off the LED
    }
    
    return 0;
}
