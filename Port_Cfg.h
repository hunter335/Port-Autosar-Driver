 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mohamed Tarek
 ******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/*
 * Macros for Dio Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                (STD_ON)

/* Pre-compile option for set pin direction  API */
#define PORT_SET_PIN_DIRECTION_API           (STD_ON)

/* Pre-compile option for set pin mode  API */
#define PORT_SET_PIN_MODE_API                (STD_ON)


 /* Enum to hold  intial values for port pin. */
typedef enum 
{
  PORT_PIN_LEVEL_HIGH,PORT_PIN_LEVEL_LOW

}Port_PinLevelValue;

/* Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_PinInternalResistor;

 /* Enum to hold  intial mode for port pin */
typedef enum 
{
  PORT_PIN_MODE_ADC,PORT_PIN_MODE_DIO,PORT_PIN_MODE_PWM0,PORT_PIN_MODE_PWM1,
  PORT_PIN_MODE_CAN,PORT_PIN_MODE_SPI,PORT_PIN_MODE_QEI,PORT_PIN_MODE_UART,
  PORT_PIN_MODE_I2C
} Port_PinInitialMode ;

/* Number of the configured Dio Channels */
#define PORT_CONFIGURED_PINS                 (43U)

/* Channel Index in the array of structures in Port_PBcfg.c */

#define PortConf_PORT_A_PIN_0_INDEX                    0
#define PortConf_PORT_A_PIN_1_INDEX                    1
#define PortConf_PORT_A_PIN_2_INDEX                    2
#define PortConf_PORT_A_PIN_3_INDEX                    3
#define PortConf_PORT_A_PIN_4_INDEX                    4
#define PortConf_PORT_A_PIN_5_INDEX                    5
#define PortConf_PORT_A_PIN_6_INDEX                    6
#define PortConf_PORT_A_PIN_7_INDEX                    7
#define PortConf_PORT_B_PIN_0_INDEX                    8
#define PortConf_PORT_B_PIN_1_INDEX                    9
#define PortConf_PORT_B_PIN_2_INDEX                    10
#define PortConf_PORT_B_PIN_3_INDEX                    11
#define PortConf_PORT_B_PIN_4_INDEX                    12
#define PortConf_PORT_B_PIN_5_INDEX                    13
#define PortConf_PORT_B_PIN_6_INDEX                    14
#define PortConf_PORT_B_PIN_7_INDEX                    15
#define PortConf_PORT_C_PIN_0_INDEX                    16
#define PortConf_PORT_C_PIN_1_INDEX                    17
#define PortConf_PORT_C_PIN_2_INDEX                    18
#define PortConf_PORT_C_PIN_3_INDEX                    19
#define PortConf_PORT_C_PIN_4_INDEX                    20
#define PortConf_PORT_C_PIN_5_INDEX                    21
#define PortConf_PORT_C_PIN_6_INDEX                    22
#define PortConf_PORT_C_PIN_7_INDEX                    23
#define PortConf_PORT_D_PIN_0_INDEX                    24
#define PortConf_PORT_D_PIN_1_INDEX                    25
#define PortConf_PORT_D_PIN_2_INDEX                    26
#define PortConf_PORT_D_PIN_3_INDEX                    27
#define PortConf_PORT_D_PIN_4_INDEX                    28
#define PortConf_PORT_D_PIN_5_INDEX                    29
#define PortConf_PORT_D_PIN_6_INDEX                    30
#define PortConf_PORT_D_PIN_7_INDEX                    31
#define PortConf_PORT_E_PIN_0_INDEX                    32
#define PortConf_PORT_E_PIN_1_INDEX                    33
#define PortConf_PORT_E_PIN_2_INDEX                    34
#define PortConf_PORT_E_PIN_3_INDEX                    35
#define PortConf_PORT_E_PIN_4_INDEX                    36
#define PortConf_PORT_E_PIN_5_INDEX                    37
#define PortConf_PORT_F_PIN_0_INDEX                    38
#define PortConf_PORT_F_PIN_1_INDEX                    39
#define PortConf_PORT_F_PIN_2_INDEX                    40
#define PortConf_PORT_F_PIN_3_INDEX                    41
#define PortConf_PORT_F_PIN_4_INDEX                    42

/*Port configured port names*/                         
#define PortConf_PORT_A                                0
#define PortConf_PORT_B                                1
#define PortConf_PORT_C                                2
#define PortConf_PORT_D                                3
#define PortConf_PORT_E                                4
#define PortConf_PORT_F                                5


/*Port configured pin IDs*/
#define PortConf_PORT_A_PIN_0                           0
#define PortConf_PORT_A_PIN_1                           1
#define PortConf_PORT_A_PIN_2                           2
#define PortConf_PORT_A_PIN_3                           3
#define PortConf_PORT_A_PIN_4                           4
#define PortConf_PORT_A_PIN_5                           5
#define PortConf_PORT_A_PIN_6                           6
#define PortConf_PORT_A_PIN_7                           7

#define PortConf_PORT_B_PIN_0                           0
#define PortConf_PORT_B_PIN_1                           1
#define PortConf_PORT_B_PIN_2                           2
#define PortConf_PORT_B_PIN_3                           3
#define PortConf_PORT_B_PIN_4                           4
#define PortConf_PORT_B_PIN_5                           5
#define PortConf_PORT_B_PIN_6                           6
#define PortConf_PORT_B_PIN_7                           7

#define PortConf_PORT_C_PIN_0                           0
#define PortConf_PORT_C_PIN_1                           1
#define PortConf_PORT_C_PIN_2                           2
#define PortConf_PORT_C_PIN_3                           3
#define PortConf_PORT_C_PIN_4                           4
#define PortConf_PORT_C_PIN_5                           5
#define PortConf_PORT_C_PIN_6                           6
#define PortConf_PORT_C_PIN_7                           7

#define PortConf_PORT_D_PIN_0                           0
#define PortConf_PORT_D_PIN_1                           1
#define PortConf_PORT_D_PIN_2                           2
#define PortConf_PORT_D_PIN_3                           3
#define PortConf_PORT_D_PIN_4                           4
#define PortConf_PORT_D_PIN_5                           5
#define PortConf_PORT_D_PIN_6                           6
#define PortConf_PORT_D_PIN_7                           7

#define PortConf_PORT_E_PIN_0                           0
#define PortConf_PORT_E_PIN_1                           1
#define PortConf_PORT_E_PIN_2                           2
#define PortConf_PORT_E_PIN_3                           3
#define PortConf_PORT_E_PIN_4                           4
#define PortConf_PORT_E_PIN_5                           5


#define PortConf_PORT_F_PIN_0                           0
#define PortConf_PORT_F_PIN_1                           1
#define PortConf_PORT_F_PIN_2                           2
#define PortConf_PORT_F_PIN_3                           3
#define PortConf_PORT_F_PIN_4                           4




#endif /* PORT_CFG_H */



/*
 UART_PINS = 0 , 1 , 8, 9 , 20 21 22 23 28 29 30 31 32 33 36 37 38 39
 SPI_PINS = 2,3,4,5,12,13,14,15,24,25,26,27,38,39,40,41
 I2C_PINS = 14,15,18,19,24,25,36,37




*/