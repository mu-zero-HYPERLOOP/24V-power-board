#include "pdu24.hpp"
#include "core_pins.h"
#include <InternalTemperature.h>

Current pdu24::m_currents[CHANNEL_COUNT];
bool pdu24::m_shorts[CHANNEL_COUNT];
bool pdu24::m_ctrl[CHANNEL_COUNT];

void pdu24::begin() {
  // initalize adc and shit
  for (size_t i = 0; i < CHANNEL_COUNT; ++i) {
    m_shorts[i] = false;
    m_currents[i] = 0_A;
    m_ctrl[i] = false;
  }
}

void pdu24::update() {
  {
    uint16_t avalue = analogRead(HP_CHANNEL1_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > HP_SHORT_CIRCUIT_THRESH) {
      m_currents[hp1] = 0_A;
      m_shorts[hp1] = true;
    } else {
      m_currents[hp1] = (v * HP_K_ILIS) / HP_R_MEAS;
      m_shorts[hp1] = false;
    }
  }

  {
    uint16_t avalue = analogRead(HP_CHANNEL2_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > HP_SHORT_CIRCUIT_THRESH) {
      m_currents[hp2] = 0_A;
      m_shorts[hp2] = true;
    } else {
      m_currents[hp2] = (v * HP_K_ILIS) / HP_R_MEAS;
      m_shorts[hp2] = false;
    }
  }

  {
    uint16_t avalue = analogRead(HP_CHANNEL3_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > HP_SHORT_CIRCUIT_THRESH) {
      m_currents[hp3] = 0_A;
      m_shorts[hp3] = true;
    } else {
      m_currents[hp3] = (v * HP_K_ILIS) / HP_R_MEAS;
      m_shorts[hp3] = false;
    }
  }

  {
    uint16_t avalue = analogRead(HP_CHANNEL4_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > HP_SHORT_CIRCUIT_THRESH) {
      m_currents[hp4] = 0_A;
      m_shorts[hp4] = true;
    } else {
      m_currents[hp4] = (v * HP_K_ILIS) / HP_R_MEAS;
      m_shorts[hp4] = false;
    }
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL1_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > LP_SHORT_CIRCUIT_THRESH) {
      m_currents[lp1] = 0_A;
      m_shorts[lp1] = true;
    } else {
      m_currents[lp1] = (v * LP_K_ILIS) / LP_R_MEAS;
      m_shorts[lp1] = false;
    }
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL2_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > LP_SHORT_CIRCUIT_THRESH) {
      m_currents[lp2] = 0_A;
      m_shorts[lp2] = true;
    } else {
      m_currents[lp2] = (v * LP_K_ILIS) / LP_R_MEAS;
      m_shorts[lp2] = false;
    }
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL3_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > LP_SHORT_CIRCUIT_THRESH) {
      m_currents[lp3] = 0_A;
      m_shorts[lp3] = true;
    } else {
      m_currents[lp3] = (v * LP_K_ILIS) / LP_R_MEAS;
      m_shorts[lp3] = false;
    }
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL4_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > LP_SHORT_CIRCUIT_THRESH) {
      m_currents[lp4] = 0_A;
      m_shorts[lp4] = true;
    } else {
      m_currents[lp4] = (v * LP_K_ILIS) / LP_R_MEAS;
      m_shorts[lp4] = false;
    }
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL5_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > LP_SHORT_CIRCUIT_THRESH) {
      m_currents[lp5] = 0_A;
      m_shorts[lp5] = true;
    } else {
      m_currents[lp5] = (v * LP_K_ILIS) / LP_R_MEAS;
      m_shorts[lp5] = false;
    }
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL6_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > LP_SHORT_CIRCUIT_THRESH) {
      m_currents[lp6] = 0_A;
      m_shorts[lp6] = true;
    } else {
      m_currents[lp6] = (v * LP_K_ILIS) / LP_R_MEAS;
      m_shorts[lp6] = false;
    }
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL7_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > LP_SHORT_CIRCUIT_THRESH) {
      m_currents[lp7] = 0_A;
      m_shorts[lp7] = true;
    } else {
      m_currents[lp7] = (v * LP_K_ILIS) / LP_R_MEAS;
      m_shorts[lp7] = false;
    }
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL8_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > LP_SHORT_CIRCUIT_THRESH) {
      m_currents[lp8] = 0_A;
      m_shorts[lp8] = true;
    } else {
      m_currents[lp8] = (v * LP_K_ILIS) / LP_R_MEAS;
      m_shorts[lp8] = false;
    }
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL9_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > LP_SHORT_CIRCUIT_THRESH) {
      m_currents[lp9] = 0_A;
      m_shorts[lp9] = true;
    } else {
      m_currents[lp9] = (v * LP_K_ILIS) / LP_R_MEAS;
      m_shorts[lp9] = false;
    }
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL10_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 4095.0f);
    if (v > LP_SHORT_CIRCUIT_THRESH) {
      m_currents[lp10] = 0_A;
      m_shorts[lp10] = true;
    } else {
      m_currents[lp10] = (v * LP_K_ILIS) / LP_R_MEAS;
      m_shorts[lp10] = false;
    }
  }
  digitalWrite(HP_CHANNEL1_CTRL_PIN, m_ctrl[hp1] && !m_shorts[hp1]);
  digitalWrite(HP_CHANNEL2_CTRL_PIN, m_ctrl[hp2] && !m_shorts[hp2]);
  digitalWrite(HP_CHANNEL3_CTRL_PIN, m_ctrl[hp3] && !m_shorts[hp3]);
  digitalWrite(HP_CHANNEL4_CTRL_PIN, m_ctrl[hp4] && !m_shorts[hp4]);
  digitalWrite(LP_CHANNEL1_CTRL_PIN, m_ctrl[lp1] && !m_shorts[lp1]);
  digitalWrite(LP_CHANNEL2_CTRL_PIN, m_ctrl[lp2] && !m_shorts[lp2]);
  digitalWrite(LP_CHANNEL3_CTRL_PIN, m_ctrl[lp3] && !m_shorts[lp3]);
  digitalWrite(LP_CHANNEL4_CTRL_PIN, m_ctrl[lp4] && !m_shorts[lp4]);
  digitalWrite(LP_CHANNEL5_CTRL_PIN, m_ctrl[lp5] && !m_shorts[lp5]);
  digitalWrite(LP_CHANNEL6_CTRL_PIN, m_ctrl[lp6] && !m_shorts[lp6]);
  digitalWrite(LP_CHANNEL7_CTRL_PIN, m_ctrl[lp7] && !m_shorts[lp7]);
  digitalWrite(LP_CHANNEL8_CTRL_PIN, m_ctrl[lp8] && !m_shorts[lp8]);
  digitalWrite(LP_CHANNEL9_CTRL_PIN, m_ctrl[lp9] && !m_shorts[lp9]);
  digitalWrite(LP_CHANNEL10_CTRL_PIN, m_ctrl[lp10] && !m_shorts[lp10]);
}

Current pdu24::sense(pdu24_channel channel) { return m_currents[channel]; }

pdu24_channel_status pdu24::status(pdu24_channel channel) {
  if (m_shorts[channel]) {
    return SHORT;
  } else {
    return m_ctrl[channel] ? ON : OFF;
  }
}

void pdu24::control(pdu24_channel channel, bool active) {
  m_ctrl[channel] = active;
}

void pdu24::set_sdc(bool closed) { digitalWrite(SDC_CTRL_PIN, closed); }

Temperature pdu24::read_mcu_temperature() {
  float temp = InternalTemperature.readTemperatureC();
  float temp_kelvin = temp - 273.15f;
  return Temperature(temp_kelvin);
}
