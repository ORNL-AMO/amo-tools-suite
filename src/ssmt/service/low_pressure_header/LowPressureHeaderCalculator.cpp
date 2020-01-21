#include "ssmt/service/low_pressure_header/LowPressureHeaderCalculator.h"

SteamSystemModelerTool::FluidProperties
LowPressureHeaderCalculator::calc(const int headerCountInput,
                                  const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                  const PressureTurbine &highToLowTurbineInput,
                                  const PressureTurbine &mediumToLowTurbineInput,
                                  const BoilerInput &boilerInput,
                                  const std::shared_ptr<PrvWithoutDesuperheating> &lowPressurePrv,
                                  const std::shared_ptr<FlashTank> &blowdownFlashTank,
                                  const LowPressureFlashedSteamIntoHeaderCalculatorDomain &lowPressureFlashedSteamIntoHeaderCalculatorDomain,
                                  const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                                  const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("LowPressureHeaderCalculator::") + std::string(__func__) + ": ";

    const std::shared_ptr<Turbine> &highToLowPressureTurbine =
            highPressureHeaderCalculationsDomain.highToLowPressureTurbine;
    const Header lowPressureHeader =
            headerFactory.make(headerCountInput, lowPressureHeaderInput, highToLowTurbineInput, mediumToLowTurbineInput,
                               boilerInput, lowPressurePrv, highToLowPressureTurbine, blowdownFlashTank,
                               lowPressureFlashedSteamIntoHeaderCalculatorDomain,
                               mediumPressureHeaderCalculationsDomain);
    std::cout << methodName << "lowPressureHeader=" << lowPressureHeader << std::endl;

    return fluidPropertiesFactory.make(lowPressureHeader);
}
