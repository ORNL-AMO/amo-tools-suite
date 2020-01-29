const test = require('tap').test
    , testRoot = require('path').resolve(__dirname, '../../')
    , bindings = require('bindings')({module_root: testRoot, bindings: 'ssmt'});

function rnd(value) {
    return Number(Math.round(value + 'e' + 6) + 'e-' + 6);
}

test('steamModeler', function (t) {
    t.type(bindings.steamModeler, 'function');

    var steamModelerInput = makeSteamModelerInput();
    var actual = bindings.steamModeler(steamModelerInput);

    //TODO remove me and assert values when actual results return
    console.log("================");
    console.log("JS steamModelerInput=");
    console.log(JSON.stringify(steamModelerInput));
    console.log("================");
    console.log("JS actual=");
    console.log(JSON.stringify(actual));
    console.log("================");

    validate(t, actual);

    // TODO resolve these
    // not used: validateHeaderOutputObj(t, actual);
    // not used (part of unused headerOutputObj): validateHeatLossOutput(t, actual);

    t.end();
});

function makeSteamModelerInput() {
    var boilerInput = {
        "fuelType": 1,
        "fuel": 1,
        "combustionEfficiency": 85,
        "blowdownRate": 2,
        "blowdownFlashed": false,
        "preheatMakeupWater": false,
        "steamTemperature": 582.5388888888889,
        "deaeratorVentRate": 0.1,
        "deaeratorPressure": 0.20474679297336734,
        "approachTemperature": null
    };

    var header1 = {
        "pressure": 4.238181393236699,
        "processSteamUsage": 22679.6,
        "condensationRecoveryRate": 50,
        "heatLoss": 0.1,
        "condensateReturnTemperature": 338.7055555555555,
        "flashCondensateReturn": false
    };
    var header2 = {
        "pressure": 1.135539393034136,
        "processSteamUsage": 10,
        "condensationRecoveryRate": 50,
        "heatLoss": 0.1,
        "flashCondensateIntoHeader": false,
        "desuperheatSteamIntoNextHighest": false,
        "desuperheatSteamTemperature": 255.3722222222222,
        "useBaselineProcessSteamUsage": true
    };
    var header3 = {
        "pressure": 0.23922059297561804,
        "processSteamUsage": 0,
        "condensationRecoveryRate": 50,
        "heatLoss": 0.1,
        "flashCondensateIntoHeader": false,
        "desuperheatSteamIntoNextHighest": false,
        "desuperheatSteamTemperature": 255.3722222222222,
        "useBaselineProcessSteamUsage": true
    };
    var headerInput = {
        highPressureHeader: header1,
        mediumPressureHeader: header2,
        lowPressureHeader: header3,
    };

    var operationsInput = {
        "sitePowerImport": 17999996.900135573,
        "makeUpWaterTemperature": 283.15,
        "operatingHoursPerYear": 8000,
        "fuelCosts": 0.0000054783614202035905,
        "electricityCosts": 0.00001388889128076,
        "makeUpWaterCosts": 0.6604301289062501
    };

    var condensingTurbine = {
        "isentropicEfficiency": null,
        "generationEfficiency": null,
        "condenserPressure": null,
        "operationType": null,
        "operationValue": null,
        "useTurbine": false
    };
    var highToLowTurbine = {
        "isentropicEfficiency": null,
        "generationEfficiency": null,
        "operationType": null,
        "operationValue1": null,
        "operationValue2": null,
        "useTurbine": false
    };
    var highToMediumTurbine = {
        "isentropicEfficiency": null,
        "generationEfficiency": null,
        "operationType": null,
        "operationValue1": null,
        "operationValue2": null,
        "useTurbine": false
    };
    var mediumToLowTurbine = {
        "isentropicEfficiency": null,
        "generationEfficiency": null,
        "operationType": null,
        "operationValue1": null,
        "operationValue2": null,
        "useTurbine": false
    };
    var turbineInput = {
        condensingTurbine: condensingTurbine,
        highToLowTurbine: highToLowTurbine,
        highToMediumTurbine: highToMediumTurbine,
        mediumToLowTurbine: mediumToLowTurbine,
    };

    return {
        isBaselineCalc: true,
        baselinePowerDemand: 1,
        boilerInput: boilerInput,
        headerInput: headerInput,
        operationsInput: operationsInput,
        turbineInput: turbineInput,
    };
}

