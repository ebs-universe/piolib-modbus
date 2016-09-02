
#include<bytebuf/bytebuf.h>
#include<prbs/prbs.h>
#include<ucdm/ucdm.h>
#include<modbus/modbus.h>
#include<time/time.h>

#include "application.h"
#include "hal_uc.h"
#include "subsystems.h"

volatile uint8_t rval=0;


static void deferred_exec(void);

static void deferred_exec(void){
    #if APP_ENABLE_MODBUS == 1
        modbus_state_machine();
    #endif
}


static void _initialize_interrupts(void);

static void _initialize_interrupts(void){
    // Needed to ensure linker includes interrupt handlers 
    // in the output.
    __uart_handler_inclusion = 1;
    __timer_handler_inclusion = 1;
    __usb_handler_inclusion = 1;
}


int main(void)
{   
    // Pre-init, and assoicated Housekeeping
    _initialize_interrupts();
    
    gpio_conf_output(BOARD_GREEN_LED_PORT,BOARD_GREEN_LED_PIN);
    gpio_conf_output(BOARD_RED_LED_PORT,BOARD_RED_LED_PIN);
    gpio_set_output_high(BOARD_RED_LED_PORT,BOARD_RED_LED_PIN);
    gpio_set_output_low(BOARD_GREEN_LED_PORT,BOARD_GREEN_LED_PIN);
    
    // uC Core Initialization
    watchdog_hold();
    power_set_full();
    clock_set_default();
    global_interrupt_enable();
    usb_init();
    
    // Subsystems Initialization
    //tm_init();
    
    #if APP_ENABLE_BCIF == 1
        bc_init();
    #endif
    #if APP_ENABLE_UCDM == 1
        app_ucdm_init();
    #endif
    #if APP_ENABLE_MODBUS == 1
        app_modbus_init();
        gpio_set_output_low(BOARD_RED_LED_PORT, BOARD_RED_LED_PIN);
        gpio_set_output_high(BOARD_GREEN_LED_PORT, BOARD_GREEN_LED_PIN);
    #endif
        
    while(1){
        deferred_exec();
    }
    return(0);
}
