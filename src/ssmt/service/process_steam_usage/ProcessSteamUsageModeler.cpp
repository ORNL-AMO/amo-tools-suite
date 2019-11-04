#include <ssmt/service/process_steam_usage/ProcessSteamUsageModeler.h>

/** These functions do not impact iteration of the model, they calculate informational values. */
ProcessSteamUsageCalculationsDomain
ProcessSteamUsageModeler::model(const double headerCountInput, const HeaderWithHighestPressure &highPressureHeaderInput,
                                const std::shared_ptr<HeaderNotHighestPressure> &mediumPressureHeaderInput,
                                const std::shared_ptr<HeaderNotHighestPressure> &lowPressureHeaderInput,
                                const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain,
                                const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain,
                                const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain) const {
    const std::string methodName = std::string("ProcessSteamUsageModeler::") + std::string(__func__) + ": ";

//     std::cout << methodName << "calculating highPressureProcessSteamUsage" << std::endl;
    //8. calculate process steam usage
    //8a. calculate high pressure process steam usage
    const ProcessSteamUsage &highPressureProcessSteamUsage =
            calc(highPressureHeaderInput, highPressureHeaderCalculationsDomain);

    std::shared_ptr<ProcessSteamUsage> lowPressureProcessUsagePtr = nullptr;
    std::shared_ptr<ProcessSteamUsage> mediumPressureProcessUsagePtr = nullptr;

    if (headerCountInput > 1) {
        // std::cout << methodName << "lowPressureHeader exists, calculating lowPressureProcessUsage" << std::endl;
        //8b. calculate low pressure process steam usage
        const SteamSystemModelerTool::FluidProperties &lowPressureHeaderOutput =
                lowPressureHeaderCalculationsDomain->lowPressureHeaderOutput;
        const SteamSystemModelerTool::FluidProperties &lowPressureCondensate =
                lowPressureHeaderCalculationsDomain->lowPressureCondensate;

        const ProcessSteamUsage &lowPressureProcessUsage =
                calc(lowPressureHeaderInput, lowPressureHeaderOutput, lowPressureCondensate);
        lowPressureProcessUsagePtr = std::make_shared<ProcessSteamUsage>(lowPressureProcessUsage);
    }

    if (headerCountInput == 3) {
        // std::cout << methodName << "mediumPressureHeader exists, calculating mediumPressureProcessUsage" << std::endl;
        //8c. calculate medium pressure process steam usage
        const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput =
                mediumPressureHeaderCalculationsDomain->mediumPressureHeaderOutput;
        const SteamSystemModelerTool::FluidProperties &mediumPressureCondensate =
                mediumPressureHeaderCalculationsDomain->mediumPressureCondensate;

        const ProcessSteamUsage &mediumPressureProcessUsage =
                calc(mediumPressureHeaderInput, mediumPressureHeaderOutput, mediumPressureCondensate);
        mediumPressureProcessUsagePtr = std::make_shared<ProcessSteamUsage>(mediumPressureProcessUsage);
    }

    return {highPressureProcessSteamUsage, lowPressureProcessUsagePtr, mediumPressureProcessUsagePtr};
}

ProcessSteamUsage
ProcessSteamUsageModeler::calc(const HeaderWithHighestPressure &highPressureHeaderInput,
                               const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain) const {
    const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput =
            highPressureHeaderCalculationsDomain.highPressureHeaderOutput;
    const SteamSystemModelerTool::FluidProperties &highPressureCondensate =
            highPressureHeaderCalculationsDomain.highPressureCondensate;

    const double highPressureHeaderInputProcessSteamUsage = highPressureHeaderInput.getProcessSteamUsage();
    const double highPressureHeaderOutputSpecificEnthalpy = highPressureHeaderOutput.specificEnthalpy;
    const double processSteamUsageEnergyFlow =
            highPressureHeaderInputProcessSteamUsage * highPressureHeaderOutputSpecificEnthalpy;
    const double highPressureCondensateSpecificEnthalpy = highPressureCondensate.specificEnthalpy;
    const double processUsage =
            highPressureHeaderInputProcessSteamUsage *
            (highPressureHeaderOutputSpecificEnthalpy - highPressureCondensateSpecificEnthalpy);

    const double pressure = highPressureHeaderOutput.pressure;
    const double temperature = highPressureHeaderOutput.temperature;
    const double energyFlow = processSteamUsageEnergyFlow;
    const double massFlow = highPressureHeaderInputProcessSteamUsage;

    return {pressure, temperature, energyFlow, massFlow, processUsage};
}

ProcessSteamUsage
ProcessSteamUsageModeler::calc(const std::shared_ptr<HeaderNotHighestPressure> &headerInput,
                               const SteamSystemModelerTool::FluidProperties &headerOutput,
                               const SteamSystemModelerTool::FluidProperties &pressureCondensate) const {
    const double headerInputProcessSteamUsage = headerInput->getProcessSteamUsage();
    const double headerOutputSpecificEnthalpy = headerOutput.specificEnthalpy;
    double processSteamUsageEnergyFlow = headerInputProcessSteamUsage * headerOutputSpecificEnthalpy;
    const double condensateSpecificEnthalpy = pressureCondensate.specificEnthalpy;
    double processUsage = headerInputProcessSteamUsage * (headerOutputSpecificEnthalpy - condensateSpecificEnthalpy);

    const double pressure = headerOutput.pressure;
    const double temperature = headerOutput.temperature;
    const double energyFlow = processSteamUsageEnergyFlow;
    const double massFlow = headerInputProcessSteamUsage;

    return {pressure, temperature, energyFlow, massFlow, processUsage};
}
