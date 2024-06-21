#include "firmware/pdu24.hpp"
#include "util/interval.h"
#include <cstdio>
#include "print.h"

constexpr Pdu24Channel channel = Pdu24Channel::hp_2_23;

int main() {
  pdu24::begin();
  
  Interval senseInterval {100_Hz};

  Interval switchInterval {5_s};

  bool state = false;
  bool notShorted = true;

  while (true) {
    pdu24::update();
  
    if (switchInterval.next()){
      state = !state;
      pdu24::control(channel, state);
    }

    if (pdu24::status(channel) == Pdu24ChannelStatus::SHORT
        && notShorted) {
      notShorted = false;
    }

    if (senseInterval.next()){
      Current i = pdu24::sense(channel);
      if (state){
        debugPrintf("CHANNEL ON\n");
      }else {
        debugPrintf("CHANNEL OFF\n");
      }
      if (notShorted){
        debugPrintf("Current = %f\n", static_cast<float>(i));
      }else {
        debugPrintf("Current = SHORT\n");
      }
      debugPrintFlush();
    }

  }
}
