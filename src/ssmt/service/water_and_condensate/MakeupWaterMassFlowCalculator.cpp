#include "ssmt/service/water_and_condensate/MakeupWaterMassFlowCalculator.h"

SteamSystemModelerTool::FluidProperties
MakeupWaterMassFlowCalculator::calc(const int headerCountInput,
                                    const HeaderWithHighestPressure &highPressureHeaderInput,
                                    const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                    const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                    const CondensingTurbine &condensingTurbineInput, const BoilerInput &boilerInput,
                                    const Boiler &boiler,
                                    const SteamSystemModelerTool::FluidProperties &returnCondensate,
                                    const SteamSystemModelerTool::SteamPropertiesOutput &makeupWaterOutput,
                                    const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                                    const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                                    const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain,
                                    const double lowPressureVentedSteam) const {
    const std::string methodName = std::string("MakeupWaterMassFlowCalculator::") + std::string(__func__) + ": ";

    // std::cout << methodName << "calculating inletHeaderFlow" << std::endl;
    const double inletHeaderFlow =
            calcInletHeaderFlow(headerCountInput, highPressureHeaderInput, lowPressureHeaderInput,
                                highPressureHeaderCalculationsDomain, lowPressureHeaderCalculationsDomain);

    // std::cout << methodName << "calculating makeupWaterMassFlow" << std::endl;
    const double makeupWaterMassFlow =
            calcMakeupWaterMassFlow(headerCountInput, mediumPressureHeaderInput, lowPressureHeaderInput,
                                    condensingTurbineInput, boilerInput, boiler, returnCondensate, inletHeaderFlow,
                                    highPressureHeaderCalculationsDomain, mediumPressureHeaderCalculationsDomain,
                                    lowPressureHeaderCalculationsDomain, lowPressureVentedSteam);

    // std::cout << methodName << "calculating energyFlow" << std::endl;
    const double energyFlow = calcMakeupWaterEnergyFlow(makeupWaterMassFlow, makeupWaterOutput);

    return {makeupWaterMassFlow, energyFlow, makeupWaterOutput};
}

double MakeupWaterMassFlowCalculator::calcInletHeaderFlow(const int headerCountInput,
                                                          const HeaderWithHighestPressure &highPressureHeaderInput,
                                                          const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                          const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                                                          const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("MakeupWaterMassFlowCalculator::") + std::string(__func__) + ": ";

    if (headerCountInput == 1) {
        // std::cout << methodName << "only 1 header, calculating inletHeaderFlow from high pressure" << std::endl;
        const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput =
                highPressureHeaderCalculationsDomain.highPressureHeaderOutput;
        return calcInletHeaderFlow(highPressureHeaderOutput, highPressureHeaderInput);
    } else {
        // std::cout << methodName << "multiple headers, calculating inletHeaderFlow from low pressure" << std::endl;
        const SteamSystemModelerTool::FluidProperties &lowPressureHeaderOutput =
                lowPressureHeaderCalculationsDomain->lowPressureHeaderOutput;
        return calcInletHeaderFlow(lowPressureHeaderOutput, lowPressureHeaderInput);
    }
}

double MakeupWaterMassFlowCalculator::calcInletHeaderFlow(
        const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
        const HeaderWithHighestPressure &highPressureHeaderInput) const {
    const std::string methodName = std::string("MakeupWaterMassFlowCalculator::") + std::string(__func__) + ": ";

    const double massFlow = highPressureHeaderOutput.massFlow;
    const double processSteamUsage = highPressureHeaderInput.getProcessSteamUsage();
    const double result = massFlow - processSteamUsage;

    // std::cout << methodName << "massFlow=" << massFlow << " - processSteamUsage=" << processSteamUsage
              //<< ": result=" << result << std::endl;

    return result;
}

double MakeupWaterMassFlowCalculator::calcInletHeaderFlow(
        const SteamSystemModelerTool::FluidProperties &lowPressureHeaderOutput,
        const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput) const {
    const std::string methodName = std::string("MakeupWaterMassFlowCalculator::") + std::string(__func__) + ": ";

    const double massFlow = lowPressureHeaderOutput.massFlow;
    const double processSteamUsage = lowPressureHeaderInput->getProcessSteamUsage();
    const double result = massFlow - processSteamUsage;

    // std::cout << methodName << "massFlow=" << massFlow << " - processSteamUsage=" << processSteamUsage
             // << ": result=" << result << std::endl;

    return result;
}

