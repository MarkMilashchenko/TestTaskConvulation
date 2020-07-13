function [outSig_I, outSig_Q] = SignalGenerator(sampleFreq, ...
                                                lfmBand, ...
                                                ampl, ...
                                                pulseDur, ...
                                                pulsePeriod, ...
                                                numPulses, ...
                                                pulseDelay, ...
                                                rotPhase, ...
                                                SNR)
%%%========================================================================
time        = 0 : 1 / sampleFreq : (pulseDur - 1 / sampleFreq);

lfmBase     = ampl * exp(1i * 2 * pi * time .* ...
                    (-lfmBand / 2 + time .* lfmBand / ...
                    (2 * pulseDur)));

lfmBase_I   = int16(real(lfmBase));
lfmBase_Q   = int16(imag(lfmBase));

%%%========================================================================
rotVecBitn  = 14;
rotVec_I    = int16(2 ^ rotVecBitn * cos(rotPhase));
rotVec_Q    = int16(2 ^ rotVecBitn * sin(rotPhase));

% init buffer for rotation vector
rotBuf_I    = int16(2 ^ rotVecBitn * cos(0));
rotBuf_Q    = int16(2 ^ rotVecBitn * sin(0));

outSigSize  = round(pulsePeriod * numPulses * sampleFreq);
outSig_I    = int16(zeros(1, outSigSize));
outSig_Q    = int16(zeros(1, outSigSize));

% cycle for pulses
for nPul = 1 : numPulses
    % find position in signal
    startPulseInd   = round(((nPul - 1) * pulsePeriod + ...
                        pulseDelay + 1 / sampleFreq) * ...
                        sampleFreq);
    pulseInd        = startPulseInd : startPulseInd + length(lfmBase) - 1;
    % calc rotated pulse ans insert its to out signal
    [outSig_I(pulseInd), outSig_Q(pulseInd)] = ...
        IntMath.PhaseRotator(lfmBase_I, lfmBase_Q, rotBuf_I, rotBuf_Q, ...
                                rotVecBitn);
    % update rotBuf
    [rotBuf_I, rotBuf_Q]    = ...
        IntMath.PhaseRotator(rotBuf_I, rotBuf_Q, rotVec_I, rotVec_Q, ...
                                rotVecBitn);
end

% Add noise
noiseAmpl   = ampl / db2mag(SNR);
outSig_I    = outSig_I + int16(noiseAmpl * randn(size(outSig_I)));
outSig_Q    = outSig_Q + int16(noiseAmpl * randn(size(outSig_I)));
end