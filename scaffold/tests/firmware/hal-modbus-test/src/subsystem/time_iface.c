



#include"time_iface.h"

#if APP_ENABLE_SYSTICK == 1
void systick_init(void){
    timer_init(APP_SYSTICK_TIMER_INTFNUM);
    timer_set_prescaler(APP_SYSTICK_TIMER_INTFNUM, TIMER_PRESCALER_DIV8);
    timer_set_top(APP_SYSTICK_TIMER_INTFNUM, 
                  ((uC_TIMER_DEFAULT_CLKSOURCE_FREQ / 8 ) / APP_SYSTICK_FREQ_Hz )
    );
    timer_enable_int_top(APP_SYSTICK_TIMER_INTFNUM);
    timer_set_mode(APP_SYSTICK_TIMER_INTFNUM, TIMER_MODE_PERIODIC);
}
#endif

#if APP_ENABLE_RTC == 1
    uint8_t time_get_rtc_time(tm_real_t * rtime_target){
        return 0;
    }
#endif
