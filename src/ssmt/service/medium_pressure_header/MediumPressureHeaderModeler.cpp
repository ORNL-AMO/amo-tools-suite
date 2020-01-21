#include "ssmt/service/medium_pressure_header/MediumPressureHeaderModeler.h"

std::shared_ptr<MediumPressureHeaderCalculationsDomain>
MediumPressureHeaderModeler::model(const int headerCountInput, const HeaderWithHighestPressure &highPressureHeaderInput,
                                   const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                   const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                   const PressureTurbine &highToMediumTurbineInput,
                                   const PressureTurbine &highToLowTurbineInput,
                                   const PressureTurbine &mediumToLowTurbineInput,
                                   const CondensingTurbine &condensingTurbineInput,
                                   const Boiler &boiler,
                                   HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("MediumPressureHeaderModeler::") + std::string(__func__) + ": ";

    std::shared_ptr<MediumPressureHeaderCalculationsDomain> mediumPressureHeaderCalculationsDomain = nullptr;

    // adjust max iterations as desired; mainly to prevent runaway modeling from unexpected issues
    const int maxIterationCount = 25;

    int iterationCount = 0;
    bool isMediumPressureHeaderBalanced = false;
    while (!isMediumPressureHeaderBalanced && iterationCount < maxIterationCount) {
        iterationCount++;
        std::cout << methodName << "running mediumPressureHeaderModeler iterationCount=" << iterationCount << std::endl;

        try {
            mediumPressureHeaderCalculationsDomain =
                    modelIteration(headerCountInput, highPressureHeaderInput, mediumPressureHeaderInput,
                                   lowPressureHeaderInput, highToMediumTurbineInput, highToLowTurbineInput,
                                   mediumToLowTurbineInput, condensingTurbineInput, boiler,
                                   highPressureHeaderCalculationsDomain);
            // no exception means it is balanced (or not in use!)
            isMediumPressureHeaderBalanced = true;

            if (mediumPressureHeaderCalculationsDomain != nullptr) {
                highPressureHeaderCalculationsDomain.highToLowPressureTurbine =
                        mediumPressureHeaderCalculationsDomain->highToLowPressureTurbineUpdated;
                highPressureHeaderCalculationsDomain.highToLowPressureTurbineIdeal =
                        mediumPressureHeaderCalculationsDomain->highToLowPressureTurbineIdealUpdated;
            }
        } catch (const ReducedSteamException &e) {
            //TODO extract methods
            const std::string &actionMessage = e.getActionMessage();
            std::cout << methodName << "ReducedSteamException: " << actionMessage
                     << "; rerunning MediumPressureHeaderModeler with updated highToLowPressureTurbine" << std::endl;
            const std::shared_ptr<Turbine> &highToLowPressureTurbine = e.getHighToLowPressureTurbineUpdated();
            const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal = e.getHighToLowPressureTurbineIdealUpdated();

            highPressureHeaderCalculationsDomain.highToLowPressureTurbine = highToLowPressureTurbine;
            highPressureHeaderCalculationsDomain.highToLowPressureTurbineIdeal = highToLowPressureTurbineIdeal;

            //2G. Calculate high to medium steam turbine if in use
            const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput =
                    highPressureHeaderCalculationsDomain.highPressureHeaderOutput;
            const std::shared_ptr<Turbine> &condensingTurbine = highPressureHeaderCalculationsDomain.condensingTurbine;
            const HighToMediumSteamTurbineCalculationsDomain &highToMediumSteamTurbineCalculationsDomain =
                    highPressureHeaderModeler.calcHighToMediumSteamTurbine(headerCountInput, highPressureHeaderInput,
                                                                           mediumPressureHeaderInput,
                                                                           lowPressureHeaderInput,
                                                                           highToMediumTurbineInput,
                                                                           highToLowTurbineInput,
                                                                           condensingTurbineInput, boiler,
                                                                           highPressureHeaderOutput, condensingTurbine,
                                                                           highToLowPressureTurbine,
                                                                           highToLowPressureTurbineIdeal);
            const std::shared_ptr<Turbine> &highToMediumPressureTurbine =
                    highToMediumSteamTurbineCalculationsDomain.highToMediumPressureTurbine;
            const std::shared_ptr<Turbine> &highToMediumPressureTurbineIdeal =
                    highToMediumSteamTurbineCalculationsDomain.highToMediumPressureTurbineIdeal;

            const std::shared_ptr<Turbine> &highToLowPressureTurbineUpdated =
                    highToMediumSteamTurbineCalculationsDomain.highToLowPressureTurbineUpdated;
            const std::shared_ptr<Turbine> &highToLowPressureTurbineIdealUpdated =
                    highToMediumSteamTurbineCalculationsDomain.highToLowPressureTurbineIdealUpdated;

            highPressureHeaderCalculationsDomain.highToMediumPressureTurbine = highToMediumPressureTurbine;
            highPressureHeaderCalculationsDomain.highToMediumPressureTurbineIdeal = highToMediumPressureTurbineIdeal;

            highPressureHeaderCalculationsDomain.highToLowPressureTurbine = highToLowPressureTurbineUpdated;
            highPressureHeaderCalculationsDomain.highToLowPressureTurbineIdeal = highToLowPressureTurbineIdealUpdated;
        }
    }

    if (!isMediumPressureHeaderBalanced) {
        const std::string msg =
                "Could not reduce enough steam from highToLowPressureTurbine to balance system in " +
                std::to_string(iterationCount) + " attempts";
        std::cout << methodName << msg << std::endl;
        throw UnableToBalanceException(msg);
    }

    return mediumPressureHeaderCalculationsDomain;
}

