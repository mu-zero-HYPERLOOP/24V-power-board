#ifndef CANZERO_H
#define CANZERO_H
#include "inttypes.h"
#include "stddef.h"
typedef struct {
  uint16_t od_index;
  uint8_t client_id;
  uint8_t server_id;
} get_req_header;
typedef struct {
  uint8_t sof;
  uint8_t eof;
  uint8_t toggle;
  uint16_t od_index;
  uint8_t client_id;
  uint8_t server_id;
} set_req_header;
typedef enum {
  pdu_channel_control_OFF = 0,
  pdu_channel_control_ON = 1,
} pdu_channel_control;
typedef enum {
  node_id_mlu1 = 0,
  node_id_master = 1,
  node_id_mlu2 = 2,
  node_id_mlu3 = 3,
  node_id_mlu4 = 4,
  node_id_mlu5 = 5,
  node_id_mlu6 = 6,
  node_id_mgu1 = 7,
  node_id_mgu2 = 8,
  node_id_motor_driver = 9,
  node_id_pdu24 = 10,
  node_id_pdu12 = 11,
  node_id_input_board = 12,
  node_id_gamepad = 13,
} node_id;
typedef struct {
  uint8_t sof;
  uint8_t eof;
  uint8_t toggle;
  uint16_t od_index;
  uint8_t client_id;
  uint8_t server_id;
} get_resp_header;
typedef enum {
  set_resp_erno_Success = 0,
  set_resp_erno_Error = 1,
} set_resp_erno;
typedef struct {
  uint16_t od_index;
  uint8_t client_id;
  uint8_t server_id;
  set_resp_erno erno;
} set_resp_header;
typedef enum {
  pdu_channel_status_OFF = 0,
  pdu_channel_status_ON = 1,
  pdu_channel_status_SHORT_CIRCUIT = 3,
} pdu_channel_status;
typedef enum {
  sdc_status_OPEN = 0,
  sdc_status_CLOSED = 1,
} sdc_status;
typedef enum {
  error_flag_OK = 0,
  error_flag_ERROR = 1,
} error_flag;
typedef enum {
  pdu_state_INIT = 0,
  pdu_state_RUNNING = 1,
} pdu_state;
typedef struct {
  uint32_t id;
  uint8_t dlc;
  uint8_t data[8];
} canzero_frame;
typedef enum : uint32_t {
  CANZERO_FRAME_IDE_BIT = 0x40000000, // 1 << 30
  CANZERO_FRAME_RTR_BIT = 0x80000000, // 1 << 31
} can_frame_id_bits;
typedef struct {
  uint32_t mask;
  uint32_t id;
} canzero_can_filter;
extern void canzero_can0_setup(uint32_t baudrate, canzero_can_filter* filters, int filter_count);
extern void canzero_can0_send(canzero_frame* frame);
extern int canzero_can0_recv(canzero_frame* frame);
extern void canzero_can1_setup(uint32_t baudrate, canzero_can_filter* filters, int filter_count);
extern void canzero_can1_send(canzero_frame* frame);
extern int canzero_can1_recv(canzero_frame* frame);
extern void canzero_request_update(uint32_t time);
extern uint32_t canzero_get_time();
extern void canzero_enter_critical();
extern void canzero_exit_critical();
static inline pdu_state canzero_get_state() {
  extern pdu_state __oe_state;
  return __oe_state;
}
static inline pdu_channel_control canzero_get_lp_channel1_control() {
  extern pdu_channel_control __oe_lp_channel1_control;
  return __oe_lp_channel1_control;
}
static inline pdu_channel_status canzero_get_lp_channel1_status() {
  extern pdu_channel_status __oe_lp_channel1_status;
  return __oe_lp_channel1_status;
}
static inline float canzero_get_lp_channel1_current() {
  extern float __oe_lp_channel1_current;
  return __oe_lp_channel1_current;
}
static inline pdu_channel_control canzero_get_lp_channel2_control() {
  extern pdu_channel_control __oe_lp_channel2_control;
  return __oe_lp_channel2_control;
}
static inline pdu_channel_status canzero_get_lp_channel2_status() {
  extern pdu_channel_status __oe_lp_channel2_status;
  return __oe_lp_channel2_status;
}
static inline float canzero_get_lp_channel2_current() {
  extern float __oe_lp_channel2_current;
  return __oe_lp_channel2_current;
}
static inline pdu_channel_control canzero_get_lp_channel3_control() {
  extern pdu_channel_control __oe_lp_channel3_control;
  return __oe_lp_channel3_control;
}
static inline pdu_channel_status canzero_get_lp_channel3_status() {
  extern pdu_channel_status __oe_lp_channel3_status;
  return __oe_lp_channel3_status;
}
static inline float canzero_get_lp_channel3_current() {
  extern float __oe_lp_channel3_current;
  return __oe_lp_channel3_current;
}
static inline pdu_channel_control canzero_get_lp_channel4_control() {
  extern pdu_channel_control __oe_lp_channel4_control;
  return __oe_lp_channel4_control;
}
static inline pdu_channel_status canzero_get_lp_channel4_status() {
  extern pdu_channel_status __oe_lp_channel4_status;
  return __oe_lp_channel4_status;
}
static inline float canzero_get_lp_channel4_current() {
  extern float __oe_lp_channel4_current;
  return __oe_lp_channel4_current;
}
static inline pdu_channel_control canzero_get_lp_channel5_control() {
  extern pdu_channel_control __oe_lp_channel5_control;
  return __oe_lp_channel5_control;
}
static inline pdu_channel_status canzero_get_lp_channel5_status() {
  extern pdu_channel_status __oe_lp_channel5_status;
  return __oe_lp_channel5_status;
}
static inline float canzero_get_lp_channel5_current() {
  extern float __oe_lp_channel5_current;
  return __oe_lp_channel5_current;
}
static inline pdu_channel_control canzero_get_lp_channel6_control() {
  extern pdu_channel_control __oe_lp_channel6_control;
  return __oe_lp_channel6_control;
}
static inline pdu_channel_status canzero_get_lp_channel6_status() {
  extern pdu_channel_status __oe_lp_channel6_status;
  return __oe_lp_channel6_status;
}
static inline float canzero_get_lp_channel6_current() {
  extern float __oe_lp_channel6_current;
  return __oe_lp_channel6_current;
}
static inline pdu_channel_control canzero_get_lp_channel7_control() {
  extern pdu_channel_control __oe_lp_channel7_control;
  return __oe_lp_channel7_control;
}
static inline pdu_channel_status canzero_get_lp_channel7_status() {
  extern pdu_channel_status __oe_lp_channel7_status;
  return __oe_lp_channel7_status;
}
static inline float canzero_get_lp_channel7_current() {
  extern float __oe_lp_channel7_current;
  return __oe_lp_channel7_current;
}
static inline pdu_channel_control canzero_get_lp_channel8_control() {
  extern pdu_channel_control __oe_lp_channel8_control;
  return __oe_lp_channel8_control;
}
static inline pdu_channel_status canzero_get_lp_channel8_status() {
  extern pdu_channel_status __oe_lp_channel8_status;
  return __oe_lp_channel8_status;
}
static inline float canzero_get_lp_channel8_current() {
  extern float __oe_lp_channel8_current;
  return __oe_lp_channel8_current;
}
static inline pdu_channel_control canzero_get_lp_channel9_control() {
  extern pdu_channel_control __oe_lp_channel9_control;
  return __oe_lp_channel9_control;
}
static inline pdu_channel_status canzero_get_lp_channel9_status() {
  extern pdu_channel_status __oe_lp_channel9_status;
  return __oe_lp_channel9_status;
}
static inline float canzero_get_lp_channel9_current() {
  extern float __oe_lp_channel9_current;
  return __oe_lp_channel9_current;
}
static inline pdu_channel_control canzero_get_lp_channel10_control() {
  extern pdu_channel_control __oe_lp_channel10_control;
  return __oe_lp_channel10_control;
}
static inline pdu_channel_status canzero_get_lp_channel10_status() {
  extern pdu_channel_status __oe_lp_channel10_status;
  return __oe_lp_channel10_status;
}
static inline float canzero_get_lp_channel10_current() {
  extern float __oe_lp_channel10_current;
  return __oe_lp_channel10_current;
}
static inline pdu_channel_control canzero_get_hp_channel1_control() {
  extern pdu_channel_control __oe_hp_channel1_control;
  return __oe_hp_channel1_control;
}
static inline pdu_channel_status canzero_get_hp_channel1_status() {
  extern pdu_channel_status __oe_hp_channel1_status;
  return __oe_hp_channel1_status;
}
static inline float canzero_get_hp_channel1_current() {
  extern float __oe_hp_channel1_current;
  return __oe_hp_channel1_current;
}
static inline pdu_channel_control canzero_get_hp_channel2_control() {
  extern pdu_channel_control __oe_hp_channel2_control;
  return __oe_hp_channel2_control;
}
static inline pdu_channel_status canzero_get_hp_channel2_status() {
  extern pdu_channel_status __oe_hp_channel2_status;
  return __oe_hp_channel2_status;
}
static inline float canzero_get_hp_channel2_current() {
  extern float __oe_hp_channel2_current;
  return __oe_hp_channel2_current;
}
static inline pdu_channel_control canzero_get_hp_channel3_control() {
  extern pdu_channel_control __oe_hp_channel3_control;
  return __oe_hp_channel3_control;
}
static inline pdu_channel_status canzero_get_hp_channel3_status() {
  extern pdu_channel_status __oe_hp_channel3_status;
  return __oe_hp_channel3_status;
}
static inline float canzero_get_hp_channel3_current() {
  extern float __oe_hp_channel3_current;
  return __oe_hp_channel3_current;
}
static inline pdu_channel_control canzero_get_hp_channel4_control() {
  extern pdu_channel_control __oe_hp_channel4_control;
  return __oe_hp_channel4_control;
}
static inline pdu_channel_status canzero_get_hp_channel4_status() {
  extern pdu_channel_status __oe_hp_channel4_status;
  return __oe_hp_channel4_status;
}
static inline float canzero_get_hp_channel4_current() {
  extern float __oe_hp_channel4_current;
  return __oe_hp_channel4_current;
}
static inline float canzero_get_total_current() {
  extern float __oe_total_current;
  return __oe_total_current;
}
static inline float canzero_get_power_estimation() {
  extern float __oe_power_estimation;
  return __oe_power_estimation;
}
static inline sdc_status canzero_get_sdc_status() {
  extern sdc_status __oe_sdc_status;
  return __oe_sdc_status;
}
static inline float canzero_get_mcu_temperature() {
  extern float __oe_mcu_temperature;
  return __oe_mcu_temperature;
}
static inline error_flag canzero_get_error_mcu_over_temperature() {
  extern error_flag __oe_error_mcu_over_temperature;
  return __oe_error_mcu_over_temperature;
}
static inline error_flag canzero_get_warn_mcu_over_temperature() {
  extern error_flag __oe_warn_mcu_over_temperature;
  return __oe_warn_mcu_over_temperature;
}
typedef struct {
  get_resp_header header;
  uint32_t data;
} canzero_message_get_resp;
static const uint32_t canzero_message_get_resp_id = 0x13F;
typedef struct {
  set_resp_header header;
} canzero_message_set_resp;
static const uint32_t canzero_message_set_resp_id = 0x15F;
typedef struct {
  float lp_channel1_current;
  float lp_channel2_current;
  float lp_channel3_current;
  float lp_channel4_current;
  float lp_channel5_current;
  float lp_channel6_current;
  float lp_channel7_current;
  float lp_channel8_current;
} canzero_message_pdu24_stream_channel_currents_1;
static const uint32_t canzero_message_pdu24_stream_channel_currents_1_id = 0x9C;
typedef struct {
  float lp_channel9_current;
  float lp_channel10_current;
  float hp_channel1_current;
  float hp_channel2_current;
  float hp_channel3_current;
  float hp_channel4_current;
} canzero_message_pdu24_stream_channel_currents_2;
static const uint32_t canzero_message_pdu24_stream_channel_currents_2_id = 0xBC;
typedef struct {
  float total_current;
  float power_estimation;
} canzero_message_pdu24_stream_power_estimation;
static const uint32_t canzero_message_pdu24_stream_power_estimation_id = 0x94;
typedef struct {
  pdu_channel_status lp_channel1_status;
  pdu_channel_status lp_channel2_status;
  pdu_channel_status lp_channel3_status;
  pdu_channel_status lp_channel4_status;
  pdu_channel_status lp_channel5_status;
  pdu_channel_status lp_channel6_status;
  pdu_channel_status lp_channel7_status;
  pdu_channel_status lp_channel8_status;
  pdu_channel_status lp_channel9_status;
  pdu_channel_status lp_channel10_status;
  pdu_channel_status hp_channel1_status;
  pdu_channel_status hp_channel2_status;
  pdu_channel_status hp_channel3_status;
  pdu_channel_status hp_channel4_status;
  sdc_status sdc_status;
  error_flag error_mcu_over_temperature;
  error_flag warn_mcu_over_temperature;
} canzero_message_pdu24_stream_channel_status;
static const uint32_t canzero_message_pdu24_stream_channel_status_id = 0x74;
typedef struct {
  pdu_state state;
} canzero_message_pdu24_stream_state;
static const uint32_t canzero_message_pdu24_stream_state_id = 0xB4;
typedef struct {
  node_id node_id;
} canzero_message_heartbeat;
static const uint32_t canzero_message_heartbeat_id = 0x17B;
typedef struct {
  get_req_header header;
} canzero_message_get_req;
static const uint32_t canzero_message_get_req_id = 0x13B;
typedef struct {
  set_req_header header;
  uint32_t data;
} canzero_message_set_req;
static const uint32_t canzero_message_set_req_id = 0x15B;
typedef struct {
  pdu_channel_control pdu24_lp_channel1_control;
  pdu_channel_control pdu24_lp_channel2_control;
  pdu_channel_control pdu24_lp_channel3_control;
  pdu_channel_control pdu24_lp_channel4_control;
  pdu_channel_control pdu24_lp_channel5_control;
  pdu_channel_control pdu24_lp_channel6_control;
  pdu_channel_control pdu24_lp_channel7_control;
  pdu_channel_control pdu24_lp_channel8_control;
  pdu_channel_control pdu24_lp_channel9_control;
  pdu_channel_control pdu24_lp_channel10_control;
  pdu_channel_control pdu24_hp_channel1_control;
  pdu_channel_control pdu24_hp_channel2_control;
  pdu_channel_control pdu24_hp_channel3_control;
  pdu_channel_control pdu24_hp_channel4_control;
  pdu_channel_control pdu24_lp_channel11_control;
  pdu_channel_control pdu24_lp_channel12_control;
} canzero_message_master_stream_pdu24_control;
static const uint32_t canzero_message_master_stream_pdu24_control_id = 0x4F;
static void canzero_serialize_canzero_message_get_resp(canzero_message_get_resp* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x13F;
  frame->dlc = 8;
  ((uint32_t*)data)[0] = (uint8_t)(msg->header.sof & (0xFF >> (8 - 1)));
  ((uint32_t*)data)[0] |= (uint8_t)(msg->header.eof & (0xFF >> (8 - 1))) << 1;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->header.toggle & (0xFF >> (8 - 1))) << 2;
  ((uint32_t*)data)[0] |= (uint16_t)(msg->header.od_index & (0xFFFF >> (16 - 13))) << 3;
  ((uint32_t*)data)[0] |= msg->header.client_id << 16;
  ((uint32_t*)data)[0] |= msg->header.server_id << 24;
  ((uint32_t*)data)[1] = msg->data;
}
static void canzero_serialize_canzero_message_set_resp(canzero_message_set_resp* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x15F;
  frame->dlc = 4;
  ((uint32_t*)data)[0] = (uint16_t)(msg->header.od_index & (0xFFFF >> (16 - 13)));
  ((uint32_t*)data)[0] |= msg->header.client_id << 13;
  ((uint32_t*)data)[0] |= msg->header.server_id << 21;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->header.erno & (0xFF >> (8 - 1))) << 29;
}
static void canzero_serialize_canzero_message_pdu24_stream_channel_currents_1(canzero_message_pdu24_stream_channel_currents_1* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x9C;
  frame->dlc = 8;
  float lp_channel1_current_0 = (msg->lp_channel1_current - 0) / 0.011764705882352941;
  if (lp_channel1_current_0 > 4278190080.000) {
    lp_channel1_current_0 = 4278190080.000;
  }
  ((uint32_t*)data)[0] = (uint32_t) lp_channel1_current_0;
  float lp_channel2_current_8 = (msg->lp_channel2_current - 0) / 0.011764705882352941;
  if (lp_channel2_current_8 > 4278190080.000) {
    lp_channel2_current_8 = 4278190080.000;
  }
  ((uint32_t*)data)[0] |= (uint32_t) lp_channel2_current_8 << 8;
  float lp_channel3_current_16 = (msg->lp_channel3_current - 0) / 0.011764705882352941;
  if (lp_channel3_current_16 > 4278190080.000) {
    lp_channel3_current_16 = 4278190080.000;
  }
  ((uint32_t*)data)[0] |= (uint32_t) lp_channel3_current_16 << 16;
  float lp_channel4_current_24 = (msg->lp_channel4_current - 0) / 0.011764705882352941;
  if (lp_channel4_current_24 > 4278190080.000) {
    lp_channel4_current_24 = 4278190080.000;
  }
  ((uint32_t*)data)[0] |= (uint32_t) lp_channel4_current_24 << 24;
  float lp_channel5_current_32 = (msg->lp_channel5_current - 0) / 0.011764705882352941;
  if (lp_channel5_current_32 > 4278190080.000) {
    lp_channel5_current_32 = 4278190080.000;
  }
  ((uint32_t*)data)[1] = (uint32_t) lp_channel5_current_32;
  float lp_channel6_current_40 = (msg->lp_channel6_current - 0) / 0.011764705882352941;
  if (lp_channel6_current_40 > 4278190080.000) {
    lp_channel6_current_40 = 4278190080.000;
  }
  ((uint32_t*)data)[1] |= (uint32_t) lp_channel6_current_40 << 8;
  float lp_channel7_current_48 = (msg->lp_channel7_current - 0) / 0.011764705882352941;
  if (lp_channel7_current_48 > 4278190080.000) {
    lp_channel7_current_48 = 4278190080.000;
  }
  ((uint32_t*)data)[1] |= (uint32_t) lp_channel7_current_48 << 16;
  float lp_channel8_current_56 = (msg->lp_channel8_current - 0) / 0.011764705882352941;
  if (lp_channel8_current_56 > 4278190080.000) {
    lp_channel8_current_56 = 4278190080.000;
  }
  ((uint32_t*)data)[1] |= (uint32_t) lp_channel8_current_56 << 24;
}
static void canzero_serialize_canzero_message_pdu24_stream_channel_currents_2(canzero_message_pdu24_stream_channel_currents_2* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0xBC;
  frame->dlc = 6;
  float lp_channel9_current_0 = (msg->lp_channel9_current - 0) / 0.011764705882352941;
  if (lp_channel9_current_0 > 4278190080.000) {
    lp_channel9_current_0 = 4278190080.000;
  }
  ((uint32_t*)data)[0] = (uint32_t) lp_channel9_current_0;
  float lp_channel10_current_8 = (msg->lp_channel10_current - 0) / 0.011764705882352941;
  if (lp_channel10_current_8 > 4278190080.000) {
    lp_channel10_current_8 = 4278190080.000;
  }
  ((uint32_t*)data)[0] |= (uint32_t) lp_channel10_current_8 << 8;
  float hp_channel1_current_16 = (msg->hp_channel1_current - 0) / 0.0392156862745098;
  if (hp_channel1_current_16 > 4278190080.000) {
    hp_channel1_current_16 = 4278190080.000;
  }
  ((uint32_t*)data)[0] |= (uint32_t) hp_channel1_current_16 << 16;
  float hp_channel2_current_24 = (msg->hp_channel2_current - 0) / 0.0392156862745098;
  if (hp_channel2_current_24 > 4278190080.000) {
    hp_channel2_current_24 = 4278190080.000;
  }
  ((uint32_t*)data)[0] |= (uint32_t) hp_channel2_current_24 << 24;
  float hp_channel3_current_32 = (msg->hp_channel3_current - 0) / 0.0392156862745098;
  if (hp_channel3_current_32 > 4278190080.000) {
    hp_channel3_current_32 = 4278190080.000;
  }
  ((uint32_t*)data)[1] = (uint32_t) hp_channel3_current_32;
  float hp_channel4_current_40 = (msg->hp_channel4_current - 0) / 0.0392156862745098;
  if (hp_channel4_current_40 > 4278190080.000) {
    hp_channel4_current_40 = 4278190080.000;
  }
  ((uint32_t*)data)[1] |= (uint32_t) hp_channel4_current_40 << 8;
}
static void canzero_serialize_canzero_message_pdu24_stream_power_estimation(canzero_message_pdu24_stream_power_estimation* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x94;
  frame->dlc = 4;
  float total_current_0 = (msg->total_current - 0) / 0.0012207217517357137;
  if (total_current_0 > 4294901760.000) {
    total_current_0 = 4294901760.000;
  }
  ((uint32_t*)data)[0] = (uint32_t) total_current_0;
  float power_estimation_16 = (msg->power_estimation - 0) / 0.030518043793392843;
  if (power_estimation_16 > 4294901760.000) {
    power_estimation_16 = 4294901760.000;
  }
  ((uint32_t*)data)[0] |= (uint32_t) power_estimation_16 << 16;
}
static void canzero_serialize_canzero_message_pdu24_stream_channel_status(canzero_message_pdu24_stream_channel_status* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x74;
  frame->dlc = 4;
  ((uint32_t*)data)[0] = (uint8_t)(msg->lp_channel1_status & (0xFF >> (8 - 2)));
  ((uint32_t*)data)[0] |= (uint8_t)(msg->lp_channel2_status & (0xFF >> (8 - 2))) << 2;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->lp_channel3_status & (0xFF >> (8 - 2))) << 4;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->lp_channel4_status & (0xFF >> (8 - 2))) << 6;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->lp_channel5_status & (0xFF >> (8 - 2))) << 8;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->lp_channel6_status & (0xFF >> (8 - 2))) << 10;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->lp_channel7_status & (0xFF >> (8 - 2))) << 12;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->lp_channel8_status & (0xFF >> (8 - 2))) << 14;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->lp_channel9_status & (0xFF >> (8 - 2))) << 16;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->lp_channel10_status & (0xFF >> (8 - 2))) << 18;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->hp_channel1_status & (0xFF >> (8 - 2))) << 20;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->hp_channel2_status & (0xFF >> (8 - 2))) << 22;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->hp_channel3_status & (0xFF >> (8 - 2))) << 24;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->hp_channel4_status & (0xFF >> (8 - 2))) << 26;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->sdc_status & (0xFF >> (8 - 1))) << 28;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->error_mcu_over_temperature & (0xFF >> (8 - 1))) << 29;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->warn_mcu_over_temperature & (0xFF >> (8 - 1))) << 30;
}
static void canzero_serialize_canzero_message_pdu24_stream_state(canzero_message_pdu24_stream_state* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0xB4;
  frame->dlc = 1;
  ((uint32_t*)data)[0] = (uint8_t)(msg->state & (0xFF >> (8 - 1)));
}
static void canzero_serialize_canzero_message_heartbeat(canzero_message_heartbeat* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x17B;
  frame->dlc = 1;
  ((uint32_t*)data)[0] = (uint8_t)(msg->node_id & (0xFF >> (8 - 4)));
}
static void canzero_serialize_canzero_message_get_req(canzero_message_get_req* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x13B;
  frame->dlc = 4;
  ((uint32_t*)data)[0] = (uint16_t)(msg->header.od_index & (0xFFFF >> (16 - 13)));
  ((uint32_t*)data)[0] |= msg->header.client_id << 13;
  ((uint32_t*)data)[0] |= msg->header.server_id << 21;
}
static void canzero_serialize_canzero_message_set_req(canzero_message_set_req* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x15B;
  frame->dlc = 8;
  ((uint32_t*)data)[0] = (uint8_t)(msg->header.sof & (0xFF >> (8 - 1)));
  ((uint32_t*)data)[0] |= (uint8_t)(msg->header.eof & (0xFF >> (8 - 1))) << 1;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->header.toggle & (0xFF >> (8 - 1))) << 2;
  ((uint32_t*)data)[0] |= (uint16_t)(msg->header.od_index & (0xFFFF >> (16 - 13))) << 3;
  ((uint32_t*)data)[0] |= msg->header.client_id << 16;
  ((uint32_t*)data)[0] |= msg->header.server_id << 24;
  ((uint32_t*)data)[1] = msg->data;
}
static void canzero_serialize_canzero_message_master_stream_pdu24_control(canzero_message_master_stream_pdu24_control* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x4F;
  frame->dlc = 2;
  ((uint32_t*)data)[0] = (uint8_t)(msg->pdu24_lp_channel1_control & (0xFF >> (8 - 1)));
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_lp_channel2_control & (0xFF >> (8 - 1))) << 1;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_lp_channel3_control & (0xFF >> (8 - 1))) << 2;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_lp_channel4_control & (0xFF >> (8 - 1))) << 3;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_lp_channel5_control & (0xFF >> (8 - 1))) << 4;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_lp_channel6_control & (0xFF >> (8 - 1))) << 5;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_lp_channel7_control & (0xFF >> (8 - 1))) << 6;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_lp_channel8_control & (0xFF >> (8 - 1))) << 7;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_lp_channel9_control & (0xFF >> (8 - 1))) << 8;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_lp_channel10_control & (0xFF >> (8 - 1))) << 9;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_hp_channel1_control & (0xFF >> (8 - 1))) << 10;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_hp_channel2_control & (0xFF >> (8 - 1))) << 11;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_hp_channel3_control & (0xFF >> (8 - 1))) << 12;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_hp_channel4_control & (0xFF >> (8 - 1))) << 13;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_lp_channel11_control & (0xFF >> (8 - 1))) << 14;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->pdu24_lp_channel12_control & (0xFF >> (8 - 1))) << 15;
}
static void canzero_deserialize_canzero_message_get_resp(canzero_frame* frame, canzero_message_get_resp* msg) {
  uint8_t* data = frame->data;
  msg->header.sof = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 1)));
  msg->header.eof = ((((uint32_t*)data)[0] >> 1) & (0xFFFFFFFF >> (32 - 1)));
  msg->header.toggle = ((((uint32_t*)data)[0] >> 2) & (0xFFFFFFFF >> (32 - 1)));
  msg->header.od_index = ((((uint32_t*)data)[0] >> 3) & (0xFFFFFFFF >> (32 - 13)));
  msg->header.client_id = ((((uint32_t*)data)[0] >> 16) & (0xFFFFFFFF >> (32 - 8)));
  msg->header.server_id = ((((uint32_t*)data)[0] >> 24) & (0xFFFFFFFF >> (32 - 8)));
  msg->data = (((uint32_t*)data)[1] & (0xFFFFFFFF >> (32 - 32)));
}
static void canzero_deserialize_canzero_message_set_resp(canzero_frame* frame, canzero_message_set_resp* msg) {
  uint8_t* data = frame->data;
  msg->header.od_index = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 13)));
  msg->header.client_id = ((((uint32_t*)data)[0] >> 13) & (0xFFFFFFFF >> (32 - 8)));
  msg->header.server_id = ((((uint32_t*)data)[0] >> 21) & (0xFFFFFFFF >> (32 - 8)));
  msg->header.erno = (set_resp_erno)((((uint32_t*)data)[0] >> 29) & (0xFFFFFFFF >> (32 - 1)));
}
static void canzero_deserialize_canzero_message_pdu24_stream_channel_currents_1(canzero_frame* frame, canzero_message_pdu24_stream_channel_currents_1* msg) {
  uint8_t* data = frame->data;
  msg->lp_channel1_current = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0;
  msg->lp_channel2_current = ((((uint32_t*)data)[0] >> 8) & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0;
  msg->lp_channel3_current = ((((uint32_t*)data)[0] >> 16) & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0;
  msg->lp_channel4_current = ((((uint32_t*)data)[0] >> 24) & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0;
  msg->lp_channel5_current = (((uint32_t*)data)[1] & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0;
  msg->lp_channel6_current = ((((uint32_t*)data)[1] >> 8) & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0;
  msg->lp_channel7_current = ((((uint32_t*)data)[1] >> 16) & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0;
  msg->lp_channel8_current = ((((uint32_t*)data)[1] >> 24) & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0;
}
static void canzero_deserialize_canzero_message_pdu24_stream_channel_currents_2(canzero_frame* frame, canzero_message_pdu24_stream_channel_currents_2* msg) {
  uint8_t* data = frame->data;
  msg->lp_channel9_current = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0;
  msg->lp_channel10_current = ((((uint32_t*)data)[0] >> 8) & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0;
  msg->hp_channel1_current = ((((uint32_t*)data)[0] >> 16) & (0xFFFFFFFF >> (32 - 8))) * 0.0392156862745098 + 0;
  msg->hp_channel2_current = ((((uint32_t*)data)[0] >> 24) & (0xFFFFFFFF >> (32 - 8))) * 0.0392156862745098 + 0;
  msg->hp_channel3_current = (((uint32_t*)data)[1] & (0xFFFFFFFF >> (32 - 8))) * 0.0392156862745098 + 0;
  msg->hp_channel4_current = ((((uint32_t*)data)[1] >> 8) & (0xFFFFFFFF >> (32 - 8))) * 0.0392156862745098 + 0;
}
static void canzero_deserialize_canzero_message_pdu24_stream_power_estimation(canzero_frame* frame, canzero_message_pdu24_stream_power_estimation* msg) {
  uint8_t* data = frame->data;
  msg->total_current = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 16))) * 0.0012207217517357137 + 0;
  msg->power_estimation = ((((uint32_t*)data)[0] >> 16) & (0xFFFFFFFF >> (32 - 16))) * 0.030518043793392843 + 0;
}
static void canzero_deserialize_canzero_message_pdu24_stream_channel_status(canzero_frame* frame, canzero_message_pdu24_stream_channel_status* msg) {
  uint8_t* data = frame->data;
  msg->lp_channel1_status = (pdu_channel_status)(((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 2)));
  msg->lp_channel2_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 2) & (0xFFFFFFFF >> (32 - 2)));
  msg->lp_channel3_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 4) & (0xFFFFFFFF >> (32 - 2)));
  msg->lp_channel4_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 6) & (0xFFFFFFFF >> (32 - 2)));
  msg->lp_channel5_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 8) & (0xFFFFFFFF >> (32 - 2)));
  msg->lp_channel6_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 10) & (0xFFFFFFFF >> (32 - 2)));
  msg->lp_channel7_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 12) & (0xFFFFFFFF >> (32 - 2)));
  msg->lp_channel8_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 14) & (0xFFFFFFFF >> (32 - 2)));
  msg->lp_channel9_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 16) & (0xFFFFFFFF >> (32 - 2)));
  msg->lp_channel10_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 18) & (0xFFFFFFFF >> (32 - 2)));
  msg->hp_channel1_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 20) & (0xFFFFFFFF >> (32 - 2)));
  msg->hp_channel2_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 22) & (0xFFFFFFFF >> (32 - 2)));
  msg->hp_channel3_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 24) & (0xFFFFFFFF >> (32 - 2)));
  msg->hp_channel4_status = (pdu_channel_status)((((uint32_t*)data)[0] >> 26) & (0xFFFFFFFF >> (32 - 2)));
  msg->sdc_status = (sdc_status)((((uint32_t*)data)[0] >> 28) & (0xFFFFFFFF >> (32 - 1)));
  msg->error_mcu_over_temperature = (error_flag)((((uint32_t*)data)[0] >> 29) & (0xFFFFFFFF >> (32 - 1)));
  msg->warn_mcu_over_temperature = (error_flag)((((uint32_t*)data)[0] >> 30) & (0xFFFFFFFF >> (32 - 1)));
}
static void canzero_deserialize_canzero_message_pdu24_stream_state(canzero_frame* frame, canzero_message_pdu24_stream_state* msg) {
  uint8_t* data = frame->data;
  msg->state = (pdu_state)(((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 1)));
}
static void canzero_deserialize_canzero_message_heartbeat(canzero_frame* frame, canzero_message_heartbeat* msg) {
  uint8_t* data = frame->data;
  msg->node_id = (node_id)(((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 4)));
}
static void canzero_deserialize_canzero_message_get_req(canzero_frame* frame, canzero_message_get_req* msg) {
  uint8_t* data = frame->data;
  msg->header.od_index = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 13)));
  msg->header.client_id = ((((uint32_t*)data)[0] >> 13) & (0xFFFFFFFF >> (32 - 8)));
  msg->header.server_id = ((((uint32_t*)data)[0] >> 21) & (0xFFFFFFFF >> (32 - 8)));
}
static void canzero_deserialize_canzero_message_set_req(canzero_frame* frame, canzero_message_set_req* msg) {
  uint8_t* data = frame->data;
  msg->header.sof = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 1)));
  msg->header.eof = ((((uint32_t*)data)[0] >> 1) & (0xFFFFFFFF >> (32 - 1)));
  msg->header.toggle = ((((uint32_t*)data)[0] >> 2) & (0xFFFFFFFF >> (32 - 1)));
  msg->header.od_index = ((((uint32_t*)data)[0] >> 3) & (0xFFFFFFFF >> (32 - 13)));
  msg->header.client_id = ((((uint32_t*)data)[0] >> 16) & (0xFFFFFFFF >> (32 - 8)));
  msg->header.server_id = ((((uint32_t*)data)[0] >> 24) & (0xFFFFFFFF >> (32 - 8)));
  msg->data = (((uint32_t*)data)[1] & (0xFFFFFFFF >> (32 - 32)));
}
static void canzero_deserialize_canzero_message_master_stream_pdu24_control(canzero_frame* frame, canzero_message_master_stream_pdu24_control* msg) {
  uint8_t* data = frame->data;
  msg->pdu24_lp_channel1_control = (pdu_channel_control)(((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_lp_channel2_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 1) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_lp_channel3_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 2) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_lp_channel4_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 3) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_lp_channel5_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 4) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_lp_channel6_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 5) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_lp_channel7_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 6) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_lp_channel8_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 7) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_lp_channel9_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 8) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_lp_channel10_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 9) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_hp_channel1_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 10) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_hp_channel2_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 11) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_hp_channel3_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 12) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_hp_channel4_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 13) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_lp_channel11_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 14) & (0xFFFFFFFF >> (32 - 1)));
  msg->pdu24_lp_channel12_control = (pdu_channel_control)((((uint32_t*)data)[0] >> 15) & (0xFFFFFFFF >> (32 - 1)));
}
void canzero_can0_poll();
void canzero_can1_poll();
uint32_t canzero_update_continue(uint32_t delta_time);
void canzero_init();
void canzero_set_state(pdu_state value);
static inline void canzero_set_lp_channel1_control(pdu_channel_control value){
  extern pdu_channel_control __oe_lp_channel1_control;
  __oe_lp_channel1_control = value;
}
void canzero_set_lp_channel1_status(pdu_channel_status value);
static inline void canzero_set_lp_channel1_current(float value){
  extern float __oe_lp_channel1_current;
  __oe_lp_channel1_current = value;
}
static inline void canzero_set_lp_channel2_control(pdu_channel_control value){
  extern pdu_channel_control __oe_lp_channel2_control;
  __oe_lp_channel2_control = value;
}
void canzero_set_lp_channel2_status(pdu_channel_status value);
static inline void canzero_set_lp_channel2_current(float value){
  extern float __oe_lp_channel2_current;
  __oe_lp_channel2_current = value;
}
static inline void canzero_set_lp_channel3_control(pdu_channel_control value){
  extern pdu_channel_control __oe_lp_channel3_control;
  __oe_lp_channel3_control = value;
}
void canzero_set_lp_channel3_status(pdu_channel_status value);
static inline void canzero_set_lp_channel3_current(float value){
  extern float __oe_lp_channel3_current;
  __oe_lp_channel3_current = value;
}
static inline void canzero_set_lp_channel4_control(pdu_channel_control value){
  extern pdu_channel_control __oe_lp_channel4_control;
  __oe_lp_channel4_control = value;
}
void canzero_set_lp_channel4_status(pdu_channel_status value);
static inline void canzero_set_lp_channel4_current(float value){
  extern float __oe_lp_channel4_current;
  __oe_lp_channel4_current = value;
}
static inline void canzero_set_lp_channel5_control(pdu_channel_control value){
  extern pdu_channel_control __oe_lp_channel5_control;
  __oe_lp_channel5_control = value;
}
void canzero_set_lp_channel5_status(pdu_channel_status value);
static inline void canzero_set_lp_channel5_current(float value){
  extern float __oe_lp_channel5_current;
  __oe_lp_channel5_current = value;
}
static inline void canzero_set_lp_channel6_control(pdu_channel_control value){
  extern pdu_channel_control __oe_lp_channel6_control;
  __oe_lp_channel6_control = value;
}
void canzero_set_lp_channel6_status(pdu_channel_status value);
static inline void canzero_set_lp_channel6_current(float value){
  extern float __oe_lp_channel6_current;
  __oe_lp_channel6_current = value;
}
static inline void canzero_set_lp_channel7_control(pdu_channel_control value){
  extern pdu_channel_control __oe_lp_channel7_control;
  __oe_lp_channel7_control = value;
}
void canzero_set_lp_channel7_status(pdu_channel_status value);
static inline void canzero_set_lp_channel7_current(float value){
  extern float __oe_lp_channel7_current;
  __oe_lp_channel7_current = value;
}
static inline void canzero_set_lp_channel8_control(pdu_channel_control value){
  extern pdu_channel_control __oe_lp_channel8_control;
  __oe_lp_channel8_control = value;
}
void canzero_set_lp_channel8_status(pdu_channel_status value);
static inline void canzero_set_lp_channel8_current(float value){
  extern float __oe_lp_channel8_current;
  __oe_lp_channel8_current = value;
}
static inline void canzero_set_lp_channel9_control(pdu_channel_control value){
  extern pdu_channel_control __oe_lp_channel9_control;
  __oe_lp_channel9_control = value;
}
void canzero_set_lp_channel9_status(pdu_channel_status value);
static inline void canzero_set_lp_channel9_current(float value){
  extern float __oe_lp_channel9_current;
  __oe_lp_channel9_current = value;
}
static inline void canzero_set_lp_channel10_control(pdu_channel_control value){
  extern pdu_channel_control __oe_lp_channel10_control;
  __oe_lp_channel10_control = value;
}
void canzero_set_lp_channel10_status(pdu_channel_status value);
static inline void canzero_set_lp_channel10_current(float value){
  extern float __oe_lp_channel10_current;
  __oe_lp_channel10_current = value;
}
static inline void canzero_set_hp_channel1_control(pdu_channel_control value){
  extern pdu_channel_control __oe_hp_channel1_control;
  __oe_hp_channel1_control = value;
}
void canzero_set_hp_channel1_status(pdu_channel_status value);
static inline void canzero_set_hp_channel1_current(float value){
  extern float __oe_hp_channel1_current;
  __oe_hp_channel1_current = value;
}
static inline void canzero_set_hp_channel2_control(pdu_channel_control value){
  extern pdu_channel_control __oe_hp_channel2_control;
  __oe_hp_channel2_control = value;
}
void canzero_set_hp_channel2_status(pdu_channel_status value);
static inline void canzero_set_hp_channel2_current(float value){
  extern float __oe_hp_channel2_current;
  __oe_hp_channel2_current = value;
}
static inline void canzero_set_hp_channel3_control(pdu_channel_control value){
  extern pdu_channel_control __oe_hp_channel3_control;
  __oe_hp_channel3_control = value;
}
void canzero_set_hp_channel3_status(pdu_channel_status value);
static inline void canzero_set_hp_channel3_current(float value){
  extern float __oe_hp_channel3_current;
  __oe_hp_channel3_current = value;
}
static inline void canzero_set_hp_channel4_control(pdu_channel_control value){
  extern pdu_channel_control __oe_hp_channel4_control;
  __oe_hp_channel4_control = value;
}
void canzero_set_hp_channel4_status(pdu_channel_status value);
static inline void canzero_set_hp_channel4_current(float value){
  extern float __oe_hp_channel4_current;
  __oe_hp_channel4_current = value;
}
static inline void canzero_set_total_current(float value){
  extern float __oe_total_current;
  __oe_total_current = value;
}
static inline void canzero_set_power_estimation(float value){
  extern float __oe_power_estimation;
  __oe_power_estimation = value;
}
void canzero_set_sdc_status(sdc_status value);
static inline void canzero_set_mcu_temperature(float value){
  extern float __oe_mcu_temperature;
  __oe_mcu_temperature = value;
}
void canzero_set_error_mcu_over_temperature(error_flag value);
void canzero_set_warn_mcu_over_temperature(error_flag value);
#endif