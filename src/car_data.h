#ifndef CAR_DATA_H
#define CAR_DATA_H

#include<stdint.h>

typedef struct {
    uint16_t rmp;
    uint16_t speed;
    uint8_t brake;
    uint8_t drs;
    uint8_t n_gear;
    uint8_t throttle;
}Car_Data;

#endif