std::shared_ptr<MediumPressureHeaderCalculationsDomain>
MediumPressureHeaderModeler::modelIteration(const int headerCountInput,
                                            const HeaderWithHighestPressure &highPressureHeaderInput,
                                            const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                            const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                            const PressureTurbine &highToMediumTurbineInput,
                                            const PressureTurbine &highToLowTurbineInput,
                                            const PressureTurbine &mediumToLowTurbineInput,
                                            const CondensingTurbine &condensingTurbineInput,
                                            const Boiler &boiler,
                                            const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("MediumPressureHeaderModeler::") + std::string(__func__) + ": ";

    //if medium pressure header exists
    if (headerCountInput == 3) {
        std::cout << methodName << "medium pressure header provided, processing" << std::endl;

        //TODO move these/trace ptrs for NPE elim, into highToMediumPrvCalculator.calc and mediumPressureHeaderCalculator.calc
        const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput =
                highPressureHeaderCalculationsDomain.highPressureHeaderOutput;
        const std::shared_ptr<FlashTank> &highPressureCondensateFlashTank =
                highPressureHeaderCalculationsDomain.highPressureCondensateFlashTank;
        const std::shared_ptr<Turbine> &condensingTurbine =
                highPressureHeaderCalculationsDomain.condensingTurbine;
        const std::shared_ptr<Turbine> &highToMediumPressureTurbine =
                highPressureHeaderCalculationsDomain.highToMediumPressureTurbine;
        const std::shared_ptr<Turbine> &highToLowPressureTurbine =
                highPressureHeaderCalculationsDomain.highToLowPressureTurbine;
        const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal =
                highPressureHeaderCalculationsDomain.highToLowPressureTurbineIdeal;

        //3A. Calculate High to Medium PRV
        std::cout << methodName << "calculating high to medium pressure PRV" << std::endl;
        const std::shared_ptr<PrvWithoutDesuperheating> &highToMediumPressurePrv =
                highToMediumPrvCalculator.calc(highPressureHeaderInput, mediumPressureHeaderInput,
                                               highToLowTurbineInput, highToMediumTurbineInput, condensingTurbineInput,
                                               highToLowPressureTurbine, highToMediumPressureTurbine, condensingTurbine,
                                               boiler, highPressureHeaderOutput);
        std::cout << methodName << "highToMediumPressurePrv=" << highToMediumPressurePrv << std::endl;

        //3B. Model Medium Pressure Header
        //3B1. Calculate inlets for medium pressure header
        std::cout << methodName << "calculating medium pressure header" << std::endl;
        const MediumPressureHeaderCalculatorOutput &mediumPressureHeaderCalculatorOutput =
                mediumPressureHeaderCalculator.calc(boiler, highToLowTurbineInput, highToLowPressureTurbine,
                                                    highToLowPressureTurbineIdeal, highPressureHeaderOutput,
                                                    mediumPressureHeaderInput, highToMediumPressurePrv,
                                                    highToMediumTurbineInput, highToMediumPressureTurbine,
                                                    highPressureCondensateFlashTank, lowPressureHeaderInput);
        std::cout << methodName << "mediumPressureHeaderCalculatorOutput=" << mediumPressureHeaderCalculatorOutput
                 << std::endl;

        const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutputOriginal =
                mediumPressureHeaderCalculatorOutput.mediumPressureHeaderOutput;
        std::shared_ptr<Turbine> highToLowPressureTurbineUpdated =
                mediumPressureHeaderCalculatorOutput.highToLowPressureTurbineUpdated;
        std::shared_ptr<Turbine> highToLowPressureTurbineIdealUpdated =
                mediumPressureHeaderCalculatorOutput.highToLowPressureTurbineIdealUpdated;

        //3C. Calculate Heat Loss for Remaining Steam in Medium Pressure Header
        std::cout << methodName << "calculating mediumPressureHeader heat loss" << std::endl;
        const HeatLoss &heatLoss = heatLossFactory.make(mediumPressureHeaderInput, mediumPressureHeaderOutputOriginal);
        std::cout << methodName << "mediumPressureHeader heatLoss=" << heatLoss << std::endl;

        std::cout << methodName << "updating mediumPressureHeader with heat loss" << std::endl;
        const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput =
                fluidPropertiesFactory.makeWithSpecificVolume(heatLoss,
                                                              mediumPressureHeaderOutputOriginal.specificVolume);
        std::cout << methodName << "mediumPressureHeaderOutput=" << mediumPressureHeaderOutput << std::endl;

        //3D. Calculate Medium Pressure Condensate
        std::cout << methodName << "calculating medium pressure condensate" << std::endl;
        const SteamSystemModelerTool::FluidProperties &mediumPressureCondensate =
                mediumPressureCondensateCalculator.calc(mediumPressureHeaderInput);
        std::cout << methodName << "mediumPressureCondensate=" << mediumPressureCondensate << std::endl;

        //3E. Calculate medium to low steam turbine if in use
        std::cout << methodName << "calculating medium to low pressure turbine" << std::endl;
        const MediumToLowPressureTurbineCalculatorOutput mediumToLowPressureTurbineCalculatorOutput =
                mediumToLowPressureTurbineCalculator.calc(highToLowTurbineInput, highToLowPressureTurbineUpdated,
                                                          highToLowPressureTurbineIdealUpdated, mediumToLowTurbineInput,
                                                          highPressureHeaderOutput, mediumPressureHeaderInput,
                                                          mediumPressureHeaderOutput, lowPressureHeaderInput,
                                                          boiler);
        std::cout << methodName << "mediumToLowPressureTurbineCalculatorOutput="
                 << mediumToLowPressureTurbineCalculatorOutput << std::endl;
        const std::shared_ptr<Turbine> &mediumToLowPressureTurbine =
                mediumToLowPressureTurbineCalculatorOutput.mediumToLowPressureTurbine;
        const std::shared_ptr<Turbine> &mediumToLowPressureTurbineIdeal =
                mediumToLowPressureTurbineCalculatorOutput.mediumToLowPressureTurbineIdeal;
        highToLowPressureTurbineUpdated = mediumToLowPressureTurbineCalculatorOutput.highToLowPressureTurbineUpdated;
        highToLowPressureTurbineIdealUpdated = mediumToLowPressureTurbineCalculatorOutput.highToLowPressureTurbineIdealUpdated;

        const MediumPressureHeaderCalculationsDomain &domain =
                {highToMediumPressurePrv, highPressureCondensateFlashTank, heatLoss, mediumPressureHeaderOutput,
                 mediumPressureCondensate, mediumToLowPressureTurbine, mediumToLowPressureTurbineIdeal,
                 highToLowPressureTurbineUpdated, highToLowPressureTurbineIdealUpdated};
        return std::make_shared<MediumPressureHeaderCalculationsDomain>(domain);
    } else {
        std::cout << methodName << "medium pressure header not provided, skipping" << std::endl;
        return nullptr;
    }
}
