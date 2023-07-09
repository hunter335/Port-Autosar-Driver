/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Private.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mustafa Elsayad
 ******************************************************************************/
#ifndef PORT_PRIVATE_H
#define PORT_PRIVATE_H
#include "Std_Types.h"

#define PA0 0
#define PA1 1
#define PA2 2
#define PA3 3
#define PA4 4
#define PA5 5
#define PA6 6
#define PA7 7
#define PB0 8
#define PB1 9
#define PB2 10
#define PB3 11
#define PB4 12
#define PB5 13
#define PB6 14
#define PB7 15
#define PC0 16
#define PC1 17
#define PC2 18
#define PC3 19
#define PC4 20
#define PC5 21
#define PC6 22
#define PC7 23
#define PD0 24
#define PD1 25
#define PD2 26
#define PD3 27
#define PD4 28
#define PD5 29
#define PD6 30
#define PD7 31
#define PE0 32
#define PE1 33
#define PE2 34
#define PE3 35
#define PE4 36
#define PE5 37
#define PF0 38
#define PF1 39
#define PF2 40
#define PF3 41
#define PF4 42

#define ADC_NUMBER_OF_PINS  12
#define PWM0_NUMBER_OF_PINS  10
#define PWM1_NUMBER_OF_PINS 10
#define UART_NUMBER_OF_PINS 18
#define SPI_NUMBER_OF_PINS  16
#define I2C_NUMBER_OF_PINS  8
#define CAN_NUMBER_OF_PINS  6
#define QEI_NUMBER_OF_PINS  9


#define PWM0_MODE_NUM  4
#define PWM1_MODE_NUM  5
#define UART_MODE_NUM  1
#define SPI_MODE_NUM   2
#define I2C_MODE_NUM   3
#define CAN_MODE_NUM   8
#define QEI_MODE_NUM   6





uint8 ADC_Pins[12]={PE3,PE2,PE1,PE0,PD3,PD2,PD1,PD0,PE5,PE4,PB4,PB5};
/*MODE = 4*/
uint8 PWM0_Pins[10]={PB6,PB7,PB4,PB5,PE4,PE5,PC4,PD0,PC5,PD1};
/*MODE =5*/
uint8 PWM1_Pins[10]={PD0,PD1,PA6,PE4,PA7,PE5,PF0,PF1,PF2,PF3};

/*MODE = 1 */
uint8 UART_Pins[18]={PA0,PA1,PF1,PF0,PB0,PB1,PD6,PD7,PC6,PC7,PC4,PC5,PE4,PE5,PD4,PD5,PE0,PE1};
/*MODE = 2*/
uint8 SPI_Pins[16]={PA2,PA3,PA4,PA5,PF2,PD0,PF3,PD1,PF0,PD2,PF1,PD3,PB4,PB5,PB6,PB7};
/*MODE = 3 */
uint8 I2C_Pins[8]={PB2,PB3,PA6,PA7,PE4,PE5,PD0,PD1};
/*MODE=8*/
uint8 CAN_Pins[6]={PB4,PE4,PB5,PE5,PA0,PA1};
/*MODE = 6 */
uint8 QEI_Pins[9]={PF4,PD3,PC4,PF0,PD6,PC5,PD7,PF1,PC6};





#endif