const test = require('tap').test
    , testRoot = require('path').resolve(__dirname, '../../')
    , bindings = require('bindings')({module_root: testRoot, bindings: 'ssmt'});

function rnd(value) {
    return Number(Math.round(value + 'e' + 6) + 'e-' + 6);
}

test('saturatedPropertiesGivenTemperature', function (t) {
    t.plan(12);
    t.type(bindings.saturatedPropertiesGivenTemperature, 'function');

    var inp = {};

    inp.saturatedTemperature = 300;

    var res = bindings.saturatedPropertiesGivenTemperature(inp);

    t.equal(rnd(res.saturatedPressure), rnd(0.0035365894130130106), 'res.saturatedPressure is ' + res.saturatedPressure);

    t.equal(res.saturatedTemperature, 300, 'res.saturatedTemperature is ' + res.saturatedTemperature);

    t.equal(rnd(res.liquidEnthalpy), rnd(112.57499081240724), 'res.liquidEnthalpy is ' + res.liquidEnthalpy);

    t.equal(rnd(res.gasEnthalpy), rnd(2549.8930083073265), 'res.gasEnthalpy is ' + res.gasEnthalpy);

    t.equal(rnd(res.evaporationEnthalpy), rnd(2437.3180174949193), 'res.evaporationEnthalpy is ' + res.evaporationEnthalpy);

    t.equal(rnd(res.liquidEntropy), rnd(0.3931236014741), 'res.liquidEntropy is ' + res.liquidEntropy);

    t.equal(rnd(res.gasEntropy), rnd(8.517536685028846), 'res.gasEntropy is ' + res.gasEntropy);

    t.equal(rnd(res.evaporationEntropy), rnd(8.124413083554746), 'res.evaporationEntropy is ' + res.evaporationEntropy);

    t.equal(rnd(res.liquidVolume), rnd(0.0010034979299339492), 'res.liquidVolume is ' + res.liquidVolume);

    t.equal(rnd(res.gasVolume), rnd(39.08205832377987), 'res.gasVolume is ' + res.gasVolume);

    t.equal(rnd(res.evaporationVolume), rnd(39.08105482584993), 'res.evaporationVolume is ' + res.evaporationVolume);

});

test('saturatedPropertiesGivenPressure', function (t) {
    t.plan(12);
    t.type(bindings.saturatedPropertiesGivenPressure, 'function');

    var inp = {};

    inp.saturatedPressure = 20;

    var res = bindings.saturatedPropertiesGivenPressure(inp);

    t.equal(res.saturatedPressure, 20, 'res.saturatedPressure is ' + res.saturatedPressure);

    t.equal(rnd(res.saturatedTemperature), rnd(638.8959115457051), 'res.saturatedTemperature is ' + res.saturatedTemperature);

    t.equal(rnd(res.liquidEnthalpy), rnd(1827.1006242178723), 'res.liquidEnthalpy is ' + res.liquidEnthalpy);

    t.equal(rnd(res.gasEnthalpy), rnd(2421.680542687896), 'res.gasEnthalpy is ' + res.gasEnthalpy);

    t.equal(rnd(res.evaporationEnthalpy), rnd(594.5799184700236), 'res.evaporationEnthalpy is ' + res.evaporationEnthalpy);

    t.equal(rnd(res.liquidEntropy), rnd(4.015381593120578), 'res.liquidEntropy is ' + res.liquidEntropy);

    t.equal(rnd(res.gasEntropy), rnd(4.946001326758373), 'res.gasEntropy is ' + res.gasEntropy);

    t.equal(rnd(res.evaporationEntropy), rnd(0.930619733637795), 'res.evaporationEntropy is ' + res.evaporationEntropy);

    t.equal(rnd(res.liquidVolume), rnd(0.0020386472456956663), 'res.liquidVolume is ' + res.liquidVolume);

    t.equal(rnd(res.gasVolume), rnd(0.005936854102266306), 'res.gasVolume is ' + res.gasVolume);

    t.equal(rnd(res.evaporationVolume), rnd(0.0038982068565706395), 'res.evaporationVolume is ' + res.evaporationVolume);
});

