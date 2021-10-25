#ifndef Timer_H
#define Timer_H
extern void Timer0in(void);
extern void Timer1in(void);
extern void GPIOF_init(void);
extern void GPIOE_init(void);
extern void GPIOD_init(void);
extern void UARTin(void);
extern char readchar(void);
extern void printchar(char c);
extern void print(char *string);
extern void GPIOPortF_Handler();
#endif
