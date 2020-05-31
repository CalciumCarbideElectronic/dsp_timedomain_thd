#include "thd.h"

#include "math.h"
#include "stdio.h"
#include "stdlib.h"

#define pi 3.14159265

TimedomainTHD *NewTimedomainTHD(TimedomainTHDConf conf) {
    static TimedomainTHD singleton;
    static PureSin puresin;
    static float *puresin_raw;
    singleton.puresin = &puresin;
    singleton.frequency = conf.frequency;
    singleton.sample_length = conf.sample_length;
    singleton.sample_rate = conf.sample_rate;

    if (puresin_raw == NULL)
        puresin_raw = malloc(sizeof(float) * conf.sample_length);

    for (int idx = 0; idx < conf.sample_length; idx++) {
        puresin_raw[idx] = sin(idx * 2 * pi * 50 / conf.sample_rate);
    }
    puresin.raw = puresin_raw;

    return &singleton;
}

float timedomain_thd_float(TimedomainTHD *self, float *samples) {
    _sync_puresin(self, samples);
    float v_rms = _calculate_rms(self, samples);
    float v_fund_rms_ti = _calculate_fund_rms(self, samples, v_rms);
    float thd = sqrtf(pow(v_rms, 2) - pow(v_fund_rms_ti, 2)) / v_fund_rms_ti *
                2 / sqrtf(2);
    return thd;
}

void _sync_puresin(TimedomainTHD *self, float *samples) {
    self->puresin->offset = 0;
    uint16_t inf = 0, sup = self->sample_length;
    uint16_t middle = (sup - inf) / 2;
    float diff = _calculate_pure_mse(self, samples);
    for (int idx = 0; idx < 90; idx++) {
        self->puresin->offset = idx;
        float diff_temp = _calculate_pure_mse(self, samples);
        if (diff_temp <= diff) {
            diff = diff_temp;
            inf = idx;
        }
    }
    self->puresin->offset = inf;
}

float _calculate_pure_mse(TimedomainTHD *self, float *samples) {
    uint16_t idx, cursor = 0;
    float accu = 0;
    for (idx = self->puresin->offset; idx < self->sample_length; idx++)
        accu += pow(samples[cursor++] - self->puresin->raw[idx], 2);
    for (idx = 0; idx < self->puresin->offset; idx++)
        accu += pow(samples[cursor++] - self->puresin->raw[idx], 2);
    return accu;
}

float _calculate_fund_rms(TimedomainTHD *self, float *samples, float rms) {
    float accu = 0, fund_rms = 0;
    uint16_t idx, cursor = 0;

    for (idx = self->puresin->offset; idx < self->sample_length; idx++)
        accu += samples[cursor++] * self->puresin->raw[idx] * sqrtf(2) * rms;
    for (idx = 0; idx < self->puresin->offset; idx++)
        accu += samples[cursor++] * self->puresin->raw[idx] * sqrtf(2) * rms;

    fund_rms = sqrtf(accu / self->sample_length);

    return fund_rms;
}

float _calculate_rms(TimedomainTHD *self, float *samples) {
    float accu = 0, rms = 0;
    for (int idx = 0; idx < self->sample_length; idx++) {
        accu += pow(samples[idx], 2);
    }
    rms = sqrtf(accu / self->sample_length);

    return rms;
}
