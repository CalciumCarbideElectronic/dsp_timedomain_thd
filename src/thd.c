#include "stdlib.h"
#include "math.h"
#include "thd.h"

float timedomain_thd_float(TimedomainTHD *self, float *samples)
{
    _sync_puresin(self, samples);
    float v_fund_rms_ti = _calculate_fund_rms(self, samples);
    float v_rms = _calculate_rms(self, samples);
    float thd = sqrtf32(pow(v_rms, 2) - pow(v_fund_rms_ti, 2)) / v_fund_rms_ti;
    return thd;
}

void _sync_puresin(TimedomainTHD *self, float *samples)
{
    self->puresin->offset = 0;
    uint16_t inf = 0, sup = self->sample_length;
    uint16_t middle = (sup - inf) / 2;
    float diff = _diff_two_signal(self, samples);
    while (sup != middle && inf != middle)
    {
        self->puresin->offset = middle;
        if (_diff_two_signal(self, samples) <= diff)
            sup = middle;
        else
            inf = middle;
        middle = (sup - inf) / 2;
    }
}

float _diff_two_signal(TimedomainTHD *self, float *samples)
{
    uint16_t idx, cursor = 0;
    float accu = 0;
    for (idx = self->puresin->offset; idx < self->sample_length; idx++)
        accu += pow(samples[cursor++] - self->puresin->raw[idx], 2);
    for (idx = 0; idx < self->puresin->offset; idx++)
        accu += pow(samples[cursor++] - self->puresin->raw[idx], 2);
    return accu;
}

float _calculate_fund_rms(TimedomainTHD *self, float *samples)
{
    return 0;
}

float _calculate_rms(TimedomainTHD *self, float *samples)
{
    return 0;
}