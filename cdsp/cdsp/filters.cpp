//
// Created by Crispin on 27/02/23.
//

#include "filters.hpp"
#include <numbers>
#include <cmath>

namespace cdsp::appnotes {
    double OnePoleFilter::lpf(double samp, int channel) {
        double de = delay[channel];

        double a = (samp + (-de)) * G;
        double b = a + de;
        double c = a + b;
        delay[channel] = c;
        return b;
    }

    double OnePoleFilter::hpf(double samp, int channel) {
        return samp - lpf(samp, channel);
    }

    void OnePoleFilter::set_cutoff(double fc, double sample_rate) {
        // equations from the appnote
        double wd = 2.f * std::numbers::pi_v<float> * fc;
        double T = 1.f / sample_rate;
        double wa = (2.f/T) * std::tan(wd * T / 2);
        double little_g = wa * T / 2.f;
        G = little_g / (1.f + little_g);
    }

    double OnePoleFilter::feedback(int channel) {
        return delay[channel] * feedback_mul;
    }

    void ThirtyFiveLPF::set_params(double fc, double q, double sat, double sample_rate) {
        double wd = 2.f * std::numbers::pi_v<double> * fc;
        double T = 1.f / sample_rate;
        double wa = (2.f/T) * std::tan(wd * T / 2.f);
        double little_g = wa * T / 2.f;
        G = little_g / (1.f + little_g);

        lpf1.G = G;
        lpf2.G = G;
        hpf1.G = G;

        lpf2.feedback_mul = (dk - dk * G) / (1.0 + little_g);
        hpf1.feedback_mul = -1.0/(1.0 + little_g);

        dk = q;
        saturation = sat;
    }

    double ThirtyFiveLPF::process(double samp, int channel) {
        double y1 = lpf1.lpf(samp, channel);
        double s35 = hpf1.feedback(channel) + lpf2.feedback(channel);
        double u = (y1 + s35) / (1.f - dk*G + dk*G*G);
        u = std::tanh(saturation * u);
        double y = dk * lpf2.lpf(u, channel);
        hpf1.hpf(y, channel);
        if (dk > 0) {
            y *= 1/dk;
        }
        return y;
    }
}
