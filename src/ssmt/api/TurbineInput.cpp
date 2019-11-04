#include <ssmt/api/TurbineInput.h>
#include <string>

TurbineInput::TurbineInput(const CondensingTurbine &condensingTurbine, const PressureTurbine &highToLowTurbine,
                           const PressureTurbine &highToMediumTurbine, const PressureTurbine &mediumToLowTurbine)
        : condensingTurbine(condensingTurbine), highToLowTurbine(highToLowTurbine),
          highToMediumTurbine(highToMediumTurbine), mediumToLowTurbine(mediumToLowTurbine) {}

std::ostream &operator<<(std::ostream &stream, const CondensingTurbineOperation &operation) {
    stream << static_cast< int >( operation );

    switch (operation) {
        case CondensingTurbineOperation::POWER_GENERATION:
            stream << "-POWER_GENERATION";
            break;
        case CondensingTurbineOperation::STEAM_FLOW:
            stream << "-STEAM_FLOW";
            break;
        default:
            std::string msg = "CondensingTurbineOperation::operator<<: operator enum not handled";
            // std::cout << msg << std::endl;
            throw std::invalid_argument(msg);
    }

    return stream;
}

std::ostream &operator<<(std::ostream &stream, const CondensingTurbine &ct) {
    return stream << "CondensingTurbine["
                  << "isentropicEfficiency=" << ct.isentropicEfficiency
                  << ", generationEfficiency=" << ct.generationEfficiency
                  << ", condenserPressure=" << ct.condenserPressure
                  << ", operationType=" << ct.operationType
                  << ", operationValue=" << ct.operationValue
                  << ", useTurbine=" << ct.useTurbine << "]";
}

std::ostream &operator<<(std::ostream &stream, const PressureTurbineOperation &operation) {
    stream << static_cast< int >( operation );

    switch (operation) {
        case PressureTurbineOperation::BALANCE_HEADER:
            stream << "-BALANCE_HEADER";
            break;
        case PressureTurbineOperation::FLOW_RANGE:
            stream << "-FLOW_RANGE";
            break;
        case PressureTurbineOperation::POWER_GENERATION:
            stream << "-POWER_GENERATION";
            break;
        case PressureTurbineOperation::POWER_RANGE:
            stream << "-POWER_RANGE";
            break;
        case PressureTurbineOperation::STEAM_FLOW:
            stream << "-STEAM_FLOW";
            break;
        default:
            std::string msg = "CondensingTurbineOperation::operator<<: operator enum not handled";
            // std::cout << msg << std::endl;
            throw std::invalid_argument(msg);
    }

    return stream;
}

std::ostream &operator<<(std::ostream &stream, const PressureTurbine &pt) {
    return stream << "PressureTurbine["
                  << "isentropicEfficiency=" << pt.isentropicEfficiency
                  << ", generationEfficiency=" << pt.generationEfficiency
                  << ", operationType=" << pt.operationType
                  << ", operationValue1=" << pt.operationValue1
                  << ", operationValue2=" << pt.operationValue2
                  << ", useTurbine=" << pt.useTurbine << "]";
}

std::ostream &operator<<(std::ostream &stream, const TurbineInput &turbineInput) {
    return stream << "TurbineInput["
                  << "condensingTurbine=" << turbineInput.condensingTurbine
                  << ", highToLowTurbine=" << turbineInput.highToLowTurbine
                  << ", highToMediumTurbine=" << turbineInput.highToMediumTurbine
                  << ", mediumToLowTurbine=" << turbineInput.mediumToLowTurbine << "]";
}

CondensingTurbine TurbineInput::getCondensingTurbine() const {
    return condensingTurbine;
}

PressureTurbine TurbineInput::getHighToLowTurbine() const {
    return highToLowTurbine;
}

PressureTurbine TurbineInput::getHighToMediumTurbine() const {
    return highToMediumTurbine;
}

PressureTurbine TurbineInput::getMediumToLowTurbine() const {
    return mediumToLowTurbine;
}

PressureTurbine::PressureTurbine(double isentropicEfficiency, double generationEfficiency,
                                 PressureTurbineOperation operationType, double operationValue1, double operationValue2,
                                 bool useTurbine)
        : isentropicEfficiency(isentropicEfficiency), generationEfficiency(generationEfficiency),
          operationType(operationType), operationValue1(operationValue1), operationValue2(operationValue2),
          useTurbine(useTurbine) {
}

double PressureTurbine::getIsentropicEfficiency() const {
    return isentropicEfficiency;
}

double PressureTurbine::getGenerationEfficiency() const {
    return generationEfficiency;
}

PressureTurbineOperation PressureTurbine::getOperationType() const {
    return operationType;
}

double PressureTurbine::getOperationValue1() const {
    return operationValue1;
}

double PressureTurbine::getOperationValue2() const {
    return operationValue2;
}

bool PressureTurbine::isUseTurbine() const {
    return useTurbine;
}

CondensingTurbine::CondensingTurbine(double isentropicEfficiency, double generationEfficiency, double condenserPressure,
                                     CondensingTurbineOperation operationType, double operationValue, bool useTurbine)
        : isentropicEfficiency(isentropicEfficiency), generationEfficiency(generationEfficiency),
          condenserPressure(condenserPressure), operationType(operationType), operationValue(operationValue),
          useTurbine(useTurbine) {
}

double CondensingTurbine::getIsentropicEfficiency() const {
    return isentropicEfficiency;
}

double CondensingTurbine::getGenerationEfficiency() const {
    return generationEfficiency;
}

double CondensingTurbine::getCondenserPressure() const {
    return condenserPressure;
}

CondensingTurbineOperation CondensingTurbine::getOperationType() const {
    return operationType;
}

double CondensingTurbine::getOperationValue() const {
    return operationValue;
}

bool CondensingTurbine::isUseTurbine() const {
    return useTurbine;
}
