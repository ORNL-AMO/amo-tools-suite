#ifndef AMO_TOOLS_SUITE_STEAMMODELEROUTPUTDATAMAPPER_H
#define AMO_TOOLS_SUITE_STEAMMODELEROUTPUTDATAMAPPER_H

#include "../NanDataConverters.h"
#include <ssmt/Boiler.h>
#include <ssmt/Deaerator.h>
#include <ssmt/SteamSystemModelerTool.h>
#include <ssmt/api/SteamModelerOutput.h>
#include <ssmt/domain/EnergyAndCostCalculationsDomain.h>
#include <ssmt/domain/ReturnCondensateCalculationsDomain.h>

class SteamModelerOutputDataMapper {
public:
    //TODO extract methods
    const void map(const SteamModelerOutput &steamModelerOutput) {
        const std::string methodName = std::string(__func__);

        logSection(methodName + ": converting to output: begin");

        const EnergyAndCostCalculationsDomain &energyAndCostCalculationsDomain = steamModelerOutput.energyAndCostCalculationsDomain;
        const Boiler &boiler = steamModelerOutput.boiler;
        const std::shared_ptr<FlashTank> &blowdownFlashTank = steamModelerOutput.blowdownFlashTank;
        const HighPressureHeaderCalculationsDomain &highPressureHeaderCalculationsDomain = steamModelerOutput.highPressureHeaderCalculationsDomain;
        const Deaerator &deaerator = steamModelerOutput.deaerator;
        const std::shared_ptr<LowPressureHeaderCalculationsDomain> &lowPressureHeaderCalculationsDomain = steamModelerOutput.lowPressureHeaderCalculationsDomain;
        const MakeupWaterAndCondensateHeaderCalculationsDomain &makeupWaterAndCondensateHeaderCalculationsDomain = steamModelerOutput.makeupWaterAndCondensateHeaderCalculationsDomain;
        const ProcessSteamUsageCalculationsDomain &processSteamUsageCalculationsDomain = steamModelerOutput.processSteamUsageCalculationsDomain;
        const std::shared_ptr<MediumPressureHeaderCalculationsDomain> &mediumPressureHeaderCalculationsDomain = steamModelerOutput.mediumPressureHeaderCalculationsDomain;
        const PowerBalanceCheckerCalculationsDomain &powerBalanceCheckerCalculationsDomain = steamModelerOutput.powerBalanceCheckerCalculationsDomain;

        const ReturnCondensateCalculationsDomain &returnCondensateCalculationsDomain = makeupWaterAndCondensateHeaderCalculationsDomain.returnCondensateCalculationsDomain;
        const SteamSystemModelerTool::FluidProperties &combinedCondensate = makeupWaterAndCondensateHeaderCalculationsDomain.combinedCondensate;
        const SteamSystemModelerTool::FluidProperties &returnCondensate = makeupWaterAndCondensateHeaderCalculationsDomain.returnCondensate;
        const SteamSystemModelerTool::FluidProperties &makeupWater = makeupWaterAndCondensateHeaderCalculationsDomain.makeupWater;
        const std::shared_ptr<HeatExchanger::Output> &heatExchangerOutput = makeupWaterAndCondensateHeaderCalculationsDomain.heatExchangerOutput;
        const SteamSystemModelerTool::FluidProperties &makeupWaterAndCondensateHeaderOutput = makeupWaterAndCondensateHeaderCalculationsDomain.makeupWaterAndCondensateHeaderOutput;

        const MakeupWaterVolumeFlowCalculationsDomain &makeupWaterVolumeFlowCalculationsDomain = makeupWaterAndCondensateHeaderCalculationsDomain.makeupWaterVolumeFlowCalculationsDomain;

        const std::shared_ptr<FlashTank> &condensateFlashTank = returnCondensateCalculationsDomain.condensateFlashTank;

        const HeatLoss &highPressureHeaderHeatLoss = highPressureHeaderCalculationsDomain.highPressureHeaderHeatLoss;
        const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput = highPressureHeaderCalculationsDomain.highPressureHeaderOutput;
        const SteamSystemModelerTool::FluidProperties &highPressureCondensate = highPressureHeaderCalculationsDomain.highPressureCondensate;
        const std::shared_ptr<FlashTank> &highPressureCondensateFlashTank = highPressureHeaderCalculationsDomain.highPressureCondensateFlashTank;
        const std::shared_ptr<Turbine> &condensingTurbine = highPressureHeaderCalculationsDomain.condensingTurbine;
        const std::shared_ptr<Turbine> &condensingTurbineIdeal = highPressureHeaderCalculationsDomain.condensingTurbineIdeal;
        const std::shared_ptr<Turbine> &highToMediumPressureTurbine = highPressureHeaderCalculationsDomain.highToMediumPressureTurbine;
        const std::shared_ptr<Turbine> &highToMediumPressureTurbineIdeal = highPressureHeaderCalculationsDomain.highToMediumPressureTurbineIdeal;
        const std::shared_ptr<Turbine> &highToLowPressureTurbine = highPressureHeaderCalculationsDomain.highToLowPressureTurbine;
        const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal = highPressureHeaderCalculationsDomain.highToLowPressureTurbineIdeal;

        const ProcessSteamUsage &highPressureProcessSteamUsage = processSteamUsageCalculationsDomain.highPressureProcessSteamUsage;

        // always in output
        mapOperations(energyAndCostCalculationsDomain, makeupWaterVolumeFlowCalculationsDomain);
        mapBoiler(boiler);
        mapDeaerator(deaerator);
        mapHighPressureHeaderHeatLoss(highPressureHeaderHeatLoss);
        mapHighPressureCondensate(highPressureCondensate);
        mapHighPressureHeaderSteam(highPressureHeaderOutput);
        mapCombinedCondensate(combinedCondensate);
        mapReturnCondensate(returnCondensate);
        mapMakeupWater(makeupWater);
        mapHeatExchanger(heatExchangerOutput);
        mapMakeupWaterAndCondensate(makeupWaterAndCondensateHeaderOutput);
        mapHighPressureProcessSteamUsage(highPressureProcessSteamUsage);

        // optional steam usages
        const std::shared_ptr<ProcessSteamUsage> &mediumPressureProcessUsage =
                processSteamUsageCalculationsDomain.mediumPressureProcessUsagePtr;
        mapProcessSteamUsage(mediumPressureProcessUsage, "mediumPressureProcessSteamUsage");

        const std::shared_ptr<ProcessSteamUsage> &lowPressureProcessUsage =
                processSteamUsageCalculationsDomain.lowPressureProcessUsagePtr;
        mapProcessSteamUsage(lowPressureProcessUsage, "lowPressureProcessSteamUsage");

        // optional turbines 0-4
        mapCondensingTurbine(condensingTurbine);
        mapCondensingTurbineIdeal(condensingTurbineIdeal);
        mapHighToMediumPressureTurbine(highToMediumPressureTurbine);
        mapHighToMediumPressureTurbineIdeal(highToMediumPressureTurbineIdeal);
        mapHighToLowPressureTurbine(highToLowPressureTurbine);
        mapHighToLowPressureTurbineIdeal(highToLowPressureTurbineIdeal);

        if (mediumPressureHeaderCalculationsDomain == nullptr) {
            logNullSkip(__func__, "mediumPressureHeaderCalculationsDomain");

            makeOutputObject("mediumPressureHeaderSteam");
            makeOutputObject("mediumPressureSteamHeatLoss");
            makeOutputObject("mediumPressureCondensate");
            makeOutputObject("mediumPressureToLowPressureTurbine");
            makeOutputObject("mediumPressureToLowPressureTurbineIdeal");
            makeOutputObject("highPressureToMediumPressurePrv");
        } else {
            const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput =
                    mediumPressureHeaderCalculationsDomain->mediumPressureHeaderOutput;
            mapMediumPressureHeaderSteam(mediumPressureHeaderOutput);

            const HeatLoss &mediumPressureHeaderHeatLoss = mediumPressureHeaderCalculationsDomain->mediumPressureHeaderHeatLoss;
            mapMediumPressureHeaderHeatLoss(mediumPressureHeaderHeatLoss);

            const SteamSystemModelerTool::FluidProperties &mediumPressureCondensate =
                    mediumPressureHeaderCalculationsDomain->mediumPressureCondensate;
            mapMediumPressureCondensate(mediumPressureCondensate);

            const std::shared_ptr<Turbine> &mediumToLowPressureTurbine =
                    mediumPressureHeaderCalculationsDomain->mediumToLowPressureTurbine;
            mapMediumToLowPressureTurbine(mediumToLowPressureTurbine);

            const std::shared_ptr<Turbine> &mediumToLowPressureTurbineIdeal =
                    mediumPressureHeaderCalculationsDomain->mediumToLowPressureTurbineIdeal;
            mapMediumToLowPressureTurbineIdeal(mediumToLowPressureTurbineIdeal);

            const std::shared_ptr<PrvWithoutDesuperheating> &highToMediumPressurePrv =
                    mediumPressureHeaderCalculationsDomain->highToMediumPressurePrv;
            mapHighToMediumPressurePrv(highToMediumPressurePrv);
        }

        //0-4 flash tanks
        mapBlowdownFlashTank(blowdownFlashTank);
        mapCondensateFlashTank(condensateFlashTank);
        mapHighPressureCondensateFlashTank(highPressureCondensateFlashTank);

        if (lowPressureHeaderCalculationsDomain == nullptr) {
            logNullSkip(__func__, "lowPressureHeaderCalculationsDomain");

            makeOutputObject("mediumPressureCondensateFlashTank");
            makeOutputObject("lowPressureHeaderSteam");
            makeOutputObject("lowPressureSteamHeatLoss");
            makeOutputObject("lowPressureCondensate");
            makeOutputObject("mediumPressureToLowPressurePrv");
        } else {
            const LowPressureFlashedSteamIntoHeaderCalculatorDomain &lowPressureFlashedSteamIntoHeaderCalculatorDomain =
                    lowPressureHeaderCalculationsDomain->lowPressureFlashedSteamIntoHeaderCalculatorDomain;
            const std::shared_ptr<FlashTank> &mediumPressureCondensateFlashTank =
                    lowPressureFlashedSteamIntoHeaderCalculatorDomain.mediumPressureCondensateFlashTank;
            mapMediumPressureCondensateFlashTank(mediumPressureCondensateFlashTank);

            const SteamSystemModelerTool::FluidProperties &lowPressureHeaderOutput =
                    lowPressureHeaderCalculationsDomain->lowPressureHeaderOutput;
            mapLowPressureHeaderSteam(lowPressureHeaderOutput);

            const HeatLoss &lowPressureHeaderHeatLoss = lowPressureHeaderCalculationsDomain->lowPressureHeaderHeatLoss;
            mapLowPressureHeaderHeatLoss(lowPressureHeaderHeatLoss);

            const SteamSystemModelerTool::FluidProperties &lowPressureCondensate =
                    lowPressureHeaderCalculationsDomain->lowPressureCondensate;
            mapLowPressureCondensate(lowPressureCondensate);

            const std::shared_ptr<PrvWithoutDesuperheating> &lowPressurePrv =
                    lowPressureHeaderCalculationsDomain->lowPressurePrv;
            mapMediumToLowPressurePrv(lowPressurePrv);
        }

        const std::shared_ptr<SteamSystemModelerTool::FluidProperties> &lowPressureVentedSteam =
                powerBalanceCheckerCalculationsDomain.lowPressureVentedSteam;
        mapLowPressureVentedSteam(lowPressureVentedSteam);

        logSection(methodName + ": converting to output: end");
    }

private:
    Local <Object> makeOutputObject(const std::string &outputName) {
        return makeOutputObjectOnObject(outputName, r);
    }

