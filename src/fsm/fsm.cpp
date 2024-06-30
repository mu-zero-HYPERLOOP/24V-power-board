#include "fsm/fsm.hpp"
#include "canzero/canzero.h"
#include "fsm/error_handling.hpp"
#include "firmware/pdu24.hpp"
#include "sdc.h"

void fsm::begin() {
  canzero_set_state(pdu_24v_state_CHANNELS_OFF);
  canzero_set_command(pdu_24v_command_NONE);
  canzero_set_sdc_status(sdc_status_OPEN);
}

void fsm::update() {
  pdu_24v_command cmd = error_handling::approve(canzero_get_command());
  switch (cmd) {
  case pdu_24v_command_NONE:
    break;
  case pdu_24v_command_START:
    canzero_set_state(pdu_24v_state_CHANNELS_ON);
    sdc::close();
    break;
  case pdu_24v_command_STOP:
    canzero_set_state(pdu_24v_state_CHANNELS_OFF);
    sdc::open();
    break;
  }
}
