#include "ssmt/service/PrvCalculator.h"

PrvWithDesuperheating
PrvCalculator::calcHighToMediumPrvWithDesuperheating(const HeaderWithHighestPressure &highPressureHeaderInput,
                                                     const PressureTurbine &highToLowTurbineInput,
                                                     const PressureTurbine &highToMediumTurbineInput,
                                                     const CondensingTurbine &condensingTurbineInput,
                                                     const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                                     const std::shared_ptr<Turbine> &highToMediumPressureTurbine,
                                                     const std::shared_ptr<Turbine> &condensingTurbine,
                                                     const Boiler &boiler,
                                                     const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                                     const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput) const {
    const double prvMassFlow =
            calcPrvMassFlow(highPressureHeaderOutput, highPressureHeaderInput, highToLowTurbineInput,
                            highToMediumTurbineInput, condensingTurbineInput, highToLowPressureTurbine,
                            highToMediumPressureTurbine, condensingTurbine);
    double feedwaterPressure = boiler.getFeedwaterProperties().pressure;
    return prvWithDesuperheatingFactory.make(highPressureHeaderOutput, prvMassFlow, mediumPressureHeaderInput,
                                             feedwaterPressure);
}

PrvWithoutDesuperheating PrvCalculator::calcHighToMediumPrvWithoutDesuperheating(
        const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
        const HeaderWithHighestPressure &highPressureHeaderInput,
        const PressureTurbine &highToLowTurbineInput,
        const PressureTurbine &highToMediumTurbineInput,
        const CondensingTurbine &condensingTurbineInput,
        const std::shared_ptr<Turbine> &highToLowPressureTurbine,
        const std::shared_ptr<Turbine> &highToMediumPressureTurbine,
        const std::shared_ptr<Turbine> &condensingTurbine,
        const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput) const {
    const double prvMassFlow =
            calcPrvMassFlow(highPressureHeaderOutput, highPressureHeaderInput, highToLowTurbineInput,
                            highToMediumTurbineInput, condensingTurbineInput, highToLowPressureTurbine,
                            highToMediumPressureTurbine, condensingTurbine);
    return prvWithoutDesuperheatingFactory.make(highPressureHeaderOutput, prvMassFlow, mediumPressureHeaderInput);
}

/** PRV mass flow is steam remaining in high pressure header.  Subtract off all usage. */
double PrvCalculator::calcPrvMassFlow(const SteamSystemModelerTool::FluidProperties &highPressureHeader,
                                      const HeaderWithHighestPressure &highPressureHeaderInput,
                                      const PressureTurbine &highToLowTurbineInput,
                                      const PressureTurbine &highToMediumTurbineInput,
                                      const CondensingTurbine &condensingTurbineInput,
                                      const std::shared_ptr<Turbine> &highToLowPressureTurbine,
                                      const std::shared_ptr<Turbine> &highToMediumPressureTurbine,
                                      const std::shared_ptr<Turbine> &condensingTurbine) const {
    double prvMassFlow = highPressureHeader.massFlow - highPressureHeaderInput.getProcessSteamUsage();

    if (highToLowTurbineInput.isUseTurbine()) {
        prvMassFlow -= getTurbineMassFlow(highToLowPressureTurbine, "highToLowPressureTurbine");
    }
    if (highToMediumTurbineInput.isUseTurbine()) {
        prvMassFlow -= getTurbineMassFlow(highToMediumPressureTurbine, "highToMediumPressureTurbine");
    }
    if (condensingTurbineInput.isUseTurbine()) {
        prvMassFlow -= getTurbineMassFlow(condensingTurbine, "condensingTurbine");
    }

    prvMassFlow = std::fmax(prvMassFlow, 0);

    return prvMassFlow;
}

double
PrvCalculator::getTurbineMassFlow(const std::shared_ptr<Turbine> &turbine, const std::string &turbineName) const {
    double massFlow = 0;

    if (turbine == nullptr) {
        // std::cout << "PrvCalculator::getTurbineMassFlow:"
        //           << " ERROR: Turbine instance '"
        //           << turbineName
        //           << "' expected to exist but was null/not existing; using 0"
        //           << std::endl;
    } else {
        massFlow = turbine->getMassFlow();
    }

    return massFlow;
}