    Local <Object> makeOutputObjectOnObject(const std::string &outputName, Local <Object> onObject) {
        Local <Object> object = Nan::New<Object>();
        Local <String> localName = Nan::New<String>(outputName).ToLocalChecked();
        Nan::Set(onObject, localName, object);
        return object;
    }

    void logSection(const std::string &message) const {
        // std::cout << std::endl;
        // std::cout << "======== " << std::endl;
        // std::cout << "======== " << message << std::endl;
        // std::cout << "======== " << std::endl;
    }

    static void logMappingStart(const std::string &methodName) {
        // std::cout << "==== " << methodName << ": begin" << std::endl;
    }

    static void logCommonStart(const std::string &methodName, const std::string &outputName) {
        logMessage(methodName, "begin: outputName=" + outputName);
    }

    static void logNullSkip(const std::string &methodName, const std::string &objectName) {
        logMessage(methodName, objectName + " is null, skipping output mapping");
    }

    static void logMessage(const std::string &methodName, const std::string &message) {
        // std::cout << "SteamModelerOutputDataMapper::" << methodName << ": " << message << std::endl;
    }

    void
    mapFluidPropertiesToObject(const std::string &variableNamePrefix,
                               const SteamSystemModelerTool::FluidProperties &properties, Local <Object> object) {
        logMessage(__func__, "variableNamePrefix=" + variableNamePrefix);

        const double pressure = properties.pressure;
        const double temperature = properties.temperature;
        const double specificEnthalpy = properties.specificEnthalpy;
        const double specificEntropy = properties.specificEntropy;
        const double quality = properties.quality;
        const double volume = properties.specificVolume;
        const double massFlow = properties.massFlow;
        const double energyFlow = properties.energyFlow;

        setRobject(variableNamePrefix + "Pressure", pressure, object);
        setRobject(variableNamePrefix + "Temperature", temperature, object);
        setRobject(variableNamePrefix + "SpecificEnthalpy", specificEnthalpy, object);
        setRobject(variableNamePrefix + "SpecificEntropy", specificEntropy, object);
        setRobject(variableNamePrefix + "Quality", quality, object);
        setRobject(variableNamePrefix + "Volume", volume, object);
        setRobject(variableNamePrefix + "MassFlow", massFlow, object);
        setRobject(variableNamePrefix + "EnergyFlow", energyFlow, object);
    }

