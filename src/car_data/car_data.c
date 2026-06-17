#include "car_data.h"

void manager_init(TelemetryManager *tmg){
  //telelemetry manager already contains all the drivers
  memset(tmg, 0, sizeof(TelemetryManager));
}

/**
 * what this function does is basically returning a Driver Stream
 * which is an instance of a driver with all the history of it's 
 * telemetry data
 */
DriverStream* manager_get_stream(uint8_t n_driver, TelemetryManager *tmg){
  if(n_driver < 1 || n_driver > N_DRIVER) return NULL;

  DriverStream *s = &tmg->drivers[n_driver];

  bool active = atomic_load_explicit(&s->isActive, memory_order_acquire);
  return active ? s : NULL;
}

/**
 * this function takes a n_driver and a telemetry manager and 
 * creates a new driver stream based on the driver number given
 * by the user
 */
bool manager_add_stream(uint8_t n_driver, TelemetryManager* tmg){
  if(n_driver < 1 || n_driver > N_DRIVER) return false;

  DriverStream *s = &tmg->drivers[n_driver];
  //check if stream is already in the manager
  if(atomic_load_explicit(&s->isActive, memory_order_acquire)) return true;

  //otherwise create a new stream data buffer for that driver 
  memset(&s->history, 0, sizeof(TelemetryHistory));
  
  atomic_store_explicit(&s->isActive, true, memory_order_release);

  return true;

}

/**
 * this function takes a driver and a frame as input, a frame is 
 * a snapshot of the current data stream that the mqtt broker is
 * feeding, every snapshot is saved in the circular buffer 
 * Telemetry History, what this function does is a simple push
 * to that buffer
 */
void history_push(TelemetryHistory *history, const Frame *frame){
  /** 
   * * here whe increment the atomic value in a relaxed way sice we 
   * * are the only writer and we want to read our last position
   * */
  uint32_t pos = atomic_load_explicit(&history->head, memory_order_relaxed);

  /** 
   * push the data, here we use & for the wrap around:
   * notice & only works for powers of 2, this is how the logic works:
   * pos = 0   → 0   & 511 = 0    write slot 0,   head → 1 
   * pos = 1   → 1   & 511 = 1    write slot 1,   head → 2
   * pos = 511 → 511 & 511 = 511  write slot 511, head → 512
   * pos = 512 → 512 & 511 = 0    write slot 0,   head → 513  ← wraparound
   * pos = 513 → 513 & 511 = 1    write slot 1,   head → 514
   * */
  history->frames[pos & (HISTORY_LEN - 1)] = *frame;

  //increment the head counter
  atomic_store_explicit(&history->head, pos + 1, memory_order_release);

}

/**
 * this function takes as input the history of a driver, the number of frames
 * to pull from the buffer and returns an array of frames
 * since we have a continuos head counter we don't need to use any modulo logic
 */
uint32_t history_snapshot(TelemetryHistory *history, uint32_t max_frames, Frame f_out[])
{
  uint32_t head = atomic_load_explicit(&history->head, memory_order_acquire);

  uint32_t available = head < HISTORY_LEN ? head : HISTORY_LEN;
  //check if the number of frames are lower than max_frames
  uint32_t count = available < max_frames ? available : max_frames;
  //oldest frame we want - unwrapped index
  uint32_t start = head - count;

  for(uint32_t i = 0; i < count; i++){
    //apply the same logic as history_push
    f_out[i] = history->frames[(start+i) & (HISTORY_LEN - 1)];
  }

  //number of actual frame pulled
  return count;

}
