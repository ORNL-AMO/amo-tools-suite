#include <ssmt/service/medium_pressure_header/MediumPressureHeaderCalculator.h>

MediumPressureHeaderCalculatorOutput
MediumPressureHeaderCalculator::calc(const Boiler &boiler, const PressureTurbine &highToLowTurbineInput,
                                     const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                     const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal,
                                     const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                     const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                     const std::shared_ptr<PrvWithoutDesuperheating> &highToMediumPressurePrv,
                                     const PressureTurbine &highToMediumTurbineInput,
                                     const std::shared_ptr<Turbine> &highToMediumPressureTurbine,
                                     const std::shared_ptr<FlashTank> &highPressureCondensateFlashTank,
                                     const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput) const {
    const std::string methodName = std::string("MediumPressureHeaderCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<Turbine> highToLowPressureTurbineUpdated = highToLowPressureTurbine;
    std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated = highToLowPressureTurbineIdeal;

//     std::cout << methodName << "calculating mediumPressureHeaderOutput" << std::endl;
    //3B1 & 3B2. Calculate medium pressure header
    const Header mediumPressureHeader =
            headerFactory.make(mediumPressureHeaderInput, highToMediumPressurePrv, highToMediumTurbineInput,
                               highToMediumPressureTurbine, highPressureCondensateFlashTank);
//     std::cout << methodName << "mediumPressureHeader=" << mediumPressureHeader << std::endl;

    const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput =
            fluidPropertiesFactory.make(mediumPressureHeader);
//     std::cout << methodName << "mediumPressureHeaderOutput=" << mediumPressureHeaderOutput << std::endl;

    //3B3. Check that medium pressure header has enough steam to be processed
    const double mediumPressureHeaderMassFlow = mediumPressureHeader.getInletMassFlow();
    const double mediumPressureHeaderInputProcessSteamUsage = mediumPressureHeaderInput->getProcessSteamUsage();

    if (mediumPressureHeaderMassFlow < mediumPressureHeaderInputProcessSteamUsage) {
        // std::cout << methodName << "mediumPressureHeaderMassFlow=" << mediumPressureHeaderMassFlow
                //   << " < mediumPressureHeaderInputProcessSteamUsage=" << mediumPressureHeaderInputProcessSteamUsage
                //   << "; attempt to adjust medium to low turbine" << std::endl;

        const SteamReducerOutput &steamReducerOutput =
                steamBalanceCheckerService.check("mediumPressureHeader", highToLowTurbineInput, lowPressureHeaderInput,
                                                 boiler, highToLowPressureTurbine, highToLowPressureTurbineIdeal,
                                                 highPressureHeaderOutput, mediumPressureHeaderInputProcessSteamUsage,
                                                 mediumPressureHeaderMassFlow);
        // std::cout << methodName << "steamReducerOutput=" << steamReducerOutput << std::endl;

        highToLowPressureTurbineUpdated = steamReducerOutput.highToLowPressureTurbineUpdated;
        highToLowPressureTurbineIdealUpdated = steamReducerOutput.highToLowPressureTurbineIdealUpdated;
    }

    return {mediumPressureHeaderOutput, highToLowPressureTurbineUpdated, highToLowPressureTurbineIdealUpdated};
}
