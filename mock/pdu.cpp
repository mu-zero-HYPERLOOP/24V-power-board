#include "firmware/pdu24.hpp"
#include <algorithm>
#include <random>

std::array<Current, pdu24::CHANNEL_COUNT> pdu24::m_currents;
std::array<bool, pdu24::CHANNEL_COUNT> pdu24::m_shorts;
std::array<bool, pdu24::CHANNEL_COUNT> pdu24::m_ctrl;

static std::random_device rd{};
static std::mt19937 gen{rd()};

static std::normal_distribution lp_dist{1.0f, 0.05f};
static std::normal_distribution hp_dist{4.0f, 0.05f};

static std::normal_distribution temp_dist{30.0f, 0.5f};

void pdu24::begin() {
  // initalize adc and shit
  for (size_t i = 0; i < CHANNEL_COUNT; ++i) {
    m_shorts[i] = false;
    m_currents[i] = 0_A;
    m_ctrl[i] = false;
  }
}

void pdu24::update() {
}

Current pdu24::sense(pdu24_channel channel) { 
  if (m_ctrl[channel]) {
    if (channel <= hp_5_20) {
      return Current(hp_dist(gen));
    }else {
      return Current(lp_dist(gen));
    }
  }else {
    return 0_A;
  }
}

pdu24_channel_status pdu24::status(pdu24_channel channel) {
  return m_ctrl[channel] ? ON : OFF;
}

void pdu24::control(pdu24_channel channel, bool active) {
  m_ctrl[channel] = active;
}

void pdu24::set_sdc(bool closed) { }

Temperature pdu24::read_mcu_temperature() {
  float celcius = temp_dist(gen);
  return Temperature(celcius - 273.15);
}

bool pdu24::any_short() {
  return std::any_of(m_shorts.begin(), m_shorts.end(),
                     [](auto x) { return x; });
}

Power pdu24::total_power_output() {
  return std::accumulate(m_currents.begin(), m_currents.end(), Power(0),
      [](Power p, Current c) { return p + Voltage(24) * c; });
}
