//
// Created by Crispin on 27/02/23.
//

#pragma once

namespace cdsp::appnotes {
    // basic one-pole filter, used as building block for more complex ones
    // stereo operation, pass 0 or 1 to channel parameter for each (could be templated for variable channel counts if needed)
    struct OnePoleFilter {
        double delay[2]{0, 0};
        double G = 0;
        double feedback_mul = 1;

        void set_cutoff(double fc, double sample_rate);

        double lpf(double samp, int channel);
        double hpf(double samp, int channel);
        // delay value * feedback_mul, provided for convenience in using as a building block
        double feedback(int channel);
    };

    // Korg-style SVF with non-linear saturation in feedback path
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
