function result = Corr32(inSig, refFun)
%CORR32 Целочисленная корреляция сигнала с опорной функцией
inSig       = double(inSig);
refFun      = double(refFun);
result      = int32(conv(inSig, refFun, 'same'));
end

