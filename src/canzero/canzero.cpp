#include "canzero.h"
uint32_t min_u32(uint32_t v, uint32_t max) {
    if (v > max) {
        return max;
    }
    return v;
}
uint64_t min_u64(uint64_t v, uint64_t max) {
    if (v > max) {
        return max;
    }
    return v;
}
uint64_t __oe_config_hash;
date_time __oe_build_time;
pdu_state __oe_state;
sdc_status __oe_sdc_status;
error_flag __oe_assertion_fault;
double __oe_loop_frequency;
pdu_channel_control __oe_cooling_pump_channel_ctrl;
float __oe_cooling_pump_channel_current;
pdu_channel_status __oe_cooling_pump_channel_status;
pdu_channel_control __oe_sdc_power_channel_ctrl;
float __oe_sdc_power_channel_current;
pdu_channel_status __oe_sdc_power_channel_status;
pdu_channel_control __oe_sdc_signal_channel_ctrl;
float __oe_sdc_signal_channel_current;
pdu_channel_status __oe_sdc_signal_channel_status;
pdu_channel_control __oe_fan_channel_ctrl;
float __oe_fan_channel_current;
pdu_channel_status __oe_fan_channel_status;
float __oe_mcu_temperature;
error_level __oe_error_level_mcu_temperature;
error_level_config __oe_error_level_config_mcu_temperature;
static void canzero_serialize_canzero_message_get_resp(canzero_message_get_resp* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x9E;
  frame->dlc = 8;
  ((uint32_t*)data)[0] = (uint8_t)(msg->m_header.m_sof & (0xFF >> (8 - 1)));
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_header.m_eof & (0xFF >> (8 - 1))) << 1;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_header.m_toggle & (0xFF >> (8 - 1))) << 2;
  ((uint32_t*)data)[0] |= (uint16_t)(msg->m_header.m_od_index & (0xFFFF >> (16 - 13))) << 3;
  ((uint32_t*)data)[0] |= msg->m_header.m_client_id << 16;
  ((uint32_t*)data)[0] |= msg->m_header.m_server_id << 24;
  ((uint32_t*)data)[1] = msg->m_data;
}
static void canzero_serialize_canzero_message_set_resp(canzero_message_set_resp* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0xBE;
  frame->dlc = 4;
  ((uint32_t*)data)[0] = (uint16_t)(msg->m_header.m_od_index & (0xFFFF >> (16 - 13)));
  ((uint32_t*)data)[0] |= msg->m_header.m_client_id << 13;
  ((uint32_t*)data)[0] |= msg->m_header.m_server_id << 21;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_header.m_erno & (0xFF >> (8 - 1))) << 29;
}
static void canzero_serialize_canzero_message_power_board24_stream_state(canzero_message_power_board24_stream_state* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x44;
  frame->dlc = 1;
  ((uint32_t*)data)[0] = (uint8_t)(msg->m_state & (0xFF >> (8 - 1)));
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_sdc_status & (0xFF >> (8 - 1))) << 1;
}
static void canzero_serialize_canzero_message_power_board24_stream_temperature(canzero_message_power_board24_stream_temperature* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x4B;
  frame->dlc = 1;
  uint32_t mcu_temperature_0 = (msg->m_mcu_temperature - -1) / 0.592156862745098;
  if (mcu_temperature_0 > 0xFF) {
    mcu_temperature_0 = 0xFF;
  }
  ((uint32_t*)data)[0] = mcu_temperature_0;
}
static void canzero_serialize_canzero_message_power_board24_stream_errors(canzero_message_power_board24_stream_errors* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x6C;
  frame->dlc = 1;
  ((uint32_t*)data)[0] = (uint8_t)(msg->m_assertion_fault & (0xFF >> (8 - 1)));
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_error_level_mcu_temperature & (0xFF >> (8 - 2))) << 1;
}
static void canzero_serialize_canzero_message_power_board24_stream_channel_status(canzero_message_power_board24_stream_channel_status* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x4C;
  frame->dlc = 1;
  ((uint32_t*)data)[0] = (uint8_t)(msg->m_cooling_pump_channel_status & (0xFF >> (8 - 2)));
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_sdc_power_channel_status & (0xFF >> (8 - 2))) << 2;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_sdc_signal_channel_status & (0xFF >> (8 - 2))) << 4;
  ((uint32_t*)data)[0] |= (uint8_t)(msg->m_fan_channel_status & (0xFF >> (8 - 2))) << 6;
}
static void canzero_serialize_canzero_message_power_board24_stream_channel_currents(canzero_message_power_board24_stream_channel_currents* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0x6D;
  frame->dlc = 8;
  uint32_t cooling_pump_channel_current_0 = (msg->m_cooling_pump_channel_current - 0) / 0.00015259021896696422;
  if (cooling_pump_channel_current_0 > 0xFFFF) {
    cooling_pump_channel_current_0 = 0xFFFF;
  }
  ((uint32_t*)data)[0] = cooling_pump_channel_current_0;
  uint32_t sdc_power_channel_current_16 = (msg->m_sdc_power_channel_current - 0) / 0.00015259021896696422;
  if (sdc_power_channel_current_16 > 0xFFFF) {
    sdc_power_channel_current_16 = 0xFFFF;
  }
  ((uint32_t*)data)[0] |= sdc_power_channel_current_16 << 16;
  uint32_t sdc_signal_channel_current_32 = (msg->m_sdc_signal_channel_current - 0) / 0.00007629510948348211;
  if (sdc_signal_channel_current_32 > 0xFFFF) {
    sdc_signal_channel_current_32 = 0xFFFF;
  }
  ((uint32_t*)data)[1] = sdc_signal_channel_current_32;
  uint32_t fan_channel_current_48 = (msg->m_fan_channel_current - 0) / 0.00007629510948348211;
  if (fan_channel_current_48 > 0xFFFF) {
    fan_channel_current_48 = 0xFFFF;
  }
  ((uint32_t*)data)[1] |= fan_channel_current_48 << 16;
}
static void canzero_serialize_canzero_message_heartbeat(canzero_message_heartbeat* msg, canzero_frame* frame) {
  uint8_t* data = frame->data;
  frame->id = 0xDF;
  frame->dlc = 1;
  ((uint32_t*)data)[0] = (uint8_t)(msg->m_node_id & (0xFF >> (8 - 4)));
}
static void canzero_deserialize_canzero_message_get_req(canzero_frame* frame, canzero_message_get_req* msg) {
  uint8_t* data = frame->data;
  msg->m_header.m_od_index = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 13)));
  msg->m_header.m_client_id = ((((uint32_t*)data)[0] >> 13) & (0xFFFFFFFF >> (32 - 8)));
  msg->m_header.m_server_id = ((((uint32_t*)data)[0] >> 21) & (0xFFFFFFFF >> (32 - 8)));
}
static void canzero_deserialize_canzero_message_set_req(canzero_frame* frame, canzero_message_set_req* msg) {
  uint8_t* data = frame->data;
  msg->m_header.m_sof = (((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 1)));
  msg->m_header.m_eof = ((((uint32_t*)data)[0] >> 1) & (0xFFFFFFFF >> (32 - 1)));
  msg->m_header.m_toggle = ((((uint32_t*)data)[0] >> 2) & (0xFFFFFFFF >> (32 - 1)));
  msg->m_header.m_od_index = ((((uint32_t*)data)[0] >> 3) & (0xFFFFFFFF >> (32 - 13)));
  msg->m_header.m_client_id = ((((uint32_t*)data)[0] >> 16) & (0xFFFFFFFF >> (32 - 8)));
  msg->m_header.m_server_id = ((((uint32_t*)data)[0] >> 24) & (0xFFFFFFFF >> (32 - 8)));
  msg->m_data = (((uint32_t*)data)[1] & (0xFFFFFFFF >> (32 - 32)));
}
static void canzero_deserialize_canzero_message_heartbeat(canzero_frame* frame, canzero_message_heartbeat* msg) {
  uint8_t* data = frame->data;
  msg->m_node_id = (node_id)(((uint32_t*)data)[0] & (0xFFFFFFFF >> (32 - 4)));
}

