/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Mustafa Elsayad
 ******************************************************************************/

#include "Port.h"
#include "Port_Private.h"
#include "Port_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Dio Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION) || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION) || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Det.h does not match the expected version"
#endif

#endif

/*Global Static Pointer to get the address of the first element in the array of the configrations structs*/
STATIC const Port_ConfigPin *Pins = NULL_PTR;
/*Global Static variable to hold the status of PORT*/
STATIC uint8 PORT_Status = PORT_NOT_INITIALIZED;

/************************************************************************************
 * Service Name: Port_Init
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters (in): ConfigPtr - Pointer to post-build configuration data
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to Initialize the Port module.
 ************************************************************************************/
void Port_Init(
    const Port_ConfigType *ConfigPtr)
{
    volatile uint32 *Port_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    volatile uint32 delay = 0;

#if PORT_DEV_ERROR_DETECT == STD_ON

    /* check if the input configuration pointer is not a NULL_PTR */
    if (ConfigPtr == NULL_PTR)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID, PORT_E_PARAM_CONFIG);
    }
    else
#endif
    {
        /*
         * Set the module state to initialized and point to the PB configuration structure using a global pointer.
         * This global pointer is global to be used by other functions to read the PB configuration structures
         */
        Pins = ConfigPtr->Port_Pins; /* address of the first Port_Pins structure --> Port_Pins[0] */

        uint8 Local_u8Counter;

        for (Local_u8Counter = 0; Local_u8Counter < PORT_CONFIGURED_PINS; Local_u8Counter++)
        {
            switch (Pins[Local_u8Counter].Port_Num)
            {
            case 0:
                Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                break;
            case 1:
                Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                break;
            case 2:
                Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                break;
            case 3:
                Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                break;
            case 4:
                Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                break;
            case 5:
                Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                break;
            }

            /* Enable clock for PORT and allow time for clock to start*/
            SYSCTL_REGCGC2_REG |= (1 << Pins[Local_u8Counter].Port_Num);
            delay = SYSCTL_REGCGC2_REG;
            /*if PD7 or PF0*/
            if (((Pins[Local_u8Counter].Port_Num == 3) && (Pins[Local_u8Counter].Pin_ID == 7)) || ((Pins[Local_u8Counter].Port_Num == 5) && (Pins[Local_u8Counter].Pin_ID == 0)))
            {
                /*Unlock then Commit*/
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_COMMIT_REG_OFFSET), Pins[Local_u8Counter].Pin_ID); /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
            }
            else if ((Pins[Local_u8Counter].Port_Num == 2) && (Pins[Local_u8Counter].Pin_ID <= 3)) /*if PC0 to PC3 */
            {
                /*JTAG pins to nothing*/
            }
            else
            {
                /*do nothing no need to Unlock */
            }
            /******* Direction & Intial level & Internal Resistor ******/
            if (Pins[Local_u8Counter].Pin_Direction == PORT_PIN_OUT)
            {
                /*intializing the pins level value according to the configration pararmeters*/
                if ((Pins[Local_u8Counter].Pin_Intial_Level_Value) == PORT_PIN_LEVEL_HIGH)
                {
                    /*casting the pointer of 32 to 8 because of adding value to a pointer*/
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                }
                else if ((Pins[Local_u8Counter].Pin_Intial_Level_Value) == PORT_PIN_LEVEL_LOW)
                {
                    /*casting the pointer of 32 to 8 because of adding value to a pointer*/
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DATA_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                }
                else
                {
                    /*Do Nothing*/
                }
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
            }
            else if (Pins[Local_u8Counter].Pin_Direction == PORT_PIN_IN)
            {
                /*Set pin as a input*/
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                if (Pins[Local_u8Counter].Pin_Inernal_Resistor_type == PULL_UP)
                {
                    /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                }
                else if (Pins[Local_u8Counter].Pin_Inernal_Resistor_type == PULL_DOWN)
                {
                    /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                }
                else
                {
                    /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_UP_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                    /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_PULL_DOWN_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                }
            }

            /*checking intial mode*/
            if (Pins[Local_u8Counter].Pin_Initial_Mode == PORT_PIN_MODE_DIO)
            {
                /*casting the pointer of 32 to 8 because of adding value to a pointer*/
                /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                /* Clear the PMCx bits for this pin */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Local_u8Counter].Pin_ID * 4));

                /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
            }
            else if (Pins[Local_u8Counter].Pin_Initial_Mode == PORT_PIN_MODE_ADC)
            {
                /*Clear Digital enable*/
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);

                /*clear the bit in dir reg to set as input*/
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);

                /*Enable analog for this pin*/
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
            }
            else if (Pins[Local_u8Counter].Pin_Initial_Mode == PORT_PIN_MODE_CAN)
            {
                /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);

                /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                /* Clear the PMCx bits for this pin */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the PMCx bits for this pin to CAN mode */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (CAN_MODE_NUM << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
            }
            else if (Pins[Local_u8Counter].Pin_Initial_Mode == PORT_PIN_MODE_I2C)
            {
                /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);

                /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                /* Clear the PMCx bits for this pin */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the PMCx bits for this pin to I2C mode */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (I2C_MODE_NUM << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
            }
            else if (Pins[Local_u8Counter].Pin_Initial_Mode == PORT_PIN_MODE_PWM0)
            {
                /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);

                /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                /* Clear the PMCx bits for this pin */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the PMCx bits for this pin to PWM0 mode */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (PWM0_MODE_NUM << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
            }
            else if (Pins[Local_u8Counter].Pin_Initial_Mode == PORT_PIN_MODE_PWM1)
            {
                /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);

                /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                /* Clear the PMCx bits for this pin */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the PMCx bits for this pin to PWM1 mode */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (PWM1_MODE_NUM << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
            }
            else if (Pins[Local_u8Counter].Pin_Initial_Mode == PORT_PIN_MODE_QEI)
            {
                /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);

                /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                /* Clear the PMCx bits for this pin */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the PMCx bits for this pin to QEI mode */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (QEI_MODE_NUM << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
            }
            else if (Pins[Local_u8Counter].Pin_Initial_Mode == PORT_PIN_MODE_SPI)
            {
                /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);

                /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                /* Clear the PMCx bits for this pin */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the PMCx bits for this pin to SPI mode */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (SPI_MODE_NUM << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
            }
            else if (Pins[Local_u8Counter].Pin_Initial_Mode == PORT_PIN_MODE_UART)
            {
                /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);

                /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                /* Clear the PMCx bits for this pin */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the PMCx bits for this pin to UART mode */
                *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (UART_MODE_NUM << (Pins[Local_u8Counter].Pin_ID * 4));
                /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
            }
            else
            {
                /*DO Nothing*/
            }
        }
        PORT_Status = PORT_INITIALIZED;

    }
}

