/*
 * CAN bus LED controller master firmware
 * Written for TI Tiva TM4C1294NCPDT
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/pin_map.h"

#include "utils/uartstdio.h"

#define PI 3.14159265359f

volatile bool errFlag = 0; // transmission error flag
unsigned int sysClock; // clockspeed in hz

void delay(unsigned int milliseconds) {
	SysCtlDelay((sysClock / 3) * (milliseconds / 1000.0f));
}

// CAN interrupt handler
void CANIntHandler(void) {

	unsigned long status = CANIntStatus(CAN0_BASE, CAN_INT_STS_CAUSE); // read interrupt status

	if(status == CAN_INT_INTID_STATUS) { // controller status interrupt
		status = CANStatusGet(CAN0_BASE, CAN_STS_CONTROL); // read back error bits, do something with them?
		errFlag = 1;
	} else if(status == 1) { // message object 1
		CANIntClear(CAN0_BASE, 1); // clear interrupt
		errFlag = 0; // clear any error flags
	} else { // should never happen
		UARTprintf("Unexpected CAN bus interrupt\n");
	}
}

int main(void) {

	tCANMsgObject msg; // the CAN message object
	unsigned int msgData; // the message data is four bytes long which we can allocate as an int32
	unsigned char *msgDataPtr = (unsigned char *)&msgData; // make a pointer to msgData so we can access individual bytes

	// Run from the PLL at 120 MHz.
	//sysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN | SYSCTL_USE_PLL | SYSCTL_CFG_VCO_480), 120000000);
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	// Set up debugging UART
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // enable UART0 GPIO peripheral
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	//UARTStdioConfig(0, 115200, sysClock); // 115200 baud
	UARTStdioConfig(0, 115200, SysCtlClockGet());

	// Set up CAN0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE); // enable CAN0 GPIO peripheral
	GPIOPinConfigure(GPIO_PE4_CAN0RX);
	GPIOPinConfigure(GPIO_PE5_CAN0TX);
	GPIOPinTypeCAN(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0);
	CANInit(CAN0_BASE);
	//CANBitRateSet(CAN0_BASE, sysClock, 500000);
	CANBitRateSet(CAN0_BASE, SysCtlClockGet(), 500000);
	CANIntRegister(CAN0_BASE, CANIntHandler); // use dynamic vector table allocation
	CANIntEnable(CAN0_BASE, CAN_INT_MASTER | CAN_INT_ERROR | CAN_INT_STATUS);
	IntEnable(INT_CAN0);
	CANEnable(CAN0_BASE);

	// Set up msg object
	msgData = 0;
	msg.ui32MsgID = 1;
	msg.ui32MsgIDMask = 0;
	msg.ui32Flags = MSG_OBJ_TX_INT_ENABLE;
	msg.ui32MsgLen = sizeof(msgDataPtr);
	msg.pui8MsgData = msgDataPtr;

	unsigned int t = 0; // loop counter
	float freq = 0.3; // frequency scaler

	while(1) {

		// set up next colour (scale sinf (0-1) to 0-255)
		msgDataPtr[0] = (0.5 + 0.5*sinf(t*freq)) * 0xFF;
		msgDataPtr[1] = (0.5 + 0.5*sinf(t*freq + (2*PI/3))) * 0xFF; // 120 degrees out of phase
		msgDataPtr[2] = (0.5 + 0.5*sinf(t*freq + (4*PI/3))) * 0xFF; // 240 degrees out of phase
		msgDataPtr[3] = 128; // 50% intensity
		
		UARTprintf("Sending colour\tr: %d\tg: %d\tb: %d\n", msgDataPtr[0], msgDataPtr[1], msgDataPtr[2]); // write colour to UART for debugging
		CANMessageSet(CAN0_BASE, 1, &msg, MSG_OBJ_TYPE_TX); // send as msg object 1
		//UARTprintf("DEBUG!\n");
		delay(100); // wait 100ms

		if(errFlag) { // check for errors
			UARTprintf("CAN Bus Error\n");
		}

		t++; // overflow is fine
	}

	return 0;
}

