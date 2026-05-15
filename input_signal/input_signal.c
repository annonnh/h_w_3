#include "math.h"
#include "stdlib.h"
#include "input_signal.h"
#include "adc_simulator/adc_simulator.h"

unsigned int read_simulated_ADC_with_sin(void)
{
    static unsigned long sampleCount = 0UL;
    static unsigned long lfsr         = 0xACE1u;   // LFSR seed — must be nonzero

    float phase;
    float signal;
    float noise;
    int   signedNoise;

    const float Ts = (float)SAMPLING_PERIOD_US * 1e-6F;
    const float f0 = 60.0F;

    // --- Sine (phase accumulator, no drift) ---
    phase  = 2.0F * 3.14159265F * f0 * (float)sampleCount * Ts;
    phase  = fmodf(phase, 2.0F * 3.14159265F);
    signal = 2048.0F + 1000.0F * sinf(phase);

    // --- Noise: 16-bit Galois LFSR (no rand(), no stdlib, MISRA-friendly) ---
    // Produces a pseudo-random integer in [0, 65535] every call
    lfsr = (lfsr >> 1u) ^ (-(lfsr & 1u) & 0xB400u);

    // Scale to [-50, +50]
    signedNoise = (int)(lfsr % 101u) - 50;
    noise       = (float)signedNoise;

    // --- Combine and saturate ---
    signal += noise;

    if (signal < 0.0F)
        signal = 0.0F;
    if (signal > (float)ADC_MAX_VALUE)
        signal = (float)ADC_MAX_VALUE;

    sampleCount++;

    return (unsigned int)signal;
}