test('steamProperties', function (t) {
    t.plan(7);
    t.type(bindings.steamProperties, 'function');

    var inp = {};

    inp.pressure = 10;
    inp.thermodynamicQuantity = 1; //1 is ENTHALPY
    inp.quantityValue = 2000;

    var res = bindings.steamProperties(inp);

    t.equal(res.pressure, 10, 'res.pressure is ' + res.pressure);

    t.equal(rnd(res.temperature), rnd(584.1494879985282), 'res.temperature is ' + res.temperature);

    t.equal(res.specificEnthalpy, 2000, 'res.specificEnthalpy is ' + res.specificEnthalpy);

    t.equal(rnd(res.specificEntropy), rnd(4.37395830066478), 'res.specificEntropy is ' + res.specificEntropy);

    t.equal(rnd(res.quality), rnd(0.44940059413065064), 'res.quality is ' + res.quality);

    t.equal(rnd(res.specificVolume), rnd(0.008904111059648875), 'res.specificVolume is ' + res.specificVolume);

});

test('boiler', function (t) {
    t.plan(8);
    t.type(bindings.boiler, 'function');

    var inp = {
        deaeratorPressure: 10, combustionEfficiency: 85, blowdownRate: 2,
        steamPressure: 20, thermodynamicQuantity: 1, //1 is ENTHALPY
        quantityValue: 2000, steamMassFlow: 45
    };

    var res = bindings.boiler(inp);

    t.equal(rnd(res.steamEnergyFlow), rnd(110753.36475088508), 'res.steamEnergyFlow is ' + res.steamEnergyFlow);
    t.equal(rnd(res.blowdownMassFlow), rnd(0.9183673469387756), 'res.blowdownMassFlow is ' + res.blowdownMassFlow);
    t.equal(rnd(res.blowdownEnergyFlow), rnd(1677.949552853158), 'res.blowdownEnergyFlow is ' + res.blowdownEnergyFlow);
    t.equal(rnd(res.feedwaterMassFlow), rnd(45.91836734693878), 'res.feedwaterMassFlow is ' + res.feedwaterMassFlow);
    t.equal(rnd(res.feedwaterEnergyFlow), rnd(64646.977066909145), 'res.feedwaterEnergyFlow is ' + res.feedwaterEnergyFlow);
    t.equal(rnd(res.boilerEnergy), rnd(47784.337236829095), 'res.boilerEnergy is ' + res.boilerEnergy);
    t.equal(rnd(res.fuelEnergy), rnd(56216.867337446), 'res.fuelEnergy is ' + res.fuelEnergy);
});

test('heatLoss', function (t) {
    t.plan(5);
    t.type(bindings.heatLoss, 'function');

    var inp = {
        inletPressure: 2.418,
        thermodynamicQuantity: 0, //0 is TEMPERATURE
        quantityValue: 521,
        inletMassFlow: 5434,
        percentHeatLoss: 2.44
    };

    var res = bindings.heatLoss(inp);

    t.equal(rnd(res.inletEnergyFlow), rnd(15642967.348653074), 'res.inletEnergyFlow is ' + res.inletEnergyFlow);
    t.equal(res.outletMassFlow, 5434, 'res.outletMassFlow is ' + res.outletMassFlow);
    t.equal(rnd(res.outletEnergyFlow), rnd(15261278.94534594), 'res.outletEnergyFlow is ' + res.outletEnergyFlow);
    t.equal(rnd(res.heatLoss), rnd(381688.4033071343), 'res.heatLoss is ' + res.heatLoss);
});

