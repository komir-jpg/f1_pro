#ifndef LAP_DATA_H
#define LAP_DATA_H

#include<inttypes.h>
#include<stdbool.h>

#define RB_SIZE 35000
#define DRIVERS 100

typedef struct{
    bool is_pit_out_lap;
    int16_t lap_duration;
    int16_t lap_number;
    float s1;
    float s2;
    float s3;
    
} Lap_Data;

typedef struct{
    LapData data[RB_SIZE];
    int8_t head;
    int8_t size;
} RingBuffer;

void init_buffer
void push(RingBuffer *buffer, Data data);




#endif