function validate(t, actual) {
    validateOperations(t, actual);
    validateBoiler(t, actual);
    validateDeaerator(t, actual);

    validateHighPressureHeaderSteam(t, actual);
    validateMakeupWater(t, actual);
    validateMakeupWaterAndCondensate(t, actual);
    validateHighPressureProcessSteamUsage(t, actual);

    validateCondensingTurbine(t, actual);
    validateHighPressureToMediumPressureTurbine(t, actual);
    validateHighPressureToLowPressureTurbine(t, actual);
    validateMediumPressureToLowPressureTurbine(t, actual);

    validateMediumPressureHeaderSteam(t, actual);
    validateLowPressureHeaderSteam(t, actual);

    validateHighPressureToMediumPressurePrv(t, actual);
    validateMediumPressureToLowPressurePrv(t, actual);

    validateBlowdownFlashTank(t, actual);
    validateCondensateFlashTank(t, actual);
    validateHighPressureCondensateFlashTank(t, actual);
    validateMediumPressureCondensateFlashTank(t, actual);

    validateVentedSteam(t, actual);
}

function validateOperations(t, actual) {
    var actualOperations = actual.operationsOutput;

    t.equal(actualOperations.powerGenerated, 1);
    t.equal(actualOperations.boilerFuelCost, 3270792.6312980186);
    t.equal(actualOperations.makeupWaterCost, 62865.242016338656);
    t.equal(actualOperations.totalOperatingCost, 5335257.873314357);
    t.equal(actualOperations.powerGenerationCost, 2001600.0000000002);
    t.equal(actualOperations.boilerFuelUsage, 597077880850.3137);
    t.equal(actualOperations.sitePowerImport, 18000000);
    t.equal(actualOperations.sitePowerDemand, 18000001);
}

function validateBoiler(t, actual) {
    var actualBoiler = actual.boilerOutput;

    t.equal(actualBoiler.steamPressure, 1.136);
    t.equal(actualBoiler.steamTemperature, 514.2);
    t.equal(actualBoiler.steamSpecificEnthalpy, 2917.838793476974);
    t.equal(actualBoiler.steamSpecificEntropy, 6.8211883441380285);
    t.equal(actualBoiler.steamQuality, 1);
    t.equal(actualBoiler.steamVolume, 0.19960611293064956);
    t.equal(actualBoiler.steamMassFlow, 26261.30665156052);
    t.equal(actualBoiler.steamEnergyFlow, 76626259.31531818);

    t.equal(actualBoiler.blowdownPressure, 1.136);
    t.equal(actualBoiler.blowdownTemperature, 458.65273574845287);
    t.equal(actualBoiler.blowdownSpecificEnthalpy, 787.5557825431341);
    t.equal(actualBoiler.blowdownSpecificEntropy, 2.1926588668498703);
    t.equal(actualBoiler.blowdownQuality, 0);
    t.equal(actualBoiler.blowdownVolume, 0.001135002372530348);
    t.equal(actualBoiler.blowdownMassFlow, 535.9450337053167);
    t.equal(actualBoiler.blowdownEnergyFlow, 422086.61041989713);

    t.equal(actualBoiler.feedwaterPressure, 0.204747);
    t.equal(actualBoiler.feedwaterTemperature, 394.1047658380901);
    t.equal(actualBoiler.feedwaterSpecificEnthalpy, 507.8439104587324);
    t.equal(actualBoiler.feedwaterSpecificEntropy, 1.5381113180769446);
    t.equal(actualBoiler.feedwaterQuality, 0);
    t.equal(actualBoiler.feedwaterVolume, 0.0010611946777477246);
    t.equal(actualBoiler.feedwaterMassFlow, 26797.251685265837);
    t.equal(actualBoiler.feedwaterEnergyFlow, 13608821.08539226);

    t.equal(actualBoiler.boilerEnergy, 63439524.84034582);
    t.equal(actualBoiler.fuelEnergy, 74634735.10628921);
    t.equal(actualBoiler.blowdownRate, 2);
    t.equal(actualBoiler.combustionEff, 85);
}

