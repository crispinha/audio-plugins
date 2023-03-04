//
// Created by Crispin on 27/02/23.
//

#pragma once

namespace cdsp::appnotes {
    struct OnePoleFilter {
        double delay[2]{0, 0};
        double G = 0;
        double feedback_mul = 1;

        void set_cutoff(double fc, double sample_rate);

        double lpf(double samp, int channel);
        double hpf(double samp, int channel);
        double feedback(int channel);
    };

    struct ThirtyFiveLPF {
        OnePoleFilter lpf1;
        OnePoleFilter lpf2;
        OnePoleFilter hpf1;

        double G = 0;
        double dk = 0;
        double saturation = 0;

        void set_params(double fc, double q, double sat, double sample_rate);

        double process(double samp, int channel);
    };

}
