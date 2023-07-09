 /******************************************************************************
 *
 * Module: Common - Platform
 *
 * File Name: Std_Types.h
 *
 * Description: General type definitions
 *
 * Author: Mustafa Elsayad
 *
 *******************************************************************************/

#ifndef STD_TYPES_H
#define STD_TYPES_H

#include "Platform_Types.h"
#include "Compiler.h"

/*Vendor ID is an ID for the company in the Autosar 
 * as an Example Mustafa Elsayad's ID = 1000*/

 #define STD_TYPES_VENDOR_ID                (1000U)

 /*
  * Module version = 1.0.0
  */
#define STD_TYPES_SW_MODULE_MAJOR_VERSION    (1U)
#define STD_TYPES_SW_MODULE_MINOR_VERSION    (1U)
#define STD_TYPES_SW_MODULE_PATCH_VERSION    (1U)

 /*
  * Autosar version = 4.0.3
  */
#define STD_TYPES_AR_RELEASE_MAJOR_VERSION    (4U)
#define STD_TYPES_AR_RELEASE_MINOR_VERSION    (0U)
#define STD_TYPES_AR_RELEASE_PATCH_VERSION    (3U)


/*Type definitions*/

 /*This type can be used as standard API return type which is shared 
  *between the RTE and the BSW modules */
typedef uint8 Std_ReturnType;

  /*This type shall be used to request the version of a 
   *BSW module using the <Module name>_GetVersionInfo() function.*/
typedef struct 
{ 
 uint16 vendorID; 
 uint16 moduleID; 
 uint8 sw_major_version; 
 uint8 sw_minor_version; 
 uint8 sw_patch_version; 
} Std_VersionInfoType; 

/*Symbol definitions */

/*E_OK, E_NOT_OK*/

#ifndef STATUSTYPEDEFINED 
 #define STATUSTYPEDEFINED 
 #define E_OK               ((Std_ReturnType)0x00U) 
 typedef unsigned char StatusType; /* OSEK compliance */ 
#endif 
#define E_NOT_OK            ((Std_ReturnType)0x01U) 

/*STD_HIGH, STD_LOW*/

#define STD_HIGH        0x01U       /* Physical state 5V or 3.3V */ 
#define STD_LOW         0x00U       /* Physical state 0V */

/*STD_ACTIVE, STD_IDLE*/

#define STD_ACTIVE      0x01U       /* Logical state active */
#define STD_IDLE        0x00U       /* Logical state idle */

/*STD_ON, STD_OFF*/

#define STD_ON          0x01U       /* Standard ON */
#define STD_OFF         0x00U       /* Standard OFF */

#endif /*STD_TYPES_H */