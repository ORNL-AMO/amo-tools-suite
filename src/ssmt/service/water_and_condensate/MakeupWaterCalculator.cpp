#include "ssmt/service/water_and_condensate/MakeupWaterCalculator.h"
#include <ssmt/SteamProperties.h>

SteamSystemModelerTool::SteamPropertiesOutput
MakeupWaterCalculator::calc(const OperationsInput &operationsInput) const {
    std::cout << "MakeupWaterCalculator::calc: calculating makeupWater" << std::endl;
    const double pressure = .101325; //atmospheric pressure
    const SteamProperties::ThermodynamicQuantity quantity = SteamProperties::ThermodynamicQuantity::TEMPERATURE;
    const double quantityValue = operationsInput.getMakeUpWaterTemperature();

    SteamProperties makeupWaterProperties = {pressure, quantity, quantityValue};

    return makeupWaterProperties.calculate();
}
