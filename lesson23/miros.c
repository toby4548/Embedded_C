#include <stdint.h>
#include "miros.h"

OSThread * volatile OS_curr; //pointer to the current thread
OSThread * volatile OS_next; //pointer to the next thread

void OS_init(void) {
    //set the pendSV interrupt priority to the lowest level
    *(uint32_t volatile *)0xE000ED20 |= (0xFFU<<16);
}

void OS_sched(void){
    //OS_next = ...
    if (OS_next != OS_curr) {
        *(uint32_t volatile *)0xE000ED04 = (1U<<28);
    }
}

void OSThread_start(
    OSThread *me,
    OSThreadHandler threadHandler,
    void *stkSto, uint32_t stkSize) {
        
    uint32_t *sp = (uint32_t*)((((uint32_t)stkSto + stkSize)/8)*8);
    uint32_t *stk_limit;  
        
    //fabric cortex-m isr stack frame for blinky1
    *(--sp) = (1U<<24); //xPSR
    *(--sp) = (uint32_t)threadHandler; //PC
    *(--sp) = 0x0000000EU; //LR
    *(--sp) = 0x0000000CU; //R12
    *(--sp) = 0x00000003U; //R3
    *(--sp) = 0x00000002U; //R2
    *(--sp) = 0x00000001U; //R1
    *(--sp) = 0x00000000U; //R0
    //fake register R4-R11
    *(--sp) = 0x0000000BU; //R11
    *(--sp) = 0x0000000AU; //R10
    *(--sp) = 0x00000009U; //R9
    *(--sp) = 0x00000008U; //R8
    *(--sp) = 0x00000007U; //R7
    *(--sp) = 0x00000006U; //R6
    *(--sp) = 0x00000005U; //R5
    *(--sp) = 0x00000004U; //R4
        
    me->sp = sp;
    stk_limit = (uint32_t*)(((((uint32_t)stkSto -1U)/8)+1U)*8);
    
    for (sp = sp - 1U;sp>=stk_limit;--sp) {
        *sp = 0xDEADBEEFU;
    }
}

__asm
void PendSV_Handler(void) {
    IMPORT  OS_curr //extern variable
    IMPORT  OS_next //extern variable
    
    //57:     __disable_irq(); 
    CPSID         I
    
    //58:     if (OS_curr != (OSThread *)0) { 
    LDR           r1,=OS_curr
    LDR           r1,[r1,#0x00]
    CBZ           r1,PendSV_restore
    
    //59:         //push register r4-t11 on the stack 
    PUSH          {r4-r11}
    
    //60:         OS_curr->sp = sp; 
    LDR           r1,=OS_curr 
    LDR           r1,[r1,#0x00]
    STR           sp,[r1,#0x00]
    //61:     } 
    
PendSV_restore
    //62:     sp = OS_next->sp; 
    LDR           r1,=OS_next
    LDR           r1,[r1,#0x00]
    LDR           sp,[r1,#0x00]
    
    //63:     OS_curr = OS_next; 
    LDR           r1,=OS_next
    LDR           r1,[r1,#0x00]
    LDR           r2,=OS_curr
    STR           r1,[r2,#0x00]
    
    //64:     //pop register r4-r11 
    POP           {r4-r11}
    
    //65:     __enable_irq(); 
    CPSIE         I
    
    //return to the next thread
    BX            lr
}
