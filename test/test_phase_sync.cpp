#include <gtest/gtest.h>
#include <math.h>
#include <stdio.h>

extern "C" {
#include "../src/thd.h"
#include "test_util.h"
}

TEST(test_primary, phase_sync) {
    TimedomainTHDConf conf = {
        .frequency = DEFAULT_FREQ,
        .sample_rate = DEFAULT_S_RATE,
        .sample_length = DEFAULT_S_LENGTH,
    };
    TimedomainTHD* thdObj = NewTimedomainTHD(conf);
    float signal[DEFAULT_S_LENGTH];
    float phi_coef;
    float mse;
    for (phi_coef = 0; phi_coef <= 2 * M_PI; phi_coef += 0.1) {
        SamplesMetaInfo info = {.length = DEFAULT_S_LENGTH,
                                .frequency = DEFAULT_FREQ,
                                .sample_rate = DEFAULT_S_RATE,
                                .init_phase = phi_coef * (float)M_PI,
                                .harmonic_coef = {1, 0.0, 0.00, 0.00, 0.0}};
        generate_samples(signal, info);
        thdObj->puresin->offset = 0;
        _sync_puresin(thdObj, signal);
        mse = _calculate_pure_mse(thdObj, signal);
        ASSERT_NEAR(0, mse, 10);
    }
}