function validateFluidProperties(t, actual, expected) {
    t.equal(actual.pressure, expected.pressure);
    t.equal(actual.temperature, expected.temperature);
    t.equal(actual.specificEnthalpy, expected.specificEnthalpy);
    t.equal(actual.specificEntropy, expected.specificEntropy);
    t.equal(actual.quality, expected.quality);
    t.equal(actual.specificVolume, expected.specificVolume);
    t.equal(actual.massFlow, expected.massFlow);
    t.equal(actual.energyFlow, expected.energyFlow);
}

function validateHighPressureHeaderSteam(t, actual) {
    var expected = {
        pressure: 1.136,
        temperature: 512.922016091814,
        specificEnthalpy: 2914.9209546834954,
        specificEntropy: 6.81550675722266,
        quality: 1,
        specificVolume: 0.19900804292495916,
        massFlow: 26261.30665156052,
        energyFlow: 76549633.05600286,
    };
    validateFluidProperties(t, actual.highPressureHeaderSteam, expected);
}

function validateMakeupWater(t, actual) {
    var expected = {
        pressure: 0.101325,
        temperature: 283.15,
        specificEnthalpy: 42.11872247100606,
        specificEntropy: 0.15107627374941596,
        quality: 0,
        specificVolume: 0.0010002985489353622,
        massFlow: 11902.742285390581,
        energyFlow: 501328.2989622743,
    };
    validateFluidProperties(t, actual.makeupWater, expected);
}

function validateMakeupWaterAndCondensate(t, actual) {
    var expected = {
        pressure: 0.204747,
        temperature: 316.30494350601737,
        specificEnthalpy: 180.8983251038269,
        specificEntropy: 0.6142383405729469,
        quality: 0,
        specificVolume: 0.0010090535069731171,
        massFlow: 23594.761880492584,
        energyFlow: 4268252.905404726,
    };
    validateFluidProperties(t, actual.makeupWaterAndCondensate, expected);
}

//FIXME correct the undefineds - actual values
function validateHighPressureProcessSteamUsage(t, actual) {
    var expected = {
        pressure: 1.136,
        temperature: 512.922016091814,
        specificEnthalpy: undefined,
        specificEntropy: undefined,
        quality: undefined,
        specificVolume: undefined,
        massFlow: 22680,
        energyFlow: 66110407.252221674
    };
    validateFluidProperties(t, actual.highPressureProcessSteamUsage, expected);
}

function validateMediumPressureHeaderSteam(t, actual) {
    var expected = {
        pressure: undefined,
        temperature: undefined,
        specificEnthalpy: undefined,
        specificEntropy: undefined,
        quality: undefined,
        specificVolume: undefined,
        massFlow: undefined,
        energyFlow: undefined,
    };
    validateFluidProperties(t, actual.mediumPressureHeaderSteam, expected);
}

function validateLowPressureHeaderSteam(t, actual) {
    var expected = {
        pressure: undefined,
        temperature: undefined,
        specificEnthalpy: undefined,
        specificEntropy: undefined,
        quality: undefined,
        specificVolume: undefined,
        massFlow: undefined,
        energyFlow: undefined,
    };
    validateFluidProperties(t, actual.lowPressureHeaderSteam, expected);
}

function validateVentedSteam(t, actual) {
    var ventedSteam = actual.ventedSteam;
    if (typeof ventedSteam !== 'undefined') {
        var expected = {
            pressure: 0,
            temperature: 0,
            specificEnthalpy: 0,
            specificEntropy: 0,
            quality: 0,
            specificVolume: 0,
            massFlow: 0,
            energyFlow: 0,
        };
        validateFluidProperties(t, ventedSteam, expected);
    }
}

