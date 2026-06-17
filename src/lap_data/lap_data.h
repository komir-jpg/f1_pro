#include<inttypes.h>
#include<stdbool.h>

#define N_MINI_SECTORS 8

typedef enum {
    NOT_AVIABLE = 0,
    YELLOW = 2048,
    GREEN = 2049,
    PURPLE = 2051,
    PIT_LANE = 2064
}Sectors;

typedef struct {
    uint64_t _id;
    double duration_S1;
    double duration_S2;
    double duration_S3;
    double duration_lap;
    Sectors mini_sector_1[N_MINI_SECTORS];
    Sectors mini_sector_2[N_MINI_SECTORS];
    Sectors mini_sector_3[N_MINI_SECTORS];
    uint16_t i1_speed;
    uint16_t i2_speed;
    uint16_t st_speed;
    uint8_t d_num;
    uint8_t l_num;
    bool is_pit_out;
    char _key[32];
}Lap;