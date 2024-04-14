#include "canzero.h"
pdu_state __oe_state;
pdu_channel_control __oe_lp_channel1_control;
pdu_channel_status __oe_lp_channel1_status;
float __oe_lp_channel1_current;
pdu_channel_control __oe_lp_channel2_control;
pdu_channel_status __oe_lp_channel2_status;
float __oe_lp_channel2_current;
pdu_channel_control __oe_lp_channel3_control;
pdu_channel_status __oe_lp_channel3_status;
float __oe_lp_channel3_current;
pdu_channel_control __oe_lp_channel4_control;
pdu_channel_status __oe_lp_channel4_status;
float __oe_lp_channel4_current;
pdu_channel_control __oe_lp_channel5_control;
pdu_channel_status __oe_lp_channel5_status;
float __oe_lp_channel5_current;
pdu_channel_control __oe_lp_channel6_control;
pdu_channel_status __oe_lp_channel6_status;
float __oe_lp_channel6_current;
pdu_channel_control __oe_lp_channel7_control;
pdu_channel_status __oe_lp_channel7_status;
float __oe_lp_channel7_current;
pdu_channel_control __oe_lp_channel8_control;
pdu_channel_status __oe_lp_channel8_status;
float __oe_lp_channel8_current;
pdu_channel_control __oe_lp_channel9_control;
pdu_channel_status __oe_lp_channel9_status;
float __oe_lp_channel9_current;
pdu_channel_control __oe_lp_channel10_control;
pdu_channel_status __oe_lp_channel10_status;
float __oe_lp_channel10_current;
pdu_channel_control __oe_hp_channel1_control;
pdu_channel_status __oe_hp_channel1_status;
float __oe_hp_channel1_current;
pdu_channel_control __oe_hp_channel2_control;
pdu_channel_status __oe_hp_channel2_status;
float __oe_hp_channel2_current;
pdu_channel_control __oe_hp_channel3_control;
pdu_channel_status __oe_hp_channel3_status;
float __oe_hp_channel3_current;
pdu_channel_control __oe_hp_channel4_control;
pdu_channel_status __oe_hp_channel4_status;
float __oe_hp_channel4_current;
float __oe_total_current;
float __oe_power_estimation;
sdc_status __oe_sdc_status;
float __oe_mcu_temperature;
error_flag __oe_error_mcu_over_temperature;
error_flag __oe_warn_mcu_over_temperature;

