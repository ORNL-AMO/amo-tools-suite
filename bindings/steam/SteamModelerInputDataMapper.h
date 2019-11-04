#ifndef AMO_TOOLS_SUITE_STEAMMODELERINPUTDATAMAPPER_H
#define AMO_TOOLS_SUITE_STEAMMODELERINPUTDATAMAPPER_H

#include "../NanDataConverters.h"
#include <ssmt/api/SteamModelerInput.h>

class SteamModelerInputDataMapper {
public:
    const SteamModelerInput map() {
        const std::string methodName = std::string("SteamModelerInputDataMapper::") + std::string(__func__) + ": ";

        logSection(methodName + "converting from input: begin");

        // catch C++ error and throw as JS error
        try {
            const bool isBaselineCalc = mapInputToBaselineCalc();
            const double baselinePowerDemand = mapInputToBaselinePowerDemand();

            const BoilerInput boilerInput = mapInputToBoilerInput();
            const HeaderInput headerInput = mapInputToHeaderInput();
            const OperationsInput operationsInput = mapInputToOperationsInput();
            const TurbineInput turbineInput = mapInputToTurbineInput();

            logSection(methodName + "converting from input: end");

            return {isBaselineCalc, baselinePowerDemand, boilerInput, headerInput, operationsInput, turbineInput};
        } catch (const std::runtime_error &e) {
            const std::string what = e.what();
            const std::string failMsg = "ERROR mapping SteamModeler input: " + what;

            // std::cout << methodName << failMsg << std::endl;

            Local <String> failMsgLocal = Nan::New<String>(failMsg).ToLocalChecked();
            ThrowError(failMsgLocal);
        }
    }

private:
    void logSection(const std::string &message) const {
        // std::cout << "======== " << std::endl;
        // std::cout << "======== " << message << std::endl;
        // std::cout << "======== " << std::endl;
    }

    bool mapInputToBaselineCalc() {
        const std::string methodName = std::string("SteamModelerInputDataMapper::") + std::string(__func__) + ": ";

        // std::cout << methodName << "begin" << std::endl;

        const bool isBaselineCalc = getBoolFromString("isBaselineCalc");

        // std::cout << methodName << "end: isBaselineCalc=" << isBaselineCalc << std::endl;

        return isBaselineCalc;
    }

    double mapInputToBaselinePowerDemand() {
        const std::string methodName = std::string("SteamModelerInputDataMapper::") + std::string(__func__) + ": ";

        // std::cout << methodName << "begin" << std::endl;

        const double baselinePowerDemand = getDouble("baselinePowerDemand");

        // std::cout << methodName << "end: baselinePowerDemand=" << baselinePowerDemand << std::endl;

        //check for NaN (if true, doesn't equal itself)
        if (baselinePowerDemand != baselinePowerDemand) {
            std::string msg = methodName + "baselinePowerDemand is not specified, aborting";
            // std::cout << msg << std::endl;
            throw std::invalid_argument(msg);
        }

        return baselinePowerDemand;
    }

    const BoilerInput mapInputToBoilerInput() {
        const std::string methodName = std::string("SteamModelerInputDataMapper::") + std::string(__func__) + ": ";

        // std::cout << methodName << "begin" << std::endl;

        Local <Object> boilerInputObject = getObject("boilerInput");

        double fuelType = getDouble("fuelType", boilerInputObject);
        double fuel = getDouble("fuel", boilerInputObject);
        double combustionEfficiency = getDouble("combustionEfficiency", boilerInputObject);
        double blowdownRate = getDouble("blowdownRate", boilerInputObject);
        bool blowdownFlashed = getBoolFromString("blowdownFlashed", boilerInputObject);
        bool preheatMakeupWater = getBoolFromString("preheatMakeupWater", boilerInputObject);
        double steamTemperature = getDouble("steamTemperature", boilerInputObject);
        double deaeratorVentRate = getDouble("deaeratorVentRate", boilerInputObject);
        double deaeratorPressure = getDouble("deaeratorPressure", boilerInputObject);
        double approachTemperature = getDouble("approachTemperature", boilerInputObject);

        const BoilerInput &boilerInput =
                {fuelType, fuel, combustionEfficiency, blowdownRate, blowdownFlashed, preheatMakeupWater,
                 steamTemperature, deaeratorVentRate, deaeratorPressure, approachTemperature};

        // std::cout << methodName << "end: boilerInput=" << boilerInput << std::endl;

        return boilerInput;
    }

