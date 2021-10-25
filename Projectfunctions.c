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
volatile int x=0;
volatile int flag=0;
volatile int statflagN=0;
volatile int statflagE=0;
volatile int statflagPN=0;
volatile int statflagPE=0;
void printchar(char c){
while((UART0_FR_R&(1<<5))!=0);
UART0_DR_R=c;
}
void print(char *string){
  while(*string){
  printchar(*(string++));
  }
}
void GPIOPortF_Handler(){
    if(flag==0){
    if(((GPIO_PORTF_DATA_R&0x01)==0)&&((GPIO_PORTE_DATA_R&0x01)==1)){
GPIO_PORTD_DATA_R|=0x01;
GPIO_PORTD_DATA_R&=~0x02;
GPIO_PORTE_DATA_R|=0x04;
GPIO_PORTE_DATA_R&=~0x03;
if(statflagPN==0){
           print("\n\rPEDESTRIAN NORTH SOUTH\n\r");
           statflagN=0;
           statflagE=0;
           statflagPN=1;
           statflagPE=0;
       }
volatile int t=0;
for(t;t<2;t++){
    TimerEnable(TIMER1_BASE, TIMER_A);
     while((TIMER1_RIS_R&0x01)!=1){}
     TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
}
TimerDisable(TIMER1_BASE, TIMER_A);
    }
    else if(((GPIO_PORTF_DATA_R&0x10)==0)&&((GPIO_PORTE_DATA_R&0x08)==0x08)){
 GPIO_PORTD_DATA_R|=0x04;
 GPIO_PORTD_DATA_R&=~0x08;
 GPIO_PORTE_DATA_R|=0x20;
 GPIO_PORTE_DATA_R&=~0x18;
 GPIO_PORTD_DATA_R&=~0x40;
 if(statflagPE==0){
            print("\n\rPEDESTRIAN EAST WEST\n\r");
            statflagN=0;
            statflagE=0;
            statflagPN=0;
            statflagPE=1;
        }
 volatile int t=0;
 for(t;t<2;t++){
     TimerEnable(TIMER1_BASE, TIMER_A);
      while((TIMER1_RIS_R&0x01)!=1){}
      TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
 }
 TimerDisable(TIMER1_BASE, TIMER_A);
     }
    GPIO_PORTD_DATA_R=0x0A;
flag=1;
TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
TimerEnable(TIMER1_BASE, TIMER_A);
    }
};
void Timer0A_Handler(){
    TimerIntClear(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
    if(x<6){
        x++;
        GPIO_PORTE_DATA_R|=0x01;
        GPIO_PORTE_DATA_R&=~0x06;
        if(statflagN==0){
            print("\n\rCARS NORTH SOUTH\n\r");
            statflagN=1;
            statflagE=0;
            statflagPN=0;
            statflagPE=0;
        }
    }
    if(6<=x&&x<9){
        x++;
        GPIO_PORTE_DATA_R|=0x02;
        GPIO_PORTE_DATA_R&=~0x05;
    } if(x==9||x==10){
        x++;
        GPIO_PORTE_DATA_R|=0x04;
        GPIO_PORTE_DATA_R&=~0x03;
    } if(11<=x&&x<17){
        x++;
        GPIO_PORTE_DATA_R|=0x08;
        GPIO_PORTE_DATA_R&=~0x30;
        if(statflagE==0){
                 print("\n\rCARS EAST WEST\n\r");
                 statflagN=0;
                 statflagE=1;
                 statflagPN=0;
                 statflagPE=0;
             }
    } if(17<=x&&x<20){
        x++;
        GPIO_PORTD_DATA_R|=0x40;
        GPIO_PORTE_DATA_R&=~0x28;
    } if(x==20){
        x=0;
        GPIO_PORTE_DATA_R|=0x20;
        GPIO_PORTE_DATA_R&=~0x18;
        GPIO_PORTD_DATA_R&=~0x40;
    }
}
void Timer1A_Handler(){
 flag=0;
    TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
    TimerIntDisable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
    TimerDisable(TIMER1_BASE,TIMER_A);
}

void GPIOF_init(){
  SYSCTL_RCGCGPIO_R |= 0x00000020;      //Initialize clock to PORTF
  while((SYSCTL_PRGPIO_R&0x00000020) == 0){};   //safety for clock initialization
  GPIO_PORTF_LOCK_R = 0x4C4F434B;
  GPIO_PORTF_CR_R = 0x1F;       //Enable change to PORTF
  GPIO_PORTF_DIR_R = 0x0E;      //Make led ports as output
  GPIO_PORTF_DEN_R = 0x1F;      // digital enable to pins
  GPIO_PORTF_PUR_R = 0x11;
  GPIOIntRegister(GPIO_PORTF_BASE, GPIOPortF_Handler);
  GPIOIntTypeSet(GPIO_PORTF_BASE ,GPIO_PIN_4,GPIO_LOW_LEVEL);
  GPIOIntTypeSet(GPIO_PORTF_BASE ,GPIO_PIN_0,GPIO_LOW_LEVEL);
  GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_4);
   GPIOIntEnable(GPIO_PORTF_BASE,GPIO_PIN_0);
 // IntPrioritySet(INT_GPIOF,0xE0);
}
void GPIOD_init(){
  SYSCTL_RCGCGPIO_R |= 0x00000008;      //Initialize clock to PORTD
  while((SYSCTL_PRGPIO_R&0x00000008) == 0){};   //safety for clock initialization
  GPIO_PORTD_LOCK_R = 0x4C4F434B;
  GPIO_PORTD_CR_R = 0xFF;       //Enable change to PORTD
  GPIO_PORTD_DIR_R = 0xFF;      //Make led ports as output
  GPIO_PORTD_DEN_R = 0xFF;      // digital enable to pins
  GPIO_PORTD_DATA_R=0x0A;
}
void GPIOE_init(){
  SYSCTL_RCGCGPIO_R |= 0x00000010;      //Initialize clock to PORTE
  while((SYSCTL_PRGPIO_R&0x00000010) == 0){};   //safety for clock initialization
  GPIO_PORTE_LOCK_R = 0x4C4F434B;
  GPIO_PORTE_CR_R = 0x2F;       //Enable change to PORTE
  GPIO_PORTE_DIR_R = 0x2F;      //Make led ports as output
  GPIO_PORTE_DEN_R = 0x2F;      // digital enable to pins
}
void Timer0in(){
SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);  //enable clock for timer 0
 while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0)) //wait till clock is connected
   {}
  TimerConfigure(TIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC)); //set timer A for periodic
  TimerLoadSet(TIMER0_BASE, TIMER_A, 65535); // set Timer A for max time (4.096 ms)
  TimerPrescaleSet(TIMER0_BASE,TIMER_A,244);  //prescaling Timer A from 4.096 ms to 1s
  TimerIntEnable(TIMER0_BASE,TIMER_TIMA_TIMEOUT);
  TimerIntRegister(TIMER0_BASE,TIMER_A, Timer0A_Handler);
