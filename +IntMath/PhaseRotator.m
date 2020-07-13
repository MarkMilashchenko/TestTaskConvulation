function [result_I, result_Q] = ...
    PhaseRotator(inVec_I, inVec_Q, rotVec_I, rotVec_Q, rotVecBitn)
%PHASESHIFTER rotate inVec on rotVec with round on rotVecBitness
inVec_I         = int32(inVec_I);
inVec_Q         = int32(inVec_Q);
rotVec_I        = int32(rotVec_I);
rotVec_Q        = int32(rotVec_Q);
% complex multiplication with rounding
result_I        = IntMath.RoundNear16(inVec_I * rotVec_I - ...
                                        inVec_Q * rotVec_Q, rotVecBitn);
result_Q        = IntMath.RoundNear16(inVec_Q * rotVec_I + ...
                                        inVec_I * rotVec_Q, rotVecBitn);
end