function validateDeaerator(t, actual) {
    var actualDeaerator = actual.deaeratorOutput;

    t.equal(actualDeaerator.feedwaterEnergyFlow, 13608821.08539226);
    t.equal(actualDeaerator.feedwaterMassFlow, 26797.251685265837);
    t.equal(actualDeaerator.feedwaterPressure, 0.204747);
    t.equal(actualDeaerator.feedwaterQuality, 0);
    t.equal(actualDeaerator.feedwaterSpecificEnthalpy, 507.8439104587324);
    t.equal(actualDeaerator.feedwaterSpecificEntropy, 1.5381113180769446);
    t.equal(actualDeaerator.feedwaterTemperature, 394.1047658380901);
    t.equal(actualDeaerator.feedwaterVolume, 0.0010611946777477246);
    t.equal(actualDeaerator.inletSteamEnergyFlow, 9413116.876752699);
    t.equal(actualDeaerator.inletSteamMassFlow, 3229.2871824288577);
    t.equal(actualDeaerator.inletSteamPressure, 1.136);
    t.equal(actualDeaerator.inletSteamQuality, 1);
    t.equal(actualDeaerator.inletSteamSpecificEnthalpy, 2914.920954683495);
    t.equal(actualDeaerator.inletSteamSpecificEntropy, 6.815506757222659);
    t.equal(actualDeaerator.inletSteamVolume, 0.199008042924959);
    t.equal(actualDeaerator.inletSteamTemperature, 512.9220160918137);
    t.equal(actualDeaerator.inletWaterEnergyFlow, 4268252.882616886);
    t.equal(actualDeaerator.inletWaterMassFlow, 23594.761754522246);
    t.equal(actualDeaerator.inletWaterPressure, 0.204747);
    t.equal(actualDeaerator.inletWaterQuality, 0);
    t.equal(actualDeaerator.inletWaterSpecificEnthalpy, 180.89832510382604);
    t.equal(actualDeaerator.inletWaterSpecificEntropy, 0.6142383405729448);
    t.equal(actualDeaerator.inletWaterVolume, 0.0010090535069731171);
    t.equal(actualDeaerator.inletWaterTemperature, 316.30494350601714);
    t.equal(actualDeaerator.ventedSteamEnergyFlow, 72548.67397732535);
    t.equal(actualDeaerator.ventedSteamMassFlow, 26.797251685265838);
    t.equal(actualDeaerator.ventedSteamPressure, 0.204747);
    t.equal(actualDeaerator.ventedSteamQuality, 1);
    t.equal(actualDeaerator.ventedSteamSpecificEnthalpy, 2707.3177066592766);
    t.equal(actualDeaerator.ventedSteamSpecificEntropy, 7.11902821487601);
    t.equal(actualDeaerator.ventedSteamVolume, 0.8664961907839751);
    t.equal(actualDeaerator.ventedSteamTemperature, 394.1047658380901);
}

function validateFlashTank(t, actual, expected) {
    t.equal(actual.inletWaterEnergyFlow, expected.inletWaterEnergyFlow);
    t.equal(actual.inletWaterMassFlow, expected.inletWaterMassFlow);
    t.equal(actual.inletWaterPressure, expected.inletWaterPressure);
    t.equal(actual.inletWaterQuality, expected.inletWaterQuality);
    t.equal(actual.inletWaterVolume, expected.inletWaterVolume);
    t.equal(actual.inletWaterSpecificEnthalpy, expected.inletWaterSpecificEnthalpy);
    t.equal(actual.inletWaterSpecificEntropy, expected.inletWaterSpecificEntropy);
    t.equal(actual.inletWaterTemperature, expected.inletWaterTemperature);
    t.equal(actual.outletGasEnergyFlow, expected.outletGasEnergyFlow);
    t.equal(actual.outletGasMassFlow, expected.outletGasMassFlow);
    t.equal(actual.outletGasPressure, expected.outletGasPressure);
    t.equal(actual.outletGasQuality, expected.outletGasQuality);
    t.equal(actual.outletGasVolume, expected.outletGasVolume);
    t.equal(actual.outletGasSpecificEnthalpy, expected.outletGasSpecificEnthalpy);
    t.equal(actual.outletGasSpecificEntropy, expected.outletGasSpecificEntropy);
    t.equal(actual.outletGasTemperature, expected.outletGasTemperature);
    t.equal(actual.outletLiquidEnergyFlow, expected.outletLiquidEnergyFlow);
    t.equal(actual.outletLiquidMassFlow, expected.outletLiquidMassFlow);
    t.equal(actual.outletLiquidPressure, expected.outletLiquidPressure);
    t.equal(actual.outletLiquidQuality, expected.outletLiquidQuality);
    t.equal(actual.outletLiquidVolume, expected.outletLiquidVolume);
    t.equal(actual.outletLiquidSpecificEnthalpy, expected.outletLiquidSpecificEnthalpy);
    t.equal(actual.outletLiquidSpecificEntropy, expected.outletLiquidSpecificEntropy);
    t.equal(actual.outletLiquidTemperature, expected.outletLiquidTemperature);
}

