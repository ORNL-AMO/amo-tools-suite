const test = require('tap').test
    , testRoot = require('path').resolve(__dirname, '../../')
    , bindings = require('bindings')({ module_root: testRoot, bindings: 'processHeat'});

function rnd(value) {
    return Number(Math.round(value + 'e' + 2) + 'e-' + 2);
}

test('AirHeatingUsingExhaust Gas', function (t) {
    t.plan(6);
    t.type(bindings.airHeatingUsingExhaust, 'function');

    var compare = function(results, expected) {
        t.equal(rnd(results.hxColdAir), rnd(expected[0]));
        t.equal(rnd(results.hxOutletExhaust), rnd(expected[1]));
        t.equal(rnd(results.energySavings), rnd(expected[2]));
        t.equal(rnd(results.heatCapacityFlue), rnd(expected[3]));
        t.equal(rnd(results.heatCapacityAir), rnd(expected[4]));
    };

    var input = {
        flueTemperature: 400,
        excessAir: 0.358,
        fireRate: 8,
        airflow: 4000,
        inletTemperature: 45,
        heaterEfficiency: 0.85,
        hxEfficiency: 0.60,
        operatingHours: 4000,

        gasFuelType: true,
        substance: 'Gas',
        CH4: 94.0,
        C2H6: 2.07,
        N2: 1.41,
        H2: 0.01,
        C3H8: 0.42,
        C4H10_CnH2n: 0.28,
        H2O: 0.0,
        CO: 1.0,
        CO2: 0.71,
        SO2: 0,
        O2: 0
    };

    compare(bindings.airHeatingUsingExhaust(input), [197829.27, 187, 930.96, 928.78, 4464]);
});

test('AirHeatingUsingExhaust Coal', function (t) {
    t.plan(6);
    t.type(bindings.airHeatingUsingExhaust, 'function');

    var compare = function(results, expected) {
        t.equal(rnd(results.hxColdAir), rnd(expected[0]));
        t.equal(rnd(results.hxOutletExhaust), rnd(expected[1]));
        t.equal(rnd(results.energySavings), rnd(expected[2]));
        t.equal(rnd(results.heatCapacityFlue), rnd(expected[3]));
        t.equal(rnd(results.heatCapacityAir), rnd(expected[4]));
    };

    var input = {
        flueTemperature: 400,
        excessAir: 0.358,
        fireRate: 8,
        airflow: 4000,
        inletTemperature: 45,
        heaterEfficiency: 0.85,
        hxEfficiency: 0.60,
        operatingHours: 4000,

        gasFuelType: false,
        substance: 'Coal',
        carbon: 75.0,
        hydrogen: 5.0,
        sulphur: 1.00,
        inertAsh: 9.0,
        o2: 7.0,
        moisture: 0.0,
        nitrogen: 1.5
    };

    compare(bindings.airHeatingUsingExhaust(input), [15621.25, 187, 73.512, 73.339, 4464]);
});