test('flashTank', function (t) {
    t.plan(6);
    t.type(bindings.flashTank, 'function');

    var inp = {
        inletWaterPressure: 4.54484,
        thermodynamicQuantity: 1, //1 is ENTHALPY
        quantityValue: 2000,
        inletWaterMassFlow: 36133,
        tankPressure: 3.3884
    };

    var res = bindings.flashTank(inp);

    t.equal(res.inletWaterEnergyFlow, 72266000, 'res.inletWaterEnergyFlow is ' + res.inletWaterEnergyFlow);
    t.equal(rnd(res.outletGasMassFlow), rnd(19667.16383431122), 'res.outletGasMassFlow is ' + res.outletGasMassFlow);
    t.equal(rnd(res.outletGasEnergyFlow), rnd(55126797.107875764), 'res.outletGasEnergyFlow is ' + res.outletGasEnergyFlow);
    t.equal(rnd(res.outletLiquidMassFlow), rnd(16465.83616568878), 'res.outletLiquidMassFlow is ' + res.outletLiquidMassFlow);
    t.equal(rnd(res.outletLiquidEnergyFlow), rnd(17139202.892124232), 'res.outletLiquidEnergyFlow is ' + res.outletLiquidEnergyFlow);
});

test('prvWithoutDesuperheating', function (t) {
    t.plan(6);
    t.type(bindings.prvWithoutDesuperheating, 'function');

    var inp = {
        inletPressure: 4.8794,
        thermodynamicQuantity: 0, //0 is TEMPERATURE
        quantityValue: 691.5,
        inletMassFlow: 37970,
        outletPressure: 4.0823
    };

    var res = bindings.prvWithoutDesuperheating(inp);

    t.equal(rnd(res.inletEnergyFlow), rnd(123147934.93161911), 'res.inletEnergyFlow is ' + res.inletEnergyFlow);
    t.equal(res.outletMassFlow, 37970, 'res.outletMassFlow is ' + res.outletMassFlow);
    t.equal(rnd(res.outletEnergyFlow), rnd(123147934.93161911), 'res.outletEnergyFlow is ' + res.outletEnergyFlow);
    t.equal(rnd(res.inletSpecificEnthalpy), rnd(3243.29562632655), 'res.inletSpecificEnthalpy is ' + res.inletSpecificEnthalpy);
    t.equal(rnd(res.outletSpecificEnthalpy), rnd(3243.29562632655), 'res.outletSpecificEnthalpy is ' + res.outletSpecificEnthalpy);
});

test('prvWithDesuperheating', function (t) {
    t.plan(6);
    t.type(bindings.prvWithDesuperheating, 'function');

    var inp = {
        inletPressure: 2.8937,
        thermodynamicQuantity: 0, //0 is TEMPERATURE
        quantityValue: 936.3,
        inletMassFlow: 17599,
        outletPressure: 0.8188,
        feedwaterPressure: 0.2937,
        feedwaterThermodynamicQuantity: 2, //2 is ENTROPY
        feedwaterQuantityValue: 5,
        desuperheatingTemp: 708.3
    };

    var res = bindings.prvWithDesuperheating(inp);

    t.equal(rnd(res.inletEnergyFlow), rnd(67367311.1113208), 'res.inletEnergyFlow is ' + res.inletEnergyFlow);
    t.equal(rnd(res.outletMassFlow), rnd(23583.469367594505), 'res.outletMassFlow is ' + res.outletMassFlow);
    t.equal(rnd(res.outletEnergyFlow), rnd(78812942.89252071), 'res.outletEnergyFlow is ' + res.outletEnergyFlow);
    t.equal(rnd(res.feedwaterMassFlow), rnd(5984.4693675945055), 'res.feedwaterMassFlow is ' + res.feedwaterMassFlow);
    t.equal(rnd(res.feedwaterEnergyFlow), rnd(11445631.781199913), 'res.feedwaterEnergyFlow is ' + res.feedwaterEnergyFlow);
});

