
#ifndef PIO_NATIVE
#include <scaffold_application.h>
#endif


/**
 * @name Application Systick and Time Configuration
 */
/**@{*/   
    #define APP_ENABLE_SYSTICK         0
    #define APP_USE_CORE_SYSTICK       1
    #define APP_ENABLE_TIME_CRON       1
    #define APP_ENABLE_TIME_SYNC       1
    #define APP_ENABLE_RTC             0
    #define APP_ENABLE_RTC_1HZ_OUT     0
/**@}*/ 



#ifndef APP_ENABLE_UCDM
#define APP_ENABLE_UCDM                     1
#endif

#ifndef APP_UCDM_MAX_REGISTERS
#define APP_UCDM_MAX_REGISTERS              250
#endif

#ifndef APP_ENABLE_UCDM_HANDLERS    
#define APP_ENABLE_UCDM_HANDLERS            1
#endif

#ifndef APP_ENABLE_UCDM_DESCRIPTORS
#define APP_ENABLE_UCDM_DESCRIPTORS         1
#endif

#ifndef APP_ENABLE_LIBVERSION_DESCRIPTORS
#define APP_ENABLE_LIBVERSION_DESCRIPTORS   0  
#endif

/**
 * @name Application Modbus Configuration
 */
/**@{*/   
    #define APP_ENABLE_MODBUS                1
    
    #if APP_ENABLE_PRIF
        #define MODBUS_TRANSPORT_INTFNUM     APP_PRIF_INTFNUM
        #define MODBUS_TRANSPORT_TYPE        APP_PRIF_TYPE
    #elif APP_ENABLE_BCIF
        #define MODBUS_TRANSPORT_INTFNUM     APP_BCIF_INTFNUM
        #define MODBUS_TRANSPORT_TYPE        APP_BCIF_TYPE
    #else 
        #define MODBUS_TRANSPORT_INTFNUM     0
        #define MODBUS_TRANSPORT_TYPE        0xFE
    #endif

    #define MODBUS_PLUGGABLE_TRANSPORTS      0
    #define MODBUS_DEFAULT_DEVICE_ADDRESS    0x05
    #define MODBUS_LIBVERSION_DESCRIPTOR     1
/**@}*/
