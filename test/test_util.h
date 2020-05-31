#ifndef TEST_UTIL_H_
#define TEST_UTIL_H_

#include <math.h>
#include <string.h>

#include "../src/thd.h"

#define DEFAULT_FREQ 50
#define DEFAULT_S_RATE 4096
#define DEFAULT_S_LENGTH 8192

typedef struct SamplesMetaInfo {
    long length;
    float frequency;
    long sample_rate;
    float init_phase;
    float harmonic_coef[5];

} SamplesMetaInfo;

void generate_samples(float* buf, SamplesMetaInfo meta);

#endif