/************************************************************************************
 * Service Name: Port_SetPinDirection
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin : Port Pin ID number & Direction :Port Pin Direction
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Sets the port pin direction.
 ************************************************************************************/
#if PORT_SET_PIN_DIRECTION_API == STD_ON
void Port_SetPinDirection(
    Port_PinType Pin,
    Port_PinDirectionType Direction)
{
    boolean errorState = FALSE;
    volatile uint32 *Port_Ptr = NULL_PTR;

#if PORT_DEV_ERROR_DETECT == STD_ON
    if (PORT_NOT_INITIALIZED == PORT_Status)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_UNINIT);
        errorState = TRUE;
    }
    else
    {
        /*No Action Required*/
    }
    if (errorState == TRUE)
    {
        /*DO NOTHING*/
    }
    else
    {
        if (PORT_CONFIGURED_PINS <= Pin)
        {
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_PARAM_PIN);
            errorState = TRUE;
        }
        else
        {
            /*No Action Required*/
        }
    }
    if (errorState == TRUE)
    {
        /*DO NOTHING*/
    }
    else
    {

        if (Pins[Pin].Pin_Direction_Changeable == STD_OFF)
        {
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_DIRECTION_UNCHANGEABLE);
            errorState = TRUE;
        }
        else
        {
            /*No Action Required*/
        }
    }

