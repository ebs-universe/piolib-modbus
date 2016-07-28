
#include<bytebuf/bytebuf.h>
#include<ucdm/ucdm.h>
#include<modbus/modbus.h>

#include "board.h"
#include "uc_pum.h"
#include "hal_uc.h"

#include "application.h"
#include "devicemap.h"

volatile uint8_t rval=0;
static void deferred_exec(void);

static void deferred_exec(void){
    #if APP_ENABLE_MODBUS == 1
        modbus_state_machine();
    #endif
}

#if APP_ENABLE_UCDM == 1
uint8_t DMAP_MAXREGS = DMAP_MAX_REGISTERS;
uint16_t ucdm_register[DMAP_MAX_REGISTERS];
uint8_t  ucdm_acctype[DMAP_MAX_REGISTERS];
void ( *ucdm_rw_handler[DMAP_MAX_REGISTERS] )(uint8_t);
void ( *ucdm_bw_handler[DMAP_MAX_REGISTERS] )(uint8_t, uint16_t);

static void _ucdm_init(void);

static void _ucdm_init(void)
{
    ucdm_init();
    
    // Initialize all UCDM registers
    // Some simple write access distribution, just enough to test the 
    // interface. All registers at addr = 5 and beyond have both register 
    // and bit write enabled.
    for (uint8_t i=5; i<DMAP_MAXREGS; i++){
        ucdm_enable_regw(i);
        ucdm_enable_bitw(i);
    }
    
}
#endif

#if APP_ENABLE_MODBUS == 1
uint16_t diagnostic_register;
uint8_t exception_status_register;
const uint8_t ucdm_modbus_base_address = UCDM_MODBUS_BASE_ADDRESS;
const uint8_t modbus_if_rxbuf_chunksize = APP_MODBUSIF_RXCHUNKSIZE;
const uint8_t modbus_if_txbuf_chunksize = APP_MODBUSIF_TXCHUNKSIZE;

static void _modbus_init(void);

static void _modbus_init(void)
{
    modbus_init();
}
#endif

static void _initialize_interrupts(void);

static void _initialize_interrupts(void){
    // Needed to ensure linker includes interrupt handlers 
    // in the output.
    __uart_handler_inclusion = 1;
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
    
    // Subsystems Initialization
    #if APP_ENABLE_UCDM == 1
        _ucdm_init();
    #endif
    #if APP_ENABLE_MODBUS == 1
        _modbus_init();
        gpio_set_output_low(BOARD_RED_LED_PORT, BOARD_RED_LED_PIN);
        gpio_set_output_high(BOARD_GREEN_LED_PORT, BOARD_GREEN_LED_PIN);
    #endif
        
    while(1){
        deferred_exec();
    }
    return(0);
}