__attribute__((weak)) void canzero_wdg_timeout(uint8_t node_id) {}

typedef enum {
  HEARTBEAT_JOB_TAG = 0,
  HEARTBEAT_WDG_JOB_TAG = 1,
  GET_RESP_FRAGMENTATION_JOB_TAG = 2,
  STREAM_INTERVAL_JOB_TAG = 3,
} job_tag;

typedef struct {
  uint32_t *buffer;
  uint8_t offset;
  uint8_t size;
  uint8_t od_index;
  uint8_t client_id;
} get_resp_fragmentation_job;

typedef struct {
  uint32_t last_schedule; 
  uint32_t stream_id;
} stream_interval_job;

#define MAX_DYN_HEARTBEATS 10
typedef struct {
  unsigned int static_wdg_armed[node_id_count];
  unsigned int static_tick_counters[node_id_count];
  unsigned int dynamic_wdg_armed[MAX_DYN_HEARTBEATS];
  unsigned int dynamic_tick_counters[MAX_DYN_HEARTBEATS];
} heartbeat_wdg_job_t;

typedef struct {
  uint32_t climax;
  uint32_t position;
  job_tag tag;
  union {
    get_resp_fragmentation_job get_fragmentation_job;
    stream_interval_job stream_job;
    heartbeat_wdg_job_t wdg_job;
  } job;
} job_t;

union job_pool_allocator_entry {
  job_t job;
  union job_pool_allocator_entry *next;
};

typedef struct {
  union job_pool_allocator_entry job[64];
  union job_pool_allocator_entry *freelist;
} job_pool_allocator;

static job_pool_allocator job_allocator;
static void job_pool_allocator_init() {
  for (uint8_t i = 1; i < 64; i++) {
    job_allocator.job[i - 1].next = job_allocator.job + i;
  }
  job_allocator.job[64 - 1].next = NULL;
  job_allocator.freelist = job_allocator.job;
}

