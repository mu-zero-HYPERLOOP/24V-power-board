#include "canzero/canzero.h"
#include "error_handling.hpp"
#include "firmware/pdu24.hpp"

pdu_24v_command fsm::error_handling::approve(pdu_24v_command cmd) {
  if (canzero_get_error_any_short() == error_flag_ERROR) {
    // affected channel is already off anyway
    return pdu_24v_command_STOP;
  }
  if (canzero_get_error_heartbeat_miss() == error_flag_ERROR) {
    // with only fans on nothing too bad can happen right?
    return pdu_24v_command_STOP;
  }

  if (canzero_get_error_level_mcu_temperature() == error_level_ERROR) {
    return pdu_24v_command_STOP;
  }

  return cmd;
}