test('deaerator', function (t) {
    t.plan(8);
    t.type(bindings.deaerator, 'function');

    var inp = {
        deaeratorPressure: 0.1998,
        ventRate: 0.4,
        feedwaterMassFlow: 41685,
        waterPressure: 0.1235,
        waterThermodynamicQuantity: 1, //1 is ENTHALPY
        waterQuantityValue: 100,
        steamPressure: 0.4777,
        steamThermodynamicQuantity: 2, //2 is ENTROPY
        steamQuantityValue: 6
    };

    var res = bindings.deaerator(inp);

    t.equal(rnd(res.feedwaterEnergyFlow), rnd(21032141.29813274), 'res.feedwaterEnergyFlow is ' + res.feedwaterEnergyFlow);
    t.equal(res.ventedSteamMassFlow, 166.74, 'res.ventedSteamMassFlow is ' + res.ventedSteamMassFlow);
    t.equal(rnd(res.ventedSteamEnergyFlow), rnd(451231.0290232193), 'res.ventedSteamEnergyFlow is ' + res.ventedSteamEnergyFlow);
    t.equal(rnd(res.inletWaterMassFlow), rnd(34305.35779780327), 'res.inletWaterMassFlow is ' + res.inletWaterMassFlow);
    t.equal(rnd(res.inletWaterEnergyFlow), rnd(3430535.779780379), 'res.inletWaterEnergyFlow is ' + res.inletWaterEnergyFlow);
    t.equal(rnd(res.inletSteamMassFlow), rnd(7546.382202196729), 'res.inletSteamMassFlow is ' + res.inletSteamMassFlow);
    t.equal(rnd(res.inletSteamEnergyFlow), rnd(18052836.54737558), 'res.inletSteamEnergyFlow is ' + res.inletSteamEnergyFlow);
});

test('header', function (t) {
    t.plan(22);
    t.type(bindings.header, 'function');

    var inp = {
        headerPressure: 0.173,
        inlets: [
            {
                pressure: 1.9332,
                thermodynamicQuantity: 0,
                quantityValue: 579.8,
                massFlow: 0.686
            },
            {
                pressure: 2.8682,
                thermodynamicQuantity: 0,
                quantityValue: 308.5,
                massFlow: 0.5019
            },
            {
                pressure: 1.0348,
                thermodynamicQuantity: 0,
                quantityValue: 458,
                massFlow: 0.5633
            },
            {
                pressure: 1.8438,
                thermodynamicQuantity: 0,
                quantityValue: 475.8,
                massFlow: 0.3082
            }
        ]
    };

    var res = bindings.header(inp);
    var header = res.header;
    var inlet1 = res.inlet1;
    var inlet4 = res.inlet4;

    t.equal(header.pressure, 0.173, header.pressure + " != 0.173");
    t.equal(rnd(header.temperature), rnd(388.8366691795), header.temperature + " != 388.8");
    t.equal(rnd(header.massFlow), rnd(2.059399999));
    t.equal(rnd(header.energyFlow), rnd(3998.8128348989585));
    t.equal(rnd(header.quality), rnd(0.6577144796131725));
    t.equal(rnd(header.specificEnthalpy), rnd(1941.7368334946873));
    t.equal(rnd(header.specificEntropy), rnd(5.226216653050388));

    t.equal(rnd(inlet1.energyFlow), rnd(2086.4298803));
    t.equal(rnd(inlet1.massFlow), rnd(0.686));
    t.equal(rnd(inlet1.pressure), rnd(1.9332));
    t.equal(rnd(inlet1.quality), 1);
    t.equal(rnd(inlet1.specificEnthalpy), rnd(3041.442974242245));
    t.equal(rnd(inlet1.specificEntropy), rnd(6.81324727632225));
    t.equal(rnd(inlet1.temperature), rnd(579.8));

    t.equal(rnd(inlet4.energyFlow), rnd(266.41815154210417));
    t.equal(rnd(inlet4.massFlow), rnd(0.3082));
    t.equal(rnd(inlet4.pressure), rnd(1.8438));
    t.equal(rnd(inlet4.quality), 0);
    t.equal(rnd(inlet4.specificEnthalpy), rnd(864.4326785921616));
    t.equal(rnd(inlet4.specificEntropy), rnd(2.3554693941761826));
    t.equal(rnd(inlet4.temperature), rnd(475.8));
});