static job_t *job_pool_allocator_alloc() {
  if (job_allocator.freelist != NULL) {
    job_t *job = &job_allocator.freelist->job;
    job_allocator.freelist = job_allocator.freelist->next;
    return job;
  } else {
    return NULL;
  }
}
static void job_pool_allocator_free(job_t *job) {
  union job_pool_allocator_entry *entry = (union job_pool_allocator_entry *)job;
  entry->next = job_allocator.freelist;
  job_allocator.freelist = entry;
}
#define SCHEDULE_HEAP_SIZE 256
typedef struct {
  job_t *heap[SCHEDULE_HEAP_SIZE]; // job**
  uint32_t size;
} job_scheduler_t;
static job_scheduler_t scheduler;
static void scheduler_promote_job(job_t *job) {
  int index = job->position;
  if (index == 0) {
    return;
  }
  int parent = (job->position - 1) / 2;
  while (scheduler.heap[parent]->climax > scheduler.heap[index]->climax) {
    job_t *tmp = scheduler.heap[parent];
    scheduler.heap[parent] = scheduler.heap[index];
    scheduler.heap[index] = tmp;
    scheduler.heap[parent]->position = parent;
    scheduler.heap[index]->position = index;
    index = parent;
    parent = (index - 1) / 2;
  }
  if (index == 0) {
    canzero_request_update(job->climax);
  }
}
static void scheduler_schedule(job_t *job) {
  if (scheduler.size >= SCHEDULE_HEAP_SIZE) {
    return;
  }
  job->position = scheduler.size;
  scheduler.heap[scheduler.size] = job;
  scheduler.size += 1;
  scheduler_promote_job(job);
}
static int scheduler_continue(job_t **job, uint32_t time) {
  *job = scheduler.heap[0];
  return scheduler.heap[0]->climax <= time;
}
static void scheduler_reschedule(uint32_t climax) {
  job_t *job = scheduler.heap[0];
  job->climax = climax;
  int index = 0;
  int hsize = scheduler.size / 2;
  while (index < hsize) {
    int left = index * 2 + 1;
    int right = left + 1;
    int min;
    if (right < scheduler.size &&
        scheduler.heap[left]->climax >= scheduler.heap[right]->climax) {
      min = right;
    } else {
    min = left;
    }
    if (climax <= scheduler.heap[min]->climax) {
      break;
    }
    scheduler.heap[index] = scheduler.heap[min];
    scheduler.heap[index]->position = index;
    index = min;
  }
  scheduler.heap[index] = job;
  scheduler.heap[index]->position = index;
}
static void scheduler_unschedule() {
  scheduler.heap[0] = scheduler.heap[scheduler.size - 1];
  scheduler.heap[0]->position = 0;
  scheduler.size -= 1;
  scheduler_reschedule(scheduler.heap[0]->climax);
}
static const uint32_t get_resp_fragmentation_interval = 10;
static void schedule_get_resp_fragmentation_job(uint32_t *fragmentation_buffer, uint8_t size, uint8_t od_index, uint8_t client_id) {
  job_t *fragmentation_job = job_pool_allocator_alloc();
  fragmentation_job->climax = canzero_get_time() + get_resp_fragmentation_interval;
  fragmentation_job->tag = GET_RESP_FRAGMENTATION_JOB_TAG;
  fragmentation_job->job.get_fragmentation_job.buffer = fragmentation_buffer;
  fragmentation_job->job.get_fragmentation_job.offset = 1;
  fragmentation_job->job.get_fragmentation_job.size = size;
  fragmentation_job->job.get_fragmentation_job.od_index = od_index;
  fragmentation_job->job.get_fragmentation_job.client_id = client_id;
  scheduler_schedule(fragmentation_job);
}

static job_t heartbeat_job;
static const uint32_t heartbeat_interval = 100;
static void schedule_heartbeat_job() {
  heartbeat_job.climax = canzero_get_time();
  heartbeat_job.tag = HEARTBEAT_JOB_TAG;
  scheduler_schedule(&heartbeat_job);
}

static job_t heartbeat_wdg_job;
static const uint32_t heartbeat_wdg_tick_duration = 50;
static void schedule_heartbeat_wdg_job() {
  heartbeat_wdg_job.climax = canzero_get_time() + 100;
  heartbeat_wdg_job.tag = HEARTBEAT_WDG_JOB_TAG;
  for (unsigned int i = 0; i < node_id_count; ++i) {
    heartbeat_wdg_job.job.wdg_job.static_tick_counters[i] = 0;
    heartbeat_wdg_job.job.wdg_job.static_wdg_armed[i] = 0;
  }
  for (unsigned int i = 0; i < MAX_DYN_HEARTBEATS; ++i) {
    heartbeat_wdg_job.job.wdg_job.dynamic_tick_counters[i] = 0;
    heartbeat_wdg_job.job.wdg_job.dynamic_wdg_armed[i] = 0;
  }
  scheduler_schedule(&heartbeat_wdg_job);
}

static job_t state_interval_job;
static const uint32_t state_interval = 0;
static void schedule_state_interval_job(){
  uint32_t time = canzero_get_time();
  state_interval_job.climax = time + state_interval;
  state_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  state_interval_job.job.stream_job.stream_id = 0;
  state_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&state_interval_job);
}
static job_t temperature_interval_job;
static const uint32_t temperature_interval = 500;
static void schedule_temperature_interval_job(){
  uint32_t time = canzero_get_time();
  temperature_interval_job.climax = time + temperature_interval;
  temperature_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  temperature_interval_job.job.stream_job.stream_id = 1;
  temperature_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&temperature_interval_job);
}
static job_t errors_interval_job;
static const uint32_t errors_interval = 0;
static void schedule_errors_interval_job(){
  uint32_t time = canzero_get_time();
  errors_interval_job.climax = time + errors_interval;
  errors_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  errors_interval_job.job.stream_job.stream_id = 2;
  errors_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&errors_interval_job);
}
static job_t channel_status_interval_job;
static const uint32_t channel_status_interval = 0;
static void schedule_channel_status_interval_job(){
  uint32_t time = canzero_get_time();
  channel_status_interval_job.climax = time + channel_status_interval;
  channel_status_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  channel_status_interval_job.job.stream_job.stream_id = 3;
  channel_status_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&channel_status_interval_job);
}
static job_t channel_currents_interval_job;
static const uint32_t channel_currents_interval = 100;
static void schedule_channel_currents_interval_job(){
  uint32_t time = canzero_get_time();
  channel_currents_interval_job.climax = time + channel_currents_interval;
  channel_currents_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  channel_currents_interval_job.job.stream_job.stream_id = 4;
  channel_currents_interval_job.job.stream_job.last_schedule = time;
  scheduler_schedule(&channel_currents_interval_job);
}

