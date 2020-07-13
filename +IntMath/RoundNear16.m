function outSig = RoundNear16(inSig, bitness)
%ROUNDNEAR16 функция целочисленно округляет на разрядность bitness
% выход int16
MASK    = int32(1);
outSig  = bitshift(inSig, -bitness, 'int32') + ...
            bitand(bitshift(inSig, -bitness + 1, 'int32'), MASK);
[row, col]  = find(outSig > intmax('int16') | outSig < intmin('int16'));
if ~isempty(row)
    outSig(row, col) = bitshift(bitshift(outSig(row, col), 16), -16);
    warning('RoundNear16(), out of range');
end
outSig  = int16(outSig);
end