    void
    mapFluidProperties(const std::string &outputName, const SteamSystemModelerTool::FluidProperties &properties) {
        logCommonStart(__func__, outputName);

        Local <Object> object = makeOutputObject(outputName);

        const double pressure = properties.pressure;
        const double temperature = properties.temperature;
        const double specificEnthalpy = properties.specificEnthalpy;
        const double specificEntropy = properties.specificEntropy;
        const double quality = properties.quality;
        const double specificVolume = properties.specificVolume;
        const double massFlow = properties.massFlow;
        const double energyFlow = properties.energyFlow;

        setRobject("pressure", pressure, object);
        setRobject("temperature", temperature, object);
        setRobject("specificEnthalpy", specificEnthalpy, object);
        setRobject("specificEntropy", specificEntropy, object);
        setRobject("quality", quality, object);
        setRobject("specificVolume", specificVolume, object);
        setRobject("massFlow", massFlow, object);
        setRobject("energyFlow", energyFlow, object);
    }

    void
    mapSteamProperties(const std::string &outputName, const SteamSystemModelerTool::SteamPropertiesOutput &properties) {
        logCommonStart(__func__, outputName);

        Local <Object> object = makeOutputObject(outputName);

        const double pressure = properties.pressure;
        const double temperature = properties.temperature;
        const double specificEnthalpy = properties.specificEnthalpy;
        const double specificEntropy = properties.specificEntropy;
        const double quality = properties.quality;
        const double volume = properties.specificVolume;

        setRobject("pressure", pressure, object);
        setRobject("temperature", temperature, object);
        setRobject("specificEnthalpy", specificEnthalpy, object);
        setRobject("specificEntropy", specificEntropy, object);
        setRobject("quality", quality, object);
        setRobject("volume", volume, object);
    }

