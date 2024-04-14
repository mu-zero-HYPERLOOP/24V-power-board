#pragma once
#include "util/metrics.h"

enum pdu24_channel {
  hp1 = 0,
  hp2,
  hp3,
  hp4,
  lp1,
  lp2,
  lp3,
  lp4,
  lp5,
  lp6,
  lp7,
  lp8,
  lp9,
  lp10,
};

enum pdu24_channel_status { OFF, ON, SHORT };

class pdu24 {
public:
  static void begin();
  
  static void update();

  static Current sense(pdu24_channel channel);

  static pdu24_channel_status status(pdu24_channel channel);

  static void control(pdu24_channel channel, bool active);

  static void set_sdc(bool closed);

  static Temperature read_mcu_temperature();

private:
  static constexpr int CHANNEL_COUNT = 14;

  static constexpr int HP_CHANNEL1_CTRL_PIN = 2;
  static constexpr int HP_CHANNEL1_SENSE_PIN = 23;

  static constexpr int HP_CHANNEL2_CTRL_PIN = 3;
  static constexpr int HP_CHANNEL2_SENSE_PIN = 22;

  static constexpr int HP_CHANNEL3_CTRL_PIN = 4;
  static constexpr int HP_CHANNEL3_SENSE_PIN = 21;

  static constexpr int HP_CHANNEL4_CTRL_PIN = 5;
  static constexpr int HP_CHANNEL4_SENSE_PIN = 20;

  static constexpr int LP_CHANNEL1_CTRL_PIN = 6;
  static constexpr int LP_CHANNEL1_SENSE_PIN = 19;

  static constexpr int LP_CHANNEL2_CTRL_PIN = 7;
  static constexpr int LP_CHANNEL2_SENSE_PIN = 18;

  static constexpr int LP_CHANNEL3_CTRL_PIN = 8;
  static constexpr int LP_CHANNEL3_SENSE_PIN = 17;

  static constexpr int LP_CHANNEL4_CTRL_PIN = 9;
  static constexpr int LP_CHANNEL4_SENSE_PIN = 16;

  static constexpr int LP_CHANNEL5_CTRL_PIN = 10;
  static constexpr int LP_CHANNEL5_SENSE_PIN = 15;

  static constexpr int LP_CHANNEL6_CTRL_PIN = 11;
  static constexpr int LP_CHANNEL6_SENSE_PIN = 14;

  static constexpr int LP_CHANNEL7_CTRL_PIN = 12;
  static constexpr int LP_CHANNEL7_SENSE_PIN = 41;

  static constexpr int LP_CHANNEL8_CTRL_PIN = 24;
  static constexpr int LP_CHANNEL8_SENSE_PIN = 40;

  static constexpr int LP_CHANNEL9_CTRL_PIN = 25;
  static constexpr int LP_CHANNEL9_SENSE_PIN = 39;

  static constexpr int LP_CHANNEL10_CTRL_PIN = 26;
  static constexpr int LP_CHANNEL10_SENSE_PIN = 38;

  static constexpr Resistance LP_R_MEAS = 910_Ohm;
  static constexpr float LP_K_ILIS = 1730.0f;
  static constexpr Voltage LP_SHORT_CIRCUIT_THRESH = 2.5_V;

  static constexpr Resistance HP_R_MEAS = 910_Ohm;
  static constexpr float HP_K_ILIS = 1730.0f;
  static constexpr Voltage HP_SHORT_CIRCUIT_THRESH = 2.5_V;

  static constexpr int SDC_CTRL_PIN = 32;

  static Current m_currents[CHANNEL_COUNT];
  static bool m_shorts[CHANNEL_COUNT];
  static bool m_ctrl[CHANNEL_COUNT];

  /* static Current m_currents[ */
};