function validateBlowdownFlashTank(t, actual) {
    var expected = {
        inletWaterEnergyFlow: 422086.61041989713,
        inletWaterMassFlow: 535.9450337053167,
        inletWaterPressure: 1.136,
        inletWaterQuality: 0,
        inletWaterVolume: 0.001135002372530348,
        inletWaterSpecificEnthalpy: 787.5557825431341,
        inletWaterSpecificEntropy: 2.1926588668498703,
        inletWaterTemperature: 458.65273574845287,
        outletGasEnergyFlow: 0,
        outletGasMassFlow: 0,
        outletGasPressure: 1.136,
        outletGasQuality: 1,
        outletGasVolume: 0.17205143404254775,
        outletGasSpecificEnthalpy: 2781.8312880011326,
        outletGasSpecificEntropy: 6.540793415910698,
        outletGasTemperature: 458.65273574845287,
        outletLiquidEnergyFlow: 422086.61041989713,
        outletLiquidMassFlow: 535.9450337053167,
        outletLiquidPressure: 1.136,
        outletLiquidQuality: 0,
        outletLiquidVolume: 0.001135002372530348,
        outletLiquidSpecificEnthalpy: 787.5557825431341,
        outletLiquidSpecificEntropy: 2.1926588668498703,
        outletLiquidTemperature: 458.65273574845287,
    };
    validateFlashTank(t, actual.blowdownFlashTank, expected);
}

function validateCondensateFlashTank(t, actual) {
    var expected = {
        inletWaterEnergyFlow: 3121906.698076338,
        inletWaterMassFlow: 11340,
        inletWaterPressure: 1.136,
        inletWaterQuality: 0,
        inletWaterVolume: 0.001019660335144356,
        inletWaterSpecificEnthalpy: 275.300414292446,
        inletWaterSpecificEntropy: 0.8997137118482235,
        inletWaterTemperature: 338.6999999999999,
        outletGasEnergyFlow: 0,
        outletGasMassFlow: 0,
        outletGasPressure: 0.204747,
        outletGasQuality: 1,
        outletGasVolume: 0.8664961907839751,
        outletGasSpecificEnthalpy: 2707.3177066592766,
        outletGasSpecificEntropy: 7.11902821487601,
        outletGasTemperature: 394.1047658380901,
        outletLiquidEnergyFlow: 3121906.698076338,
        outletLiquidMassFlow: 11340,
        outletLiquidPressure: 1.136,
        outletLiquidQuality: 0,
        outletLiquidVolume: 0.001019660335144356,
        outletLiquidSpecificEnthalpy: 275.300414292446,
        outletLiquidSpecificEntropy: 0.8997137118482235,
        outletLiquidTemperature: 338.6999999999999,
    };
    validateFlashTank(t, actual.condensateFlashTank, expected);
}