    void
    mapSteamPropertiesToObject(const std::string &variableNamePrefix,
                               const SteamSystemModelerTool::SteamPropertiesOutput &properties, Local <Object> object) {
        logMessage(__func__, "variableNamePrefix=" + variableNamePrefix);

        const double pressure = properties.pressure;
        const double temperature = properties.temperature;
        const double specificEnthalpy = properties.specificEnthalpy;
        const double specificEntropy = properties.specificEntropy;
        const double quality = properties.quality;
        const double volume = properties.specificVolume;

        setRobject(variableNamePrefix + "Pressure", pressure, object);
        setRobject(variableNamePrefix + "Temperature", temperature, object);
        setRobject(variableNamePrefix + "SpecificEnthalpy", specificEnthalpy, object);
        setRobject(variableNamePrefix + "SpecificEntropy", specificEntropy, object);
        setRobject(variableNamePrefix + "Quality", quality, object);
        setRobject(variableNamePrefix + "Volume", volume, object);
    }

    void
    mapProcessSteamUsage(const std::string &outputName, const ProcessSteamUsage &processSteamUsage) {
        logCommonStart(__func__, outputName);

        Local <Object> object = makeOutputObject(outputName);

        const double energyFlow = processSteamUsage.energyFlow;
        const double massFlow = processSteamUsage.massFlow;
        const double temperature = processSteamUsage.temperature;
        const double pressure = processSteamUsage.pressure;
        const double processUsage = processSteamUsage.processUsage;

        setRobject("energyFlow", energyFlow, object);
        setRobject("massFlow", massFlow, object);
        setRobject("temperature", temperature, object);
        setRobject("pressure", pressure, object);
        setRobject("processUsage", processUsage, object);
    }

    void mapOperations(const EnergyAndCostCalculationsDomain &energyDomain,
                       const MakeupWaterVolumeFlowCalculationsDomain &waterDomain) {
        logMappingStart(__func__);

        Local <Object> object = makeOutputObject("operationsOutput");

        const double powerGenerated = energyDomain.powerGenerated;
        const double boilerFuelCost = energyDomain.boilerFuelCost;
        const double makeupWaterCost = energyDomain.makeupWaterCost;
        const double totalOperatingCost = energyDomain.totalOperatingCost;
        const double powerGenerationCost = energyDomain.powerGenerationCost;
        const double boilerFuelUsage = energyDomain.boilerFuelUsage;
        const double sitePowerImport = energyDomain.sitePowerImport;
        const double sitePowerDemand = energyDomain.powerDemand;

        const double makeupWaterVolumeFlow = waterDomain.makeupWaterVolumeFlow;
        const double makeupWaterVolumeFlowAnnual = waterDomain.makeupWaterVolumeFlowAnnual;

        setRobject("powerGenerated", powerGenerated, object);
        setRobject("boilerFuelCost", boilerFuelCost, object);
        setRobject("makeupWaterCost", makeupWaterCost, object);
        setRobject("totalOperatingCost", totalOperatingCost, object);
        setRobject("powerGenerationCost", powerGenerationCost, object);
        setRobject("boilerFuelUsage", boilerFuelUsage, object);
        setRobject("sitePowerImport", sitePowerImport, object);
        setRobject("sitePowerDemand", sitePowerDemand, object);

        setRobject("makeupWaterVolumeFlow", makeupWaterVolumeFlow, object);
        setRobject("makeupWaterVolumeFlowAnnual", makeupWaterVolumeFlowAnnual, object);
    }

