#include "car_data.h"
<<<<<<< HEAD
=======

#include <pthread.h>
>>>>>>> 32fa8000f4fb5f9ab416ec3da083d5c0b0d518b2
#include <stdint.h>

// static init
RingBuffer db_drivers[DRIVER_NUM];

int init_buffer(RingBuffer *buffer) {
  for (int i = 0; i < DRIVER_NUM; i++) {
    db_drivers[i].head = 0;
    db_drivers[i].size = 0;
    int rc = pthread_mutex_init(&db_drivers[i].mutex, NULL);
    if (rc != 0) {
      return -1;
    }
  }
  return 0;
}

void push(uint8_t n_driver, Car_Data data) {
  if (n_driver > 99) {
    return;
  }
  // the buffer is indexed using the dirvers number
  RingBuffer *b = &db_drivers[n_driver];

  pthread_mutex_lock(&b->mutex);
  b->data[b->head] = data;

  // move the head
  b->head = (b->head + 1) % DATA_SIZE;

  if (b->size <= DATA_SIZE) {
    b->size++;
  }
  pthread_mutex_unlock(&b->mutex);
}

uint16_t snapshot(uint8_t n_driver, uint16_t u_size, Car_Data *dest) {
  if (n_driver > 99) {
    return -1;
  }
  // we want to pop the last size car data in the struct
  // since we do not have a tail
  RingBuffer *b = &db_drivers[n_driver];
  pthread_mutex_lock(&b->mutex);
  if (b->size == 0) {
    return 0;
  }

  uint16_t items = b->size < u_size ? b->size : u_size;
  uint16_t start_index = (b->head - u_size + DATA_SIZE) % DATA_SIZE;

  for (uint16_t i = 0; i < items; i++) {
    uint16_t real_index = (start_index + i) % DATA_SIZE;
    dest[i] = b->data[real_index];
  }
  pthread_mutex_unlock(&b->mutex);
  return items;
}
