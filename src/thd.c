#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "thd.h"
#include "math.h"

#define pi 3.14159265

float timedomain_thd_float(TimedomainTHD *self, float *samples)
{
    _sync_puresin(self, samples);
    float v_rms = _calculate_rms(self, samples);
    float v_fund_rms_ti = _calculate_fund_rms(self, samples, v_rms);
    float thd = sqrtf(pow(v_rms, 2) - pow(v_fund_rms_ti, 2)) / v_fund_rms_ti * 2 / sqrtf(2);
    return thd;
}

void _sync_puresin(TimedomainTHD *self, float *samples)
{
    self->puresin->offset = 0;
    uint16_t inf = 0, sup = self->sample_length;
    uint16_t middle = (sup - inf) / 2;
    float diff = _diff_two_signal(self, samples);
    // while (sup != middle && inf != middle)
    // {
    //     self->puresin->offset = middle;
    //     if (_diff_two_signal(self, samples) <= diff)
    //         sup = middle;
    //     else
    //         inf = middle;
    //     middle = (sup - inf) / 2;
    // }
    
    for (int idx = 0; idx < 90; idx++)
    {
        self->puresin->offset = idx;
        float diff_temp = _diff_two_signal(self, samples);
        if (diff_temp <= diff)
        {
            diff = diff_temp;
            inf = idx;
        }
    }

    self->puresin->offset = inf;
    printf("offset:%d ", inf);
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

float _calculate_fund_rms(TimedomainTHD *self, float *samples, float rms)
{
    float accu = 0, fund_rms = 0;
    uint16_t idx, cursor = 0;

    for (idx = self->puresin->offset; idx < self->sample_length; idx++)
        accu += samples[cursor++] * self->puresin->raw[idx] * sqrtf(2) * rms;
    for (idx = 0; idx < self->puresin->offset; idx++)
        accu += samples[cursor++] * self->puresin->raw[idx] * sqrtf(2) * rms;

    fund_rms = sqrtf(accu / self->sample_length);

    return fund_rms;
}

float _calculate_rms(TimedomainTHD *self, float *samples)
{
    float accu = 0, rms = 0;
    for (int idx = 0; idx < self->sample_length; idx++)
    {
        accu += pow(samples[idx], 2);
    }
    rms = sqrtf(accu / self->sample_length);

    return rms;
}

int main()
{
    FILE *fp1 = NULL;
    FILE *fp2 = NULL;
    FILE *fp3 = NULL;

    fp1 = fopen("sin.csv", "w");
    fp2 = fopen("signal.csv", "w");
    fp3 = fopen("phi.csv", "w");

    TimedomainTHD THD1;
    PureSin Sin1;
    int sample_length = 8192;
    int sample_rate = 4096;

    THD1.frequency = 50;
    THD1.sample_length = sample_length;
    THD1.sample_rate = sample_rate;
    THD1.puresin = & Sin1;
    

    float raw[sample_length];
    for (int idx = 0; idx < sample_length; idx++)
    {
        raw[idx] = sin(idx * 2 * pi * 50 / sample_rate);
    }

    Sin1.offset = 0;
    Sin1.raw = raw;


    float signal[sample_length];
    float A[5] = {1,0.1,0.05,0,0};
    float phi = pi * 2/3;

    for (phi = 0; phi < 2 * pi; phi += 0.01)
    {
        for (int idx = 0; idx < sample_length; idx++)
        {
            signal[idx] = 0;
            for (int a_idx = 0; a_idx < 5; a_idx++)
            {
                signal[idx] += A[a_idx] * sin(idx * 2 * pi * 50 * (a_idx + 1) / sample_rate + phi);
            }
        }

        // _sync_puresin(&THD1, signal);
        float thd = timedomain_thd_float(&THD1, signal);
        printf("phi:%f thd:%f\n", phi, thd);

        fprintf(fp3, "%f,%f\n", phi, thd);
    }


    // for (int idx = 0; idx < sample_length; idx++)
    // {
    //     fprintf(fp1, "%f\n", raw[idx]);
    //     fprintf(fp2, "%f\n", signal[idx]);
    // }

    uint16_t cursor = 0;
    for (int idx = THD1.puresin->offset; idx < THD1.sample_length; idx++)
    {
        fprintf(fp1, "%f,%f\n", raw[cursor], THD1.puresin->raw[idx]);
        fprintf(fp2, "%f\n", signal[cursor++]);
    }
    for (int idx = 0; idx < THD1.puresin->offset; idx++)
    {
        fprintf(fp1, "%f,%f\n", raw[cursor], THD1.puresin->raw[idx]);
        fprintf(fp2, "%f\n", signal[cursor++]);
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;

}
