function outSig = RoundNear32(inSig, bitness)
%ROUNDNEAR32 функция целочисленно округляет на разрядность bitness
% выход int32
if ~isa(inSig, 'int32')
    error('RoundNear32(), input must be int32')
end
MASK    = int32(1);
outSig  = bitshift(inSig, -bitness, 'int32') + ...
            bitand(bitshift(inSig, -bitness + 1, 'int32'), MASK);
outSig  = int32(outSig);
end

