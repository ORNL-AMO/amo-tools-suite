#include "ssmt/service/high_pressure_header/HighPressureHeaderModeler.h"

HighPressureHeaderCalculationsDomain
HighPressureHeaderModeler::model(const int headerCountInput, const HeaderWithHighestPressure &highPressureHeaderInput,
                                 const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                 const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                 const PressureTurbine &highToMediumTurbineInput,
                                 const PressureTurbine &highToLowTurbineInput,
                                 const CondensingTurbine &condensingTurbineInput, const Boiler &boiler) const {
    const std::string methodName = std::string("HighPressureHeaderModeler::") + std::string(__func__) + ": ";

    //2A. Calculate High Pressure Header
//     std::cout << methodName << "calculating high pressure header" << std::endl;
    const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutputOriginal =
            highPressureHeaderCalculator.calc(highPressureHeaderInput, boiler);
//     std::cout << methodName << "highPressureHeaderOutputOriginal=" << highPressureHeaderOutputOriginal << std::endl;

    //2B. Calculate Heat Loss of steam in high pressure header
//     std::cout << methodName << "calculating highPressureHeader heat loss" << std::endl;
    const HeatLoss &heatLoss = heatLossFactory.make(highPressureHeaderInput, highPressureHeaderOutputOriginal);
//     std::cout << methodName << "highPressureHeader heatLoss=" << heatLoss << std::endl;

//     std::cout << methodName << "updating highPressureHeader with heat loss" << std::endl;
    const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput = fluidPropertiesFactory.make(heatLoss);
//     std::cout << methodName << "highPressureHeaderOutput=" << highPressureHeaderOutput << std::endl;

    //2C. Calculate High Pressure Condensate
//     std::cout << methodName << "calculating high pressure condensate" << std::endl;
    const SteamSystemModelerTool::FluidProperties &highPressureCondensate =
            highPressureCondensateCalculator.calc(highPressureHeaderInput, boiler);
//     std::cout << methodName << "highPressureCondensate=" << highPressureCondensate << std::endl;

    //2D. Calculate High Pressure Flash Tank if 3 header and on
//     std::cout << methodName << "calculating high pressure flash tank" << std::endl;
    const std::shared_ptr<FlashTank> &highPressureCondensateFlashTank =
            highPressureFlashTankCalculator.calc(headerCountInput, mediumPressureHeaderInput, highPressureCondensate);
//     std::cout << methodName << "highPressureCondensateFlashTank=" << highPressureCondensateFlashTank << std::endl;

    //2E. Calculate condensing turbine
//     std::cout << methodName << "calculating condensing turbine" << std::endl;
    const std::shared_ptr<Turbine> &condensingTurbine =
            condensingTurbineCalculator.calc(condensingTurbineInput, highPressureHeaderOutput, false);
//     std::cout << methodName << "condensingTurbine=" << condensingTurbine << std::endl;
    const std::shared_ptr<Turbine> &condensingTurbineIdeal =
            condensingTurbineCalculator.calc(condensingTurbineInput, highPressureHeaderOutput, true);
//     std::cout << methodName << "condensingTurbineIdeal=" << condensingTurbineIdeal << std::endl;

    //2F. Calculate high to low steam turbine if in use
//     std::cout << methodName << "calculating highToLowPressureTurbine" << std::endl;
    const std::shared_ptr<Turbine> &highToLowPressureTurbine =
            highToLowSteamTurbineCalculator.calc(headerCountInput, highToLowTurbineInput, highPressureHeaderOutput,
                                                 highPressureHeaderInput, condensingTurbineInput, condensingTurbine,
                                                 lowPressureHeaderInput, boiler, false);
//     std::cout << methodName << "highToLowPressureTurbine=" << highToLowPressureTurbine << std::endl;
    const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal =
            highToLowSteamTurbineCalculator.calc(headerCountInput, highToLowTurbineInput, highPressureHeaderOutput,
                                                 highPressureHeaderInput, condensingTurbineInput, condensingTurbine,
                                                 lowPressureHeaderInput, boiler, true);
//     std::cout << methodName << "highToLowPressureTurbineIdeal=" << highToLowPressureTurbineIdeal << std::endl;

    //2G. Calculate high to medium steam turbine if in use
    const HighToMediumSteamTurbineCalculationsDomain &highToMediumSteamTurbineCalculationsDomain =
            calcHighToMediumSteamTurbine(headerCountInput, highPressureHeaderInput, mediumPressureHeaderInput,
                                         lowPressureHeaderInput, highToMediumTurbineInput, highToLowTurbineInput,
                                         condensingTurbineInput, boiler, highPressureHeaderOutput, condensingTurbine,
                                         highToLowPressureTurbine, highToLowPressureTurbineIdeal);
    const std::shared_ptr<Turbine> &highToMediumPressureTurbine =
            highToMediumSteamTurbineCalculationsDomain.highToMediumPressureTurbine;
    const std::shared_ptr<Turbine> &highToMediumPressureTurbineIdeal =
            highToMediumSteamTurbineCalculationsDomain.highToMediumPressureTurbineIdeal;

    const std::shared_ptr<Turbine> &highToLowPressureTurbineUpdated =
            highToMediumSteamTurbineCalculationsDomain.highToLowPressureTurbineUpdated;
    const std::shared_ptr<Turbine> &highToLowPressureTurbineIdealUpdated =
            highToMediumSteamTurbineCalculationsDomain.highToLowPressureTurbineIdealUpdated;

    return {highPressureHeaderOutput, heatLoss, highPressureCondensate, highPressureCondensateFlashTank,
            condensingTurbine, condensingTurbineIdeal, highToMediumPressureTurbine, highToMediumPressureTurbineIdeal,
            highToLowPressureTurbineUpdated, highToLowPressureTurbineIdealUpdated};
}

HighToMediumSteamTurbineCalculationsDomain
HighPressureHeaderModeler::calcHighToMediumSteamTurbine(const int headerCountInput,
                                                        const HeaderWithHighestPressure &highPressureHeaderInput,
                                                        const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                                        const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                                        const PressureTurbine &highToMediumTurbineInput,
                                                        const PressureTurbine &highToLowTurbineInput,
                                                        const CondensingTurbine &condensingTurbineInput,
                                                        const Boiler &boiler,
                                                        const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                        const std::shared_ptr<Turbine> &condensingTurbine,
                                                        const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                        const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal) const {
    const std::string methodName = std::string("HighPressureHeaderModeler::") + std::string(__func__) + ": ";

//     std::cout << methodName << "calculating high to medium steam turbine" << std::endl;
    const HighToMediumSteamTurbineCalculationsDomain &highToMediumSteamTurbineCalculationsDomain =
            highToMediumSteamTurbineCalculator.calc(headerCountInput, highPressureHeaderOutput, highPressureHeaderInput,
                                                    condensingTurbineInput, condensingTurbine, highToLowTurbineInput,
                                                    highToLowPressureTurbine, highToLowPressureTurbineIdeal,
                                                    highToMediumTurbineInput, mediumPressureHeaderInput,
                                                    lowPressureHeaderInput, boiler);
//     std::cout << methodName << "highToMediumSteamTurbineCalculationsDomain="
        //       << highToMediumSteamTurbineCalculationsDomain << std::endl;

    return highToMediumSteamTurbineCalculationsDomain;
}
