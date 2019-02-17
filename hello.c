#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

/**
 * hello.c
 */
int main(void)
{

    volatile uint32_t ui32Loop;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_UART0);

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOH) || !SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOK))
    {
    }


    GPIOPinTypeGPIOInput(GPIO_PORTH_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTH_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_6);
    GPIOPinTypeGPIOOutput(GPIO_PORTK_BASE, GPIO_PIN_7);

    int button_pressed_times = 0;
    int num_of_LEDs = 4;
    int32_t pressed = 0;
    int32_t last_pressed = 0;

    while(1)
    {
        // Delay
        for (ui32Loop = 0; ui32Loop < 100000; ui32Loop++) {
        }

        last_pressed = pressed;
        pressed = GPIOPinRead(GPIO_PORTH_BASE, GPIO_PIN_0);


        if (last_pressed == 0 && pressed == 1) {
            button_pressed_times++;

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
