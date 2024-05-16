#include <Arduino.h>
#include "canzero/canzero.h"

void __assert_func(const char *, int, const char *, const char *) {
  canzero_set_assertion_fault(error_flag_ERROR);
  // update once more to transmit error information afterwards just loop!
  canzero_update_continue(canzero_get_time()); 
  while (true) {
    Serial.println("FUCKED");
  }
}
