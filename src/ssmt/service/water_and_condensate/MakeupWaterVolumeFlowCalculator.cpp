#include "ssmt/service/water-and-condensate/MakeupWaterVolumeFlowCalculator.h"

MakeupWaterVolumeFlowCalculationsDomain
MakeupWaterVolumeFlowCalculator::calc(const SteamSystemModelerTool::FluidProperties &makeupWaterAndMassFlow,
                                      const OperationsInput &operationsInput) const {
    const double makeupWaterVolumeFlow = calcMakeupWaterVolumeFlow(makeupWaterAndMassFlow);

    const double operatingHoursPerYear = operationsInput.getOperatingHoursPerYear();
    const double makeupWaterVolumeFlowAnnual =
            calcMakeupWaterVolumeFlowAnnual(makeupWaterVolumeFlow, operatingHoursPerYear);

    return {makeupWaterVolumeFlow, makeupWaterVolumeFlowAnnual};
}

/** Calculate volume flow in kg/hr. */
double MakeupWaterVolumeFlowCalculator::calcMakeupWaterVolumeFlow(
        const SteamSystemModelerTool::FluidProperties &makeupWaterAndMassFlow) const {
    const std::string methodName = std::string("MakeupWaterVolumeFlowCalculator::") + std::string(__func__) + ": ";

    const double massFlow = makeupWaterAndMassFlow.massFlow;
    const double specificVolume = makeupWaterAndMassFlow.specificVolume;

    const double volumeFlow = specificVolume * massFlow;

    // std::cout << methodName << "massFlow=" << massFlow << ", specificVolume" << specificVolume <<
    //           ": result=" << volumeFlow << std::endl;

    return volumeFlow;
}

double MakeupWaterVolumeFlowCalculator::calcMakeupWaterVolumeFlowAnnual(const double makeupWaterVolumeFlow,
                                                                        const double operatingHoursPerYear) const {
    const std::string methodName = std::string("MakeupWaterVolumeFlowCalculator::") + std::string(__func__) + ": ";

    const double volumeFlowAnnual = makeupWaterVolumeFlow * operatingHoursPerYear;

    // std::cout << methodName << "makeupWaterVolumeFlow=" << makeupWaterVolumeFlow <<
    //           ", operatingHoursPerYear=" << operatingHoursPerYear << ": result=" << volumeFlowAnnual << std::endl;

    return volumeFlowAnnual;
}
