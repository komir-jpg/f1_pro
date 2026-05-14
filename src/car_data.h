#ifndef CAR_DATA_H
#define CAR_DATA_H

#include <stdint.h>
#include <pthread.h>

#define DATA_SIZE 35000
#define DRIVER_NUM 100

typedef struct {
    uint16_t rmp;
    uint16_t speed;
    uint8_t brake;
    uint8_t drs;
    uint8_t n_gear;
    uint8_t throttle;
}Car_Data;

typedef struct{
    Car_Data data[DATA_SIZE];
    uint16_t head;
    uint16_t size;
    pthread_mutex_t mutex;
} RingBuffer;


int init_buffer(RingBuffer *buffer);
void push(uint8_t n_dirver, Car_Data data);
uint16_t snapshot(uint8_t driver, uint16_t get_size, Car_Data* dest);
#endif