function validateHighPressureCondensateFlashTank(t, actual) {
    var expected = {
        inletWaterEnergyFlow: undefined,
        inletWaterMassFlow: undefined,
        inletWaterPressure: undefined,
        inletWaterQuality: undefined,
        inletWaterVolume: undefined,
        inletWaterSpecificEnthalpy: undefined,
        inletWaterSpecificEntropy: undefined,
        inletWaterTemperature: undefined,
        outletGasEnergyFlow: undefined,
        outletGasMassFlow: undefined,
        outletGasPressure: undefined,
        outletGasQuality: undefined,
        outletGasVolume: undefined,
        outletGasSpecificEnthalpy: undefined,
        outletGasSpecificEntropy: undefined,
        outletGasTemperature: undefined,
        outletLiquidEnergyFlow: undefined,
        outletLiquidMassFlow: undefined,
        outletLiquidPressure: undefined,
        outletLiquidQuality: undefined,
        outletLiquidVolume: undefined,
        outletLiquidSpecificEnthalpy: undefined,
        outletLiquidSpecificEntropy: undefined,
        outletLiquidTemperature: undefined,
    };
    validateFlashTank(t, actual.highPressureCondensateFlashTank, expected);
}

function validateMediumPressureCondensateFlashTank(t, actual) {
    var expected = {
        inletWaterEnergyFlow: undefined,
        inletWaterMassFlow: undefined,
        inletWaterPressure: undefined,
        inletWaterQuality: undefined,
        inletWaterVolume: undefined,
        inletWaterSpecificEnthalpy: undefined,
        inletWaterSpecificEntropy: undefined,
        inletWaterTemperature: undefined,
        outletGasEnergyFlow: undefined,
        outletGasMassFlow: undefined,
        outletGasPressure: undefined,
        outletGasQuality: undefined,
        outletGasVolume: undefined,
        outletGasSpecificEnthalpy: undefined,
        outletGasSpecificEntropy: undefined,
        outletGasTemperature: undefined,
        outletLiquidEnergyFlow: undefined,
        outletLiquidMassFlow: undefined,
        outletLiquidPressure: undefined,
        outletLiquidQuality: undefined,
        outletLiquidVolume: undefined,
        outletLiquidSpecificEnthalpy: undefined,
        outletLiquidSpecificEntropy: undefined,
        outletLiquidTemperature: undefined,
    };
    validateFlashTank(t, actual.mediumPressureCondensateFlashTank, expected);
}

function validateHeaderOutputObj(t, actual) {
    var actualHeader = actual.headerOutput;

    t.equal(actualHeader.energyFlow, 0);
    t.equal(actualHeader.massFlow, 0);
    t.equal(actualHeader.pressure, 0);
    t.equal(actualHeader.quality, 0);
    t.equal(actualHeader.specificEnthalpy, 0);
    t.equal(actualHeader.specificEntropy, 0);
    t.equal(actualHeader.temperature, 0);
    //t.equal(actualHeader.heatLoss, 0);
}

function validateHeatLossOutput(t, actual) {
    var actualHeatLoss = actual.heatLossOutput;

    t.equal(actualHeatLoss.heatLoss, 0);
    t.equal(actualHeatLoss.inletEnergyFlow, 0);
    t.equal(actualHeatLoss.inletMassFlow, 0);
    t.equal(actualHeatLoss.inletPressure, 0);
    t.equal(actualHeatLoss.inletQuality, 0);
    t.equal(actualHeatLoss.inletSpecificEnthalpy, 0);
    t.equal(actualHeatLoss.inletSpecificEntropy, 0);
    t.equal(actualHeatLoss.inletTemperature, 0);
    t.equal(actualHeatLoss.outletEnergyFlow, 0);
    t.equal(actualHeatLoss.outletMassFlow, 0);
    t.equal(actualHeatLoss.outletPressure, 0);
    t.equal(actualHeatLoss.outletQuality, 0);
    t.equal(actualHeatLoss.outletSpecificEnthalpy, 0);
    t.equal(actualHeatLoss.outletSpecificEntropy, 0);
    t.equal(actualHeatLoss.outletTemperature, 0);
}

