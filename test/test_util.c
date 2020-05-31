#include "test_util.h"

void generate_samples(float* buf, SamplesMetaInfo meta) {
    {
        memset(buf, 0, sizeof(float) * meta.length);
        for (int idx = 0; idx < meta.length; idx++) {
            for (int harmonic_idx = 0; harmonic_idx < 5; harmonic_idx++) {
                buf[idx] += meta.harmonic_coef[harmonic_idx] *
                            sin(idx * 2 * M_PI * meta.frequency *
                                    (harmonic_idx + 1) / meta.sample_rate +
                                meta.init_phase);
            }
        }
    }
}
