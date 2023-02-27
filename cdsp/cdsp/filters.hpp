//
// Created by Crispin on 27/02/23.
//

#pragma once

namespace cdsp::appnotes {
    struct OnePoleFilter {
        float delay[2]{0, 0};
        float g = 0;

        void set_cutoff(float fc, float sample_rate);

        float lpf(float samp, int channel);
        float hpf(float samp, int channel);
    };

}