#endif
    if (errorState == TRUE)
    {
        /*Do Nothing*/
    }
    else
    {
        switch (Pins[Pin].Port_Num)
        {
        case 0:
            Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
            break;
        case 1:
            Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
            break;
        case 2:
            Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
            break;
        case 3:
            Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
            break;
        case 4:
            Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
            break;
        case 5:
            Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
            break;
        }

        if (Direction == PORT_PIN_IN)
        {
            /* Clear the corresponding bit in the GPIODIR register to make the pin as input pin*/
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Pins[Pin].Pin_ID);
        }
        else if (Direction == PORT_PIN_OUT)
        {
            /* Set the corresponding bit in the GPIODIR register to make the pin as Output pin*/
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Pins[Pin].Pin_ID);
        }
        else
        {
            /*No Action Required*/
        }
    }
}
#endif
/************************************************************************************
 * Service Name: Port_RefreshPortDirection
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in):
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Refreshes port direction
 ************************************************************************************/
void Port_RefreshPortDirection(void)
{
    boolean errorState = FALSE;
    volatile uint32 *Port_Ptr = NULL_PTR;
#if PORT_DEV_ERROR_DETECT == STD_ON
    if (PORT_NOT_INITIALIZED == PORT_Status)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFRESH_PORT_DIRECTION_SID, PORT_E_UNINIT);
        errorState = TRUE;
    }
    else
    {
        /*Do Nothing*/
    }

#endif

    if (errorState == TRUE)
    {
        /*Do Nothing*/
    }
    else
    {

        uint8 Local_u8Counter;

        for (Local_u8Counter = 0; Local_u8Counter < PORT_CONFIGURED_PINS; Local_u8Counter++)
        {
            switch (Pins[Local_u8Counter].Port_Num)
            {
            case 0:
                Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                break;
            case 1:
                Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                break;
            case 2:
                Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                break;
            case 3:
                Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                break;
            case 4:
                Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                break;
            case 5:
                Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                break;
            }

            if (Pins[Local_u8Counter].Pin_Direction_Changeable == STD_ON)
            {
                /*Do Nothing*/
            }
            else
            {
                if (GET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Pins[Local_u8Counter].Pin_ID) == PORT_PIN_OUT)
                {
                    /* Set the corresponding bit in the GPIODIR register to make the pin as Output pin*/
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                }
                else /*if input make it input*/
                {
                    /* Clear the corresponding bit in the GPIODIR register to make the pin as input pin*/
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Pins[Local_u8Counter].Pin_ID);
                }
            }
        }
    }
}
/************************************************************************************
 * Service Name: Port_GetVersionInfo
 * Service ID[hex]: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): VersionInfo - Pointer to where to store the version information of this module.
 * Return value: None
 * Description: Function to get the version information of this module.
 ************************************************************************************/
#if PORT_VERSION_INFO_API == STD_ON
void PORT_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
#if PORT_DEV_ERROR_DETECT == STD_ON
    if (versioninfo == NULL_PTR)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
    }
    else
#endif
    {
        /* Copy the vendor Id */
        versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
        /* Copy the module Id */
        versioninfo->moduleID = (uint16)PORT_MODULE_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
    }
}
#endif

/************************************************************************************
 * Service Name: Port_SetPinMode
 * Service ID[hex]: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin : Port Pin ID number & Mode :Port Pin Mode
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Sets the port pin mode.
 ************************************************************************************/
