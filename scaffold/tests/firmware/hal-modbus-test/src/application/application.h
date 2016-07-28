

#ifndef APPLICATION_H
#define APPLICATION_H

#define APP_ENABLE_BCIF         0
#define APP_ENABLE_UCDM         1



/**
 * @name Modbus Interface Configuration
 */
/**@{*/ 
    /**
    * @name Modbus Interface Constants
    */
    /**@{*/ 
    #define MODBUS_UART               1
    #define MODBUS_USBCDC             2
    /**@}*/ 
    /**
    * @name Modbus Interface Configuration
    */
    /**@{*/ 
    #define APP_ENABLE_MODBUS         1
    #define APP_MODBUSIF_TYPE         MODBUS_UART
    #define APP_MODBUSIF_INTFNUM      1
    /**@}*/ 
/**@}*/ 

#endif