test('turbine', function (t) {
    t.plan(31);
    t.type(bindings.turbine, 'function');

    var inp = {
        solveFor: 0, // outlet properties - unknown to solve for
        inletPressure: 4.2112,
        inletQuantity: 0, // SteamProperties::ThermodynamicQuantity::temperature
        inletQuantityValue: 888,
        turbineProperty: 0, // massFlow
        isentropicEfficiency: 40.1,
        generatorEfficiency: 94.2,
        massFlowOrPowerOut: 15844, // massFlow in this case
        outletSteamPressure: 3.4781
    };

    var res = bindings.turbine(inp);
    t.equal(rnd(res.inletPressure), rnd(4.2112));
    t.equal(rnd(res.inletTemperature), rnd(888));
    t.equal(rnd(res.inletSpecificEnthalpy), rnd(3707.397118));
    t.equal(rnd(res.inletSpecificEntropy), rnd(7.384098));
    t.equal(rnd(res.inletQuality), rnd(1));
    t.equal(rnd(res.inletEnergyFlow), rnd(58739999.930197));

    t.equal(rnd(res.outletPressure), rnd(3.4781));
    t.equal(rnd(res.outletTemperature), rnd(872.338861));
    t.equal(rnd(res.outletSpecificEnthalpy), rnd(3677.155392));
    t.equal(rnd(res.outletSpecificEntropy), rnd(7.436479));
    t.equal(rnd(res.outletQuality), rnd(1));
    t.equal(rnd(res.outletEnergyFlow), rnd(58260850.026976));

    t.equal(rnd(res.massFlow), rnd(15844));
    t.equal(rnd(res.isentropicEfficiency), rnd(40.1));
    t.equal(rnd(res.energyOut), rnd(479149.903221));
    t.equal(rnd(res.powerOut), rnd(451359.208834));
    t.equal(rnd(res.generatorEfficiency), rnd(94.2));

    inp = {
        solveFor: 1, // isentropicEfficiency - unknown to solve for
        inletPressure: 5.5627,
        inletQuantity: 0, // SteamProperties::ThermodynamicQuantity::temperature
        inletQuantityValue: 823.8,
        turbineProperty: 1, // powerOut
        generatorEfficiency: 82,
        massFlowOrPowerOut: 1000, // powerOut in this case
        outletSteamPressure: 4.4552,
        outletQuantity: 0, // SteamProperties::ThermodynamicQuantity::temperature
        outletQuantityValue: 798.1
    };

    res = bindings.turbine(inp);
    t.equal(rnd(res.inletSpecificEnthalpy), rnd(3546.905437));
    t.equal(rnd(res.inletSpecificEntropy), rnd(7.071209));
    t.equal(rnd(res.inletQuality), rnd(1));
    t.equal(rnd(res.inletEnergyFlow), rnd(88851.276592));

    t.equal(rnd(res.outletSpecificEnthalpy), rnd(3498.223025));
    t.equal(rnd(res.outletSpecificEntropy), rnd(7.110366));
    t.equal(rnd(res.outletQuality), rnd(1));
    t.equal(rnd(res.outletEnergyFlow), rnd(87631.764397));

    t.equal(rnd(res.massFlow), rnd(25.050365));
    t.equal(rnd(res.isentropicEfficiency), rnd(61.105109));
    t.equal(rnd(res.energyOut), rnd(1219.512195));
    t.equal(rnd(res.powerOut), rnd(1000));
    t.equal(rnd(res.generatorEfficiency), rnd(82));

});