#if PORT_SET_PIN_MODE_API == STD_ON
void Port_SetPinMode(
    Port_PinType Pin,
    Port_PinModeType Mode)
{
    boolean errorState = FALSE;
    volatile uint32 *Port_Ptr = NULL_PTR;

#if PORT_DEV_ERROR_DETECT == STD_ON
    if (PORT_NOT_INITIALIZED == PORT_Status)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
        errorState = TRUE;
    }
    else
    {
        /*No Action Required*/
    }
    if (errorState == TRUE)
    {
        /*DO Nothing*/
    }
    else
    {

        if (PORT_CONFIGURED_PINS <= Pin)
        {
            Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_PIN);
            errorState = TRUE;
        }
        else
        {
            /*No Action Required*/
        }
        if (errorState == TRUE)
        {
            /*DO Nothing*/
        }
        else
        {
            if (Pins[Pin].Pin_Mode_Changeable == STD_OFF)
            {
                Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_MODE_UNCHANGEABLE);
                errorState = TRUE;
            }
            else
            {
                /*No Action Required*/
            }
            if (errorState == TRUE)
            {
                /*DO Nothing*/
            }
            else
            {
                if (14 < Mode)
                {
                    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
                    errorState = TRUE;
                }
                else
                {
                    /*No Action Required*/
                }
                if (Mode == PORT_PIN_MODE_ADC)
                {
                    uint8 Counter;
                    boolean Flag = 0;
                    for (Counter = 0; Counter < ADC_NUMBER_OF_PINS; Counter++)
                    {
                        if (ADC_Pins[Counter] == Pin)
                        {
                            Flag = 1;
                            break;
                        }
                        else
                        {
                            /*No Action Required*/
                        }
                    }
                    if (Flag == 0)
                    {
                        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
                        errorState = TRUE;
                    }
                    else
                    {
                        /*No Action Required*/
                    }
                }
                else if (Mode == PORT_PIN_MODE_CAN)
                {
                    uint8 Counter;
                    boolean Flag = 0;
                    for (Counter = 0; Counter < CAN_NUMBER_OF_PINS; Counter++)
                    {
                        if (CAN_Pins[Counter] == Pin)
                        {
                            Flag = 1;
                            break;
                        }
                        else
                        {
                            /*No Action Required*/
                        }
                    }
                    if (Flag == 0)
                    {
                        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
                        errorState = TRUE;
                    }
                    else
                    {
                        /*No Action Required*/
                    }
                }
                else if (Mode == PORT_PIN_MODE_I2C)
                {
                    uint8 Counter;
                    boolean Flag = 0;
                    for (Counter = 0; Counter < I2C_NUMBER_OF_PINS; Counter++)
                    {
                        if (I2C_Pins[Counter] == Pin)
                        {
                            Flag = 1;
                            break;
                        }
                        else
                        {
                            /*No Action Required*/
                        }
                    }
                    if (Flag == 0)
                    {
                        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
                        errorState = TRUE;
                    }
                    else
                    {
                        /*No Action Required*/
                    }
                }
                else if (Mode == PORT_PIN_MODE_SPI)
                {
                    uint8 Counter;
                    boolean Flag = 0;
                    for (Counter = 0; Counter < SPI_NUMBER_OF_PINS; Counter++)
                    {
                        if (SPI_Pins[Counter] == Pin)
                        {
                            Flag = 1;
                            break;
                        }
                        else
                        {
                            /*No Action Required*/
                        }
                    }
                    if (Flag == 0)
                    {
                        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
                        errorState = TRUE;
                    }
                    else
                    {
                        /*No Action Required*/
                    }
                }
                else if (Mode == PORT_PIN_MODE_QEI)
                {
                    uint8 Counter;
                    boolean Flag = 0;
                    for (Counter = 0; Counter < QEI_NUMBER_OF_PINS; Counter++)
                    {
                        if (QEI_Pins[Counter] == Pin)
                        {
                            Flag = 1;
                            break;
                        }
                        else
                        {
                            /*No Action Required*/
                        }
                    }
                    if (Flag == 0)
                    {
                        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
                        errorState = TRUE;
                    }
                    else
                    {
                        /*No Action Required*/
                    }
                }
                else if (Mode == PORT_PIN_MODE_UART)
                {
                    uint8 Counter;
                    boolean Flag = 0;
                    for (Counter = 0; Counter < UART_NUMBER_OF_PINS; Counter++)
                    {
                        if (UART_Pins[Counter] == Pin)
                        {
                            Flag = 1;
                            break;
                        }
                        else
                        {
                            /*No Action Required*/
                        }
                    }
                    if (Flag == 0)
                    {
                        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
                        errorState = TRUE;
                    }
                    else
                    {
                        /*No Action Required*/
                    }
                }
                else if (Mode == PORT_PIN_MODE_PWM0)
                {
                    uint8 Counter;
                    boolean Flag = 0;
                    for (Counter = 0; Counter < PWM0_NUMBER_OF_PINS; Counter++)
                    {
                        if (PWM0_Pins[Counter] == Pin)
                        {
                            Flag = 1;
                            break;
                        }
                        else
                        {
                            /*No Action Required*/
                        }
                    }
                    if (Flag == 0)
                    {
                        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
                        errorState = TRUE;
                    }
                    else
                    {
                        /*No Action Required*/
                    }
                }
                else if (Mode == PORT_PIN_MODE_PWM1)
                {
                    uint8 Counter;
                    boolean Flag = 0;
                    for (Counter = 0; Counter < PWM1_NUMBER_OF_PINS; Counter++)
                    {
                        if (PWM1_Pins[Counter] == Pin)
                        {
                            Flag = 1;
                            break;
                        }
                        else
                        {
                            /*No Action Required*/
                        }
                    }
                    if (Flag == 0)
                    {
                        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
                        errorState = TRUE;
                    }
                    else
                    {
                        /*No Action Required*/
                    }
                }
                else
                {
                    /*No Action Required*/
                }
            }
        }
    }
