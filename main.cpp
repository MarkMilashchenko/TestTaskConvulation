#include <iostream>
#include <chrono>
#include "filter.h"

int main() {
    auto time_point1 = std::chrono::steady_clock::now();
    std::ifstream input("test_pulse_signal.bin", std::ios::binary | std::ios::in);
    if (!input) {
        return 1;
    }

    int32_t signalProp[10];
    for (size_t i = 0; i < 10; ++i) {
        input.read((char*)(signalProp + i), sizeof(int32_t));
    }

    int32_t N_signal = signalProp[SIGNAL_SIZE] / 4;
    fftw_complex* inputSignal = getInputSignal(input, N_signal);
    input.close();

    int32_t sampleFreq = signalProp[SAMPLE_FREQ];
    int32_t duration = signalProp[DURATION];
    int32_t N_IRF = duration * ((double)sampleFreq / 1e9);
    fftw_complex* IRF = createIRF(signalProp, N_signal, N_IRF);

    auto time_point2 = std::chrono::steady_clock::now();
    auto time = time_point2 - time_point1;
    std::cout << "Loading data time: " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << " micro_s\n";

    fftw_complex* convRes = Conv(IRF, N_IRF, inputSignal, N_signal);
    int32_t convSize = N_signal + N_IRF - 1;

    auto time_point3 = std::chrono::steady_clock::now();
    time = time_point3 - time_point2;
    std::cout << "Conv Calculation time (Slow): " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << " micro_s\n";

    fastConv(IRF, N_IRF, inputSignal, N_signal);

    auto time_point4 = std::chrono::steady_clock::now();
    time = time_point4 - time_point3;
    std::cout << "Conv Calculation time (Fast): " << std::chrono::duration_cast<std::chrono::microseconds>(time).count() << " micro_s\n";

    double normCoef = getNormCoef(convRes, convSize);
    saveResults("direct_conv.bin", convRes, convSize, signalProp, normCoef);

    normCoef = getNormCoef(inputSignal, N_signal);
    saveResults("fast_conv.bin", inputSignal, N_signal, signalProp, normCoef);

    fftw_free(inputSignal);
    fftw_free(IRF);
    fftw_free(convRes);
    
    return 0;
}