function validatePrvOutput(t, actual) {
    t.equal(actual.feedwaterEnergyFlow, undefined);
    t.equal(actual.feedwaterMassFlow, undefined);
    t.equal(actual.feedwaterPressure, undefined);
    t.equal(actual.feedwaterQuality, undefined);
    t.equal(actual.feedwaterVolume, undefined);
    t.equal(actual.feedwaterSpecificEnthalpy, undefined);
    t.equal(actual.feedwaterSpecificEntropy, undefined);
    t.equal(actual.feedwaterTemperature, undefined);
    t.equal(actual.inletEnergyFlow, undefined);
    t.equal(actual.inletMassFlow, undefined);
    t.equal(actual.inletPressure, undefined);
    t.equal(actual.inletQuality, undefined);
    t.equal(actual.inletVolume, undefined);
    t.equal(actual.inletSpecificEnthalpy, undefined);
    t.equal(actual.inletSpecificEntropy, undefined);
    t.equal(actual.inletTemperature, undefined);
    t.equal(actual.outletEnergyFlow, undefined);
    t.equal(actual.outletMassFlow, undefined);
    t.equal(actual.outletPressure, undefined);
    t.equal(actual.outletQuality, undefined);
    t.equal(actual.outletVolume, undefined);
    t.equal(actual.outletSpecificEnthalpy, undefined);
    t.equal(actual.outletSpecificEntropy, undefined);
    t.equal(actual.outletTemperature, undefined);
}

function validateHighPressureToMediumPressurePrv(t, actual) {
    validatePrvOutput(t, actual.highPressureToMediumPressurePrv);
}

function validateMediumPressureToLowPressurePrv(t, actual) {
    validatePrvOutput(t, actual.mediumPressureToLowPressurePrv);
}

function validateTurbineOutput(t, actual, expected) {
    t.equal(actual.energyOut, expected.energyOut);
    t.equal(actual.generatorEfficiency, expected.generatorEfficiency);
    t.equal(actual.inletEnergyFlow, expected.inletEnergyFlow);
    t.equal(actual.inletPressure, expected.inletPressure);
    t.equal(actual.inletQuality, expected.inletQuality);
    t.equal(actual.inletVolume, expected.inletVolume);
    t.equal(actual.inletSpecificEnthalpy, expected.inletSpecificEnthalpy);
    t.equal(actual.inletSpecificEntropy, expected.inletSpecificEntropy);
    t.equal(actual.inletTemperature, expected.inletTemperature);
    t.equal(actual.isentropicEfficiency, expected.isentropicEfficiency);
    t.equal(actual.massFlow, expected.massFlow);
    t.equal(actual.outletEnergyFlow, expected.outletEnergyFlow);
    t.equal(actual.outletPressure, expected.outletPressure);
    t.equal(actual.outletQuality, expected.outletQuality);
    t.equal(actual.outletVolume, expected.outletVolume);
    t.equal(actual.outletSpecificEnthalpy, expected.outletSpecificEnthalpy);
    t.equal(actual.outletSpecificEntropy, expected.outletSpecificEntropy);
    t.equal(actual.outletTemperature, expected.outletTemperature);
    t.equal(actual.powerOut, expected.powerOut);
    t.equal(actual.outletIdealPressure, expected.outletIdealPressure);
    t.equal(actual.outletIdealTemperature, expected.outletIdealTemperature);
    t.equal(actual.outletIdealSpecificEnthalpy, expected.outletIdealSpecificEnthalpy);
    t.equal(actual.outletIdealSpecificEntropy, expected.outletIdealSpecificEntropy);
    t.equal(actual.outletIdealQuality, expected.outletIdealQuality);
    t.equal(actual.outletIdealVolume, expected.outletIdealVolume);
}

