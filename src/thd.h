#ifndef THD_H
#define THD_H
#include "stdint.h"

typedef struct PureSin {
    uint16_t offset;
    float *raw;
} PureSin;

typedef struct TimedomainTHD {
    float frequency;
    float sample_rate;
    float sample_length;
    PureSin *puresin;
} TimedomainTHD;

typedef struct TimedomainTHDConf {
    float frequency;
    float sample_rate;
    float sample_length;
} TimedomainTHDConf;

TimedomainTHD *NewTimedomainTHD(TimedomainTHDConf conf);
float timedomain_thd_float(TimedomainTHD *self, float *samples);
void _sync_puresin(TimedomainTHD *self, float *samples);
float _calculate_pure_mse(TimedomainTHD *self, float *samples);
float _calculate_fund_rms(TimedomainTHD *self, float *samples, float rms);
float _calculate_rms(TimedomainTHD *self, float *samples);

#endif