    const HeaderInput mapInputToHeaderInput() {
        const std::string methodName = std::string("SteamModelerInputDataMapper::") + std::string(__func__) + ": ";

        // std::cout << methodName << "begin" << std::endl;

        Local <Object> headerInputObject = getObject("headerInput");

        Local <Object> highPressureObject = getObject("highPressureHeader", headerInputObject);
        Local <Object> mediumPressureObject = getObject("mediumPressureHeader", headerInputObject);
        Local <Object> lowPressureObject = getObject("lowPressureHeader", headerInputObject);

        HeaderWithHighestPressure highPressure = mapInputToHeaderWithHighestPressure(highPressureObject);
        std::shared_ptr<HeaderNotHighestPressure> mediumPressure =
                mapInputToHeaderNotHighestPressure(mediumPressureObject, "mediumPressureObject");
        std::shared_ptr<HeaderNotHighestPressure> lowPressure =
                mapInputToHeaderNotHighestPressure(lowPressureObject, "lowPressureObject");

        const HeaderInput &headerInput = {highPressure, mediumPressure, lowPressure};

        // std::cout << methodName << "end: headerInput=" << headerInput << std::endl;

        return headerInput;
    }

    const OperationsInput mapInputToOperationsInput() {
        const std::string methodName = std::string("SteamModelerInputDataMapper::") + std::string(__func__) + ": ";

        // std::cout << methodName << "begin" << std::endl;

        Local <Object> operationsInputObject = getObject("operationsInput");

        double sitePowerImport = getDouble("sitePowerImport", operationsInputObject);
        double makeUpWaterTemperature = getDouble("makeUpWaterTemperature", operationsInputObject);
        double operatingHoursPerYear = getDouble("operatingHoursPerYear", operationsInputObject);
        double fuelCosts = getDouble("fuelCosts", operationsInputObject);
        double electricityCosts = getDouble("electricityCosts", operationsInputObject);
        double makeUpWaterCosts = getDouble("makeUpWaterCosts", operationsInputObject);

        const OperationsInput &operationsInput =
                {sitePowerImport, makeUpWaterTemperature, operatingHoursPerYear, fuelCosts, electricityCosts,
                 makeUpWaterCosts};

        // std::cout << methodName << "end: operationsInput=" << operationsInput << std::endl;

        return operationsInput;
    }

    const TurbineInput mapInputToTurbineInput() {
        const std::string methodName = std::string("SteamModelerInputDataMapper::") + std::string(__func__) + ": ";

        // std::cout << methodName << "begin" << std::endl;

        Local <Object> turbineInputObject = getObject("turbineInput");
        Local <Object> highToLowTurbineObject = getObject("highToLowTurbine", turbineInputObject);
        Local <Object> highToMediumTurbineObject = getObject("highToMediumTurbine", turbineInputObject);
        Local <Object> mediumToLowTurbineObject = getObject("mediumToLowTurbine", turbineInputObject);

        CondensingTurbine condensingTurbine = mapInputToTurbineCondensingTurbine(turbineInputObject);
        PressureTurbine highToLowTurbine =
                mapInputToTurbinePressureTurbine(highToLowTurbineObject, "highToLowTurbine");
        PressureTurbine highToMediumTurbine =
                mapInputToTurbinePressureTurbine(highToMediumTurbineObject, "highToMediumTurbine");
        PressureTurbine mediumToLowTurbine =
                mapInputToTurbinePressureTurbine(mediumToLowTurbineObject, "mediumToLowTurbine");

        const TurbineInput &turbineInput =
                {condensingTurbine, highToLowTurbine, highToMediumTurbine, mediumToLowTurbine};
        // std::cout << methodName << "end" << std::endl;

        return turbineInput;
    }

    HeaderWithHighestPressure mapInputToHeaderWithHighestPressure(Local <Object> headerObject) {
        const std::string methodName = std::string("SteamModelerInputDataMapper::") + std::string(__func__) + ": ";

        // std::cout << methodName << "begin" << std::endl;

        double pressure = getDouble("pressure", headerObject);
        double processSteamUsage = getDouble("processSteamUsage", headerObject);
        double condensationRecoveryRate = getDouble("condensationRecoveryRate", headerObject);
        double heatLoss = getDouble("heatLoss", headerObject);

        double condensateReturnTemperature = getDouble("condensateReturnTemperature", headerObject);
        bool flashCondensateReturn = getBoolFromString("flashCondensateReturn", headerObject);

        const HeaderWithHighestPressure &header =
                {pressure, processSteamUsage, condensationRecoveryRate, heatLoss, condensateReturnTemperature,
                 flashCondensateReturn};

        // std::cout << methodName << "end: header=" << header << std::endl;

        return header;
    }