    void mapBoiler(const Boiler &boiler) {
        logMappingStart(__func__);

        Local <Object> object = makeOutputObject("boilerOutput");

        const SteamSystemModelerTool::FluidProperties &steamProperties = boiler.getSteamProperties();
        const SteamSystemModelerTool::FluidProperties &blowdownProperties = boiler.getBlowdownProperties();
        const SteamSystemModelerTool::FluidProperties &feedwaterProperties = boiler.getFeedwaterProperties();

        mapFluidPropertiesToObject("steam", steamProperties, object);
        mapFluidPropertiesToObject("blowdown", blowdownProperties, object);
        mapFluidPropertiesToObject("feedwater", feedwaterProperties, object);

        const double boilerEnergy = boiler.getBoilerEnergy();
        const double fuelEnergy = boiler.getFuelEnergy();
        const double blowdownRate = boiler.getBlowdownRate();
        const double combustionEff = boiler.getCombustionEfficiency();

        setRobject("boilerEnergy", boilerEnergy, object);
        setRobject("fuelEnergy", fuelEnergy, object);
        setRobject("blowdownRate", blowdownRate, object);
        setRobject("combustionEff", combustionEff, object);
    }

    void mapHeatLoss(const HeatLoss &headerHeatLoss, const std::string &outputName) {
        logMappingStart(__func__);
        const double heatLoss = headerHeatLoss.getHeatLoss();
        const SteamSystemModelerTool::FluidProperties &inletProperties = headerHeatLoss.getInletProperties();
        const SteamSystemModelerTool::FluidProperties &outletProperties = headerHeatLoss.getOutletProperties();

        Local <Object> object = makeOutputObject(outputName);
        setRobject("heatLoss", heatLoss, object);
        mapFluidPropertiesToObject("inlet", inletProperties, object);
        mapFluidPropertiesToObject("outlet", outletProperties, object);
    }

    void mapHighPressureHeaderHeatLoss(const HeatLoss &heatLoss) {
        logMappingStart(__func__);
        mapHeatLoss(heatLoss, "highPressureSteamHeatLoss");
    }

    void mapHighPressureHeaderSteam(const SteamSystemModelerTool::FluidProperties &highPressureHeaderOutput) {
        logMappingStart(__func__);
        mapFluidProperties("highPressureHeaderSteam", highPressureHeaderOutput);
    }

    void mapCombinedCondensate(const SteamSystemModelerTool::FluidProperties &combinedCondensate) {
        logMappingStart(__func__);

        mapFluidProperties("combinedCondensate", combinedCondensate);
    }

    void mapReturnCondensate(const SteamSystemModelerTool::FluidProperties &returnCondensate) {
        logMappingStart(__func__);
        mapFluidProperties("returnCondensate", returnCondensate);
    }

    void mapHighPressureCondensate(const SteamSystemModelerTool::FluidProperties &highPressureCondensate) {
        logMappingStart(__func__);
        mapFluidProperties("highPressureCondensate", highPressureCondensate);
    }

    void mapMakeupWater(const SteamSystemModelerTool::FluidProperties &makeupWater) {
        logMappingStart(__func__);
        mapFluidProperties("makeupWater", makeupWater);
    }

    void mapHeatExchanger(const std::shared_ptr<HeatExchanger::Output> &heatExchangerOutput) {
        logMappingStart(__func__);

        const std::string &outputName = "heatExchanger";

        Local <Object> object = makeOutputObject(outputName);

        if (heatExchangerOutput == nullptr) {
            logNullSkip(__func__, outputName);
            makeOutputObjectOnObject("coldOutlet", object);
            makeOutputObjectOnObject("hotOutlet", object);
        } else {
            const SteamSystemModelerTool::FluidProperties &coldOutlet = heatExchangerOutput->coldOutlet;
            const SteamSystemModelerTool::FluidProperties &hotOutlet = heatExchangerOutput->hotOutlet;

            mapFluidPropertiesToObject("coldOutlet", coldOutlet, object);
            mapFluidPropertiesToObject("hotOutlet", hotOutlet, object);
        }
    }

