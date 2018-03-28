int counter = 0;
int main() {
    *((unsigned int *)0x400FE608U) = 0x20U; //Turn off Port F clock gating
    *((unsigned int *)0x40025400U) = 0x0eU; //set pin to output
    *((unsigned int *)0x4002551CU) = 0x0eU; //set output type to digital output
    
    while (1) {
        int counter = 0;
        while (counter<10000000) {
            counter++;
        }
        *((unsigned int *)0x400253FCU) = 0x02U; //set GPIO data to turn on the LED
        
        counter = 0;
        while (counter<10000000) {
            counter++;
        }
        *((unsigned int *)0x400253FCU) = 0x00U; //set GPIO data to turn off the LED
    }
    
    return 0;
}
