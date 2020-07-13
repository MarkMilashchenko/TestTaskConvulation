#pragma once

#include <fstream>
#include <cmath>
#include <string>
#include <fftw3.h>

enum SIGNAL_PROPORTIES {
    SAMPLE_FREQ,
    LFM_BAND,
    AMPL,
    DURATION, // нано
    PERIOD, // нано
    PULSES_NUM,
    PULSE_DELAY,
    ROT_PHASE,
    SNR,
    SIGNAL_SIZE
};

double getNormCoef(fftw_complex* data, int32_t N);

fftw_complex* getInputSignal(std::ifstream& input, int32_t N);

fftw_complex* createIRF(int32_t* signalProp, int32_t totalSize, int32_t N);

fftw_complex* Conv(fftw_complex* F1, int32_t N1, fftw_complex* F2, int32_t N2);

void fastConv(fftw_complex* F1, int32_t N1, fftw_complex* F2, int32_t N2);

void saveResults(std::string const& filename, fftw_complex* F, int32_t N, int32_t* baseSignalProp, double const& normCoef);