//FIXME actuals undefined
function validateCondensingTurbine(t, actual) {
    var expected = {
        energyOut: 100,
        generatorEfficiency: 1,
        inletEnergyFlow: 1026109.2942220301,
        inletPressure: 1.136,
        inletQuality: 1,
        inletVolume: 0.199008042924959,
        inletSpecificEnthalpy: 2914.920954683495,
        inletSpecificEntropy: 6.815506757222659,
        inletTemperature: 512.9220160918137,
        isentropicEfficiency: 1,
        massFlow: 352.01959510200373,
        outletEnergyFlow: 1026009.2942220301,
        outletPressure: 1,
        outletQuality: 1,
        outletVolume: 0.22607153693359688,
        outletSpecificEnthalpy: 2914.636879588268,
        outletSpecificEntropy: 6.871299121498748,
        outletTemperature: 510.32081495211025,
        powerOut: 1,
        outletIdealPressure: undefined,
        outletIdealTemperature: undefined,
        outletIdealSpecificEnthalpy: undefined,
        outletIdealSpecificEntropy: undefined,
        outletIdealQuality: undefined,
        outletIdealVolume: undefined,
    };
    validateTurbineOutput(t, actual.condensingTurbine, expected);
}

function validateHighPressureToMediumPressureTurbine(t, actual) {
    var expected = {
        energyOut: undefined,
        generatorEfficiency: undefined,
        inletEnergyFlow: undefined,
        inletPressure: undefined,
        inletQuality: undefined,
        inletVolume: undefined,
        inletSpecificEnthalpy: undefined,
        inletSpecificEntropy: undefined,
        inletTemperature: undefined,
        isentropicEfficiency: undefined,
        massFlow: undefined,
        outletEnergyFlow: undefined,
        outletPressure: undefined,
        outletQuality: undefined,
        outletVolume: undefined,
        outletSpecificEnthalpy: undefined,
        outletSpecificEntropy: undefined,
        outletTemperature: undefined,
        powerOut: undefined,
        outletIdealPressure: undefined,
        outletIdealTemperature: undefined,
        outletIdealSpecificEnthalpy: undefined,
        outletIdealSpecificEntropy: undefined,
        outletIdealQuality: undefined,
        outletIdealVolume: undefined,
    };
    validateTurbineOutput(t, actual.highPressureToMediumPressureTurbine, expected);
}

function validateHighPressureToLowPressureTurbine(t, actual) {
    var expected = {
        energyOut: undefined,
        generatorEfficiency: undefined,
        inletEnergyFlow: undefined,
        inletPressure: undefined,
        inletQuality: undefined,
        inletVolume: undefined,
        inletSpecificEnthalpy: undefined,
        inletSpecificEntropy: undefined,
        inletTemperature: undefined,
        isentropicEfficiency: undefined,
        massFlow: undefined,
        outletEnergyFlow: undefined,
        outletPressure: undefined,
        outletQuality: undefined,
        outletVolume: undefined,
        outletSpecificEnthalpy: undefined,
        outletSpecificEntropy: undefined,
        outletTemperature: undefined,
        powerOut: undefined,
        outletIdealPressure: undefined,
        outletIdealTemperature: undefined,
        outletIdealSpecificEnthalpy: undefined,
        outletIdealSpecificEntropy: undefined,
        outletIdealQuality: undefined,
        outletIdealVolume: undefined,
    };
    validateTurbineOutput(t, actual.highPressureToLowPressureTurbine, expected);
}

function validateMediumPressureToLowPressureTurbine(t, actual) {
    var expected = {
        energyOut: undefined,
        generatorEfficiency: undefined,
        inletEnergyFlow: undefined,
        inletPressure: undefined,
        inletQuality: undefined,
        inletVolume: undefined,
        inletSpecificEnthalpy: undefined,
        inletSpecificEntropy: undefined,
        inletTemperature: undefined,
        isentropicEfficiency: undefined,
        massFlow: undefined,
        outletEnergyFlow: undefined,
        outletPressure: undefined,
        outletQuality: undefined,
        outletVolume: undefined,
        outletSpecificEnthalpy: undefined,
        outletSpecificEntropy: undefined,
        outletTemperature: undefined,
        powerOut: undefined,
        outletIdealPressure: undefined,
        outletIdealTemperature: undefined,
        outletIdealSpecificEnthalpy: undefined,
        outletIdealSpecificEntropy: undefined,
        outletIdealQuality: undefined,
        outletIdealVolume: undefined,
    };
    validateTurbineOutput(t, actual.mediumPressureToLowPressureTurbine, expected);
}
