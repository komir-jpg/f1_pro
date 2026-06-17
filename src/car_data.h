#ifndef CAR_DATA_H
#define CAR_DATA_H

#include <stdint.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <string.h>

#define HISTORY_LEN 4096
#define N_DRIVER 99

typedef struct {
    uint64_t _id;
    char _key[32];
    uint32_t timestamp;
    uint16_t rmp;
    uint16_t speed;
    uint8_t brake;
    uint8_t drs;
    uint8_t n_gear;
    uint8_t throttle;
}Frame;

typedef struct{
    Frame frames[HISTORY_LEN];
   _Atomic uint16_t head;
}TelemetryHistory;

typedef struct{
    TelemetryHistory history;
    _Atomic bool isActive;
}DriverStream;

typedef struct{
    DriverStream drivers[N_DRIVER+1];
}TelemetryManager;

void manager_init(TelemetryManager *tmg);
void history_push(TelemetryHistory *history, const Frame *frame);
DriverStream* manager_get_stream(uint8_t n_driver, TelemetryManager *tmg);
bool manager_add_stream(uint8_t n_driver, TelemetryManager *tmg);
uint32_t history_snapshot(TelemetryHistory *history, uint32_t max_frames, 
    Frame f_out[]);
#endif