static void schedule_jobs(uint32_t time) {
  for (uint8_t i = 0; i < 100; ++i) {
    canzero_enter_critical();
    job_t *job;
    if (!scheduler_continue(&job, time)) {
      canzero_exit_critical();
      return;
    }
    switch (job->tag) {
      case STREAM_INTERVAL_JOB_TAG: {
        switch (job->job.stream_job.stream_id) {
      case 0: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 500);
        canzero_exit_critical();
        canzero_message_power_board24_stream_state stream_message;
        stream_message.m_state = __oe_state;
        stream_message.m_sdc_status = __oe_sdc_status;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_power_board24_stream_state(&stream_message, &stream_frame);
        canzero_can1_send(&stream_frame);
        break;
      }
      case 1: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 500);
        canzero_exit_critical();
        canzero_message_power_board24_stream_temperature stream_message;
        stream_message.m_mcu_temperature = __oe_mcu_temperature;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_power_board24_stream_temperature(&stream_message, &stream_frame);
        canzero_can1_send(&stream_frame);
        break;
      }
      case 2: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 1000);
        canzero_exit_critical();
        canzero_message_power_board24_stream_errors stream_message;
        stream_message.m_assertion_fault = __oe_assertion_fault;
        stream_message.m_error_level_mcu_temperature = __oe_error_level_mcu_temperature;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_power_board24_stream_errors(&stream_message, &stream_frame);
        canzero_can0_send(&stream_frame);
        break;
      }
      case 3: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 500);
        canzero_exit_critical();
        canzero_message_power_board24_stream_channel_status stream_message;
        stream_message.m_cooling_pump_channel_status = __oe_cooling_pump_channel_status;
        stream_message.m_sdc_power_channel_status = __oe_sdc_power_channel_status;
        stream_message.m_sdc_signal_channel_status = __oe_sdc_signal_channel_status;
        stream_message.m_fan_channel_status = __oe_fan_channel_status;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_power_board24_stream_channel_status(&stream_message, &stream_frame);
        canzero_can0_send(&stream_frame);
        break;
      }
      case 4: {
        job->job.stream_job.last_schedule = time;
        scheduler_reschedule(time + 100);
        canzero_exit_critical();
        canzero_message_power_board24_stream_channel_currents stream_message;
        stream_message.m_cooling_pump_channel_current = __oe_cooling_pump_channel_current;
        stream_message.m_sdc_power_channel_current = __oe_sdc_power_channel_current;
        stream_message.m_sdc_signal_channel_current = __oe_sdc_signal_channel_current;
        stream_message.m_fan_channel_current = __oe_fan_channel_current;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_power_board24_stream_channel_currents(&stream_message, &stream_frame);
        canzero_can1_send(&stream_frame);
        break;
      }
        default:
          canzero_exit_critical();
          break;
        }
        break;
      }
      case HEARTBEAT_JOB_TAG: {
        scheduler_reschedule(time + heartbeat_interval);
        canzero_exit_critical();
        canzero_message_heartbeat heartbeat;
        heartbeat.m_node_id = node_id_power_board24;
        canzero_frame heartbeat_frame;
        canzero_serialize_canzero_message_heartbeat(&heartbeat, &heartbeat_frame);
        canzero_can1_send(&heartbeat_frame);
        break;
      }
      case HEARTBEAT_WDG_JOB_TAG: {
        scheduler_reschedule(time + heartbeat_wdg_tick_duration);
        canzero_exit_critical();
        for (unsigned int i = 0; i < node_id_count; ++i) {
          heartbeat_wdg_job.job.wdg_job.static_tick_counters[i] 
            += heartbeat_wdg_job.job.wdg_job.static_wdg_armed[i];
        }
        for (unsigned int i = 0; i < MAX_DYN_HEARTBEATS; ++i) {
          heartbeat_wdg_job.job.wdg_job.dynamic_tick_counters[i] 
            += heartbeat_wdg_job.job.wdg_job.dynamic_wdg_armed[i];
        }
        for (unsigned int i = 0; i < node_id_count; ++i) {
          if (heartbeat_wdg_job.job.wdg_job.static_tick_counters[i] >= 4) {
            canzero_wdg_timeout(i);
          }
        }
        for (unsigned int i = 0; i < MAX_DYN_HEARTBEATS; ++i) {
          if (heartbeat_wdg_job.job.wdg_job.dynamic_tick_counters[i] >= 4) {
            canzero_wdg_timeout(node_id_count + i);
          }
        }
        break;
      }
      case GET_RESP_FRAGMENTATION_JOB_TAG: {
        get_resp_fragmentation_job *fragmentation_job = &job->job.get_fragmentation_job;
        canzero_message_get_resp fragmentation_response;
        fragmentation_response.m_header.m_sof = 0;
        fragmentation_response.m_header.m_toggle = fragmentation_job->offset % 2;
        fragmentation_response.m_header.m_od_index = fragmentation_job->od_index;
        fragmentation_response.m_header.m_client_id = fragmentation_job->client_id;
        fragmentation_response.m_header.m_server_id = 0xC;
        fragmentation_response.m_data = fragmentation_job->buffer[fragmentation_job->offset];
        fragmentation_job->offset += 1;
        if (fragmentation_job->offset == fragmentation_job->size) {
          fragmentation_response.m_header.m_eof = 1;
          scheduler_unschedule();
        } else {
          fragmentation_response.m_header.m_eof = 0;
          scheduler_reschedule(time + get_resp_fragmentation_interval);
        }
        canzero_exit_critical();
        canzero_frame fragmentation_frame;
        canzero_serialize_canzero_message_get_resp(&fragmentation_response, &fragmentation_frame);
        canzero_can1_send(&fragmentation_frame);
        break;
      }
      default: {
        canzero_exit_critical();
        break;
      }
    }
  }
}

static uint32_t scheduler_next_job_timeout() {
  return scheduler.heap[0]->climax;
}

