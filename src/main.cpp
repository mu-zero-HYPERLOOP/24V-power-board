/**
 * @author      : kistenklaus (karlsasssie@gmail.com)
 * @file        : main
 * @created     : Sonntag Apr 14, 2024 18:24:07 CEST
 */

#include "canzero/canzero.h"
#include "firmware/pdu24.hpp"
#include "util/ema.hpp"
#include "util/metrics.h"
#include "util/timestamp.h"
#include <Arduino.h>

static constexpr Temperature ERROR_MCU_OVER_TEMP = 80_Celcius;
static constexpr Duration ERROR_MCU_OVER_TEMP_TIME_THRESH = 10_s;
static constexpr Temperature WARN_MCU_OVER_TEMP = 50_Celcius;
static constexpr Duration WARN_MCU_OVER_TEMP_TIME_THRESH = 10_s;

static inline pdu_channel_status to_canzero_status(pdu24_channel_status ch) {
  switch (ch) {
  case pdu24_channel_status::ON:
    return pdu_channel_status_ON;
  case pdu24_channel_status::OFF:
    return pdu_channel_status_ON;
  case pdu24_channel_status::SHORT:
    return pdu_channel_status_SHORT_CIRCUIT;
  }
}

int main() {
  canzero_init();

  canzero_set_state(pdu_state_INIT);
  canzero_set_hp_channel1_status(pdu_channel_status_OFF);
  canzero_set_hp_channel2_status(pdu_channel_status_OFF);
  canzero_set_hp_channel3_status(pdu_channel_status_OFF);
  canzero_set_hp_channel4_status(pdu_channel_status_OFF);
  canzero_set_lp_channel1_status(pdu_channel_status_OFF);
  canzero_set_lp_channel2_status(pdu_channel_status_OFF);
  canzero_set_lp_channel3_status(pdu_channel_status_OFF);
  canzero_set_lp_channel4_status(pdu_channel_status_OFF);
  canzero_set_lp_channel5_status(pdu_channel_status_OFF);
  canzero_set_lp_channel6_status(pdu_channel_status_OFF);
  canzero_set_lp_channel7_status(pdu_channel_status_OFF);
  canzero_set_lp_channel8_status(pdu_channel_status_OFF);
  canzero_set_lp_channel9_status(pdu_channel_status_OFF);
  canzero_set_lp_channel10_status(pdu_channel_status_OFF);

  canzero_set_hp_channel1_current(0);
  canzero_set_hp_channel2_current(0);
  canzero_set_hp_channel3_current(0);
  canzero_set_hp_channel4_current(0);
  canzero_set_lp_channel1_current(0);
  canzero_set_lp_channel2_current(0);
  canzero_set_lp_channel3_current(0);
  canzero_set_lp_channel4_current(0);
  canzero_set_lp_channel5_current(0);
  canzero_set_lp_channel6_current(0);
  canzero_set_lp_channel7_current(0);
  canzero_set_lp_channel8_current(0);
  canzero_set_lp_channel9_current(0);
  canzero_set_lp_channel10_current(0);

  canzero_set_hp_channel1_control(pdu_channel_control_OFF);
  canzero_set_hp_channel2_control(pdu_channel_control_OFF);
  canzero_set_hp_channel3_control(pdu_channel_control_OFF);
  canzero_set_hp_channel4_control(pdu_channel_control_OFF);
  canzero_set_lp_channel1_control(pdu_channel_control_OFF);
  canzero_set_lp_channel2_control(pdu_channel_control_OFF);
  canzero_set_lp_channel3_control(pdu_channel_control_OFF);
  canzero_set_lp_channel4_control(pdu_channel_control_OFF);
  canzero_set_lp_channel5_control(pdu_channel_control_OFF);
  canzero_set_lp_channel6_control(pdu_channel_control_OFF);
  canzero_set_lp_channel7_control(pdu_channel_control_OFF);
  canzero_set_lp_channel8_control(pdu_channel_control_OFF);
  canzero_set_lp_channel9_control(pdu_channel_control_OFF);
  canzero_set_lp_channel10_control(pdu_channel_control_OFF);

  canzero_set_error_mcu_over_temperature(error_flag_OK);
  canzero_set_warn_mcu_over_temperature(error_flag_OK);

  canzero_set_sdc_status(sdc_status_OPEN);
  pdu24::set_sdc(false);
  canzero_set_mcu_temperature(0);
  canzero_set_total_current(0);
  canzero_set_power_estimation(0);

  Serial.begin(38400);

  delay(3000);

  pdu24::begin();

  Timestamp last_error_mcu_over_temp_ok = Timestamp::now();
  Timestamp last_warn_mcu_over_temp_ok = Timestamp::now();

  ExponentialMovingAverage<Temperature> mcu_temp_filter(
      0.1, WARN_MCU_OVER_TEMP - 1_K);

  Timestamp last_iteration = Timestamp::now();

  while (true) {
    Timestamp now = Timestamp::now();
    Duration dt = now - last_iteration;
    last_iteration = now;

    canzero_set_state(pdu_state_RUNNING);

    pdu24::update();

    canzero_set_hp_channel1_status(to_canzero_status(pdu24::status(hp1)));
    canzero_set_hp_channel2_status(to_canzero_status(pdu24::status(hp2)));
    canzero_set_hp_channel3_status(to_canzero_status(pdu24::status(hp3)));
    canzero_set_hp_channel4_status(to_canzero_status(pdu24::status(hp4)));
    canzero_set_lp_channel1_status(to_canzero_status(pdu24::status(lp1)));
    canzero_set_lp_channel2_status(to_canzero_status(pdu24::status(lp2)));
    canzero_set_lp_channel3_status(to_canzero_status(pdu24::status(lp3)));
    canzero_set_lp_channel4_status(to_canzero_status(pdu24::status(lp4)));
    canzero_set_lp_channel5_status(to_canzero_status(pdu24::status(lp5)));
    canzero_set_lp_channel6_status(to_canzero_status(pdu24::status(lp6)));
    canzero_set_lp_channel7_status(to_canzero_status(pdu24::status(lp7)));
    canzero_set_lp_channel8_status(to_canzero_status(pdu24::status(lp8)));
    canzero_set_lp_channel9_status(to_canzero_status(pdu24::status(lp9)));
    canzero_set_lp_channel10_status(to_canzero_status(pdu24::status(lp10)));

    Current i_hp1 = pdu24::sense(hp1);
    Current i_hp2 = pdu24::sense(hp2);
    Current i_hp3 = pdu24::sense(hp3);
    Current i_hp4 = pdu24::sense(hp4);

    Current i_lp1 = pdu24::sense(lp1);
    Current i_lp2 = pdu24::sense(lp2);
    Current i_lp3 = pdu24::sense(lp3);
    Current i_lp4 = pdu24::sense(lp4);
    Current i_lp5 = pdu24::sense(lp5);
    Current i_lp6 = pdu24::sense(lp6);
    Current i_lp7 = pdu24::sense(lp7);
    Current i_lp8 = pdu24::sense(lp8);
    Current i_lp9 = pdu24::sense(lp9);
    Current i_lp10 = pdu24::sense(lp10);

    Current i_sum = i_hp1 + i_hp2 + i_hp3 + i_hp4 + i_lp1 + i_lp2 + i_lp3 +
                    i_lp4 + i_lp5 + i_lp6 + i_lp7 + i_lp8 + i_lp9 + i_lp10;

    Power p = i_sum * 24_V;

    canzero_set_hp_channel1_current(static_cast<float>(i_hp1));
    canzero_set_hp_channel2_current(static_cast<float>(i_hp2));
    canzero_set_hp_channel3_current(static_cast<float>(i_hp3));
    canzero_set_hp_channel4_current(static_cast<float>(i_hp4));

    canzero_set_lp_channel1_current(static_cast<float>(i_lp1));
    canzero_set_lp_channel2_current(static_cast<float>(i_lp2));
    canzero_set_lp_channel3_current(static_cast<float>(i_lp3));
    canzero_set_lp_channel4_current(static_cast<float>(i_lp4));
    canzero_set_lp_channel5_current(static_cast<float>(i_lp5));
    canzero_set_lp_channel6_current(static_cast<float>(i_lp6));
    canzero_set_lp_channel7_current(static_cast<float>(i_lp7));
    canzero_set_lp_channel8_current(static_cast<float>(i_lp8));
    canzero_set_lp_channel9_current(static_cast<float>(i_lp9));
    canzero_set_lp_channel10_current(static_cast<float>(i_lp10));

    canzero_set_total_current(static_cast<float>(i_sum));
    canzero_set_power_estimation(static_cast<float>(p));

    bool any_short =
        pdu24::status(lp1) == SHORT || pdu24::status(lp2) == SHORT ||
        pdu24::status(lp3) == SHORT || pdu24::status(lp4) == SHORT ||
        pdu24::status(lp5) == SHORT || pdu24::status(lp6) == SHORT ||
        pdu24::status(lp7) == SHORT || pdu24::status(lp8) == SHORT ||
        pdu24::status(lp9) == SHORT || pdu24::status(lp10) == SHORT ||
        pdu24::status(hp1) == SHORT || pdu24::status(hp2) == SHORT ||
        pdu24::status(hp3) == SHORT || pdu24::status(hp4) == SHORT;

    pdu24::set_sdc(!any_short);
    canzero_set_sdc_status(any_short ? sdc_status_OPEN : sdc_status_CLOSED);

    pdu24::control(hp1,
                   canzero_get_hp_channel1_control() == pdu_channel_control_ON);
    pdu24::control(hp2,
                   canzero_get_hp_channel2_control() == pdu_channel_control_ON);
    pdu24::control(hp3,
                   canzero_get_hp_channel3_control() == pdu_channel_control_ON);
    pdu24::control(hp4,
                   canzero_get_hp_channel4_control() == pdu_channel_control_ON);
    pdu24::control(lp1,
                   canzero_get_lp_channel1_control() == pdu_channel_control_ON);
    pdu24::control(lp2,
                   canzero_get_lp_channel2_control() == pdu_channel_control_ON);
    pdu24::control(lp3,
                   canzero_get_lp_channel3_control() == pdu_channel_control_ON);
    pdu24::control(lp4,
                   canzero_get_lp_channel4_control() == pdu_channel_control_ON);
    pdu24::control(lp5,
                   canzero_get_lp_channel5_control() == pdu_channel_control_ON);
    pdu24::control(lp6,
                   canzero_get_lp_channel6_control() == pdu_channel_control_ON);
    pdu24::control(lp7,
                   canzero_get_lp_channel7_control() == pdu_channel_control_ON);
    pdu24::control(lp8,
                   canzero_get_lp_channel8_control() == pdu_channel_control_ON);
    pdu24::control(lp9,
                   canzero_get_lp_channel9_control() == pdu_channel_control_ON);
    pdu24::control(lp10, canzero_get_lp_channel10_control() ==
                             pdu_channel_control_ON);

    mcu_temp_filter.push(pdu24::read_mcu_temperature());
    Temperature mcu_temp = mcu_temp_filter.get();

    canzero_set_mcu_temperature(static_cast<float>(mcu_temp) + 273.15f);

    if (mcu_temp > ERROR_MCU_OVER_TEMP) {
      if (Timestamp::now() - last_error_mcu_over_temp_ok >
          ERROR_MCU_OVER_TEMP_TIME_THRESH) {
        canzero_set_error_mcu_over_temperature(error_flag_ERROR);
      }
    } else {
      last_error_mcu_over_temp_ok = Timestamp::now();
      canzero_set_error_mcu_over_temperature(error_flag_OK);
    }

    if (mcu_temp > WARN_MCU_OVER_TEMP) {
      if (Timestamp::now() - last_warn_mcu_over_temp_ok >
          WARN_MCU_OVER_TEMP_TIME_THRESH) {
        canzero_set_warn_mcu_over_temperature(error_flag_ERROR);
      }
    } else {
      last_warn_mcu_over_temp_ok = Timestamp::now();
      canzero_set_warn_mcu_over_temperature(error_flag_OK);
    }

    canzero_can0_poll();
    canzero_can1_poll();
    canzero_update_continue(canzero_get_time());
  }
}

void __assert_func(const char *, int, const char *, const char *) {
  while (true) {
    Serial.println("FUCKED");
  }
}
