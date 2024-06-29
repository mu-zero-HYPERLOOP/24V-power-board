/**
 * @author      : kistenklaus (karlsasssie@gmail.com)
 * @file        : main
 * @created     : Sonntag Apr 14, 2024 18:24:07 CEST
 */

#include "print.h"
#include "canzero/canzero.h"
#include "sdc.h"
#include "channel_control.h"
#include "defaults.h"
#include "error_level_range_check.h"
#include "firmware/pdu24.hpp"
#include "util/boxcar.h"
#include "util/interval.h"
#include "util/timing.h"
#include "fsm/fsm.hpp"
#include <cstdio>

static IntervalTiming main_loop_interval_timer;

static Interval mcu_temperature_interval(100_Hz);
static BoxcarFilter<Temperature, 100> mcu_temperature_filter(24_Celcius);
static ErrorLevelRangeCheck<EXPECT_UNDER>
    mcu_temp_check(canzero_get_mcu_temperature,
                   canzero_get_error_level_config_mcu_temperature,
                   canzero_set_error_level_mcu_temperature);

int main() {
  canzero_init();

  can_defaults();
  canzero_update_continue(canzero_get_time());
  
  pdu24::begin();
  sdc::begin();
  fsm::begin();

  while (true) {

    canzero_can0_poll();
    canzero_can1_poll();

    pdu24::update();
    canzero_set_error_any_short(pdu24::any_short() ? error_flag_ERROR : error_flag_OK);
    fsm::update();
    channel_control(canzero_get_state());

    // =========== Temperature ======
    if (mcu_temperature_interval.next()) {
      Temperature mcu_temperature = pdu24::read_mcu_temperature();
      mcu_temperature_filter.push(mcu_temperature);
      canzero_set_mcu_temperature(mcu_temperature_filter.get().as_celcius());
      mcu_temp_check.check();
    }

    // =========== main loop frequency ==========
    main_loop_interval_timer.tick();
    canzero_set_loop_frequency(
        static_cast<float>(main_loop_interval_timer.frequency()));

    canzero_update_continue(canzero_get_time());
  }
}
