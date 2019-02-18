#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/rom_map.h"

/**
 * hello.c
 */

volatile int32_t pressed = 0;

void toggleButtonPressed(){
    GPIOIntClear(GPIO_PORTH_BASE, GPIO_INT_PIN_0);
    pressed = 1;
}

void Timer0IntHandler()
{
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_MATCH);
    pressed = 1;
}

int main(void)
{

    volatile uint32_t ui32Loop;
    volatile uint32_t g_ui32SysClock;

    g_ui32SysClock = MAP_SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                             SYSCTL_OSC_MAIN |
                                             SYSCTL_USE_PLL |
                                             SYSCTL_CFG_VCO_480), 120000000);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, g_ui32SysClock);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOH) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK) || !SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0))
    {
    }


    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_6);
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_7);

    GPIOIntRegister(GPIO_PORTH_BASE, toggleButtonPressed);
    GPIOIntTypeSet(GPIO_PORTH_BASE, GPIO_PIN_0, GPIO_RISING_EDGE);
    GPIOIntEnable(GPIO_PORTH_BASE, GPIO_PIN_0);

    TimerIntRegister(TIMER0_BASE, TIMER_A, Timer0IntHandler);

    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_MATCH);
    TimerMatchSet(TIMER0_BASE, TIMER_A, 0x400);
    TimerEnable(TIMER0_BASE, TIMER_A);

    int button_pressed_times = 0;
    int num_of_LEDs = 4;
//  int32_t last_pressed = 0;

    while(1)
    {
//        // Delay
//        for (ui32Loop = 0; ui32Loop < 100000; ui32Loop++) {
//        }
//
//        last_pressed = pressed;
//        pressed = GPIOPinRead(GPIO_PORTH_BASE, GPIO_PIN_0);

          if (pressed) {
       // if (last_pressed == 0 && pressed == 1) {
            button_pressed_times++;
            pressed = 0;
            //time_out = 0;

            if (button_pressed_times % num_of_LEDs == 1) {
                GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_1, GPIO_PIN_1);
                GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_6, 0x0);
                GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_7, 0x0);
            }
            else if (button_pressed_times % num_of_LEDs == 2) {
                GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_1, GPIO_PIN_1);
                GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_6, GPIO_PIN_6);
                GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_7, 0x0);
            }
            else if (button_pressed_times % num_of_LEDs == 3) {
                GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_1, GPIO_PIN_1);
                GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_6, GPIO_PIN_6);
                GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_7, GPIO_PIN_7);
            }
            else {
                GPIOPinWrite(GPIO_PORTH_BASE, GPIO_PIN_1, 0x0);
                GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_6, 0x0);
                GPIOPinWrite(GPIO_PORTK_BASE, GPIO_PIN_7, 0x0);
            }
        }

    }
}