double
MakeupWaterMassFlowCalculator::calcMakeupWaterMassFlow(const int headerCountInput,
                                                       const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                                       const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                       const CondensingTurbine &condensingTurbineInput,
                                                       const BoilerInput &boilerInput, const Boiler &boiler,
                                                       const SteamSystemModelerTool::FluidProperties &returnCondensate,
                                                       const double inletHeaderFlow,
                                                       const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                                                       const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                                                       const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain,
                                                       const double lowPressureVentedSteam) const {
    const std::string methodName = std::string("MakeupWaterMassFlowCalculator::") + std::string(__func__) + ": ";

    double makeupWaterMassFlow = calcMakeupWaterMassFlow(boilerInput, boiler);

    if (headerCountInput > 1) {
        // std::cout << methodName << "multiple headers, adjusting makeupWaterMassFlow" << std::endl;

        if (headerCountInput == 3 && mediumPressureHeaderInput->isDesuperheatSteamIntoNextHighest()) {
            // std::cout << methodName
                    //   << "mediumPressureHeaderInput.isDesuperheatSteamIntoNextHighest,"
                    //   << " adding highToMediumPressurePrv feedwaterMassFlow to makeupWaterMassFlow" << std::endl;
            const std::shared_ptr<PrvWithoutDesuperheating> &highToMediumPressurePrv =
                    mediumPressureHeaderCalculationsDomain->highToMediumPressurePrv;
            makeupWaterMassFlow =
                    addPrvFeedwaterMassFlowToMakeupWaterMassFlow(highToMediumPressurePrv, makeupWaterMassFlow);
        } else {
            // std::cout << methodName
                    //   << "not 3 headers and not mediumPressureHeaderInput.isDesuperheatSteamIntoNextHighest,"
                    //   << " skipping highToMediumPressurePrv feedwaterMassFlow"
                    //   << std::endl;
        }

        if (lowPressureHeaderInput->isDesuperheatSteamIntoNextHighest()) {
            // std::cout << methodName
                    //   << "lowPressureHeaderInput.isDesuperheatSteamIntoNextHighest,"
                    //   << " adding lowPressurePrv feedwaterMassFlow to makeupWaterMassFlow" << std::endl;
            const std::shared_ptr<PrvWithoutDesuperheating> &lowPressurePrv =
                    lowPressureHeaderCalculationsDomain->lowPressurePrv;
            makeupWaterMassFlow =
                    addPrvFeedwaterMassFlowToMakeupWaterMassFlow(lowPressurePrv, makeupWaterMassFlow);
        } else {
            // std::cout << methodName
                    //   << "not lowPressureHeaderInput.isDesuperheatSteamIntoNextHighest,"
                    //   << " skipping lowPressurePrv feedwaterMassFlow"
                    //   << std::endl;
        }

        if (condensingTurbineInput.isUseTurbine()) {
            // std::cout << methodName
                  //    << "condensingTurbine exists, subtracting its mass flow from makeupWaterMassFlow" << std::endl;
            const std::shared_ptr<Turbine> &condensingTurbine =
                    highPressureHeaderCalculationsDomain.condensingTurbine;
            const double massFlow = condensingTurbine->getMassFlow();
            makeupWaterMassFlow -= massFlow;
            // std::cout << methodName << "massFlow=" << massFlow << ": result=" << makeupWaterMassFlow << std::endl;
        } else {
            // std::cout << methodName
                    //   << "not condensingTurbineInput.isUseTurbine,"
                    //   << " skipping condensingTurbine massFlow"
                    //   << std::endl;
        }
    }

    const double returnCondensateMassFlow = returnCondensate.massFlow;
    const double result = makeupWaterMassFlow - returnCondensateMassFlow - inletHeaderFlow + lowPressureVentedSteam;
    // std::cout << methodName << "makeupWaterMassFlow=" << makeupWaterMassFlow
            //   << " - returnCondensateMassFlow=" << returnCondensateMassFlow
            //   << " - inletHeaderFlow=" << inletHeaderFlow
            //   << " + lowPressureVentedSteam=" << lowPressureVentedSteam
            //   << ": result=" << result
            //   << std::endl;

    return result;
}

double
MakeupWaterMassFlowCalculator::calcMakeupWaterMassFlow(const BoilerInput &boilerInput, const Boiler &boiler) const {
    const std::string methodName = std::string("MakeupWaterMassFlowCalculator::") + std::string(__func__) + ": ";

    const double massFlow = boiler.getFeedwaterProperties().massFlow;
    const double deaeratorVentRate = boilerInput.getDeaeratorVentRate();
    const double result = massFlow * (1 + deaeratorVentRate / 100);

    // std::cout << methodName << "massFlow=" << massFlow << " * (1 + deaeratorVentRate=" << deaeratorVentRate
            //   << " / 100): result=" << result << std::endl;

    return result;
}

double
MakeupWaterMassFlowCalculator::addPrvFeedwaterMassFlowToMakeupWaterMassFlow(
        const std::shared_ptr<PrvWithoutDesuperheating> &prv, double makeupWaterMassFlow) const {
    const std::string methodName = std::string("MakeupWaterMassFlowCalculator::") + std::string(__func__) + ": ";

    const double feedwaterMassFlow = getFeedwaterMassFlow(prv);
    const double result = makeupWaterMassFlow + feedwaterMassFlow;

    // std::cout << methodName << "makeupWaterMassFlow=" << makeupWaterMassFlow
            //   << " + feedwaterMassFlow=" << feedwaterMassFlow << ": result=" << result << std::endl;

    return result;
}

double
MakeupWaterMassFlowCalculator::getFeedwaterMassFlow(const std::shared_ptr<PrvWithoutDesuperheating> &prv) const {
    double massFlow = 0;

    if (prv != nullptr) {
        if (prv->isWithDesuperheating()) {
            std::shared_ptr<PrvWithDesuperheating> with =
                    std::static_pointer_cast<PrvWithDesuperheating>(prv);
            massFlow = with->getFeedwaterMassFlow();
        }
    }

    return massFlow;
}

double
MakeupWaterMassFlowCalculator::calcMakeupWaterEnergyFlow(double massFlow,
                                                         const SteamSystemModelerTool::SteamPropertiesOutput &makeupWater) const {
    const std::string methodName = std::string("MakeupWaterMassFlowCalculator::") + std::string(__func__) + ": ";

    const double specificEnthalpy = makeupWater.specificEnthalpy;
    const double result = massFlow * specificEnthalpy;

    // std::cout << methodName << "massFlow=" << massFlow << " * specificEnthalpy=" << specificEnthalpy
          //   << ": result=" << result << std::endl;

    return result;
}
