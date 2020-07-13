clc;
clear all;
close all;
%%%========================================================================
% Генерация сигнала, представляющего собой последовательность ЛЧМ импульсов
% с задаными параметрами. Сгенерированный сигнал записывается в файл
% test_pulse_signal.bin 
%%%========================================================================
% Параметры сигнала
sampleFreq  = 360e6;            % Частота дискретизации, Гц
lfmBand     = 50e6;             % Девиация частоты, Гц
ampl        = 2^12;             % Амлитуда импульсов в отсчетах, max = 2^15-1
pulseDur    = 2e-6;             % Длительность импульса, с
pulsePeriod = 20e-6;            % Период повторения импульсов, с
numPulses   = 7;                % Количество импульсов в сигнале
pulseDelay  = 0;                % Задержка первого импульса, с
rotPhase    = deg2rad(-10);     % Набег фазы между импульсами, рад
SNR         = 20;               % Отношение сигнал/шум генерируемого сигнала, дБ

[testSig_I, testSig_Q] = SignalGenerator(sampleFreq, ...
                                            lfmBand, ...
                                            ampl, ...
                                            pulseDur, ...
                                            pulsePeriod, ...
                                            numPulses, ...
                                            pulseDelay, ...
                                            rotPhase, ...
                                            SNR);
                                        
figure(1)
hold on
plot(testSig_I, 'k')
plot(testSig_Q, 'r')
hold off

fullSig = reshape([testSig_I; testSig_Q], 1, []);

% Записываем файл
outFile = 'test_pulse_signal.bin';
fOut = fopen(outFile, 'w');
% Заголовок файла
fwrite(fOut, int32(sampleFreq), 'int32');
fwrite(fOut, int32(lfmBand), 'int32');
fwrite(fOut, int32(ampl), 'int32');
fwrite(fOut, int32(pulseDur * 1e9), 'int32');%fwrite(fOut, int32(pulseDur), 'int32');
fwrite(fOut, int32(pulsePeriod * 1e9), 'int32');%fwrite(fOut, int32(pulsePeriod), 'int32');
fwrite(fOut, int32(numPulses), 'int32');
fwrite(fOut, int32(pulseDelay), 'int32');
fwrite(fOut, int32(rotPhase), 'int32');
fwrite(fOut, int32(SNR), 'int32');
% Размер сигнала в байтах
fwrite(fOut, uint32(length(fullSig) * 2), 'uint32');
% Сигнал
fwrite(fOut, fullSig, 'int16');

fclose(fOut);