    void
    mapMakeupWaterAndCondensate(const SteamSystemModelerTool::FluidProperties &makeupWaterAndCondensateHeaderOutput) {
        logMappingStart(__func__);
        mapFluidProperties("makeupWaterAndCondensate", makeupWaterAndCondensateHeaderOutput);
    }

    void mapHighPressureProcessSteamUsage(const ProcessSteamUsage &steamUsage) {
        logMappingStart(__func__);
        mapProcessSteamUsage("highPressureProcessSteamUsage", steamUsage);
    }

    void mapProcessSteamUsage(const std::shared_ptr<ProcessSteamUsage> &steamUsage, const std::string &outputName) {
        logMappingStart(__func__);

        if (steamUsage == nullptr) {
            logNullSkip(__func__, outputName);
            makeOutputObject(outputName);
        } else {
            mapProcessSteamUsage(outputName, *steamUsage);
        }
    }

    void mapMediumPressureHeaderSteam(const SteamSystemModelerTool::FluidProperties &mediumPressureHeaderOutput) {
        logMappingStart(__func__);
        mapFluidProperties("mediumPressureHeaderSteam", mediumPressureHeaderOutput);
    }

    void mapMediumPressureHeaderHeatLoss(const HeatLoss &heatLoss) {
        logMappingStart(__func__);
        mapHeatLoss(heatLoss, "mediumPressureSteamHeatLoss");
    }

    void mapMediumPressureCondensate(const SteamSystemModelerTool::FluidProperties &mediumPressureCondensate) {
        logMappingStart(__func__);
        mapFluidProperties("mediumPressureCondensate", mediumPressureCondensate);
    }

    void mapLowPressureHeaderSteam(const SteamSystemModelerTool::FluidProperties &lowPressureHeaderOutput) {
        logMappingStart(__func__);
        mapFluidProperties("lowPressureHeaderSteam", lowPressureHeaderOutput);
    }

    void mapLowPressureHeaderHeatLoss(const HeatLoss &heatLoss) {
        logMappingStart(__func__);
        mapHeatLoss(heatLoss, "lowPressureSteamHeatLoss");
    }

    void mapLowPressureCondensate(const SteamSystemModelerTool::FluidProperties &lowPressureCondensate) {
        logMappingStart(__func__);
        mapFluidProperties("lowPressureCondensate", lowPressureCondensate);
    }

    void mapLowPressureVentedSteam(
            const std::shared_ptr<SteamSystemModelerTool::FluidProperties> &lowPressureVentedSteamPtr) {
        logMappingStart(__func__);

        const std::string outputName = "lowPressureVentedSteam";
        if (lowPressureVentedSteamPtr == nullptr) {
            logNullSkip(__func__, outputName);
            makeOutputObject(outputName);
        } else {
            SteamSystemModelerTool::FluidProperties lowPressureVentedSteam = *lowPressureVentedSteamPtr;
            mapFluidProperties(outputName, lowPressureVentedSteam);
        }
    }

    void mapDeaerator(const Deaerator &deaerator) {
        logMappingStart(__func__);

        Local <Object> object = makeOutputObject("deaeratorOutput");

        const SteamSystemModelerTool::FluidProperties &feedwaterProperties = deaerator.getFeedwaterProperties();
        const SteamSystemModelerTool::FluidProperties &inletSteamProperties = deaerator.getInletSteamProperties();
        const SteamSystemModelerTool::FluidProperties &inletWaterProperties = deaerator.getInletWaterProperties();
        const SteamSystemModelerTool::FluidProperties &ventedSteamProperties = deaerator.getVentedSteamProperties();

        mapFluidPropertiesToObject("feedwater", feedwaterProperties, object);
        mapFluidPropertiesToObject("inletSteam", inletSteamProperties, object);
        mapFluidPropertiesToObject("inletWater", inletWaterProperties, object);
        mapFluidPropertiesToObject("ventedSteam", ventedSteamProperties, object);
    }

    void mapFlashTank(const std::string &outputName, const std::shared_ptr<FlashTank> &flashTank) {
        logCommonStart(__func__, outputName);

        Local <Object> object = makeOutputObject(outputName);

        if (flashTank == nullptr) {
            logNullSkip(__func__, "flashTank " + outputName);
        } else {
            const SteamSystemModelerTool::FluidProperties &inletWaterProperties = flashTank->getInletWaterProperties();
            const SteamSystemModelerTool::FluidProperties &outletGasSaturatedProperties = flashTank->getOutletGasSaturatedProperties();
            const SteamSystemModelerTool::FluidProperties &outletLiquidSaturatedProperties = flashTank->getOutletLiquidSaturatedProperties();

            mapFluidPropertiesToObject("inletWater", inletWaterProperties, object);
            mapFluidPropertiesToObject("outletGas", outletGasSaturatedProperties, object);
            mapFluidPropertiesToObject("outletLiquid", outletLiquidSaturatedProperties, object);
        }
    }

