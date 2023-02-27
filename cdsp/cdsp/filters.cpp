//
// Created by Crispin on 27/02/23.
//

#include "filters.hpp"
#include <numbers>
#include <cmath>

namespace cdsp::appnotes {
    float OnePoleFilter::lpf(float samp, int channel) {
//        float cu = cutoff / (1 + cutoff);
        float de = delay[channel];

        float a = (samp + (-de)) * g;
        float b = a + de;
        float c = a + b;
        delay[channel] = c;
        return b;

//        float v = (samp - de)*g;
//        float LP = v +de;
//        delay[channel] = LP + v;
//        return LP;
    }

    float OnePoleFilter::hpf(float samp, int channel) {
        return samp - lpf(samp, channel);
    }

    void OnePoleFilter::set_cutoff(float fc, float sample_rate) {
        // magic from the article
        float wd = 2.f * std::numbers::pi_v<float> * fc;
        float T = 1.f / sample_rate;
        float wa = (2.f/T) * std::tan(wd * T / 2);
        float little_g = wa * T / 2.f;
        g = little_g / (1.f + little_g);
    }
}
