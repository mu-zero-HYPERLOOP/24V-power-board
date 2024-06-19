#include "canzero/canzero.h"
#include "channel_control.h"
#include "firmware/pdu24.hpp"
#include "pinout.h"
#include "util/boxcar.h"
#include "util/interval.h"

constexpr size_t DEFAULT_FILTER_SIZE = 100;
constexpr Frequency DEFAULT_SENSE_FREQUENCY = 1_kHz;

static Interval cooling_pump_sense_interval(DEFAULT_SENSE_FREQUENCY);
static BoxcarFilter<Current, DEFAULT_FILTER_SIZE>
    cooling_pump_current_filter(0_A);

static Interval sdc_power_sense_interval(DEFAULT_SENSE_FREQUENCY);
static BoxcarFilter<Current, DEFAULT_FILTER_SIZE> sdc_power_current_filter(0_A);

static Interval sdc_signal_sense_interval(DEFAULT_SENSE_FREQUENCY);
static BoxcarFilter<Current, DEFAULT_FILTER_SIZE>
    sdc_signal_current_filter(0_A);

static Interval fan_sense_interval(DEFAULT_SENSE_FREQUENCY);
static BoxcarFilter<Current, DEFAULT_FILTER_SIZE> fan_current_filter(0_A);

static Interval power_sense_interval(DEFAULT_SENSE_FREQUENCY);
static BoxcarFilter<Power, DEFAULT_FILTER_SIZE> total_power_filter(0_W);


static pdu_channel_status
pdu24_status_to_canzero_status(pdu24_channel_status status) {
  switch (status) {
  case OFF:
    return pdu_channel_status_OFF;
  case ON:
    return pdu_channel_status_ON;
  case SHORT:
    return pdu_channel_status_SHORT_CIRCUIT;
  }
}

void channel_control() {
  // ========= COOLING PUMP =========
  pdu24::control(COOLING_PUMP_CHANNEL,
                 canzero_get_cooling_pump_channel_ctrl() ==
                     pdu_channel_control_ON);
  if (cooling_pump_sense_interval.next()) {
    cooling_pump_current_filter.push(pdu24::sense(COOLING_PUMP_CHANNEL));
    canzero_set_cooling_pump_channel_current(
        static_cast<float>(cooling_pump_current_filter.get()));
  }
  canzero_set_cooling_pump_channel_status(
      pdu24_status_to_canzero_status(pdu24::status(COOLING_PUMP_CHANNEL)));

  // ========== SDC BOARD POWER ==========
  pdu24::control(SDC_POWER_CHANNEL, canzero_get_sdc_board_power_channel_ctrl() ==
                                        pdu_channel_control_ON);
  if (sdc_power_sense_interval.next()) {
    sdc_power_current_filter.push(pdu24::sense(SDC_POWER_CHANNEL));
    canzero_set_sdc_board_power_channel_current(
        static_cast<float>(sdc_power_current_filter.get()));
  }
  canzero_set_sdc_board_power_channel_status(
      pdu24_status_to_canzero_status(pdu24::status(SDC_POWER_CHANNEL)));

  // ========== SDC SIGNAL ==========
  pdu24::control(SDC_SIGNAL_CHANNEL, canzero_get_sdc_signal_channel_ctrl() ==
                                         pdu_channel_control_ON);
  if (sdc_signal_sense_interval.next()) {
    sdc_signal_current_filter.push(pdu24::sense(SDC_SIGNAL_CHANNEL));
    canzero_set_sdc_signal_channel_current(
        static_cast<float>(sdc_signal_current_filter.get()));
  }
  canzero_set_sdc_signal_channel_status(
      pdu24_status_to_canzero_status(pdu24::status(SDC_SIGNAL_CHANNEL)));

  // =========== FAN CHANNELS ========
  pdu24::control(FAN_CHANNEL,
                 canzero_get_fan_channel_ctrl() == pdu_channel_control_ON);
  if (fan_sense_interval.next()) {
    fan_current_filter.push(pdu24::sense(FAN_CHANNEL));
    canzero_set_fan_channel_current(
        static_cast<float>(fan_current_filter.get()));
  }
  canzero_set_fan_channel_status(
      pdu24_status_to_canzero_status(pdu24::status(FAN_CHANNEL)));


  if (power_sense_interval.next()) {
    total_power_filter.push(pdu24::total_power_output());
    canzero_set_total_power(static_cast<float>(total_power_filter.get()));
  }
}

