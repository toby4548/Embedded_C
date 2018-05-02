#ifndef MIROS_H
#define MIROS_H

#include <stdint.h>

typedef struct {
    void *sp; //stack pointer
} OSThread;

typedef void (*OSThreadHandler) ();

void OS_init(void);

//this function must be called with interrupts DISABLED
void OS_sched(void);

void OSThread_start(
    OSThread *me,
    OSThreadHandler threadHandler,
    void *stkSto, uint32_t stkSize);

void PendSV_Handler(void);

#endif /* MIROS_H*/
