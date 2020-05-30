
#include "stdint.h"
typedef struct PureSin
{
    uint16_t offset;
    float *raw;
} PureSin;

typedef struct TimedomainTHD
{
    float frequency;
    float sample_rate;
    float sample_length;
    PureSin *puresin;
} TimedomainTHD;

float timedomain_thd_float(TimedomainTHD *self, float *samples);
void _sync_puresin(TimedomainTHD *self, float *samples);
float _diff_two_signal(TimedomainTHD *self, float *samples);
float _calculate_fund_rms(TimedomainTHD *self, float *samples, float rms);
float _calculate_rms(TimedomainTHD *self, float *samples);