#include "ssmt/service/low_pressure_header/LowPressurePrvCalculator.h"

std::shared_ptr<PrvWithoutDesuperheating>
LowPressurePrvCalculator::calc(const int headerCountInput, const HeaderWithHighestPressure &highPressureHeaderInput,
                               const PressureTurbine &highToLowTurbineInput,
                               const CondensingTurbine &condensingTurbineInput,
                               const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                               const PressureTurbine &mediumToLowTurbineInput,
                               const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                               const Boiler &boiler,
                               const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                               const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("LowPressurePrvCalculator::") + std::string(__func__) + ": ";

    const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput =
            highPressureHeaderCalculationsDomain.highPressureHeaderOutput;
    const std::shared_ptr<Turbine> &highToLowPressureTurbine =
            highPressureHeaderCalculationsDomain.highToLowPressureTurbine;
    const std::shared_ptr<Turbine> &condensingTurbine =
            highPressureHeaderCalculationsDomain.condensingTurbine;

    const SteamSystemModelerTool::FluidProperties &headerOutput =
            determineHeader(headerCountInput, mediumPressureHeaderCalculationsDomain, highPressureHeaderOutput);

    double prvMassFlow =
            calcPrvMassFlow(headerCountInput, highPressureHeaderInput, highPressureHeaderOutput, highToLowTurbineInput,
                            highToLowPressureTurbine, condensingTurbineInput, condensingTurbine,
                            mediumPressureHeaderInput, mediumToLowTurbineInput,
                            mediumPressureHeaderCalculationsDomain);
    // std::cout << methodName << "prvMassFlow=" << prvMassFlow << std::endl;

    return makePrv(lowPressureHeaderInput, boiler, headerOutput, prvMassFlow);
}

std::shared_ptr<PrvWithoutDesuperheating>
LowPressurePrvCalculator::makePrv(const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                  const Boiler &boiler,
                                  const SteamSystemModelerTool::FluidProperties &headerOutput,
                                  double prvMassFlow) const {
    const std::string methodName = std::string("LowPressurePrvCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<PrvWithoutDesuperheating> prvPtr;

    if (lowPressureHeaderInput->isDesuperheatSteamIntoNextHighest()) {
        // std::cout << methodName << "lowPressureHeaderInput->isDesuperheatSteamIntoNextHighest,"
                //   << " making PrvWithDesuperheating" << std::endl;
        double feedwaterPressure = boiler.getFeedwaterProperties().pressure;
        const PrvWithDesuperheating &prv =
                prvWithDesuperheatingFactory.make(headerOutput, prvMassFlow, lowPressureHeaderInput,
                                                  feedwaterPressure);
        // std::cout << methodName << "lowPressurePrv=" << prv << std::endl;
        prvPtr = std::make_shared<PrvWithDesuperheating>(prv);
    } else {
        // std::cout << methodName << "lowPressureHeaderInput-> not isDesuperheatSteamIntoNextHighest,"
                 // << " making PrvWithoutDesuperheating" << std::endl;
        const PrvWithoutDesuperheating &prv =
                prvWithoutDesuperheatingFactory.make(headerOutput, prvMassFlow, lowPressureHeaderInput);
        // std::cout << methodName << "lowPressurePrv=" << prv << std::endl;
        prvPtr = std::make_shared<PrvWithoutDesuperheating>(prv);
    }

    return prvPtr;
}

SteamSystemModelerTool::FluidProperties LowPressurePrvCalculator::determineHeader(const int headerCountInput,
                                                                                  const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                                                                                  const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput) const {
    if (headerCountInput == 2) {
        return highPressureHeaderOutput;
    } else {
        return mediumPressureHeaderCalculationsDomain->mediumPressureHeaderOutput;
    }
}

double
LowPressurePrvCalculator::calcPrvMassFlow(const int headerCountInput,
                                          const HeaderWithHighestPressure &highPressureHeaderInput,
                                          const SteamSystemModelerTool::FluidProperties &highPressureHeader,
                                          const PressureTurbine &highToLowTurbineInput,
                                          const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                          const CondensingTurbine &condensingTurbineInput,
                                          const std::shared_ptr<Turbine> &condensingTurbine,
                                          const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                          const PressureTurbine &mediumToLowTurbineInput,
                                          const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("LowPressurePrvCalculator::") + std::string(__func__) + ": ";

    // std::cout << methodName << "calculating low pressure PRV mass flow" << std::endl;

    double prvMassFlow = 0;

    //either medium to low or high to low
    if (headerCountInput == 2) {
        // std::cout << methodName
                //   << "2 headers exist, subtracting high pressure header process steam usage from its mass flow"
                //   << std::endl;
        //if 2 headers, next highest is high pressure
        prvMassFlow = highPressureHeader.massFlow - highPressureHeaderInput.getProcessSteamUsage();
        // std::cout << methodName << "prvMassFlow=" << prvMassFlow << std::endl;


        if (highToLowTurbineInput.isUseTurbine()) {
            // std::cout << methodName
                    //   << "using highToLowPressureTurbine, subtracting its massFlow from prvMassFlow" << std::endl;
            prvMassFlow = prvMassFlow - highToLowPressureTurbine->getMassFlow();
            // std::cout << methodName << "updated prvMassFlow=" << prvMassFlow << std::endl;
        }

        if (condensingTurbineInput.isUseTurbine()) {
            // std::cout << methodName
                    //   << "using condensingTurbine, subtracting its massFlow from prvMassFlow" << std::endl;
            prvMassFlow = prvMassFlow - condensingTurbine->getMassFlow();
            // std::cout << methodName << "updated prvMassFlow=" << prvMassFlow << std::endl;
        }
    } else if (headerCountInput == 3) {
        // std::cout << methodName
                //   << "3 headers exist, subtracting medium pressure header process steam usage from its mass flow"
                //   << std::endl;

        // if 3 headers, next highest is medium pressure
        const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput =
                mediumPressureHeaderCalculationsDomain->mediumPressureHeaderOutput;
        prvMassFlow = mediumPressureHeaderOutput.massFlow - mediumPressureHeaderInput->getProcessSteamUsage();
        // std::cout << methodName << "prvMassFlow=" << prvMassFlow << std::endl;

        if (mediumToLowTurbineInput.isUseTurbine()) {
            // std::cout << methodName
                    //   << "using mediumToLowPressureTurbine, subtracting its massFlow from prvMassFlow" << std::endl;
            const std::shared_ptr<Turbine> &mediumToLowPressureTurbine =
                    mediumPressureHeaderCalculationsDomain->mediumToLowPressureTurbine;
            prvMassFlow = prvMassFlow - mediumToLowPressureTurbine->getMassFlow();
            // std::cout << methodName << "updated prvMassFlow=" << prvMassFlow << std::endl;
        }
    }

    return fmax(prvMassFlow, 0);
}