    std::shared_ptr<HeaderNotHighestPressure>
    mapInputToHeaderNotHighestPressure(Local <Object> headerObject, const std::string &objectName) {
        const std::string methodName = std::string("SteamModelerInputDataMapper::") + std::string(__func__) + ": ";

        std::shared_ptr<HeaderNotHighestPressure> header = nullptr;

        // std::cout << methodName << "begin: objectName=" << objectName << std::endl;

        if (isHeaderSpecified(headerObject)) {
            double pressure = getDouble("pressure", headerObject);
            double processSteamUsage = getDouble("processSteamUsage", headerObject);
            double condensationRecoveryRate = getDouble("condensationRecoveryRate", headerObject);
            double heatLoss = getDouble("heatLoss", headerObject);

            bool flashCondensateIntoHeader = getBoolFromString("flashCondensateIntoHeader", headerObject);

            bool desuperheatSteamIntoNextHighest = getBoolFromString("desuperheatSteamIntoNextHighest", headerObject);
            double desuperheatSteamTemperature = getDouble("desuperheatSteamTemperature", headerObject);

            header =
                    std::make_shared<HeaderNotHighestPressure>(pressure, processSteamUsage, condensationRecoveryRate,
                                                               heatLoss, flashCondensateIntoHeader,
                                                               desuperheatSteamIntoNextHighest,
                                                               desuperheatSteamTemperature);
            // std::cout << methodName << "end: header=" << *header << std::endl;
        } else {
            auto msg = std::string(methodName + "end: headerObject does not exist for " + objectName + ", skipping it");
            // std::cout << msg << std::endl;
        }

        return header;
    }

    /** If not null/empty, use a field to determine if header exists. */
    bool isHeaderSpecified(Local <Object> headerObject) {
        bool isSpecified;

        if (headerObject.IsEmpty()) {
            isSpecified = false;
        } else {
            double pressure = getDouble("pressure", headerObject);
            isSpecified = !std::isnan(pressure);
        }

        return isSpecified;
    }

    CondensingTurbine mapInputToTurbineCondensingTurbine(Local <Object> turbineInputObject) {
        const std::string methodName = std::string("SteamModelerInputDataMapper::") + std::string(__func__) + ": ";

        // std::cout << methodName << "begin" << std::endl;

        Local <Object> condensingTurbineObject = getObject("condensingTurbine", turbineInputObject);

        double isentropicEfficiency = getDouble("isentropicEfficiency", condensingTurbineObject);
        double generationEfficiency = getDouble("generationEfficiency", condensingTurbineObject);
        double condenserPressure = getDouble("condenserPressure", condensingTurbineObject);
        int operationTypeValue = getInteger("operationType", condensingTurbineObject);
        CondensingTurbineOperation operationType = static_cast< CondensingTurbineOperation >(operationTypeValue);
        double operationValue = getDouble("operationValue", condensingTurbineObject);
        bool useTurbine = getBoolFromString("useTurbine", condensingTurbineObject);

        CondensingTurbine condensingTurbine =
                {isentropicEfficiency, generationEfficiency, condenserPressure, operationType, operationValue,
                 useTurbine};

        // std::cout << methodName << "end: condensingTurbine=" << condensingTurbine << std::endl;

        return condensingTurbine;
    }

    PressureTurbine
    mapInputToTurbinePressureTurbine(Local <Object> pressureTurbineObject, const std::string &objectName) {
        const std::string methodName = std::string("SteamModelerInputDataMapper::") + std::string(__func__) + ": ";

        // std::cout << methodName << "begin: objectName=" << objectName << std::endl;

        double isentropicEfficiency = getDouble("isentropicEfficiency", pressureTurbineObject);
        double generationEfficiency = getDouble("generationEfficiency", pressureTurbineObject);
        int operationTypeValue = getInteger("operationType", pressureTurbineObject);
        PressureTurbineOperation operationType = static_cast< PressureTurbineOperation >(operationTypeValue);
        double operationValue1 = getDouble("operationValue1", pressureTurbineObject);
        double operationValue2 = getDouble("operationValue2", pressureTurbineObject);
        bool useTurbine = getBoolFromString("useTurbine", pressureTurbineObject);

        PressureTurbine pressureTurbine =
                {isentropicEfficiency, generationEfficiency, operationType, operationValue1, operationValue2,
                 useTurbine};

        // std::cout << methodName << "end: object " << objectName << "=" << pressureTurbine << std::endl;

        return pressureTurbine;
    }
};

#endif //AMO_TOOLS_SUITE_STEAMMODELERINPUTDATAMAPPER_H
