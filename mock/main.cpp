#include "canzero/canzero.h"
#include "defaults.h"
#include <unistd.h>
#include "channel_control.h"

int main() {
  canzero_init();

  can_defaults();

  while (true) {
    canzero_can0_poll();
    canzero_can1_poll();

    channel_control();

    canzero_update_continue(canzero_get_time());
    usleep(100);
  }
}