    void mapBlowdownFlashTank(const std::shared_ptr<FlashTank> &flashTank) {
        logMappingStart(__func__);
        mapFlashTank("blowdownFlashTank", flashTank);
    }

    void mapCondensateFlashTank(const std::shared_ptr<FlashTank> &flashTank) {
        logMappingStart(__func__);
        mapFlashTank("condensateFlashTank", flashTank);
    }

    void mapHighPressureCondensateFlashTank(std::shared_ptr<FlashTank> const &flashTank) {
        logMappingStart(__func__);
        mapFlashTank("highPressureCondensateFlashTank", flashTank);
    }

    void mapMediumPressureCondensateFlashTank(std::shared_ptr<FlashTank> const &flashTank) {
        logMappingStart(__func__);
        mapFlashTank("mediumPressureCondensateFlashTank", flashTank);
    }

    void
    mapPrv(const std::string &outputName, const std::shared_ptr<PrvWithoutDesuperheating> &prvWithout) {
        logCommonStart(__func__, outputName);

        if (prvWithout == nullptr) {
            logNullSkip(__func__, "for " + outputName + ", PRV");
        } else {
            logMessage(__func__, "for " + outputName + ", PRV exists, mapping output");
            Local <Object> object = makeOutputObject(outputName);

            const SteamSystemModelerTool::SteamPropertiesOutput &inletProperties = prvWithout->getInletProperties();
            const double inletMassFlow = prvWithout->getInletMassFlow();
            const double inletEnergyFlow = prvWithout->getInletEnergyFlow();
            mapPrv(prvWithout, "inlet", inletProperties, inletMassFlow, inletEnergyFlow, object);

            const SteamSystemModelerTool::SteamPropertiesOutput &outletProperties = prvWithout->getOutletProperties();
            const double outletMassFlow = prvWithout->getOutletMassFlow();
            const double outletEnergyFlow = prvWithout->getOutletEnergyFlow();
            mapPrv(prvWithout, "outlet", outletProperties, outletMassFlow, outletEnergyFlow, object);

            if (prvWithout->isWithDesuperheating()) {
                std::shared_ptr<PrvWithDesuperheating> prvWith =
                        std::static_pointer_cast<PrvWithDesuperheating>(prvWithout);

                const SteamSystemModelerTool::SteamPropertiesOutput &feedwaterProperties =
                        prvWith->getFeedwaterProperties();
                const double feedwaterMassFlow = prvWith->getFeedwaterMassFlow();
                const double feedwaterEnergyFlow = prvWith->getFeedwaterEnergyFlow();
                mapPrv(prvWithout, "feedwater", feedwaterProperties, feedwaterMassFlow, feedwaterEnergyFlow, object);
            }
        }
    }

    void mapPrv(const std::shared_ptr<PrvWithoutDesuperheating> &prvWithout, const std::string &variableNamePrefix,
                const SteamSystemModelerTool::SteamPropertiesOutput &steamProperties, const double massFlow,
                const double energyFlow, const Local <Object> &object) {
        mapSteamPropertiesToObject(variableNamePrefix, steamProperties, object);
        setRobject(variableNamePrefix + "MassFlow", massFlow, object);
        setRobject(variableNamePrefix + "EnergyFlow", energyFlow, object);
    }

    void mapHighToMediumPressurePrv(const std::shared_ptr<PrvWithoutDesuperheating> &highToMediumPressurePrv) {
        logMappingStart(__func__);
        mapPrv("highPressureToMediumPressurePrv", highToMediumPressurePrv);
    }

    void mapMediumToLowPressurePrv(const std::shared_ptr<PrvWithoutDesuperheating> &lowPressurePrv) {
        logMappingStart(__func__);
        mapPrv("mediumPressureToLowPressurePrv", lowPressurePrv);
    }

