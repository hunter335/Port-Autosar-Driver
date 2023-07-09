/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mustafa Elsayad
 ******************************************************************************/
#ifndef PORT_H
#define PORT_H

/* Id for the company in the AUTOSAR
 * for example Mustafa Elsayad's ID = 1000 :) */

#define PORT_VENDOR_ID                                  (1000U)

/*Port Module ID*/
#define PORT_MODULE_ID                                  (124U)

/*Port Instance ID*/
#define PORT_INSTANCE_ID                                  (0U)


/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Dio Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* PORT Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between Port_Cfg.h and Dio.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Dio.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif

/* Non AUTOSAR files */
#include "Common_Macros.h"

/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
 /*Service id for Port_init*/
 #define PORT_INIT_SID                           (uint8)0x00

  /*Service id for Port_SetPinDirection*/
 #define PORT_SET_PIN_DIRECTION_SID              (uint8)0x01

  /*Service id for Port_RefreshPortDirection*/
 #define PORT_REFRESH_PORT_DIRECTION_SID         (uint8)0x02

  /*Service id for Port_GetVersionInfo*/
 #define PORT_GET_VERSION_INFO_SID                 (uint8)0x03

  /*Service id for Port_SetPinMode*/
 #define PORT_SET_PIN_MODE_SID                   (uint8)0x04


/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/  
 /* Incorrect Port Pin ID passed to Port_SetPinDirection  */
 #define PORT_E_PARAM_PIN                     0x0A

 /* Port Pin not configured as changeable  */
#define PORT_E_DIRECTION_UNCHANGEABLE         0x0B

 /* Port_Init service called with wrong parameter */
#define PORT_E_PARAM_CONFIG                   0x0C

/* Port Pin Mode passed not valid */
#define PORT_E_PARAM_INVALID_MODE             0x0D

/* Port_SetPinMode service called when the mode is unchangeable */
#define PORT_E_MODE_UNCHANGEABLE              0x0E        

/* API service called prior to module initialization */ 
#define PORT_E_UNINIT                         0x0F

/*Port_GetVersionInfo Api called with a NULL Pointer Parameter */
#define PORT_E_PARAM_POINTER                  0x10


 /*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/
 /* Data for the symbolic name of a port pin */
typedef uint8 Port_PinType ;

 /* Data for the symbolic name of a port */
typedef uint8 Port_PortType ;

/*Different modes for port pin*/
typedef uint8 Port_PinModeType; 

 /* Possible directions of a port pin. */
typedef enum
{
  PORT_PIN_IN,PORT_PIN_OUT
} Port_PinDirectionType ;





typedef struct 
{
  /* Member contains the Number of the port */
  Port_PortType Port_Num ;

  /* Member contains the ID of the Pin*/
  Port_PinType Pin_ID ;

  /* Member contains the mode of the Pin*/
  Port_PinDirectionType Pin_Direction;

 /* Member contains the intial level value of the Pin*/
  boolean Pin_Intial_Level_Value;

 /* Member contains the internal resistor type for PIN*/
  uint8 Pin_Inernal_Resistor_type;

   /* Member contains the internal resistor type for PIN*/
  uint8 Pin_Initial_Mode;

   /* Member contains if the direction changeable during run time for a port pin*/
  boolean Pin_Direction_Changeable;

   /* Member contains if the mode changeable during run time for a port pin*/
  boolean Pin_Mode_Changeable;   
  

} Port_ConfigPin;

/* Type of the external data structure containing the initialization data for this module */
typedef struct 
{
  Port_ConfigPin Port_Pins[PORT_CONFIGURED_PINS];


} Port_ConfigType;


 
/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/*Initializes the Port Driver module.*/
void Port_Init( 
 const Port_ConfigType* ConfigPtr 
);

/* Function for PORT Get Version Info API */
#if (PORT_VERSION_INFO_API == STD_ON)
void PORT_GetVersionInfo(Std_VersionInfoType *versioninfo);
#endif

/*Function to Set the port pin direction during runtime*/
#if PORT_SET_PIN_DIRECTION_API == STD_ON
void Port_SetPinDirection(
    Port_PinType Pin,
    Port_PinDirectionType Direction);
#endif

/*Function to Set the port pin mode during runtime*/
#if PORT_SET_PIN_MODE_API == STD_ON
void Port_SetPinMode(
    Port_PinType Pin,
    Port_PinModeType Mode);
#endif
/*Refreshes port direction*/
void Port_RefreshPortDirection(void);


 /*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/ 
/* Extern PB structures to be used by Port and other modules */

extern const Port_ConfigType Port_Configuration ;












#endif /* PORT_H */

