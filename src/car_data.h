#ifndef CAR_DATA_H
#define CAR_DATA_H

#include<stdint.h>

#define DATA_SIZE 35000
#define ELEM_SIZE 100

typedef struct {
    uint16_t rmp;
    uint16_t speed;
    uint8_t brake;
    uint8_t drs;
    uint8_t n_gear;
    uint8_t throttle;
}Car_Data;

typedef struct{
    Car_Data[DATA_SIZE];
    int8_t head;
    int8_t size;
} RingBuffer;

void init_buffer(RingBuffer *buffer);
void push(int8_t n_dirver, Car_data data);
void snapshot(int8_t driver, int8_t get_size,Car_Data* dest);
#endif
