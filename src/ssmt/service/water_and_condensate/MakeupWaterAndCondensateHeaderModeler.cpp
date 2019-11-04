#include "ssmt/service/water_and_condensate/MakeupWaterAndCondensateHeaderModeler.h"

MakeupWaterAndCondensateHeaderCalculationsDomain
MakeupWaterAndCondensateHeaderModeler::model(const int headerCountInput,
                                             const HeaderWithHighestPressure &highPressureHeaderInput,
                                             const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                             const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                             const BoilerInput &boilerInput, const OperationsInput &operationsInput,
                                             const CondensingTurbine &condensingTurbineInput, const Boiler &boiler,
                                             const std::shared_ptr<FlashTank> &blowdownFlashTank,
                                             const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                                             const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                                             const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    const std::string methodName =
            std::string("MakeupWaterAndCondensateHeaderModeler::") + std::string(__func__) + ": ";

    //5A. Calculate Combined Return Condensate
//     std::cout << methodName << "calculating combinedCondensateHeader" << std::endl;
    const Header &combinedCondensateHeader =
            combinedCondensateCalculator.calc(headerCountInput, highPressureHeaderCalculationsDomain,
                                              mediumPressureHeaderCalculationsDomain,
                                              lowPressureHeaderCalculationsDomain);
//     std::cout << methodName << "combinedCondensateHeader=" << combinedCondensateHeader << std::endl;

//     std::cout << methodName << "calculating combinedCondensate" << std::endl;
    const SteamSystemModelerTool::FluidProperties &combinedCondensate =
            fluidPropertiesFactory.make(combinedCondensateHeader);
//     std::cout << methodName << "combinedCondensate=" << combinedCondensate << std::endl;

    //5B. Calculate return condensate
//     std::cout << methodName << "calculating returnCondensate" << std::endl;
    const SteamSystemModelerTool::FluidProperties &returnCondensate =
            returnCondensateCalculator.calc(highPressureHeaderInput, combinedCondensateHeader);
//     std::cout << methodName << "returnCondensate=" << returnCondensate << std::endl;

    //5C. Flash return condensate if selected
//     std::cout << methodName << "calculating flash returnCondensate" << std::endl;
    const ReturnCondensateCalculationsDomain &returnCondensateCalculationsDomain =
            returnCondensateCalculator.flash(highPressureHeaderInput, boilerInput, returnCondensate);
    const SteamSystemModelerTool::FluidProperties returnCondensateFlashed =
            returnCondensateCalculationsDomain.returnCondensateFlashed;
//     std::cout << methodName << "returnCondensateCalculationsDomain=" << returnCondensateCalculationsDomain << std::endl;

    //5D. Calculate Makeup Water
//     std::cout << methodName << "calculating makeupWaterOnly" << std::endl;
    const SteamSystemModelerTool::SteamPropertiesOutput &makeupWaterOnly = makeupWaterCalculator.calc(operationsInput);
//     std::cout << methodName << "makeupWaterOnly=" << makeupWaterOnly << std::endl;

    //5E. Calculate makeup water mass flow
//     std::cout << methodName << "calculating makeupWater" << std::endl;
    const double lowPressureVentedSteam = 0;  // don't have a value yet
    const SteamSystemModelerTool::FluidProperties &makeupWater =
            makeupWaterMassFlowCalculator.calc(headerCountInput, highPressureHeaderInput, mediumPressureHeaderInput,
                                               lowPressureHeaderInput, condensingTurbineInput, boilerInput, boiler,
                                               returnCondensateFlashed, makeupWaterOnly,
                                               highPressureHeaderCalculationsDomain,
                                               mediumPressureHeaderCalculationsDomain,
                                               lowPressureHeaderCalculationsDomain, lowPressureVentedSteam);
//     std::cout << methodName << "makeupWater=" << makeupWater << std::endl;

//     std::cout << methodName << "calculating makeupWaterVolumeFlow" << std::endl;
    const MakeupWaterVolumeFlowCalculationsDomain &makeupWaterVolumeFlowCalculationsDomain =
            makeupWaterVolumeFlowCalculator.calc(makeupWater, operationsInput);
//     std::cout << methodName << "makeupWaterVolumeFlowCalculationsDomain=" << makeupWaterVolumeFlowCalculationsDomain
        //       << std::endl;

    //5F. Run heat exchange if pre heating makeup water
//     std::cout << methodName << "calculating heatExchangerOutput" << std::endl;
    std::shared_ptr<HeatExchanger::Output> heatExchangerOutput =
            heatExchangerCalculator.calc(boilerInput, boiler, makeupWater, blowdownFlashTank);
//     std::cout << methodName << "heatExchangerOutput=" << heatExchangerOutput << std::endl;

    //5G. Calculate makeup water and condensate combined
//     std::cout << methodName << "calculating makeupWaterAndCondensateHeaderOutput" << std::endl;
    const SteamSystemModelerTool::FluidProperties &makeupWaterAndCondensateHeaderOutput =
            makeupWaterAndCondensateHeaderCalculator.calc(boilerInput, condensingTurbineInput, returnCondensateFlashed,
                                                          heatExchangerOutput, makeupWater,
                                                          highPressureHeaderCalculationsDomain);
//     std::cout << methodName << "makeupWaterAndCondensateHeaderOutput=" << makeupWaterAndCondensateHeaderOutput
        //       << std::endl;

    return {combinedCondensate, returnCondensateFlashed, returnCondensateCalculationsDomain, makeupWater,
            makeupWaterVolumeFlowCalculationsDomain, heatExchangerOutput, makeupWaterAndCondensateHeaderOutput};
}
