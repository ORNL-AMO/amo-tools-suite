#include <nan.h>
#include "calculator.h"

using namespace Nan;

NAN_MODULE_INIT(InitCalculator) {
    Nan::Set(target, New<String>("electricityReduction").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(electricityReduction)).ToLocalChecked());

    Nan::Set(target, New<String>("naturalGasReduction").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(naturalGasReduction)).ToLocalChecked());

    Nan::Set(target, New<String>("compressedAirReduction").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(compressedAirReduction)).ToLocalChecked());
}

NODE_MODULE(calculator, InitCalculator)