static uint32_t __oe_config_hash_rx_fragmentation_buffer[2];
static uint32_t __oe_build_time_rx_fragmentation_buffer[2];
static uint32_t __oe_loop_frequency_rx_fragmentation_buffer[2];
static uint32_t __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[7];
static void canzero_handle_get_req(canzero_frame* frame) {
  canzero_message_get_req msg;
  canzero_deserialize_canzero_message_get_req(frame, &msg);
  if (msg.m_header.m_server_id != 12) {
    return;
  }
  canzero_message_get_resp resp{};
  switch (msg.m_header.m_od_index) {
  case 0: {
    {
      uint64_t masked = (__oe_config_hash & (0xFFFFFFFFFFFFFFFF >> (64 - 64)));
      __oe_config_hash_rx_fragmentation_buffer[0] = ((uint32_t*)&masked)[0];
      __oe_config_hash_rx_fragmentation_buffer[1] = ((uint32_t*)&masked)[1];
    }
    resp.m_data = __oe_config_hash_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_config_hash_rx_fragmentation_buffer, 2, 0, msg.m_header.m_client_id);
    break;
  }
  case 1: {
    __oe_build_time_rx_fragmentation_buffer[0] = (__oe_build_time.m_year & (0xFFFFFFFF >> (32 - 16)));
    __oe_build_time_rx_fragmentation_buffer[0] |= ((__oe_build_time.m_month & (0xFFFFFFFF >> (32 - 8))) << 16);
    __oe_build_time_rx_fragmentation_buffer[0] |= ((__oe_build_time.m_day & (0xFFFFFFFF >> (32 - 8))) << 24);
    __oe_build_time_rx_fragmentation_buffer[1] = (__oe_build_time.m_hour & (0xFFFFFFFF >> (32 - 8)));
    __oe_build_time_rx_fragmentation_buffer[1] |= ((__oe_build_time.m_min & (0xFFFFFFFF >> (32 - 8))) << 8);
    __oe_build_time_rx_fragmentation_buffer[1] |= ((__oe_build_time.m_sec & (0xFFFFFFFF >> (32 - 8))) << 16);

    resp.m_data = __oe_build_time_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_build_time_rx_fragmentation_buffer, 2, 1, msg.m_header.m_client_id);
    break;
  }
  case 2: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_state) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 3: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_sdc_status) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 4: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_assertion_fault) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 5: {
    {
      uint64_t masked = (min_u64((__oe_loop_frequency - ((double)0)) / (double)0.00000000005421010862427522, 0xFFFFFFFFFFFFFFFFull) & (0xFFFFFFFFFFFFFFFF >> (64 - 64)));
      __oe_loop_frequency_rx_fragmentation_buffer[0] = ((uint32_t*)&masked)[0];
      __oe_loop_frequency_rx_fragmentation_buffer[1] = ((uint32_t*)&masked)[1];
    }
    resp.m_data = __oe_loop_frequency_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_loop_frequency_rx_fragmentation_buffer, 2, 5, msg.m_header.m_client_id);
    break;
  }
  case 6: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_cooling_pump_channel_ctrl) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 7: {
    resp.m_data |= min_u32((__oe_cooling_pump_channel_current - (0)) / 0.00015259021896696422, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 8: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_cooling_pump_channel_status) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 9: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_sdc_power_channel_ctrl) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 10: {
    resp.m_data |= min_u32((__oe_sdc_power_channel_current - (0)) / 0.00015259021896696422, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 11: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_sdc_power_channel_status) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 12: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_sdc_signal_channel_ctrl) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 13: {
    resp.m_data |= min_u32((__oe_sdc_signal_channel_current - (0)) / 0.00007629510948348211, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 14: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_sdc_signal_channel_status) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 15: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_fan_channel_ctrl) & (0xFF >> (8 - 1)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 16: {
    resp.m_data |= min_u32((__oe_fan_channel_current - (0)) / 0.00007629510948348211, 0xFFFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 17: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_fan_channel_status) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 18: {
    resp.m_data |= min_u32((__oe_mcu_temperature - (-1)) / 0.592156862745098, 0xFF) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 19: {
    resp.m_data |= ((uint32_t)(((uint8_t)__oe_error_level_mcu_temperature) & (0xFF >> (8 - 2)))) << 0;
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 1;
    resp.m_header.m_toggle = 0;
    break;
  }
  case 20: {
    __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[0] = (__oe_error_level_config_mcu_temperature.m_ignore_info & (0xFFFFFFFF >> (32 - 1)));
    {
    uint32_t masked = (min_u32((__oe_error_level_config_mcu_temperature.m_info_thresh - ((float)-10000)) / (float)0.000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
      __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[0] |= (masked << 1);
      __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[1] = (masked >> 31);
    }
    {
    uint32_t masked = (min_u32((__oe_error_level_config_mcu_temperature.m_info_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
      __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[1] |= (masked << 1);
      __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[2] = (masked >> 31);
    }
    __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[2] |= ((__oe_error_level_config_mcu_temperature.m_ignore_warning & (0xFFFFFFFF >> (32 - 1))) << 1);
    {
    uint32_t masked = (min_u32((__oe_error_level_config_mcu_temperature.m_warning_thresh - ((float)-10000)) / (float)0.000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
      __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[2] |= (masked << 2);
      __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[3] = (masked >> 30);
    }
    {
    uint32_t masked = (min_u32((__oe_error_level_config_mcu_temperature.m_warning_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
      __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[3] |= (masked << 2);
      __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[4] = (masked >> 30);
    }
    __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[4] |= ((__oe_error_level_config_mcu_temperature.m_ignore_error & (0xFFFFFFFF >> (32 - 1))) << 2);
    {
    uint32_t masked = (min_u32((__oe_error_level_config_mcu_temperature.m_error_thresh - ((float)-10000)) / (float)0.000004656612874161595, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
      __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[4] |= (masked << 3);
      __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[5] = (masked >> 29);
    }
    {
    uint32_t masked = (min_u32((__oe_error_level_config_mcu_temperature.m_error_timeout - ((float)0)) / (float)0.000000013969838622484784, 0xFFFFFFFFul) & (0xFFFFFFFF >> (32 - 32)));
      __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[5] |= (masked << 3);
      __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[6] = (masked >> 29);
    }

    resp.m_data = __oe_error_level_config_mcu_temperature_rx_fragmentation_buffer[0];
    resp.m_header.m_sof = 1;
    resp.m_header.m_eof = 0;
    resp.m_header.m_toggle = 0;
    schedule_get_resp_fragmentation_job(__oe_error_level_config_mcu_temperature_rx_fragmentation_buffer, 7, 20, msg.m_header.m_client_id);
    break;
  }
  }
  resp.m_header.m_od_index = msg.m_header.m_od_index;
  resp.m_header.m_client_id = msg.m_header.m_client_id;
  resp.m_header.m_server_id = msg.m_header.m_server_id;
  canzero_frame resp_frame;
  canzero_serialize_canzero_message_get_resp(&resp, &resp_frame);
  canzero_can1_send(&resp_frame);
}
static uint32_t config_hash_tmp_tx_fragmentation_buffer[2];
static uint32_t config_hash_tmp_tx_fragmentation_offset = 0;
static uint32_t build_time_tmp_tx_fragmentation_buffer[2];
static uint32_t build_time_tmp_tx_fragmentation_offset = 0;
static uint32_t loop_frequency_tmp_tx_fragmentation_buffer[2];
static uint32_t loop_frequency_tmp_tx_fragmentation_offset = 0;
static uint32_t error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[7];
static uint32_t error_level_config_mcu_temperature_tmp_tx_fragmentation_offset = 0;
static void canzero_handle_set_req(canzero_frame* frame) {
  canzero_message_set_req msg;
  canzero_deserialize_canzero_message_set_req(frame, &msg);
  if (msg.m_header.m_server_id != 12) {
    return;
  }
  canzero_message_set_resp resp{};
  switch (msg.m_header.m_od_index) {
  case 0 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      config_hash_tmp_tx_fragmentation_offset = 0;
    }else {
      config_hash_tmp_tx_fragmentation_offset += 1;
      if (config_hash_tmp_tx_fragmentation_offset >= 2) {
        return;
      }
    }
    config_hash_tmp_tx_fragmentation_buffer[config_hash_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    uint64_t config_hash_tmp;
    config_hash_tmp = (uint64_t)config_hash_tmp_tx_fragmentation_buffer[0] | (((uint64_t)(config_hash_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 32)))) << 32);
    canzero_set_config_hash(config_hash_tmp);
    break;
  }
  case 1 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      build_time_tmp_tx_fragmentation_offset = 0;
    }else {
      build_time_tmp_tx_fragmentation_offset += 1;
      if (build_time_tmp_tx_fragmentation_offset >= 2) {
        return;
      }
    }
    build_time_tmp_tx_fragmentation_buffer[build_time_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    date_time build_time_tmp;
    build_time_tmp.m_year = (build_time_tmp_tx_fragmentation_buffer[0] & (0xFFFFFFFF >> (32 - 16)));
    build_time_tmp.m_month = (build_time_tmp_tx_fragmentation_buffer[0] >> 16) & (0xFFFFFFFF >> (32 - 8));
    build_time_tmp.m_day = (build_time_tmp_tx_fragmentation_buffer[0] >> 24) & (0xFFFFFFFF >> (32 - 8));
    build_time_tmp.m_hour = (build_time_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 8)));
    build_time_tmp.m_min = (build_time_tmp_tx_fragmentation_buffer[1] >> 8) & (0xFFFFFFFF >> (32 - 8));
    build_time_tmp.m_sec = (build_time_tmp_tx_fragmentation_buffer[1] >> 16) & (0xFFFFFFFF >> (32 - 8));
    canzero_set_build_time(build_time_tmp);
    break;
  }
  case 2 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    pdu_state state_tmp;
    state_tmp = ((pdu_state)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_state(state_tmp);
    break;
  }
  case 3 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    sdc_status sdc_status_tmp;
    sdc_status_tmp = ((sdc_status)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_sdc_status(sdc_status_tmp);
    break;
  }
  case 4 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_flag assertion_fault_tmp;
    assertion_fault_tmp = ((error_flag)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_assertion_fault(assertion_fault_tmp);
    break;
  }
  case 5 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      loop_frequency_tmp_tx_fragmentation_offset = 0;
    }else {
      loop_frequency_tmp_tx_fragmentation_offset += 1;
      if (loop_frequency_tmp_tx_fragmentation_offset >= 2) {
        return;
      }
    }
    loop_frequency_tmp_tx_fragmentation_buffer[loop_frequency_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    double loop_frequency_tmp;
    loop_frequency_tmp = ((uint64_t)loop_frequency_tmp_tx_fragmentation_buffer[0] | (((uint64_t)(loop_frequency_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 32)))) << 32)) * 0.00000000005421010862427522 + 0;
    canzero_set_loop_frequency(loop_frequency_tmp);
    break;
  }
  case 6 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    pdu_channel_control cooling_pump_channel_ctrl_tmp;
    cooling_pump_channel_ctrl_tmp = ((pdu_channel_control)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_cooling_pump_channel_ctrl(cooling_pump_channel_ctrl_tmp);
    break;
  }
  case 7 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float cooling_pump_channel_current_tmp;
    cooling_pump_channel_current_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.00015259021896696422 + 0);
    canzero_set_cooling_pump_channel_current(cooling_pump_channel_current_tmp);
    break;
  }
  case 8 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    pdu_channel_status cooling_pump_channel_status_tmp;
    cooling_pump_channel_status_tmp = ((pdu_channel_status)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_cooling_pump_channel_status(cooling_pump_channel_status_tmp);
    break;
  }
  case 9 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    pdu_channel_control sdc_power_channel_ctrl_tmp;
    sdc_power_channel_ctrl_tmp = ((pdu_channel_control)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_sdc_power_channel_ctrl(sdc_power_channel_ctrl_tmp);
    break;
  }
  case 10 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float sdc_power_channel_current_tmp;
    sdc_power_channel_current_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.00015259021896696422 + 0);
    canzero_set_sdc_power_channel_current(sdc_power_channel_current_tmp);
    break;
  }
  case 11 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    pdu_channel_status sdc_power_channel_status_tmp;
    sdc_power_channel_status_tmp = ((pdu_channel_status)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_sdc_power_channel_status(sdc_power_channel_status_tmp);
    break;
  }
  case 12 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    pdu_channel_control sdc_signal_channel_ctrl_tmp;
    sdc_signal_channel_ctrl_tmp = ((pdu_channel_control)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_sdc_signal_channel_ctrl(sdc_signal_channel_ctrl_tmp);
    break;
  }
  case 13 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float sdc_signal_channel_current_tmp;
    sdc_signal_channel_current_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.00007629510948348211 + 0);
    canzero_set_sdc_signal_channel_current(sdc_signal_channel_current_tmp);
    break;
  }
  case 14 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    pdu_channel_status sdc_signal_channel_status_tmp;
    sdc_signal_channel_status_tmp = ((pdu_channel_status)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_sdc_signal_channel_status(sdc_signal_channel_status_tmp);
    break;
  }
  case 15 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    pdu_channel_control fan_channel_ctrl_tmp;
    fan_channel_ctrl_tmp = ((pdu_channel_control)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_fan_channel_ctrl(fan_channel_ctrl_tmp);
    break;
  }
  case 16 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float fan_channel_current_tmp;
    fan_channel_current_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 16))) * 0.00007629510948348211 + 0);
    canzero_set_fan_channel_current(fan_channel_current_tmp);
    break;
  }
  case 17 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    pdu_channel_status fan_channel_status_tmp;
    fan_channel_status_tmp = ((pdu_channel_status)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_fan_channel_status(fan_channel_status_tmp);
    break;
  }
  case 18 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    float mcu_temperature_tmp;
    mcu_temperature_tmp = (float)(((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 8))) * 0.592156862745098 + -1);
    canzero_set_mcu_temperature(mcu_temperature_tmp);
    break;
  }
  case 19 : {
    if (msg.m_header.m_sof != 1 || msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 1) {
      return;
    }
    error_level error_level_mcu_temperature_tmp;
    error_level_mcu_temperature_tmp = ((error_level)((msg.m_data >> 0) & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_error_level_mcu_temperature(error_level_mcu_temperature_tmp);
    break;
  }
  case 20 : {
    if (msg.m_header.m_sof == 1) {
      if (msg.m_header.m_toggle != 0 || msg.m_header.m_eof != 0) {
        return; //TODO proper error response frame!
      }
      error_level_config_mcu_temperature_tmp_tx_fragmentation_offset = 0;
    }else {
      error_level_config_mcu_temperature_tmp_tx_fragmentation_offset += 1;
      if (error_level_config_mcu_temperature_tmp_tx_fragmentation_offset >= 7) {
        return;
      }
    }
    error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[error_level_config_mcu_temperature_tmp_tx_fragmentation_offset] = msg.m_data;
    if (msg.m_header.m_eof == 0) {
      return;
    }
    error_level_config error_level_config_mcu_temperature_tmp;
    error_level_config_mcu_temperature_tmp.m_ignore_info = ((bool_t)((error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[0] & (0xFFFFFFFF >> (32 - 1)))));
    error_level_config_mcu_temperature_tmp.m_info_thresh = ((uint64_t)(error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[0] >> 1) | ((uint64_t)(error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[1] & (0xFFFFFFFF >> (32 - 1))) << 31)) * 0.000004656612874161595 + -10000;
    error_level_config_mcu_temperature_tmp.m_info_timeout = ((uint64_t)(error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[1] >> 1) | ((uint64_t)(error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[2] & (0xFFFFFFFF >> (32 - 1))) << 31)) * 0.000000013969838622484784 + 0;
    error_level_config_mcu_temperature_tmp.m_ignore_warning = ((bool_t)((error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[2] >> 1) & (0xFFFFFFFF >> (32 - 1))));
    error_level_config_mcu_temperature_tmp.m_warning_thresh = ((uint64_t)(error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[2] >> 2) | ((uint64_t)(error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[3] & (0xFFFFFFFF >> (32 - 2))) << 30)) * 0.000004656612874161595 + -10000;
    error_level_config_mcu_temperature_tmp.m_warning_timeout = ((uint64_t)(error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[3] >> 2) | ((uint64_t)(error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[4] & (0xFFFFFFFF >> (32 - 2))) << 30)) * 0.000000013969838622484784 + 0;
    error_level_config_mcu_temperature_tmp.m_ignore_error = ((bool_t)((error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[4] >> 2) & (0xFFFFFFFF >> (32 - 1))));
    error_level_config_mcu_temperature_tmp.m_error_thresh = ((uint64_t)(error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[4] >> 3) | ((uint64_t)(error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[5] & (0xFFFFFFFF >> (32 - 3))) << 29)) * 0.000004656612874161595 + -10000;
    error_level_config_mcu_temperature_tmp.m_error_timeout = ((uint64_t)(error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[5] >> 3) | ((uint64_t)(error_level_config_mcu_temperature_tmp_tx_fragmentation_buffer[6] & (0xFFFFFFFF >> (32 - 3))) << 29)) * 0.000000013969838622484784 + 0;
    canzero_set_error_level_config_mcu_temperature(error_level_config_mcu_temperature_tmp);
    break;
  }
  default:
    return;
  }
  resp.m_header.m_od_index = msg.m_header.m_od_index;
  resp.m_header.m_client_id = msg.m_header.m_client_id;
  resp.m_header.m_server_id = msg.m_header.m_server_id;
  resp.m_header.m_erno = set_resp_erno_Success;
  canzero_frame resp_frame;
  canzero_serialize_canzero_message_set_resp(&resp, &resp_frame);
  canzero_can0_send(&resp_frame);

}
__attribute__((weak)) void canzero_handle_heartbeat(canzero_frame* frame) {
  canzero_message_heartbeat msg;
  canzero_deserialize_canzero_message_heartbeat(frame, &msg);
}
void canzero_can0_poll() {
  canzero_frame frame;
  while (canzero_can0_recv(&frame)) {
    switch (frame.id) {
      case 0xBF:
        canzero_handle_set_req(&frame);
        break;
    }
  }
}
void canzero_can1_poll() {
  canzero_frame frame;
  while (canzero_can1_recv(&frame)) {
    switch (frame.id) {
      case 0x9F:
        canzero_handle_get_req(&frame);
        break;
      case 0xDF:
        canzero_handle_heartbeat(&frame);
        break;
    }
  }
}
uint32_t canzero_update_continue(uint32_t time){
  schedule_jobs(time);
  return scheduler_next_job_timeout();
}
#define COMPUTE_BUILD_YEAR \
    ( (__DATE__[ 7] - '0') * 1000 + \
        (__DATE__[ 8] - '0') *  100 + \
        (__DATE__[ 9] - '0') *   10 + \
        (__DATE__[10] - '0') \
    )
