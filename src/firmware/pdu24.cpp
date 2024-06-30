#include "pdu24.hpp"
#include "core_pins.h"
#include "canzero/canzero.h"
#include <Arduino.h>
#include <InternalTemperature.h>
#include <algorithm>
#include <numeric>

std::array<Current, pdu24::CHANNEL_COUNT> pdu24::m_currents;
std::array<bool, pdu24::CHANNEL_COUNT> pdu24::m_shorts;
std::array<bool, pdu24::CHANNEL_COUNT> pdu24::m_ctrl;

void pdu24::begin() {
  // initalize adc and shit
  for (size_t i = 0; i < CHANNEL_COUNT; ++i) {
    m_shorts[i] = false;
    m_currents[i] = 0_A;
    m_ctrl[i] = false;
  }
  pinMode(SDC_CTRL_PIN, OUTPUT);
}

void pdu24::update() {
  {
    uint16_t avalue = analogRead(HP_CHANNEL_2_23_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[hp_2_23] = (v * HP_K_ILIS) / HP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(HP_CHANNEL_3_2_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[hp_3_22] = (v * HP_K_ILIS) / HP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(HP_CHANNEL_4_21_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[hp_4_21] = (v * HP_K_ILIS) / HP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(HP_CHANNEL_5_20_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[hp_5_20] = (v * HP_K_ILIS) / HP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL_6_19_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[lp_6_19] = (v * LP_K_ILIS) / LP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL_7_18_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[lp_7_18] = (v * LP_K_ILIS) / LP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL_8_17_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024);
    m_currents[lp_8_17] = (v * LP_K_ILIS) / LP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL_9_16_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[lp_9_16] = (v * LP_K_ILIS) / LP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL_10_15_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[lp_10_15] = (v * LP_K_ILIS) / LP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL_11_14_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[lp_11_14] = (v * LP_K_ILIS) / LP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL_12_41_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[lp_12_41] = (v * LP_K_ILIS) / LP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL_24_40_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[lp_24_40] = (v * LP_K_ILIS) / LP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL_25_39_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[lp_25_39] = (v * LP_K_ILIS) / LP_R_MEAS;
  }

  {
    uint16_t avalue = analogRead(LP_CHANNEL_26_38_SENSE_PIN);
    Voltage v = Voltage(static_cast<float>(avalue) * 3.3 / 1024.0f);
    m_currents[lp_26_38] = (v * LP_K_ILIS) / LP_R_MEAS;
  }

  digitalWrite(HP_CHANNEL_2_23_CTRL_PIN, m_ctrl[hp_2_23]);
  digitalWrite(HP_CHANNEL_3_22_CTRL_PIN, m_ctrl[hp_3_22]);
  digitalWrite(HP_CHANNEL_4_21_CTRL_PIN, m_ctrl[hp_4_21]);
  digitalWrite(HP_CHANNEL_5_20_CTRL_PIN, m_ctrl[hp_5_20]);
  digitalWrite(LP_CHANNEL_6_19_CTRL_PIN, m_ctrl[lp_6_19]);
  digitalWrite(LP_CHANNEL_7_18_CTRL_PIN, m_ctrl[lp_7_18]);
  digitalWrite(LP_CHANNEL_8_17_CTRL_PIN, m_ctrl[lp_8_17]);
  digitalWrite(LP_CHANNEL_9_16_CTRL_PIN, m_ctrl[lp_9_16]);
  digitalWrite(LP_CHANNEL_10_15_CTRL_PIN, m_ctrl[lp_10_15]);
  digitalWrite(LP_CHANNEL_11_14_CTRL_PIN, m_ctrl[lp_11_14]);
  digitalWrite(LP_CHANNEL_12_41_CTRL_PIN, m_ctrl[lp_12_41]);
  digitalWrite(LP_CHANNEL_24_40_CTRL_PIN, m_ctrl[lp_24_40]);
  digitalWrite(LP_CHANNEL_25_39_CTRL_PIN, m_ctrl[lp_25_39]);
  digitalWrite(LP_CHANNEL_26_38_CTRL_PIN, m_ctrl[lp_26_38]);

  m_shorts[hp_2_23] = m_currents[hp_2_23] >= HP_SHORT_CIRCUIT_THRESH;
  m_shorts[hp_3_22] = m_currents[hp_3_22] >= HP_SHORT_CIRCUIT_THRESH;
  m_shorts[hp_4_21] = m_currents[hp_4_21] >= HP_SHORT_CIRCUIT_THRESH;
  m_shorts[hp_5_20] = m_currents[hp_5_20] >= HP_SHORT_CIRCUIT_THRESH;
  m_shorts[lp_6_19] = m_currents[lp_6_19] >= LP_SHORT_CIRCUIT_THRESH;
  m_shorts[lp_7_18] = m_currents[lp_7_18] >= LP_SHORT_CIRCUIT_THRESH;
  m_shorts[lp_8_17] = m_currents[lp_8_17] >= LP_SHORT_CIRCUIT_THRESH;
  m_shorts[lp_9_16] = m_currents[lp_9_16] >= LP_SHORT_CIRCUIT_THRESH;
  m_shorts[lp_10_15] = m_currents[lp_10_15] >= LP_SHORT_CIRCUIT_THRESH;
  m_shorts[lp_11_14] = m_currents[lp_11_14] >= LP_SHORT_CIRCUIT_THRESH;
  m_shorts[lp_12_41] = m_currents[lp_12_41] >= LP_SHORT_CIRCUIT_THRESH;
  m_shorts[lp_24_40] = m_currents[lp_24_40] >= LP_SHORT_CIRCUIT_THRESH;
  m_shorts[lp_25_39] = m_currents[lp_25_39] >= LP_SHORT_CIRCUIT_THRESH;
  m_shorts[lp_26_38] = m_currents[lp_26_38] >= LP_SHORT_CIRCUIT_THRESH;

  if (m_shorts[hp_2_23])
    m_currents[hp_2_23] = 0_A;
  if (m_shorts[hp_3_22])
    m_currents[hp_3_22] = 0_A;
  if (m_shorts[hp_4_21])
    m_currents[hp_4_21] = 0_A;
  if (m_shorts[hp_5_20])
    m_currents[hp_5_20] = 0_A;
  if (m_shorts[lp_6_19])
    m_currents[lp_6_19] = 0_A;
  if (m_shorts[lp_7_18])
    m_currents[lp_7_18] = 0_A;
  if (m_shorts[lp_8_17])
    m_currents[lp_8_17] = 0_A;
  if (m_shorts[lp_9_16])
    m_currents[lp_9_16] = 0_A;
  if (m_shorts[lp_10_15])
    m_currents[lp_10_15] = 0_A;
  if (m_shorts[lp_11_14])
    m_currents[lp_11_14] = 0_A;
  if (m_shorts[lp_12_41])
    m_currents[lp_12_41] = 0_A;
  if (m_shorts[lp_24_40])
    m_currents[lp_24_40] = 0_A;
  if (m_shorts[lp_25_39])
    m_currents[lp_25_39] = 0_A;
  if (m_shorts[lp_26_38])
    m_currents[lp_26_38] = 0_A;
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

void pdu24::set_sdc(bool closed) { 
  digitalWrite(SDC_CTRL_PIN, closed);
  canzero_set_sdc_status(closed ? sdc_status_CLOSED : sdc_status_OPEN);
}

Temperature pdu24::read_mcu_temperature() {
  float temp = InternalTemperature.readTemperatureC();
  float temp_kelvin = temp - 273.15f;
  return Temperature(temp_kelvin);
}

bool pdu24::any_short() {
  return std::any_of(m_shorts.begin(), m_shorts.end(),
                     [](auto x) { return x; });
}

Power pdu24::total_power_output() {
  return std::accumulate(m_currents.begin(), m_currents.end(), Power(0),
      [](Power p, Current c) { return p + Voltage(24) * c; });
}