test('heatExchanger', function(t) {
    t.plan(32);
    input = {
        approachTemp: 10,
        coldInletDensity: 999.7015401695024,
        coldInletEnergyFlow: 262530220.6437767,
        coldInletMassFlow: 6233100.275643423,
        coldInletPressure: 0.101325,
        coldInletQuality: 0,
        coldInletSpecificEnthalpy: 42.11872247100606,
        coldInletSpecificEntropy: 0.15107627374941596,
        coldInletSpecificVolume: 0.0010002985489353622,
        coldInletTemperature: 283.15,
        hotInletDensity: 866.6499783621422,
        hotInletEnergyFlow: 81817968.94625004,
        hotInletMassFlow: 96858.44750370324,
        hotInletPressure: 1.5,
        hotInletQuality: 0,
        hotInletSpecificEnthalpy: 844.716914785588,
        hotInletSpecificEntropy: 2.314681630438997,
        hotInletSpecificVolume: 0.0011538683724309003,
        hotInletTemperature: 471.4452428824141,
    }

    res = bindings.heatExchanger(input);

    t.equal(rnd(res.hotOutletPressure), rnd(1.5));
    t.equal(rnd(res.hotOutletTemperature), rnd(293.15));
    t.equal(rnd(res.hotOutletSpecificEnthalpy), rnd(85.328522053958));
    t.equal(rnd(res.hotOutletSpecificEntropy), rnd(0.2961917501711791));
    t.equal(rnd(res.hotOutletQuality), rnd(0));
    t.equal(rnd(res.hotOutletSpecificVolume), rnd(0.0010011563539881817));
    t.equal(rnd(res.hotOutletMassFlow), rnd(96858.44750370324));
    t.equal(rnd(res.hotOutletEnergyFlow), rnd(8264788.173931874));

    t.equal(rnd(res.coldOutletPressure), rnd(0.101325));
    t.equal(rnd(res.coldOutletTemperature), rnd(285.9640221738703));
    t.equal(rnd(res.coldOutletSpecificEnthalpy), rnd(53.91913920098477));
    t.equal(rnd(res.coldOutletSpecificEntropy), rnd(0.19254607216780856));
    t.equal(rnd(res.coldOutletQuality), rnd(0));
    t.equal(rnd(res.coldOutletSpecificVolume), rnd(0.0010005983512689238));
    t.equal(rnd(res.coldOutletMassFlow), rnd(6233100.275643423));
    t.equal(rnd(res.coldOutletEnergyFlow), rnd(336083401.4161143));

    // Test 2
    input = {
        approachTemp: 10,
        coldInletDensity: 999.7015401695024,
        coldInletEnergyFlow: 662268.2787552822,
        coldInletMassFlow: 15723.845356686172,
        coldInletPressure: 0.101325,
        coldInletQuality: 0,
        coldInletSpecificEnthalpy: 42.11872247100606,
        coldInletSpecificEntropy: 0.15107627374941596,
        coldInletSpecificVolume: 0.0010002985489353622,
        coldInletTemperature: 283.15,
        hotInletDensity: 798.3582064389988,
        hotInletEnergyFlow: 749645.9791564117,
        hotInletMassFlow: 689.376530177306,
        hotInletPressure: 4,
        hotInletQuality: 0,
        hotInletSpecificEnthalpy: 1087.4260238647878,
        hotInletSpecificEntropy: 2.7966534294996848,
        hotInletSpecificVolume: 0.0012525705778868427,
        hotInletTemperature: 523.5075191240655
    }

    res = bindings.heatExchanger(input);

    t.equal(rnd(res.hotOutletPressure), rnd(4));
    t.equal(rnd(res.hotOutletTemperature), rnd(293.15));
    t.equal(rnd(res.hotOutletSpecificEnthalpy), rnd(87.6755402690778));
    t.equal(rnd(res.hotOutletSpecificEntropy), rnd(0.2956648955932186));
    t.equal(rnd(res.hotOutletQuality), rnd(0));
    t.equal(rnd(res.hotOutletSpecificVolume), rnd(0.0010000175423266124));
    t.equal(rnd(res.hotOutletMassFlow), rnd(689.376530177306));
    t.equal(rnd(res.hotOutletEnergyFlow), rnd(60441.45973211752));

    t.equal(rnd(res.coldOutletPressure), rnd(0.101325));
    t.equal(rnd(res.coldOutletTemperature), rnd(293.6129961502827));
    t.equal(rnd(res.coldOutletSpecificEnthalpy), rnd(85.95052721024926));
    t.equal(rnd(res.coldOutletSpecificEntropy), rnd(0.30308657179435106));
    t.equal(rnd(res.coldOutletQuality), rnd(0));
    t.equal(rnd(res.coldOutletSpecificVolume), rnd(0.0010018940982539372));
    t.equal(rnd(res.coldOutletMassFlow), rnd(15723.845356686172));
    t.equal(rnd(res.coldOutletEnergyFlow), rnd(1351472.7981796062));
});