#endif
    if (errorState == TRUE)
    {
        /*Do Nothing*/
    }
    else
    {
        switch (Pins[Pin].Port_Num)
        {
        case 0:
            Port_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
            break;
        case 1:
            Port_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
            break;
        case 2:
            Port_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
            break;
        case 3:
            Port_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
            break;
        case 4:
            Port_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
            break;
        case 5:
            Port_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
            break;
        }

        if (Mode == PORT_PIN_MODE_DIO)
        {
            /*casting the pointer of 32 to 8 because of adding value to a pointer*/
            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Pin].Pin_ID);
            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Pin].Pin_ID);
            /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Pin].Pin_ID * 4));

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Pin].Pin_ID);
        }
        else if (Mode == PORT_PIN_MODE_ADC)
        {
            /*Clear Digital enable*/
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Pin].Pin_ID);

            /*clear the bit in dir reg to set as input*/
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIR_REG_OFFSET), Pins[Pin].Pin_ID);

            /*Enable analog for this pin*/
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Pin].Pin_ID);
        }
        else if (Mode == PORT_PIN_MODE_CAN)
        {
            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Pin].Pin_ID);

            /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Pin].Pin_ID);
            /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Pin].Pin_ID * 4));
            /* Set the PMCx bits for this pin to CAN mode */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (CAN_MODE_NUM << (Pins[Pin].Pin_ID * 4));
            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Pin].Pin_ID);
        }
        else if (Mode == PORT_PIN_MODE_I2C)
        {
            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Pin].Pin_ID);

            /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Pin].Pin_ID);
            /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Pin].Pin_ID * 4));
            /* Set the PMCx bits for this pin to I2C mode */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (I2C_MODE_NUM << (Pins[Pin].Pin_ID * 4));
            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Pin].Pin_ID);
        }
        else if (Mode == PORT_PIN_MODE_PWM0)
        {
            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Pin].Pin_ID);

            /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Pin].Pin_ID);
            /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Pin].Pin_ID * 4));
            /* Set the PMCx bits for this pin to PWM0 mode */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (PWM0_MODE_NUM << (Pins[Pin].Pin_ID * 4));
            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Pin].Pin_ID);
        }
        else if (Mode == PORT_PIN_MODE_PWM1)
        {
            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Pin].Pin_ID);

            /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Pin].Pin_ID);
            /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Pin].Pin_ID * 4));
            /* Set the PMCx bits for this pin to PWM1 mode */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (PWM1_MODE_NUM << (Pins[Pin].Pin_ID * 4));
            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Pin].Pin_ID);
        }
        else if (Mode == PORT_PIN_MODE_QEI)
        {
            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Pin].Pin_ID);

            /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Pin].Pin_ID);
            /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Pin].Pin_ID * 4));
            /* Set the PMCx bits for this pin to QEI mode */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (QEI_MODE_NUM << (Pins[Pin].Pin_ID * 4));
            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Pin].Pin_ID);
        }
        else if (Mode == PORT_PIN_MODE_SPI)
        {
            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Pin].Pin_ID);

            /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Pin].Pin_ID);
            /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Pin].Pin_ID * 4));
            /* Set the PMCx bits for this pin to SPI mode */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (SPI_MODE_NUM << (Pins[Pin].Pin_ID * 4));
            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Pin].Pin_ID);
        }
        else if (Mode == PORT_PIN_MODE_UART)
        {
            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pins[Pin].Pin_ID);

            /* enable Alternative function for this pin by Setting the corresponding bit in GPIOAFSEL register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pins[Pin].Pin_ID);
            /* Clear the PMCx bits for this pin */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pins[Pin].Pin_ID * 4));
            /* Set the PMCx bits for this pin to UART mode */
            *(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_CTL_REG_OFFSET) |= (UART_MODE_NUM << (Pins[Pin].Pin_ID * 4));
            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)Port_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pins[Pin].Pin_ID);
        }
        else
        {
            /*DO Nothing*/
        }
    }
}
#endif