#define COMPUTE_BUILD_DAY \
    ( \
        ((__DATE__[4] >= '0') ? (__DATE__[4] - '0') * 10 : 0) + \
        (__DATE__[5] - '0') \
    )
#define BUILD_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_MONTH_IS_DEC (__DATE__[0] == 'D')
#define COMPUTE_BUILD_MONTH \
    ( \
        (BUILD_MONTH_IS_JAN) ?  1 : \
        (BUILD_MONTH_IS_FEB) ?  2 : \
        (BUILD_MONTH_IS_MAR) ?  3 : \
        (BUILD_MONTH_IS_APR) ?  4 : \
        (BUILD_MONTH_IS_MAY) ?  5 : \
        (BUILD_MONTH_IS_JUN) ?  6 : \
        (BUILD_MONTH_IS_JUL) ?  7 : \
        (BUILD_MONTH_IS_AUG) ?  8 : \
        (BUILD_MONTH_IS_SEP) ?  9 : \
        (BUILD_MONTH_IS_OCT) ? 10 : \
        (BUILD_MONTH_IS_NOV) ? 11 : \
        (BUILD_MONTH_IS_DEC) ? 12 : \
        /* error default */  99 \
    )
#define COMPUTE_BUILD_HOUR ((__TIME__[0] - '0') * 10 + __TIME__[1] - '0')
#define COMPUTE_BUILD_MIN  ((__TIME__[3] - '0') * 10 + __TIME__[4] - '0')
#define COMPUTE_BUILD_SEC  ((__TIME__[6] - '0') * 10 + __TIME__[7] - '0')
#define BUILD_DATE_IS_BAD (__DATE__[0] == '?')
#define BUILD_YEAR  ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_YEAR)
#define BUILD_MONTH ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_MONTH)
#define BUILD_DAY   ((BUILD_DATE_IS_BAD) ? 99 : COMPUTE_BUILD_DAY)
#define BUILD_TIME_IS_BAD (__TIME__[0] == '?')
#define BUILD_HOUR  ((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_HOUR)
#define BUILD_MIN   ((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_MIN)
#define BUILD_SEC   ((BUILD_TIME_IS_BAD) ? 99 :  COMPUTE_BUILD_SEC)
void canzero_init() {
  __oe_config_hash = 6037466839717168065ull;
  __oe_build_time = {
    .m_year = BUILD_YEAR,
    .m_month = BUILD_MONTH,
    .m_day = BUILD_DAY,
    .m_hour = BUILD_HOUR,
    .m_min = BUILD_MIN,
    .m_sec = BUILD_SEC
  };
  canzero_can0_setup(1000000, NULL, 0);
  canzero_can1_setup(1000000, NULL, 0);

  job_pool_allocator_init();
  scheduler.size = 0;
  schedule_heartbeat_job();
  schedule_state_interval_job();
  schedule_temperature_interval_job();
  schedule_errors_interval_job();
  schedule_channel_status_interval_job();
  schedule_channel_currents_interval_job();

}
void canzero_set_state(pdu_state value) {
  extern pdu_state __oe_state;
  if (__oe_state != value) {
    __oe_state = value;
    uint32_t time = canzero_get_time();
    if (state_interval_job.climax > state_interval_job.job.stream_job.last_schedule + 0) {
      state_interval_job.climax = state_interval_job.job.stream_job.last_schedule + 0;
      scheduler_promote_job(&state_interval_job);
    }
  }
}
void canzero_set_sdc_status(sdc_status value) {
  extern sdc_status __oe_sdc_status;
  if (__oe_sdc_status != value) {
    __oe_sdc_status = value;
    uint32_t time = canzero_get_time();
    if (state_interval_job.climax > state_interval_job.job.stream_job.last_schedule + 0) {
      state_interval_job.climax = state_interval_job.job.stream_job.last_schedule + 0;
      scheduler_promote_job(&state_interval_job);
    }
  }
}
void canzero_set_assertion_fault(error_flag value) {
  extern error_flag __oe_assertion_fault;
  if (__oe_assertion_fault != value) {
    __oe_assertion_fault = value;
    uint32_t time = canzero_get_time();
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 0) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 0;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}
void canzero_set_cooling_pump_channel_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_cooling_pump_channel_status;
  if (__oe_cooling_pump_channel_status != value) {
    __oe_cooling_pump_channel_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_sdc_power_channel_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_sdc_power_channel_status;
  if (__oe_sdc_power_channel_status != value) {
    __oe_sdc_power_channel_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_sdc_signal_channel_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_sdc_signal_channel_status;
  if (__oe_sdc_signal_channel_status != value) {
    __oe_sdc_signal_channel_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_fan_channel_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_fan_channel_status;
  if (__oe_fan_channel_status != value) {
    __oe_fan_channel_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_error_level_mcu_temperature(error_level value) {
  extern error_level __oe_error_level_mcu_temperature;
  if (__oe_error_level_mcu_temperature != value) {
    __oe_error_level_mcu_temperature = value;
    uint32_t time = canzero_get_time();
    if (errors_interval_job.climax > errors_interval_job.job.stream_job.last_schedule + 0) {
      errors_interval_job.climax = errors_interval_job.job.stream_job.last_schedule + 0;
      scheduler_promote_job(&errors_interval_job);
    }
  }
}