
#include <gtest/gtest.h>
#include <math.h>
#include <stdio.h>

extern "C" {
#include "../src/thd.h"
#include "test_util.h"
}

TEST(test_primary, small_thd) {
    TimedomainTHDConf conf = {
        .frequency = DEFAULT_FREQ,
        .sample_rate = DEFAULT_S_RATE,
        .sample_length = DEFAULT_S_LENGTH,
    };
    TimedomainTHD* thdObj = NewTimedomainTHD(conf);
    float signal[DEFAULT_S_LENGTH];
    SamplesMetaInfo info = {.length = DEFAULT_S_LENGTH,
                            .frequency = DEFAULT_FREQ,
                            .sample_rate = DEFAULT_S_RATE,
                            .init_phase = 0.3 * (float)M_PI,
                            .harmonic_coef = {1, 0.05, 0.04, 0.03, 0.02}};
    generate_samples(signal, info);
    float thd = timedomain_thd_float(thdObj, signal);
    printf("thd: %f", thd);
}
