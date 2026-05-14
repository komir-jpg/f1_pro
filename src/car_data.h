#ifndef CAR_DATA_H
#define CAR_DATA_H

#include<stdint.h>

#define HISTORY_LEN 35000
#define N_DRIVER 99

typedef struct {
    uint16_t rmp;
    uint16_t speed;
    uint8_t brake;
    uint8_t drs;
    uint8_t n_gear;
    uint8_t throttle;
    uint64_t _id;
    char _key[32];
}Frame;

typedef struct{
    Frame frames[HISTORY_LEN];
   _Atomic uint16_t head;
}TelemetryHistory;

typedef struct{
    TelemetryHistory history;
    bool isActive;
}DriverStream;

typedef struct{
    DriverStream drivers[N_DRIVER+1];
}TelemetryManager;

void manager_init(TelemetryManager *tmg);
DriverStream* manager_get_stream(uint8_t n_driver, TelemetryManager *tmg);
//user subscribes to a driver -> add a new driver to the TelemetryManager 
void manager_add_stream(uint8_t n_driver, TelemetryManager *tmg);
void push(uint8_t n_dirver, Car_Data data);
uint16_t snapshot(uint8_t driver, uint16_t get_size, Car_Data* dest);
#endif
