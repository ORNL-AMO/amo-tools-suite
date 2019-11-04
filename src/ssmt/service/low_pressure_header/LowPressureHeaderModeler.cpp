#include "ssmt/service/low_pressure_header/LowPressureHeaderModeler.h"

std::shared_ptr<LowPressureHeaderCalculationsDomain>
LowPressureHeaderModeler::model(const int headerCountInput, const HeaderWithHighestPressure &highPressureHeaderInput,
                                const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                const PressureTurbine &highToLowTurbineInput,
                                const PressureTurbine &mediumToLowTurbineInput,
                                const CondensingTurbine &condensingTurbineInput, const BoilerInput &boilerInput,
                                const Boiler &boiler, const std::shared_ptr<FlashTank> &blowdownFlashTank,
                                const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                                const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("LowPressureHeaderModeler::") + std::string(__func__) + ": ";

    //if low pressure header exists
    if (headerCountInput > 1) {
        // std::cout << methodName << "low pressure header provided, processing" << std::endl;

        //4A. Calculate to low pressure PRV
        // std::cout << methodName << "calculating low pressure PRV" << std::endl;
        const std::shared_ptr<PrvWithoutDesuperheating> &lowPressurePrv =
                lowPressurePrvCalculator.calc(headerCountInput, highPressureHeaderInput, highToLowTurbineInput,
                                              condensingTurbineInput, mediumPressureHeaderInput,
                                              mediumToLowTurbineInput, lowPressureHeaderInput, boiler,
                                              highPressureHeaderCalculationsDomain,
                                              mediumPressureHeaderCalculationsDomain);

        //4B. Calculate flashed steam into low pressure header if selected
        // std::cout << methodName << "calculating condensateFlashTank" << std::endl;
        LowPressureFlashedSteamIntoHeaderCalculatorDomain lowPressureFlashedSteamIntoHeaderCalculatorDomain =
                lowPressureFlashedSteamIntoHeaderCalculator.calc(headerCountInput, lowPressureHeaderInput,
                                                                 mediumPressureHeaderInput,
                                                                 highPressureHeaderCalculationsDomain,
                                                                 mediumPressureHeaderCalculationsDomain);
        // std::cout << methodName << "lowPressureFlashedSteamIntoHeaderCalculatorDomain="
                //   << lowPressureFlashedSteamIntoHeaderCalculatorDomain << std::endl;

        //4C. Model Low Pressure Header
        // std::cout << methodName << "calculating lowPressureHeader" << std::endl;
        const SteamSystemModelerTool::FluidProperties &lowPressureHeaderOutput =
                lowPressureHeaderCalculator.calc(headerCountInput, lowPressureHeaderInput, highToLowTurbineInput,
                                                 mediumToLowTurbineInput, boilerInput, lowPressurePrv,
                                                 blowdownFlashTank, lowPressureFlashedSteamIntoHeaderCalculatorDomain,
                                                 highPressureHeaderCalculationsDomain,
                                                 mediumPressureHeaderCalculationsDomain);
        // std::cout << methodName << "lowPressureHeaderOutput=" << lowPressureHeaderOutput << std::endl;

        //4D. Calculate Heat Loss for Remaining Steam in Low Pressure Header
        // std::cout << methodName << "calculating lowPressureHeader heat loss" << std::endl;
        const HeatLoss &heatLoss = heatLossFactory.make(lowPressureHeaderInput, lowPressureHeaderOutput);
        // std::cout << methodName << "lowPressureHeader heatLoss=" << heatLoss << std::endl;

        // std::cout << methodName << "updating lowPressureHeader with heat loss" << std::endl;
        const SteamSystemModelerTool::FluidProperties &lowPressureHeaderOutputUpdated =
                fluidPropertiesFactory.makeWithSpecificVolume(heatLoss, lowPressureHeaderOutput.specificVolume);
        // std::cout << methodName << "lowPressureHeaderOutput=" << lowPressureHeaderOutputUpdated << std::endl;

        //4E. Calculate Low Pressure Condensate
        // std::cout << methodName << "calculating lowPressureCondensate" << std::endl;
        const SteamSystemModelerTool::FluidProperties lowPressureCondensate =
                lowPressureCondensateCalculator.calc(lowPressureHeaderInput);
        // std::cout << methodName << "lowPressureCondensate=" << lowPressureCondensate << std::endl;

        const LowPressureHeaderCalculationsDomain domain =
                {lowPressurePrv, lowPressureHeaderOutputUpdated, heatLoss, lowPressureCondensate,
                 lowPressureFlashedSteamIntoHeaderCalculatorDomain};
        return std::make_shared<LowPressureHeaderCalculationsDomain>(domain);
    } else {
        // std::cout << methodName << "medium pressure header not provided, skipping" << std::endl;
        return nullptr;
    }
}
