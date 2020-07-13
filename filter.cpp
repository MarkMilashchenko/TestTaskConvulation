#include "filter.h"

double getNormCoef(fftw_complex* data, int32_t N) {
    double coeff = 1.;
    double buff = 0.;
    for (int32_t i = 0; i < N; ++i) {
        buff = fmax(fabs(data[i][0]), fabs(data[i][1]));
        if (coeff < buff) {
            coeff = buff;
        }
    }
    return coeff;
}

fftw_complex* getInputSignal(std::ifstream& input, int32_t N) {
    fftw_complex* res = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * N);
    input.seekg(10 * sizeof(int32_t), std::ios::beg);
    
    int16_t buff = 0;
    for (int32_t i = 0; i < N; ++i) {
        input.read((char*)(&buff), sizeof(int16_t));
        res[i][0] = buff;
        input.read((char*)(&buff), sizeof(int16_t));
        res[i][1] = buff;
    }
    return res;
}

fftw_complex* createIRF(int32_t* signalProp, int32_t totalSize, int32_t N) {
    fftw_complex* res = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * totalSize);

    int32_t sampleFreq = signalProp[SAMPLE_FREQ];
    double lfmBand = signalProp[LFM_BAND];
    double signalDuration = (double)signalProp[DURATION] / 1e9;

    double Tn = 0, dt = 0, arg = 0;
    double ampl = (double)signalProp[AMPL];
    for (int32_t i = 0; i < N; ++i) {
        Tn = (double)i / sampleFreq;
        dt = (signalDuration - Tn);
        arg = 3.14 * dt * lfmBand * (-1. + dt / signalDuration);
        res[i][0] = ampl * cos(arg);
        res[i][1] = ampl * sin(arg);
    }
    return res;
}

fftw_complex* Conv(fftw_complex* F1, int32_t N1, fftw_complex* F2, int32_t N2) {
    if (N1 > N2) {
        std::swap(F1, F2);
        std::swap(N1, N2);
    }

    int32_t size = N1 + N2 - 1;
    fftw_complex* res = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * size);

    int32_t idx = 0;
    int32_t i = 0;
    int32_t j = 0;
    for (i = 0; i < N1 - 1; ++i) {
        for (j = i; j >= 0; --j) {
            idx = i - j;
            res[i][0] += (F1[j][0] * F2[idx][0] - F1[j][1] * F2[idx][1]);
            res[i][1] += (F1[j][0] * F2[idx][1] + F1[j][1] * F2[idx][0]);
        }
    }

    for (i = N1 - 1; i < N2; ++i) {
        for (j = 0; j < N1; ++j) {
            idx = i - j;
            res[i][0] += (F1[j][0] * F2[idx][0] - F1[j][1] * F2[idx][1]);
            res[i][1] += (F1[j][0] * F2[idx][1] + F1[j][1] * F2[idx][0]);
        }
    }

    for (i = N2; i < size; ++i) {
        for (j = i - N2 + 1; j < N1; ++j) {
            idx = i - j;
            res[i][0] += (F1[j][0] * F2[idx][0] - F1[j][1] * F2[idx][1]);
            res[i][1] += (F1[j][0] * F2[idx][1] + F1[j][1] * F2[idx][0]);
        }
    }
    return res;
}

void fastConv(fftw_complex* F1, int32_t N1, fftw_complex* F2, int32_t N2) {
    if (N1 > N2) {
        std::swap(F1, F2);
        std::swap(N1, N2);
    }

    fftw_plan p = fftw_plan_dft_1d(N2, F1, F1, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);
    p = fftw_plan_dft_1d(N2, F2, F2, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    double real = 0., img = 0.;
    for (int32_t i = 0; i < N2; ++i) {
        real = (F1[i][0] * F2[i][0] - F1[i][1] * F2[i][1]);
        img = (F1[i][0] * F2[i][1] + F1[i][1] * F2[i][0]);
        F2[i][0] = real;
        F2[i][1] = img;
    }

    p = fftw_plan_dft_1d(N2, F2, F2, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    fftw_destroy_plan(p);
}

void saveResults(std::string const& filename, fftw_complex* F, int32_t N, int32_t* baseSignalProp, double const& normCoef) {
    std::ofstream output(filename, std::ios::binary | std::ios::out);
    if (!output) {
        return;
    }
    int32_t buff = 0;   
    for (int32_t i = 0; i < 9; ++i) {
        buff = (int32_t)baseSignalProp[i];
        output.write((char*)&buff, sizeof(int32_t));
    }
    int32_t byteSize = N * 4;
    output.write((char*)&byteSize, sizeof(int32_t));

    int16_t buff_16 = 0;
    double maxAmpl = pow(2, 15) / normCoef; 
    for (int32_t i = 0; i < N; ++i) {
        buff_16 = (int16_t)(F[i][0] * maxAmpl);
        output.write((char*)(&buff_16), sizeof(int16_t));
        buff_16 = (int16_t)(F[i][1] * maxAmpl);
        output.write((char*)(&buff_16), sizeof(int16_t));
    }

    output.close();
}