    void mapTurbine(const std::string &outputName, const std::shared_ptr<Turbine> &turbine) {
        logCommonStart(__func__, outputName);

        Local <Object> object = makeOutputObject(outputName);

        if (turbine == nullptr) {
            logNullSkip(__func__, "for " + outputName + ", turbine");
        } else {
            logMessage(__func__, "for " + outputName + ", turbine exists, mapping output");

            const SteamSystemModelerTool::SteamPropertiesOutput &inletProperties = turbine->getInletProperties();
            const SteamSystemModelerTool::SteamPropertiesOutput &outletProperties = turbine->getOutletProperties();

            const double energyOut = turbine->getEnergyOut();
            const double generatorEfficiency = turbine->getGeneratorEfficiency();

            const double inletEnergyFlow = turbine->getInletEnergyFlow();
            const double inletPressure = inletProperties.pressure;
            const double inletQuality = inletProperties.quality;
            const double inletVolume = inletProperties.specificVolume;
            const double inletSpecificEnthalpy = inletProperties.specificEnthalpy;
            const double inletSpecificEntropy = inletProperties.specificEntropy;
            const double inletTemperature = inletProperties.temperature;

            const double isentropicEfficiency = turbine->getIsentropicEfficiency();
            const double massFlow = turbine->getMassFlow();
            const double outletEnergyFlow = turbine->getOutletEnergyFlow();
            const double outletPressure = outletProperties.pressure;
            const double outletQuality = outletProperties.quality;
            const double outletVolume = outletProperties.specificVolume;
            const double outletSpecificEnthalpy = outletProperties.specificEnthalpy;
            const double outletSpecificEntropy = outletProperties.specificEntropy;
            const double outletTemperature = outletProperties.temperature;
            const double powerOut = turbine->getPowerOut();

            setRobject("energyOut", energyOut, object);
            setRobject("generatorEfficiency", generatorEfficiency, object);
            setRobject("inletEnergyFlow", inletEnergyFlow, object);
            setRobject("inletPressure", inletPressure, object);
            setRobject("inletQuality", inletQuality, object);
            setRobject("inletVolume", inletVolume, object);
            setRobject("inletSpecificEnthalpy", inletSpecificEnthalpy, object);
            setRobject("inletSpecificEntropy", inletSpecificEntropy, object);
            setRobject("inletTemperature", inletTemperature, object);
            setRobject("isentropicEfficiency", isentropicEfficiency, object);
            setRobject("massFlow", massFlow, object);
            setRobject("outletEnergyFlow", outletEnergyFlow, object);
            setRobject("outletPressure", outletPressure, object);
            setRobject("outletQuality", outletQuality, object);
            setRobject("outletVolume", outletVolume, object);
            setRobject("outletSpecificEnthalpy", outletSpecificEnthalpy, object);
            setRobject("outletSpecificEntropy", outletSpecificEntropy, object);
            setRobject("outletTemperature", outletTemperature, object);
            setRobject("powerOut", powerOut, object);
        }
    }

    void mapCondensingTurbine(const std::shared_ptr<Turbine> &condensingTurbine) {
        logMappingStart(__func__);
        mapTurbine("condensingTurbine", condensingTurbine);
    }

    void mapCondensingTurbineIdeal(const std::shared_ptr<Turbine> &condensingTurbineIdeal) {
        logMappingStart(__func__);
        mapTurbine("condensingTurbineIdeal", condensingTurbineIdeal);
    }

    void mapHighToMediumPressureTurbine(const std::shared_ptr<Turbine> &highToMediumPressureTurbine) {
        logMappingStart(__func__);
        mapTurbine("highPressureToMediumPressureTurbine", highToMediumPressureTurbine);
    }

    void mapHighToMediumPressureTurbineIdeal(const std::shared_ptr<Turbine> &highToMediumPressureTurbineIdeal) {
        logMappingStart(__func__);
        mapTurbine("highPressureToMediumPressureTurbineIdeal", highToMediumPressureTurbineIdeal);
    }

    void mapHighToLowPressureTurbine(const std::shared_ptr<Turbine> &highToLowPressureTurbine) {
        logMappingStart(__func__);
        mapTurbine("highPressureToLowPressureTurbine", highToLowPressureTurbine);
    }

    void mapHighToLowPressureTurbineIdeal(const std::shared_ptr<Turbine> &highToLowPressureTurbineIdeal) {
        logMappingStart(__func__);
        mapTurbine("highPressureToLowPressureTurbineIdeal", highToLowPressureTurbineIdeal);
    }

    void mapMediumToLowPressureTurbine(const std::shared_ptr<Turbine> &mediumToLowPressureTurbine) {
        logMappingStart(__func__);
        mapTurbine("mediumPressureToLowPressureTurbine", mediumToLowPressureTurbine);
    }

    void mapMediumToLowPressureTurbineIdeal(const std::shared_ptr<Turbine> &mediumToLowPressureTurbineIdeal) {
        logMappingStart(__func__);
        mapTurbine("mediumPressureToLowPressureTurbineIdeal", mediumToLowPressureTurbineIdeal);
    }
};

#endif //AMO_TOOLS_SUITE_STEAMMODELEROUTPUTDATAMAPPER_H
