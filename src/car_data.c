#include "car_data.h"

//static init
RingBuffer db_drivers[ELEM_SIZE];

void init_buffer(RingBuffer *buffer){
   for(int i = 0; i < ELEM_SIZE; i++){
       db_drivers[i].head = 0;
       db_drivers[i].size = 0;
   }
}

void push(int8_t n_driver, Car_data data){
    if(n_driver <= 0 || n_driver > 99) {return;} 
    //the buffer is indexed using the dirvers number
    RingBuffer *b = &db_drivers[n_driver];

    b->Car_Data[head] = data;

    //move the head 
    b->head = (b->head + 1) % DATA_SIZE;

    if(b->size <= DATA_SIZE) {b->size++;}
}

int snapshot(int8_t n_driver, int8_t size, Car_Data* dest){
    if(n_driver <= 0 || n_driver > 99) {return -1;} 
    //we want to pop the last size car data in the struct
    //since we do not have a tail
    RingBuffer *b = &db_drivers[n_driver];
    
}