IntPrioritySet(INT_TIMER0A,0xE0);
  TimerEnable(TIMER0_BASE, TIMER_A);
   }
void Timer1in(){
SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);  //enable clock for timer 1
 while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1)) //wait till clock is connected
   {}
  TimerConfigure(TIMER1_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PERIODIC)); //set timer A for periodic
  TimerLoadSet(TIMER1_BASE, TIMER_A, 65535); // set Timer A for max time (4.096 ms)
  TimerPrescaleSet(TIMER1_BASE,TIMER_A,244);  //prescaling Timer A from 4.096 ms to 1 sec
  TimerIntRegister(TIMER1_BASE,TIMER_A, Timer1A_Handler);
  IntPrioritySet(INT_TIMER0A,0);
   }
void UARTin(){
SYSCTL_RCGCUART_R|=0X0001;
 SYSCTL_RCGCGPIO_R |= 0x00000001;
UART0_CTL_R &= ~0x0001;
UART0_CC_R=0X0;
UART0_IBRD_R=104;
UART0_FBRD_R=11;
UART0_LCRH_R=(0x3<<5);
GPIO_PORTA_AFSEL_R|=0X03;
GPIO_PORTA_PCTL_R=0X011;
GPIO_PORTA_DEN_R|=0X03;
UART0_CTL_R=0x0301;
}
