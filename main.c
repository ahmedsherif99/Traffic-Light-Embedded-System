#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc\tm4c123gh6pm.h"
#include "inc\hw_timer.h"
#include "inc\hw_gpio.h"
#include "driverlib\timer.h"
#include "driverlib\gpio.h"
#include "driverlib\sysctl.h"
#include "inc\tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "inc/hw_ints.h"
#include "tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/GPIO.h"
#include "Projectfunctions.h"
int main(void)
{
    GPIOF_init();
    GPIOD_init();
    GPIOE_init();
    Timer0in();
    Timer1in();
    UARTin();
    while(1){

    }
	return 0;
}
