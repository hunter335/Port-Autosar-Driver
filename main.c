#include <assert.h>
#include "Port.h"
#include "Det.h"


#define INVALID_PIN 100
boolean Det_TestLastReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId) 
{
    boolean result = (Det_ModuleId == ModuleId) && (Det_InstanceId == InstanceId) && 
                     (Det_ApiId == ApiId) && (Det_ErrorId == ErrorId);
    Det_ModuleId = 0;
    Det_InstanceId = 0;
    Det_ApiId = 0;
    Det_ErrorId = 0;
    
    return result;
}
/************************************************************************************
* Test ID :TestCase1
* Description: Test for port ( RefreshDirection, SetPinMode and SetPinDirection) API's report a PORT_E_UNINIT DET 
*              error in case the Port_Init is not called yet.
* Pre-requisits: Port Module is not initialized before Run this test case.
* Test Procedure : 1. Call Port_RefreshPortDirection DET error PORT_E_UNINIT should be reported.
*                  2. Call Port_SetPinDirection      DET error PORT_E_UNINIT should be reported.
*                  3. Call Port_SetPinMode           DET error PORT_E_UNINIT should be reported.
*************************************************************************************/
void TestCase1(void)
{
    Port_RefreshPortDirection();
    assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_REFRESH_PORT_DIRECTION_SID,PORT_E_UNINIT));
    
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)   
    Port_SetPinDirection(PortConf_PORT_A_PIN_0,PORT_PIN_IN);
    assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_SET_PIN_DIRECTION_SID,PORT_E_UNINIT));
#endif    
    
#if (PORT_SET_PIN_MODE_API == STD_ON)   
    Port_SetPinMode(PortConf_PORT_A_PIN_0,PORT_PIN_MODE_UART);
    assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_SET_PIN_MODE_SID,PORT_E_UNINIT));
#endif    
}



/************************************************************************************
* Test ID :TestCase2
* Description: Test for Port_Init and Port_GetVersionInfo API's report the correct DET 
*              error in case the API called with NULL pointer.
* Pre-requisits: No Need
* Test Procedure : 1. Call Port_Init           DET error  Port_E_PARAM_CONFIG should be reported.
*                  2. Call Port_GetVersionInfo DET error  Port_E_PARAM_POINTER should be reported.
*************************************************************************************/
void TestCase2(void)
{
    Port_Init(NULL_PTR);
    assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_INIT_SID,PORT_E_PARAM_CONFIG));
    
#if (PORT_VERSION_INFO_API == STD_ON)
    PORT_GetVersionInfo(NULL_PTR);
    assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_GET_VERSION_INFO_SID,PORT_E_PARAM_POINTER));
#endif    
}

/************************************************************************************
* Test ID :TestCase3
* Description: Test for port (  SetPinMode and SetPinDirection) API's report a PORT_E_PARAM_PIN DET 
*              error in case the Port_SetPinDirection OR Port_SetPinMode is called with wrong pin number.
* Pre-requisits: Port Module is  initialized before Run this test case.
* Test Procedure :*1. Call Port_SetPinDirection      DET error PORT_E_PARAM_PIN DET  should be reported.
*                  2. Call Port_SetPinMode           DET error PORT_E_PARAM_PIN DET  should be reported.
*************************************************************************************/
void TestCase3(void)
{

    
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)   
    Port_SetPinDirection(INVALID_PIN,PORT_PIN_IN);
    assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_SET_PIN_DIRECTION_SID,PORT_E_PARAM_PIN));
#endif    
    
#if (PORT_SET_PIN_MODE_API == STD_ON)   
    Port_SetPinMode(INVALID_PIN,PORT_PIN_MODE_UART);
    assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_SET_PIN_MODE_SID,PORT_E_PARAM_PIN));
#endif    
}
/************************************************************************************
* Test ID :TestCase4
* Description: Test for port (  SetPinMode and SetPinDirection) API's report a PORT_E_DIRECTION_UNCHANGEABLE or PORT_E_MODE_UNCHANGEABLE 
               DET error in case the Port_SetPinDirection or Port_SetPinMode is  called with unchangeable pin mode or direction.
* Pre-requisits: Port Module is  initialized before Run this test case.
* Test Procedure :*1. Call Port_SetPinDirection      DET error PORT_E_DIRECTION_UNCHANGEABLE DET  should be reported.
*                  2. Call Port_SetPinMode           DET error PORT_E_MODE_UNCHANGEABLE      DET  should be reported.
*************************************************************************************/
void TestCase4(void)
{

    
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)   
    Port_SetPinDirection(PortConf_PORT_A_PIN_6,PORT_PIN_OUT);
    assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_SET_PIN_DIRECTION_SID,PORT_E_DIRECTION_UNCHANGEABLE));
#endif    
    
#if (PORT_SET_PIN_MODE_API == STD_ON)   
    Port_SetPinMode(PortConf_PORT_A_PIN_6,PORT_PIN_MODE_UART);
    assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_SET_PIN_MODE_SID,PORT_E_MODE_UNCHANGEABLE));
#endif    
}
/************************************************************************************
* Test ID :TestCase4
* Description: Test for port   SetPinMode API's report a PORT_E_PARAM_INVALID_MODE  
               DET error in case the  Port_SetPinMode is  called with invalid mode for the pin .
* Pre-requisits: Port Module is  initialized before Run this test case.
* Test Procedure :*1. Call Port_SetPinMode           DET error PORT_E_PARAM_INVALID_MODE      DET  should be reported.
*************************************************************************************/
void TestCase5(void)
{  
    
#if (PORT_SET_PIN_MODE_API == STD_ON)   
    Port_SetPinMode(PortConf_PORT_A_PIN_7,PORT_PIN_MODE_UART);
    assert(Det_TestLastReportError(PORT_MODULE_ID,PORT_INSTANCE_ID,PORT_SET_PIN_MODE_SID,PORT_E_PARAM_INVALID_MODE));
#endif    
}
void main(void)
{

  TestCase1();
  Port_Init(&Port_Configuration);
  TestCase2();
  TestCase3();
  TestCase4();
  TestCase5();
  
  while(1);

  
  
}
