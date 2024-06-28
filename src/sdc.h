#pragma once
#include "canzero/canzero.h"
#include "firmware/pdu24.hpp"

namespace sdc {

  inline void begin() {
    pdu24::set_sdc(false);
    canzero_set_sdc_status(sdc_status_OPEN);
  }

  inline void open() {
    pdu24::set_sdc(false);
    canzero_set_sdc_status(sdc_status_OPEN);
  }

  inline void close() {
    pdu24::set_sdc(true);
    canzero_set_sdc_status(sdc_status_CLOSED);
  }
}
