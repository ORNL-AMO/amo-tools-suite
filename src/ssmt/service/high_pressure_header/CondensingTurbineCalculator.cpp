#include "ssmt/service/high_pressure_header/CondensingTurbineCalculator.h"

const std::shared_ptr<Turbine>
CondensingTurbineCalculator::calc(const CondensingTurbine &condensingTurbineInput,
                                  const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput,
                                  const bool isCalcIdeal) const {
    const std::string methodName = std::string("CondensingTurbineCalculator::") + std::string(__func__) + ": ";

    std::shared_ptr<Turbine> condensingTurbinePtr = nullptr;
    if (condensingTurbineInput.isUseTurbine()) {
        // std::cout << methodName << "condensingTurbineInput isUseTurbine, calculating condensingTurbine" << std::endl;
        const Turbine condensingTurbine =
                turbineFactory.make(highPressureHeaderOutput, condensingTurbineInput, isCalcIdeal);
        condensingTurbinePtr = std::make_shared<Turbine>(condensingTurbine);
    } else {
        // std::cout << methodName << "condensingTurbineInput not isUseTurbine, skipping" << std::endl;
    }

    return condensingTurbinePtr;
}