typedef enum {
  HEARTBEAT_JOB_TAG = 0,
  GET_RESP_FRAGMENTATION_JOB_TAG = 1,
  STREAM_INTERVAL_JOB_TAG = 2,
} job_tag;
typedef struct {
  uint32_t *buffer;
  uint8_t offset;
  uint8_t size;
  uint8_t od_index;
  uint8_t server_id;
} get_resp_fragmentation_job;
typedef struct {
  uint32_t command_resp_msg_id;
  uint8_t bus_id;
} command_resp_timeout_job;
typedef struct {
  uint32_t last_schedule; 
  uint32_t stream_id;
} stream_interval_job;
typedef struct {
  uint32_t climax;
  uint32_t position;
  job_tag tag;
  union {
    get_resp_fragmentation_job get_fragmentation_job;
    stream_interval_job stream_interval_job;
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
static void schedule_get_resp_fragmentation_job(uint32_t *fragmentation_buffer, uint8_t size, uint8_t od_index, uint8_t server_id) {
  job_t *fragmentation_job = job_pool_allocator_alloc();
  fragmentation_job->climax = canzero_get_time() + get_resp_fragmentation_interval;
  fragmentation_job->tag = GET_RESP_FRAGMENTATION_JOB_TAG;
  fragmentation_job->job.get_fragmentation_job.buffer = fragmentation_buffer;
  fragmentation_job->job.get_fragmentation_job.offset = 1;
  fragmentation_job->job.get_fragmentation_job.size = size;
  fragmentation_job->job.get_fragmentation_job.od_index = od_index;
  fragmentation_job->job.get_fragmentation_job.server_id = server_id;
  scheduler_schedule(fragmentation_job);
}
static job_t heartbeat_job;
static const uint32_t heartbeat_interval = 100;
static void schedule_heartbeat_job() {
  heartbeat_job.climax = canzero_get_time() + heartbeat_interval;
  heartbeat_job.tag = HEARTBEAT_JOB_TAG;
  scheduler_schedule(&heartbeat_job);
}
static job_t channel_currents_1_interval_job;
static const uint32_t channel_currents_1_interval = 250;
static void schedule_channel_currents_1_interval_job(){
  uint32_t time = canzero_get_time();
  channel_currents_1_interval_job.climax = time + channel_currents_1_interval;
  channel_currents_1_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  channel_currents_1_interval_job.job.stream_interval_job.stream_id = 0;
  channel_currents_1_interval_job.job.stream_interval_job.last_schedule = time;
  scheduler_schedule(&channel_currents_1_interval_job);
}
static job_t channel_currents_2_interval_job;
static const uint32_t channel_currents_2_interval = 250;
static void schedule_channel_currents_2_interval_job(){
  uint32_t time = canzero_get_time();
  channel_currents_2_interval_job.climax = time + channel_currents_2_interval;
  channel_currents_2_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  channel_currents_2_interval_job.job.stream_interval_job.stream_id = 1;
  channel_currents_2_interval_job.job.stream_interval_job.last_schedule = time;
  scheduler_schedule(&channel_currents_2_interval_job);
}
static job_t power_estimation_interval_job;
static const uint32_t power_estimation_interval = 1000;
static void schedule_power_estimation_interval_job(){
  uint32_t time = canzero_get_time();
  power_estimation_interval_job.climax = time + power_estimation_interval;
  power_estimation_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  power_estimation_interval_job.job.stream_interval_job.stream_id = 2;
  power_estimation_interval_job.job.stream_interval_job.last_schedule = time;
  scheduler_schedule(&power_estimation_interval_job);
}
static job_t channel_status_interval_job;
static const uint32_t channel_status_interval = 0;
static void schedule_channel_status_interval_job(){
  uint32_t time = canzero_get_time();
  channel_status_interval_job.climax = time + channel_status_interval;
  channel_status_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  channel_status_interval_job.job.stream_interval_job.stream_id = 3;
  channel_status_interval_job.job.stream_interval_job.last_schedule = time;
  scheduler_schedule(&channel_status_interval_job);
}
static job_t state_interval_job;
static const uint32_t state_interval = 50;
static void schedule_state_interval_job(){
  uint32_t time = canzero_get_time();
  state_interval_job.climax = time + state_interval;
  state_interval_job.tag = STREAM_INTERVAL_JOB_TAG;
  state_interval_job.job.stream_interval_job.stream_id = 4;
  state_interval_job.job.stream_interval_job.last_schedule = time;
  scheduler_schedule(&state_interval_job);
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
      switch (job->job.stream_interval_job.stream_id) {
      case 0: {
        job->job.stream_interval_job.last_schedule = time;
        scheduler_reschedule(time + 250);
        canzero_exit_critical();
        canzero_message_pdu24_stream_channel_currents_1 stream_message;
        stream_message.lp_channel1_current = __oe_lp_channel1_current;
        stream_message.lp_channel2_current = __oe_lp_channel2_current;
        stream_message.lp_channel3_current = __oe_lp_channel3_current;
        stream_message.lp_channel4_current = __oe_lp_channel4_current;
        stream_message.lp_channel5_current = __oe_lp_channel5_current;
        stream_message.lp_channel6_current = __oe_lp_channel6_current;
        stream_message.lp_channel7_current = __oe_lp_channel7_current;
        stream_message.lp_channel8_current = __oe_lp_channel8_current;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_pdu24_stream_channel_currents_1(&stream_message, &stream_frame);
        canzero_can1_send(&stream_frame);
        break;
      }
      case 1: {
        job->job.stream_interval_job.last_schedule = time;
        scheduler_reschedule(time + 250);
        canzero_exit_critical();
        canzero_message_pdu24_stream_channel_currents_2 stream_message;
        stream_message.lp_channel9_current = __oe_lp_channel9_current;
        stream_message.lp_channel10_current = __oe_lp_channel10_current;
        stream_message.hp_channel1_current = __oe_hp_channel1_current;
        stream_message.hp_channel2_current = __oe_hp_channel2_current;
        stream_message.hp_channel3_current = __oe_hp_channel3_current;
        stream_message.hp_channel4_current = __oe_hp_channel4_current;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_pdu24_stream_channel_currents_2(&stream_message, &stream_frame);
        canzero_can1_send(&stream_frame);
        break;
      }
      case 2: {
        job->job.stream_interval_job.last_schedule = time;
        scheduler_reschedule(time + 1000);
        canzero_exit_critical();
        canzero_message_pdu24_stream_power_estimation stream_message;
        stream_message.total_current = __oe_total_current;
        stream_message.power_estimation = __oe_power_estimation;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_pdu24_stream_power_estimation(&stream_message, &stream_frame);
        canzero_can0_send(&stream_frame);
        break;
      }
      case 3: {
        job->job.stream_interval_job.last_schedule = time;
        scheduler_reschedule(time + 3000);
        canzero_exit_critical();
        canzero_message_pdu24_stream_channel_status stream_message;
        stream_message.lp_channel1_status = __oe_lp_channel1_status;
        stream_message.lp_channel2_status = __oe_lp_channel2_status;
        stream_message.lp_channel3_status = __oe_lp_channel3_status;
        stream_message.lp_channel4_status = __oe_lp_channel4_status;
        stream_message.lp_channel5_status = __oe_lp_channel5_status;
        stream_message.lp_channel6_status = __oe_lp_channel6_status;
        stream_message.lp_channel7_status = __oe_lp_channel7_status;
        stream_message.lp_channel8_status = __oe_lp_channel8_status;
        stream_message.lp_channel9_status = __oe_lp_channel9_status;
        stream_message.lp_channel10_status = __oe_lp_channel10_status;
        stream_message.hp_channel1_status = __oe_hp_channel1_status;
        stream_message.hp_channel2_status = __oe_hp_channel2_status;
        stream_message.hp_channel3_status = __oe_hp_channel3_status;
        stream_message.hp_channel4_status = __oe_hp_channel4_status;
        stream_message.sdc_status = __oe_sdc_status;
        stream_message.error_mcu_over_temperature = __oe_error_mcu_over_temperature;
        stream_message.warn_mcu_over_temperature = __oe_warn_mcu_over_temperature;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_pdu24_stream_channel_status(&stream_message, &stream_frame);
        canzero_can0_send(&stream_frame);
        break;
      }
      case 4: {
        job->job.stream_interval_job.last_schedule = time;
        scheduler_reschedule(time + 500);
        canzero_exit_critical();
        canzero_message_pdu24_stream_state stream_message;
        stream_message.state = __oe_state;
        canzero_frame stream_frame;
        canzero_serialize_canzero_message_pdu24_stream_state(&stream_message, &stream_frame);
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
      heartbeat.node_id = node_id_pdu24;
      canzero_frame heartbeat_frame;
      canzero_serialize_canzero_message_heartbeat(&heartbeat, &heartbeat_frame);
      canzero_can0_send(&heartbeat_frame);
      break;
    }
    case GET_RESP_FRAGMENTATION_JOB_TAG: {
      get_resp_fragmentation_job *fragmentation_job = &job->job.get_fragmentation_job;
      canzero_message_get_resp fragmentation_response;
      fragmentation_response.header.sof = 0;
      fragmentation_response.header.toggle = fragmentation_job->offset % 2;
      fragmentation_response.header.od_index = fragmentation_job->od_index;
      fragmentation_response.header.client_id = 0xA;
      fragmentation_response.header.server_id = fragmentation_job->server_id;
      fragmentation_response.data = fragmentation_job->buffer[fragmentation_job->offset];
      fragmentation_job->offset += 1;
      if (fragmentation_job->offset == fragmentation_job->size) {
        fragmentation_response.header.eof = 1;
        scheduler_unschedule();
      } else {
        fragmentation_response.header.eof = 0;
        scheduler_reschedule(time + get_resp_fragmentation_interval);
      }
      canzero_exit_critical();
      canzero_frame fragmentation_frame;
      canzero_serialize_canzero_message_get_resp(&fragmentation_response, &fragmentation_frame);
      canzero_can1_send(&fragmentation_frame);
      break;
    }
    default:
      canzero_exit_critical();
      break;
    }
  }
}
static uint32_t scheduler_next_job_timeout(){
  return scheduler.heap[0]->climax;
}
static void canzero_handle_get_req(canzero_frame* frame) {
  canzero_message_get_req msg;
  canzero_deserialize_canzero_message_get_req(frame, &msg);
  if (msg.header.server_id != 10) {
    return;
  }
  canzero_message_get_resp resp;
  switch (msg.header.od_index) {
  case 0: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_state) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 1: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel1_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 2: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel1_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 3: {
    resp.data |= ((uint32_t)((__oe_lp_channel1_current - (0)) / 0.011764705882352941)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 4: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel2_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 5: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel2_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 6: {
    resp.data |= ((uint32_t)((__oe_lp_channel2_current - (0)) / 0.011764705882352941)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 7: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel3_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 8: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel3_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 9: {
    resp.data |= ((uint32_t)((__oe_lp_channel3_current - (0)) / 0.011764705882352941)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 10: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel4_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 11: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel4_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 12: {
    resp.data |= ((uint32_t)((__oe_lp_channel4_current - (0)) / 0.011764705882352941)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 13: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel5_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 14: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel5_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 15: {
    resp.data |= ((uint32_t)((__oe_lp_channel5_current - (0)) / 0.011764705882352941)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 16: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel6_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 17: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel6_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 18: {
    resp.data |= ((uint32_t)((__oe_lp_channel6_current - (0)) / 0.011764705882352941)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 19: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel7_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 20: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel7_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 21: {
    resp.data |= ((uint32_t)((__oe_lp_channel7_current - (0)) / 0.011764705882352941)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 22: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel8_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 23: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel8_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 24: {
    resp.data |= ((uint32_t)((__oe_lp_channel8_current - (0)) / 0.011764705882352941)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 25: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel9_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 26: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel9_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 27: {
    resp.data |= ((uint32_t)((__oe_lp_channel9_current - (0)) / 0.011764705882352941)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 28: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel10_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 29: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_lp_channel10_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 30: {
    resp.data |= ((uint32_t)((__oe_lp_channel10_current - (0)) / 0.011764705882352941)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 31: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_hp_channel1_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 32: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_hp_channel1_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 33: {
    resp.data |= ((uint32_t)((__oe_hp_channel1_current - (0)) / 0.0392156862745098)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 34: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_hp_channel2_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 35: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_hp_channel2_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 36: {
    resp.data |= ((uint32_t)((__oe_hp_channel2_current - (0)) / 0.0392156862745098)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 37: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_hp_channel3_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 38: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_hp_channel3_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 39: {
    resp.data |= ((uint32_t)((__oe_hp_channel3_current - (0)) / 0.0392156862745098)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 40: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_hp_channel4_control) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 41: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_hp_channel4_status) & (0xFF >> (8 - 2)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 42: {
    resp.data |= ((uint32_t)((__oe_hp_channel4_current - (0)) / 0.0392156862745098)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 43: {
    resp.data |= ((uint32_t)((__oe_total_current - (0)) / 0.0012207217517357137)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 44: {
    resp.data |= ((uint32_t)((__oe_power_estimation - (0)) / 0.030518043793392843)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 45: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_sdc_status) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 46: {
    resp.data |= ((uint32_t)((__oe_mcu_temperature - (0)) / 0.7843137254901961)) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 47: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_error_mcu_over_temperature) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  case 48: {
    resp.data |= ((uint32_t)(((uint8_t)__oe_warn_mcu_over_temperature) & (0xFF >> (8 - 1)))) << 0;
    resp.header.sof = 1;
    resp.header.eof = 1;
    resp.header.toggle = 0;
    break;
  }
  }
  resp.header.od_index = msg.header.od_index;
  resp.header.client_id = msg.header.client_id;
  resp.header.server_id = msg.header.server_id;
  canzero_frame resp_frame;
  canzero_serialize_canzero_message_get_resp(&resp, &resp_frame);
  canzero_can1_send(&resp_frame);
}
static void canzero_handle_set_req(canzero_frame* frame) {
  canzero_message_set_req msg;
  canzero_deserialize_canzero_message_set_req(frame, &msg);
  if (msg.header.server_id != 10) {
    return;
  }
  canzero_message_set_resp resp;
  switch (msg.header.od_index) {
  case 0 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_state state_tmp;
    state_tmp = (pdu_state)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_state(state_tmp);
    break;
  }
  case 1 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control lp_channel1_control_tmp;
    lp_channel1_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_lp_channel1_control(lp_channel1_control_tmp);
    break;
  }
  case 2 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status lp_channel1_status_tmp;
    lp_channel1_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_lp_channel1_status(lp_channel1_status_tmp);
    break;
  }
  case 3 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float lp_channel1_current_tmp;
    lp_channel1_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0);
    canzero_set_lp_channel1_current(lp_channel1_current_tmp);
    break;
  }
  case 4 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control lp_channel2_control_tmp;
    lp_channel2_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_lp_channel2_control(lp_channel2_control_tmp);
    break;
  }
  case 5 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status lp_channel2_status_tmp;
    lp_channel2_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_lp_channel2_status(lp_channel2_status_tmp);
    break;
  }
  case 6 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float lp_channel2_current_tmp;
    lp_channel2_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0);
    canzero_set_lp_channel2_current(lp_channel2_current_tmp);
    break;
  }
  case 7 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control lp_channel3_control_tmp;
    lp_channel3_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_lp_channel3_control(lp_channel3_control_tmp);
    break;
  }
  case 8 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status lp_channel3_status_tmp;
    lp_channel3_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_lp_channel3_status(lp_channel3_status_tmp);
    break;
  }
  case 9 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float lp_channel3_current_tmp;
    lp_channel3_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0);
    canzero_set_lp_channel3_current(lp_channel3_current_tmp);
    break;
  }
  case 10 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control lp_channel4_control_tmp;
    lp_channel4_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_lp_channel4_control(lp_channel4_control_tmp);
    break;
  }
  case 11 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status lp_channel4_status_tmp;
    lp_channel4_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_lp_channel4_status(lp_channel4_status_tmp);
    break;
  }
  case 12 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float lp_channel4_current_tmp;
    lp_channel4_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0);
    canzero_set_lp_channel4_current(lp_channel4_current_tmp);
    break;
  }
  case 13 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control lp_channel5_control_tmp;
    lp_channel5_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_lp_channel5_control(lp_channel5_control_tmp);
    break;
  }
  case 14 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status lp_channel5_status_tmp;
    lp_channel5_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_lp_channel5_status(lp_channel5_status_tmp);
    break;
  }
  case 15 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float lp_channel5_current_tmp;
    lp_channel5_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0);
    canzero_set_lp_channel5_current(lp_channel5_current_tmp);
    break;
  }
  case 16 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control lp_channel6_control_tmp;
    lp_channel6_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_lp_channel6_control(lp_channel6_control_tmp);
    break;
  }
  case 17 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status lp_channel6_status_tmp;
    lp_channel6_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_lp_channel6_status(lp_channel6_status_tmp);
    break;
  }
  case 18 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float lp_channel6_current_tmp;
    lp_channel6_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0);
    canzero_set_lp_channel6_current(lp_channel6_current_tmp);
    break;
  }
  case 19 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control lp_channel7_control_tmp;
    lp_channel7_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_lp_channel7_control(lp_channel7_control_tmp);
    break;
  }
  case 20 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status lp_channel7_status_tmp;
    lp_channel7_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_lp_channel7_status(lp_channel7_status_tmp);
    break;
  }
  case 21 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float lp_channel7_current_tmp;
    lp_channel7_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0);
    canzero_set_lp_channel7_current(lp_channel7_current_tmp);
    break;
  }
  case 22 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control lp_channel8_control_tmp;
    lp_channel8_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_lp_channel8_control(lp_channel8_control_tmp);
    break;
  }
  case 23 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status lp_channel8_status_tmp;
    lp_channel8_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_lp_channel8_status(lp_channel8_status_tmp);
    break;
  }
  case 24 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float lp_channel8_current_tmp;
    lp_channel8_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0);
    canzero_set_lp_channel8_current(lp_channel8_current_tmp);
    break;
  }
  case 25 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control lp_channel9_control_tmp;
    lp_channel9_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_lp_channel9_control(lp_channel9_control_tmp);
    break;
  }
  case 26 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status lp_channel9_status_tmp;
    lp_channel9_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_lp_channel9_status(lp_channel9_status_tmp);
    break;
  }
  case 27 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float lp_channel9_current_tmp;
    lp_channel9_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0);
    canzero_set_lp_channel9_current(lp_channel9_current_tmp);
    break;
  }
  case 28 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control lp_channel10_control_tmp;
    lp_channel10_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_lp_channel10_control(lp_channel10_control_tmp);
    break;
  }
  case 29 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status lp_channel10_status_tmp;
    lp_channel10_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_lp_channel10_status(lp_channel10_status_tmp);
    break;
  }
  case 30 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float lp_channel10_current_tmp;
    lp_channel10_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.011764705882352941 + 0);
    canzero_set_lp_channel10_current(lp_channel10_current_tmp);
    break;
  }
  case 31 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control hp_channel1_control_tmp;
    hp_channel1_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_hp_channel1_control(hp_channel1_control_tmp);
    break;
  }
  case 32 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status hp_channel1_status_tmp;
    hp_channel1_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_hp_channel1_status(hp_channel1_status_tmp);
    break;
  }
  case 33 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float hp_channel1_current_tmp;
    hp_channel1_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.0392156862745098 + 0);
    canzero_set_hp_channel1_current(hp_channel1_current_tmp);
    break;
  }
  case 34 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control hp_channel2_control_tmp;
    hp_channel2_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_hp_channel2_control(hp_channel2_control_tmp);
    break;
  }
  case 35 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status hp_channel2_status_tmp;
    hp_channel2_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_hp_channel2_status(hp_channel2_status_tmp);
    break;
  }
  case 36 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float hp_channel2_current_tmp;
    hp_channel2_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.0392156862745098 + 0);
    canzero_set_hp_channel2_current(hp_channel2_current_tmp);
    break;
  }
  case 37 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control hp_channel3_control_tmp;
    hp_channel3_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_hp_channel3_control(hp_channel3_control_tmp);
    break;
  }
  case 38 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status hp_channel3_status_tmp;
    hp_channel3_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_hp_channel3_status(hp_channel3_status_tmp);
    break;
  }
  case 39 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float hp_channel3_current_tmp;
    hp_channel3_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.0392156862745098 + 0);
    canzero_set_hp_channel3_current(hp_channel3_current_tmp);
    break;
  }
  case 40 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_control hp_channel4_control_tmp;
    hp_channel4_control_tmp = (pdu_channel_control)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_hp_channel4_control(hp_channel4_control_tmp);
    break;
  }
  case 41 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    pdu_channel_status hp_channel4_status_tmp;
    hp_channel4_status_tmp = (pdu_channel_status)((msg.data & (0xFFFFFFFF >> (32 - 2))));
    canzero_set_hp_channel4_status(hp_channel4_status_tmp);
    break;
  }
  case 42 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float hp_channel4_current_tmp;
    hp_channel4_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.0392156862745098 + 0);
    canzero_set_hp_channel4_current(hp_channel4_current_tmp);
    break;
  }
  case 43 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float total_current_tmp;
    total_current_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 16))) * 0.0012207217517357137 + 0);
    canzero_set_total_current(total_current_tmp);
    break;
  }
  case 44 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float power_estimation_tmp;
    power_estimation_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 16))) * 0.030518043793392843 + 0);
    canzero_set_power_estimation(power_estimation_tmp);
    break;
  }
  case 45 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    sdc_status sdc_status_tmp;
    sdc_status_tmp = (sdc_status)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_sdc_status(sdc_status_tmp);
    break;
  }
  case 46 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    float mcu_temperature_tmp;
    mcu_temperature_tmp = (float)((msg.data & (0xFFFFFFFF >> (32 - 8))) * 0.7843137254901961 + 0);
    canzero_set_mcu_temperature(mcu_temperature_tmp);
    break;
  }
  case 47 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    error_flag error_mcu_over_temperature_tmp;
    error_mcu_over_temperature_tmp = (error_flag)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_error_mcu_over_temperature(error_mcu_over_temperature_tmp);
    break;
  }
  case 48 : {
    if (msg.header.sof != 1 || msg.header.toggle != 0 || msg.header.eof != 1) {
      return;
    }
    error_flag warn_mcu_over_temperature_tmp;
    warn_mcu_over_temperature_tmp = (error_flag)((msg.data & (0xFFFFFFFF >> (32 - 1))));
    canzero_set_warn_mcu_over_temperature(warn_mcu_over_temperature_tmp);
    break;
  }
  default:
    return;
  }
  resp.header.od_index = msg.header.od_index;
  resp.header.client_id = msg.header.client_id;
  resp.header.server_id = msg.header.server_id;
  resp.header.erno = set_resp_erno_Success;
  canzero_frame resp_frame;
  canzero_serialize_canzero_message_set_resp(&resp, &resp_frame);
  canzero_can1_send(&resp_frame);

}
static void canzero_handle_master_stream_pdu24_control(canzero_frame* frame) {
  canzero_message_master_stream_pdu24_control msg;
  canzero_deserialize_canzero_message_master_stream_pdu24_control(frame, &msg);
  canzero_set_lp_channel1_control(msg.pdu24_lp_channel1_control);
  canzero_set_lp_channel2_control(msg.pdu24_lp_channel2_control);
  canzero_set_lp_channel3_control(msg.pdu24_lp_channel3_control);
  canzero_set_lp_channel4_control(msg.pdu24_lp_channel4_control);
  canzero_set_lp_channel5_control(msg.pdu24_lp_channel5_control);
  canzero_set_lp_channel6_control(msg.pdu24_lp_channel6_control);
  canzero_set_lp_channel7_control(msg.pdu24_lp_channel7_control);
  canzero_set_lp_channel8_control(msg.pdu24_lp_channel8_control);
  canzero_set_lp_channel9_control(msg.pdu24_lp_channel9_control);
  canzero_set_lp_channel10_control(msg.pdu24_lp_channel10_control);
  canzero_set_hp_channel1_control(msg.pdu24_hp_channel1_control);
  canzero_set_hp_channel2_control(msg.pdu24_hp_channel2_control);
  canzero_set_hp_channel3_control(msg.pdu24_hp_channel3_control);
  canzero_set_hp_channel4_control(msg.pdu24_hp_channel4_control);
}
__attribute__((weak)) void canzero_handle_heartbeat(canzero_frame* frame) {
  canzero_message_heartbeat msg;
  canzero_deserialize_canzero_message_heartbeat(frame, &msg);
}
void canzero_can0_poll() {
  canzero_frame frame;
  while (canzero_can0_recv(&frame)) {
    switch (frame.id) {
      case 0x13B:
        canzero_handle_get_req(&frame);
        break;
      case 0x15B:
        canzero_handle_set_req(&frame);
        break;
      case 0x4F:
        canzero_handle_master_stream_pdu24_control(&frame);
        break;
      case 0x17B:
        canzero_handle_heartbeat(&frame);
        break;
    }
  }
}
void canzero_can1_poll() {
  canzero_frame frame;
  while (canzero_can1_recv(&frame)) {
    switch (frame.id) {
    }
  }
}
uint32_t canzero_update_continue(uint32_t time){
  schedule_jobs(time);
  return scheduler_next_job_timeout();
}
void canzero_init() {
  canzero_can0_setup(1000000, NULL, 0);
  canzero_can1_setup(1000000, NULL, 0);

  job_pool_allocator_init();
  scheduler.size = 0;
  schedule_heartbeat_job();
  schedule_channel_currents_1_interval_job();
  schedule_channel_currents_2_interval_job();
  schedule_power_estimation_interval_job();
  schedule_channel_status_interval_job();
  schedule_state_interval_job();

}
void canzero_set_state(pdu_state value) {
  extern pdu_state __oe_state;
  if (__oe_state != value) {
    __oe_state = value;
    uint32_t time = canzero_get_time();
    if (state_interval_job.climax > state_interval_job.job.stream_interval_job.last_schedule + 50) {
      state_interval_job.climax = state_interval_job.job.stream_interval_job.last_schedule + 50;
      scheduler_promote_job(&state_interval_job);
    }
  }
}
void canzero_set_lp_channel1_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_lp_channel1_status;
  if (__oe_lp_channel1_status != value) {
    __oe_lp_channel1_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_lp_channel2_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_lp_channel2_status;
  if (__oe_lp_channel2_status != value) {
    __oe_lp_channel2_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_lp_channel3_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_lp_channel3_status;
  if (__oe_lp_channel3_status != value) {
    __oe_lp_channel3_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_lp_channel4_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_lp_channel4_status;
  if (__oe_lp_channel4_status != value) {
    __oe_lp_channel4_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_lp_channel5_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_lp_channel5_status;
  if (__oe_lp_channel5_status != value) {
    __oe_lp_channel5_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_lp_channel6_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_lp_channel6_status;
  if (__oe_lp_channel6_status != value) {
    __oe_lp_channel6_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_lp_channel7_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_lp_channel7_status;
  if (__oe_lp_channel7_status != value) {
    __oe_lp_channel7_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_lp_channel8_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_lp_channel8_status;
  if (__oe_lp_channel8_status != value) {
    __oe_lp_channel8_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_lp_channel9_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_lp_channel9_status;
  if (__oe_lp_channel9_status != value) {
    __oe_lp_channel9_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_lp_channel10_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_lp_channel10_status;
  if (__oe_lp_channel10_status != value) {
    __oe_lp_channel10_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_hp_channel1_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_hp_channel1_status;
  if (__oe_hp_channel1_status != value) {
    __oe_hp_channel1_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_hp_channel2_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_hp_channel2_status;
  if (__oe_hp_channel2_status != value) {
    __oe_hp_channel2_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_hp_channel3_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_hp_channel3_status;
  if (__oe_hp_channel3_status != value) {
    __oe_hp_channel3_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_hp_channel4_status(pdu_channel_status value) {
  extern pdu_channel_status __oe_hp_channel4_status;
  if (__oe_hp_channel4_status != value) {
    __oe_hp_channel4_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_sdc_status(sdc_status value) {
  extern sdc_status __oe_sdc_status;
  if (__oe_sdc_status != value) {
    __oe_sdc_status = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_error_mcu_over_temperature(error_flag value) {
  extern error_flag __oe_error_mcu_over_temperature;
  if (__oe_error_mcu_over_temperature != value) {
    __oe_error_mcu_over_temperature = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
void canzero_set_warn_mcu_over_temperature(error_flag value) {
  extern error_flag __oe_warn_mcu_over_temperature;
  if (__oe_warn_mcu_over_temperature != value) {
    __oe_warn_mcu_over_temperature = value;
    uint32_t time = canzero_get_time();
    if (channel_status_interval_job.climax > channel_status_interval_job.job.stream_interval_job.last_schedule + 0) {
      channel_status_interval_job.climax = channel_status_interval_job.job.stream_interval_job.last_schedule + 0;
      scheduler_promote_job(&channel_status_interval_job);
    }
  }
}
