#pragma once

#include "canzero/canzero.h"
static void can_defaults() {
  canzero_set_state(pdu_state_INIT);
  canzero_set_sdc_status(sdc_status_OPEN);
  canzero_set_assertion_fault(error_flag_OK);

  canzero_set_cooling_pump_channel_ctrl(pdu_channel_control_OFF);
  canzero_set_cooling_pump_channel_status(pdu_channel_status_OFF);
  canzero_set_cooling_pump_channel_current(0);

  canzero_set_sdc_power_channel_ctrl(pdu_channel_control_OFF);
  canzero_set_sdc_power_channel_status(pdu_channel_status_OFF);
  canzero_set_sdc_power_channel_current(0);

  canzero_set_sdc_signal_channel_ctrl(pdu_channel_control_OFF);
  canzero_set_sdc_signal_channel_status(pdu_channel_status_OFF);
  canzero_set_sdc_signal_channel_current(0);

  canzero_set_fan_channel_ctrl(pdu_channel_control_OFF);
  canzero_set_fan_channel_status(pdu_channel_status_OFF);
  canzero_set_fan_channel_current(0);

  canzero_set_error_level_mcu_temperature(error_level_OK);
  canzero_set_error_level_config_mcu_temperature(error_level_config{
      .m_ignore_info = bool_t_FALSE,
      .m_info_thresh = 60,
      .m_info_timeout = 1,
      .m_ignore_warning = bool_t_FALSE,
      .m_warning_thresh = 80,
      .m_warning_timeout = 1,
      .m_ignore_error = bool_t_FALSE,
      .m_error_thresh = 90,
      .m_error_timeout = 1,
  });
  canzero_set_mcu_temperature(0);
}