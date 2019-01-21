#ifndef AMO_TOOLS_SUITE_SSMT_H
#define AMO_TOOLS_SUITE_SSMT_H

#include <nan.h>
#include <node.h>
#include <iostream>
#include "ssmt/SaturatedProperties.h"
#include "ssmt/SteamSystemModelerTool.h"
#include "ssmt/SteamProperties.h"
#include "ssmt/HeatLoss.h"
#include "ssmt/Boiler.h"
#include "ssmt/HeatLoss.h"
#include "ssmt/FlashTank.h"
#include "ssmt/PRV.h"
#include "ssmt/Deaerator.h"
#include "ssmt/Header.h"
#include "ssmt/Turbine.h"
#include "ssmt/HeatExchanger.h"
#include <string>
#include <stdexcept>
#include <array>
#include <cmath>

using namespace Nan;
using namespace v8;

Local <Object> inp;
Local <Object> r;

/**
 * Get the value for the specified name from the specified object.
 * @param name Name (variable name) of the value on the specified object.
 * @param sourceObject The specified object to get the value from.
 * @return The object.
 */
Local <Value> getValue(std::string const &name, Local <Object> sourceObject) {
    if (sourceObject.IsEmpty()) {
        auto msg = std::string(
                "getValue: sourceObject is empty/does not exist; trying to get value name=" + name).c_str();
        std::cout << msg << std::endl;

        ThrowTypeError(msg);
    }

    Local <String> localName = Nan::New<String>(name).ToLocalChecked();
    Local <Value> value = sourceObject->Get(localName);
    if (value->IsUndefined()) {
        auto msg = std::string("getValue method in ssmt.h: " + name + " not present in sourceObject").c_str();
        std::cout << "getValue: " << msg << std::endl;

        ThrowTypeError(msg);
    }
    return value;
}

/**
 * Get the object for the specified name from the specified object.
 * @param name Name (variable name) of the object on the specified object.
 * @param sourceObject The specified object to get the object from.
 * @return The object.
 */
Local <Object> getObject(std::string const &name, Local <Object> sourceObject) {
    Local <Value> value = getValue(name, sourceObject);
    return value->ToObject();
}

/**
 * Get the object for the specified name from the root input object (inp).
 * @param name Name (variable name) of the object.
 * @return The object.
 */
Local <Object> getObject(std::string const &name) {
    return getObject(name, inp);
}

/**
 * Get the double value for the specified name from the specified object.
 * @param name Name (variable name) of the value.
 * @param sourceObject The specified object to get the double value from.
 * @return The value as a double.
 */
double getDouble(std::string const &name, Local <Object> sourceObject) {
    Local <Value> value = getValue(name, sourceObject);
    return value->NumberValue();
}

/**
 * Get the double value for the specified name from the root input object (inp).
 * @param name Name (variable name) of the value.
 * @return The value as a double.
 */
double getDouble(std::string const &name) {
    return getDouble(name, inp);
}

/**
 * Get the boolean value for the specified name from the specified object.
 * @param name Name (variable name) of the value.
 * @param sourceObject The specified object to get the boolean value from.
 * @return The value as a bool.
 */
bool getBool(std::string const &name, Local <Object> sourceObject) {
    Local <Value> value = getValue(name, sourceObject);
    return value->BooleanValue();
}

/**
 * Get the boolean value for the specified name from the root input object (inp).
 * @param name Name (variable name) of the value.
 * @return The value as a bool.
 */
bool getBool(std::string const &name) {
    return getBool(name, inp);
}

/**
 * Get the integer value for the specified name from the specified object.
 * @param name Name (variable name) of the value.
 * @param sourceObject The specified object to get the integer value from.
 * @return The value as a integer.
 */
int getInteger(std::string const &name, Local <Object> sourceObject) {
    Local <Value> value = getValue(name, sourceObject);
    return value->IntegerValue();
}

/**
 * Get the integer value for the specified name from the root input object (inp).
 * @param name Name (variable name) of the value.
 * @return The value as an integer.
 */
int getInteger(std::string const &name) {
    return getInteger(name, inp);
}

/**
 * Get the array for the specified name from the specified object.
 * @param name Name (variable name) of the array.
 * @param sourceObject The specified object to get the array from.
 * @return The value as a array.
 */
Local <Array> getArray(std::string const &name, Local <Object> sourceObject) {
    Local <Value> value = getValue(name, sourceObject);
    Local <Array> array = Local<Array>::Cast(value);

    return array;
}

void setRobject(std::string const &name, double number, Local <Object> obj) {
    Local <String> localName = Nan::New<String>(name).ToLocalChecked();
    Local <Number> localNumber = Nan::New<Number>(number);
    Nan::Set(obj, localName, localNumber);
}

void setR(std::string const &name, double number) {
    setRobject(name, number, r);
}

SteamProperties::ThermodynamicQuantity thermodynamicQuantity() {
    unsigned val = static_cast<unsigned>(getDouble("thermodynamicQuantity"));
    return static_cast<SteamProperties::ThermodynamicQuantity>(val);
}

SteamProperties::ThermodynamicQuantity feedwaterThermodynamicQuantity() {
    unsigned val = static_cast<unsigned>(getDouble("feedwaterThermodynamicQuantity"));
    return static_cast<SteamProperties::ThermodynamicQuantity>(val);
}

SteamProperties::ThermodynamicQuantity waterThermodynamicQuantity() {
    unsigned val = static_cast<unsigned>(getDouble("waterThermodynamicQuantity"));
    return static_cast<SteamProperties::ThermodynamicQuantity>(val);
}

SteamProperties::ThermodynamicQuantity steamThermodynamicQuantity() {
    unsigned val = static_cast<unsigned>(getDouble("steamThermodynamicQuantity"));
    return static_cast<SteamProperties::ThermodynamicQuantity>(val);
}

NAN_METHOD(steamPropertiesData) {
    inp = info[0]->ToObject();
    r = Nan::New<Object>();


    if (getBool("wantEntropy")) {
        std::vector<std::array<double, 3>> results;
        auto const pressure = getDouble("pressure");

        for (double entropy = 0; entropy < 10.000001; entropy += 0.1) {
            auto const sp = SteamProperties(pressure, SteamProperties::ThermodynamicQuantity::ENTROPY,
                                            entropy).calculate();
            if (std::isnan(sp.pressure) || std::isnan(sp.temperature) || std::isnan(sp.specificEntropy)
                || sp.pressure < 0 || sp.temperature < 0 || sp.specificEntropy < 0) {
                continue;
            }
            results.push_back({{sp.pressure, sp.temperature, sp.specificEntropy}});
        }

        Handle <Array> data = Array::New(v8::Isolate::GetCurrent(), results.size());
        for (std::size_t i = 0; i < results.size(); i++) {
            auto obj = Nan::New<Object>();
            setRobject("pressure", results.at(i).at(0), obj);
            setRobject("temperature", results.at(i).at(1), obj);
            setRobject("entropy", results.at(i).at(2), obj);
            data->Set(i, obj);
        }
        Nan::Set(r, Nan::New<String>("results").ToLocalChecked(), data);
        info.GetReturnValue().Set(r);
    } else {
        std::vector<std::array<double, 4>> results;
        double const temperature = getDouble("temperature");

        auto const iterate = [&results, temperature](double pressureStart, double pressureEnd, double pressureStep) {
            const double a = 0.000000001;
            for (auto pressure = pressureStart; pressure <= pressureEnd + a; pressure += pressureStep) {
                auto const sp = SteamProperties(pressure, SteamProperties::ThermodynamicQuantity::TEMPERATURE,
                                                temperature).calculate();
                if (std::isnan(sp.pressure) || std::isnan(sp.temperature) || std::isnan(sp.specificEnthalpy)
                    || std::isnan(sp.specificVolume) || sp.pressure < 0 || sp.temperature < 0
                    || sp.specificEnthalpy < 0 || sp.specificVolume < 0) {
                    continue;
                }
                results.push_back({{sp.pressure, sp.temperature, sp.specificEnthalpy, sp.specificVolume}});
            }
        };

        iterate(0.0005, 0.01, 0.0005);
        iterate(0.015, 0.1, 0.005);
        iterate(0.11, 0.5, 0.01);
        iterate(0.55, 1, 0.05);
        iterate(1.1, 5, 0.1);
        iterate(5.5, 10, 0.5);
        iterate(11, 25, 1);
        iterate(30, 100, 5);

        Handle <Array> data = Array::New(v8::Isolate::GetCurrent(), results.size());
        for (std::size_t i = 0; i < results.size(); i++) {
            auto obj = Nan::New<Object>();
            setRobject("pressure", results.at(i).at(0), obj);
            setRobject("temperature", results.at(i).at(1), obj);
            setRobject("enthalpy", results.at(i).at(2), obj);
            setRobject("volume", results.at(i).at(3), obj);
            data->Set(i, obj);
        }
        Nan::Set(r, Nan::New<String>("results").ToLocalChecked(), data);
        info.GetReturnValue().Set(r);
    }
}

NAN_METHOD(saturatedPressure) {
    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    SaturatedPressure sp(getDouble("saturatedTemperature"));
    setR("saturatedPressure", sp.calculate());
    info.GetReturnValue().Set(r);
}

NAN_METHOD(saturatedTemperature) {
    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    SaturatedTemperature st(getDouble("saturatedPressure"));
    setR("saturatedTemperature", st.calculate());
    info.GetReturnValue().Set(r);
}

NAN_METHOD(saturatedPropertiesGivenTemperature) {
    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    double const pressure = SaturatedPressure(getDouble("saturatedTemperature")).calculate();
    auto const results = SaturatedProperties(pressure, getDouble("saturatedTemperature")).calculate();

    setR("saturatedPressure", results.pressure);
    setR("saturatedTemperature", results.temperature);
    setR("liquidEnthalpy", results.liquidSpecificEnthalpy);
    setR("gasEnthalpy", results.gasSpecificEnthalpy);
    setR("evaporationEnthalpy", results.evaporationSpecificEnthalpy);
    setR("liquidEntropy", results.liquidSpecificEntropy);
    setR("gasEntropy", results.gasSpecificEntropy);
    setR("evaporationEntropy", results.evaporationSpecificEntropy);
    setR("liquidVolume", results.liquidSpecificVolume);
    setR("gasVolume", results.gasSpecificVolume);
    setR("evaporationVolume", results.evaporationSpecificVolume);
    info.GetReturnValue().Set(r);
}

NAN_METHOD(saturatedPropertiesGivenPressure) {

    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    double const temperature = SaturatedTemperature(getDouble("saturatedPressure")).calculate();
    auto const results = SaturatedProperties(getDouble("saturatedPressure"), temperature).calculate();
    setR("saturatedPressure", results.pressure);
    setR("saturatedTemperature", results.temperature);
    setR("liquidEnthalpy", results.liquidSpecificEnthalpy);
    setR("gasEnthalpy", results.gasSpecificEnthalpy);
    setR("evaporationEnthalpy", results.evaporationSpecificEnthalpy);
    setR("liquidEntropy", results.liquidSpecificEntropy);
    setR("gasEntropy", results.gasSpecificEntropy);
    setR("evaporationEntropy", results.evaporationSpecificEntropy);
    setR("liquidVolume", results.liquidSpecificVolume);
    setR("gasVolume", results.gasSpecificVolume);
    setR("evaporationVolume", results.evaporationSpecificVolume);
    info.GetReturnValue().Set(r);
}


NAN_METHOD(steamProperties) {
    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    try {
        SteamProperties::ThermodynamicQuantity quantity = thermodynamicQuantity();
        auto const results = SteamProperties(getDouble("pressure"), quantity, getDouble("quantityValue")).calculate();
        setR("pressure", results.pressure);
        setR("temperature", results.temperature);
        setR("specificEnthalpy", results.specificEnthalpy);
        setR("specificEntropy", results.specificEntropy);
        setR("quality", results.quality);
        setR("specificVolume", results.specificVolume);
    } catch (std::runtime_error const &e) {
        std::string const what = e.what();
        ThrowError(std::string("std::runtime_error thrown in steamProperties - ssmt.h: " + what).c_str());
    }
    info.GetReturnValue().Set(r);
}

NAN_METHOD(boiler) {

    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    try {
        SteamProperties::ThermodynamicQuantity quantityType = thermodynamicQuantity();

        auto const b = Boiler(getDouble("deaeratorPressure"), getDouble("combustionEfficiency"),
                              getDouble("blowdownRate"),
                              getDouble("steamPressure"), quantityType, getDouble("quantityValue"),
                              getDouble("steamMassFlow"));

        auto const results = b.getSteamProperties();
        setR("steamPressure", results.pressure);
        setR("steamTemperature", results.temperature);
        setR("steamSpecificEnthalpy", results.specificEnthalpy);
        setR("steamSpecificEntropy", results.specificEntropy);
        setR("steamQuality", results.quality);
        setR("steamMassFlow", results.massFlow);
        setR("steamEnergyFlow", results.energyFlow);

        auto const results2 = b.getBlowdownProperties();
        setR("blowdownPressure", results2.pressure);
        setR("blowdownTemperature", results2.temperature);
        setR("blowdownSpecificEnthalpy", results2.specificEnthalpy);
        setR("blowdownSpecificEntropy", results2.specificEntropy);
        setR("blowdownQuality", results2.quality);
        setR("blowdownMassFlow", results2.massFlow);
        setR("blowdownEnergyFlow", results2.energyFlow);

        auto const results3 = b.getFeedwaterProperties();
        setR("feedwaterPressure", results3.pressure);
        setR("feedwaterTemperature", results3.temperature);
        setR("feedwaterSpecificEnthalpy", results3.specificEnthalpy);
        setR("feedwaterSpecificEntropy", results3.specificEntropy);
        setR("feedwaterQuality", results3.quality);
        setR("feedwaterMassFlow", results3.massFlow);
        setR("feedwaterEnergyFlow", results3.energyFlow);
        setR("boilerEnergy", b.getBoilerEnergy());
        setR("fuelEnergy", b.getFuelEnergy());
    } catch (std::runtime_error const &e) {
        std::string const what = e.what();
        ThrowError(std::string("std::runtime_error thrown in boiler - ssmt.h: " + what).c_str());
    }

    info.GetReturnValue().Set(r);
}

NAN_METHOD(heatLoss) {
    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    try {
        SteamProperties::ThermodynamicQuantity quantityType = thermodynamicQuantity();
        auto const hl = HeatLoss(getDouble("inletPressure"), quantityType, getDouble("quantityValue"),
                                 getDouble("inletMassFlow"),
                                 getDouble("percentHeatLoss"));
        auto const results = hl.getInletProperties();

        setR("inletPressure", results.pressure);
        setR("inletTemperature", results.temperature);
        setR("inletSpecificEnthalpy", results.specificEnthalpy);
        setR("inletSpecificEntropy", results.specificEntropy);
        setR("inletQuality", results.quality);
        setR("inletMassFlow", results.massFlow);
        setR("inletEnergyFlow", results.energyFlow);

        auto const results2 = hl.getOutletProperties();
        setR("outletPressure", results2.pressure);
        setR("outletTemperature", results2.temperature);
        setR("outletSpecificEnthalpy", results2.specificEnthalpy);
        setR("outletSpecificEntropy", results2.specificEntropy);
        setR("outletQuality", results2.quality);
        setR("outletMassFlow", results2.massFlow);
        setR("outletEnergyFlow", results2.energyFlow);
        setR("heatLoss", hl.getHeatLoss());
    } catch (std::runtime_error const &e) {
        std::string const what = e.what();
        ThrowError(std::string("std::runtime_error thrown in heatLoss - ssmt.h: " + what).c_str());
    }

    info.GetReturnValue().Set(r);
}

NAN_METHOD(flashTank) {
    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    try {
        SteamProperties::ThermodynamicQuantity quantityType = thermodynamicQuantity();

        auto const ft = FlashTank(getDouble("inletWaterPressure"), quantityType, getDouble("quantityValue"),
                                  getDouble("inletWaterMassFlow"), getDouble("tankPressure"));

        auto const results = ft.getInletWaterProperties();
        setR("inletWaterPressure", results.pressure);
        setR("inletWaterTemperature", results.temperature);
        setR("inletWaterSpecificEnthalpy", results.specificEnthalpy);
        setR("inletWaterSpecificEntropy", results.specificEntropy);
        setR("inletWaterQuality", results.quality);
        setR("inletWaterMassFlow", results.massFlow);
        setR("inletWaterEnergyFlow", results.energyFlow);

        auto const results2 = ft.getOutletGasSaturatedProperties();
        setR("outletGasPressure", results2.pressure);
        setR("outletGasTemperature", results2.temperature);
        setR("outletGasSpecificEnthalpy", results2.specificEnthalpy);
        setR("outletGasSpecificEntropy", results2.specificEntropy);
        setR("outletGasQuality", 1);
        setR("outletGasMassFlow", results2.massFlow);
        setR("outletGasEnergyFlow", results2.energyFlow);

        auto const results3 = ft.getOutletLiquidSaturatedProperties();
        setR("outletLiquidPressure", results3.pressure);
        setR("outletLiquidTemperature", results3.temperature);
        setR("outletLiquidSpecificEnthalpy", results3.specificEnthalpy);
        setR("outletLiquidSpecificEntropy", results3.specificEntropy);
        setR("outletLiquidQuality", 0);
        setR("outletLiquidMassFlow", results3.massFlow);
        setR("outletLiquidEnergyFlow", results3.energyFlow);
    } catch (std::runtime_error const &e) {
        std::string const what = e.what();
        ThrowError(std::string("std::runtime_error thrown in flashTank - ssmt.h: " + what).c_str());
    }

    info.GetReturnValue().Set(r);
}

NAN_METHOD(prvWithoutDesuperheating) {
    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    try {
        SteamProperties::ThermodynamicQuantity quantityType = thermodynamicQuantity();
        auto const pwod = PrvWithoutDesuperheating(getDouble("inletPressure"), quantityType, getDouble("quantityValue"),
                                                   getDouble("inletMassFlow"), getDouble("outletPressure"));

        auto const results = pwod.getInletProperties();
        setR("inletPressure", results.pressure);
        setR("inletTemperature", results.temperature);
        setR("inletSpecificEnthalpy", results.specificEnthalpy);
        setR("inletSpecificEntropy", results.specificEntropy);
        setR("inletQuality", results.quality);
        setR("inletMassFlow", pwod.getInletMassFlow());
        setR("inletEnergyFlow", pwod.getInletEnergyFlow());

        auto const results2 = pwod.getOutletProperties();
        setR("outletPressure", results2.pressure);
        setR("outletTemperature", results2.temperature);
        setR("outletSpecificEnthalpy", results2.specificEnthalpy);
        setR("outletSpecificEntropy", results2.specificEntropy);
        setR("outletQuality", results2.quality);
        setR("outletMassFlow", pwod.getOutletMassFlow());
        setR("outletEnergyFlow", pwod.getOutletEnergyFlow());
    } catch (std::runtime_error const &e) {
        std::string const what = e.what();
        ThrowError(std::string("std::runtime_error thrown in prvWithoutDesuperheating - ssmt.h: " + what).c_str());
    }

    info.GetReturnValue().Set(r);
}

NAN_METHOD(prvWithDesuperheating) {

    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    try {
        SteamProperties::ThermodynamicQuantity quantityType = thermodynamicQuantity();
        SteamProperties::ThermodynamicQuantity feedwaterQuantityType = feedwaterThermodynamicQuantity();

        auto const pwd = PrvWithDesuperheating(getDouble("inletPressure"), quantityType, getDouble("quantityValue"),
                                               getDouble("inletMassFlow"),
                                               getDouble("outletPressure"), getDouble("feedwaterPressure"),
                                               feedwaterQuantityType,
                                               getDouble("feedwaterQuantityValue"), getDouble("desuperheatingTemp"));

        auto const results = pwd.getInletProperties();
        setR("inletPressure", results.pressure);
        setR("inletTemperature", results.temperature);
        setR("inletSpecificEnthalpy", results.specificEnthalpy);
        setR("inletSpecificEntropy", results.specificEntropy);
        setR("inletQuality", results.quality);
        setR("inletMassFlow", pwd.getInletMassFlow());
        setR("inletEnergyFlow", pwd.getInletEnergyFlow());

        auto const results2 = pwd.getOutletProperties();
        setR("outletPressure", results2.pressure);
        setR("outletTemperature", results2.temperature);
        setR("outletSpecificEnthalpy", results2.specificEnthalpy);
        setR("outletSpecificEntropy", results2.specificEntropy);
        setR("outletQuality", results2.quality);
        setR("outletMassFlow", pwd.getOutletMassFlow());
        setR("outletEnergyFlow", pwd.getOutletEnergyFlow());

        auto const results3 = pwd.getFeedwaterProperties();
        setR("feedwaterPressure", results3.pressure);
        setR("feedwaterTemperature", results3.temperature);
        setR("feedwaterSpecificEnthalpy", results3.specificEnthalpy);
        setR("feedwaterSpecificEntropy", results3.specificEntropy);
        setR("feedwaterQuality", results3.quality);
        setR("feedwaterMassFlow", pwd.getFeedwaterMassFlow());
        setR("feedwaterEnergyFlow", pwd.getFeedwaterEnergyFlow());
    } catch (std::runtime_error const &e) {
        std::string const what = e.what();
        ThrowError(std::string("std::runtime_error thrown in prvWithDesuperheating - ssmt.h: " + what).c_str());
    }

    info.GetReturnValue().Set(r);
}

NAN_METHOD(deaerator) {
    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    try {
        SteamProperties::ThermodynamicQuantity waterQuantityType = waterThermodynamicQuantity();
        SteamProperties::ThermodynamicQuantity steamQuantityType = steamThermodynamicQuantity();

        Deaerator d(getDouble("deaeratorPressure"), getDouble("ventRate"), getDouble("feedwaterMassFlow"),
                    getDouble("waterPressure"),
                    waterQuantityType, getDouble("waterQuantityValue"), getDouble("steamPressure"), steamQuantityType,
                    getDouble("steamQuantityValue"));

        auto const results = d.getFeedwaterProperties();
        setR("feedwaterPressure", results.pressure);
        setR("feedwaterTemperature", results.temperature);
        setR("feedwaterSpecificEnthalpy", results.specificEnthalpy);
        setR("feedwaterSpecificEntropy", results.specificEntropy);
        setR("feedwaterQuality", results.quality);
        setR("feedwaterMassFlow", results.massFlow);
        setR("feedwaterEnergyFlow", results.energyFlow);

        auto const results2 = d.getVentedSteamProperties();
        setR("ventedSteamPressure", results2.pressure);
        setR("ventedSteamTemperature", results2.temperature);
        setR("ventedSteamSpecificEnthalpy", results2.specificEnthalpy);
        setR("ventedSteamSpecificEntropy", results2.specificEntropy);
        setR("ventedSteamQuality", results2.quality);
        setR("ventedSteamMassFlow", results2.massFlow);
        setR("ventedSteamEnergyFlow", results2.energyFlow);

        auto const results3 = d.getInletWaterProperties();
        setR("inletWaterPressure", results3.pressure);
        setR("inletWaterTemperature", results3.temperature);
        setR("inletWaterSpecificEnthalpy", results3.specificEnthalpy);
        setR("inletWaterSpecificEntropy", results3.specificEntropy);
        setR("inletWaterQuality", results3.quality);
        setR("inletWaterMassFlow", results3.massFlow);
        setR("inletWaterEnergyFlow", results3.energyFlow);

        auto const results4 = d.getInletSteamProperties();
        setR("inletSteamPressure", results4.pressure);
        setR("inletSteamTemperature", results4.temperature);
        setR("inletSteamSpecificEnthalpy", results4.specificEnthalpy);
        setR("inletSteamSpecificEntropy", results4.specificEntropy);
        setR("inletSteamQuality", results4.quality);
        setR("inletSteamMassFlow", results4.massFlow);
        setR("inletSteamEnergyFlow", results4.energyFlow);
    } catch (std::runtime_error const &e) {
        std::string const what = e.what();
        ThrowError(std::string("std::runtime_error thrown in deaerator - ssmt.h: " + what).c_str());
    }

    info.GetReturnValue().Set(r);
}

NAN_METHOD(header) {
    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    auto const headerPressure = getDouble("headerPressure");

    Local <String> arrayStr = Nan::New<String>("inlets").ToLocalChecked();
    auto array = inp->ToObject()->Get(arrayStr);
    v8::Local <v8::Array> arr = v8::Local<v8::Array>::Cast(array);

    auto pressureStr = Nan::New<String>("pressure").ToLocalChecked();
    auto temperatureStr = Nan::New<String>("temperature").ToLocalChecked();
    auto qualityStr = Nan::New<String>("quality").ToLocalChecked();
    auto massFlowStr = Nan::New<String>("massFlow").ToLocalChecked();
    auto specificEnthalpyStr = Nan::New<String>("specificEnthalpy").ToLocalChecked();
    auto specificEntropyStr = Nan::New<String>("specificEntropy").ToLocalChecked();
    auto energyFlowStr = Nan::New<String>("energyFlow").ToLocalChecked();

    std::vector<Inlet> inlets;

    for (std::size_t i = 0; i < arr->Length(); i++) {
        auto const pressure = arr->Get(i)->ToObject()->Get(pressureStr)->NumberValue();
        unsigned val = static_cast<unsigned>(arr->Get(i)->ToObject()->Get(
                Nan::New<String>("thermodynamicQuantity").ToLocalChecked())->NumberValue());
        auto const quantity = static_cast<SteamProperties::ThermodynamicQuantity>(val);
        auto const quantityValue = arr->Get(i)->ToObject()->Get(
                Nan::New<String>("quantityValue").ToLocalChecked())->NumberValue();
        auto const massFlow = arr->Get(i)->ToObject()->Get(massFlowStr)->NumberValue();
        inlets.emplace_back(Inlet(pressure, quantity, quantityValue, massFlow));

        Local <Object> obj = Nan::New<Object>();
        Local <String> inletNum = Nan::New<String>("inlet" + std::to_string(i + 1)).ToLocalChecked();
        auto const inletProps = inlets[i].getInletProperties();

        Nan::Set(obj, pressureStr, Nan::New<Number>(inletProps.pressure));
        Nan::Set(obj, temperatureStr, Nan::New<Number>(inletProps.temperature));
        Nan::Set(obj, qualityStr, Nan::New<Number>(inletProps.quality));
        Nan::Set(obj, specificEnthalpyStr, Nan::New<Number>(inletProps.specificEnthalpy));
        Nan::Set(obj, specificEntropyStr, Nan::New<Number>(inletProps.specificEntropy));
        Nan::Set(obj, energyFlowStr, Nan::New<Number>(inlets[i].getInletEnergyFlow()));
        Nan::Set(obj, massFlowStr, Nan::New<Number>(inlets[i].getMassFlow()));

        Nan::Set(r, inletNum, obj);
    }

    try {
        auto header = Header(headerPressure, inlets);
        Local <String> headerStr = Nan::New<String>("header").ToLocalChecked();

        auto const headerProps = header.getHeaderProperties();

        Local <Object> obj = Nan::New<Object>();

        Nan::Set(obj, pressureStr, Nan::New<Number>(header.getHeaderPressure()));
        Nan::Set(obj, temperatureStr, Nan::New<Number>(headerProps.temperature));
        Nan::Set(obj, qualityStr, Nan::New<Number>(headerProps.quality));
        Nan::Set(obj, specificEnthalpyStr, Nan::New<Number>(headerProps.specificEnthalpy));
        Nan::Set(obj, specificEntropyStr, Nan::New<Number>(headerProps.specificEntropy));
        Nan::Set(obj, energyFlowStr, Nan::New<Number>(header.getInletEnergyFlow()));
        Nan::Set(obj, massFlowStr, Nan::New<Number>(header.getInletMassFlow()));

        Nan::Set(r, headerStr, obj);
    } catch (std::runtime_error const &e) {
        std::string const what = e.what();
        ThrowError(std::string("std::runtime_error thrown in header - ssmt.h: " + what).c_str());
    }
    info.GetReturnValue().Set(r);
}

NAN_METHOD(turbine) {
    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    unsigned val = static_cast<unsigned>(inp->ToObject()->Get(
            Nan::New<String>("solveFor").ToLocalChecked())->NumberValue());
    Turbine::Solve solveFor = static_cast<Turbine::Solve>(val);
    val = static_cast<unsigned>(inp->ToObject()->Get(
            Nan::New<String>("turbineProperty").ToLocalChecked())->NumberValue());
    Turbine::TurbineProperty turbineProperty = static_cast<Turbine::TurbineProperty>(val);
    val = static_cast<unsigned>(inp->ToObject()->Get(
            Nan::New<String>("inletQuantity").ToLocalChecked())->NumberValue());
    SteamProperties::ThermodynamicQuantity inletQuantity = static_cast<SteamProperties::ThermodynamicQuantity>(val);
    std::shared_ptr<Turbine> t;

    try {
        if (solveFor == Turbine::Solve::OutletProperties) {
            t = std::shared_ptr<Turbine>(
                    new Turbine(solveFor, getDouble("inletPressure"), inletQuantity, getDouble("inletQuantityValue"),
                                turbineProperty, getDouble("isentropicEfficiency"), getDouble("generatorEfficiency"),
                                getDouble("massFlowOrPowerOut"), getDouble("outletSteamPressure")));
        } else {
            unsigned val = static_cast<unsigned>(inp->ToObject()->Get(
                    Nan::New<String>("outletQuantity").ToLocalChecked())->NumberValue());
            auto const outletQuantity = static_cast<SteamProperties::ThermodynamicQuantity>(val);
            t = std::shared_ptr<Turbine>(
                    new Turbine(solveFor, getDouble("inletPressure"), inletQuantity, getDouble("inletQuantityValue"),
                                turbineProperty, getDouble("generatorEfficiency"), getDouble("massFlowOrPowerOut"),
                                getDouble("outletSteamPressure"), outletQuantity, getDouble("outletQuantityValue")));
        }
    } catch (std::runtime_error const &e) {
        std::string const what = e.what();
        ThrowError(std::string("std::runtime_error thrown in turbine - ssmt.h: " + what).c_str());
    }

    auto results = t->getInletProperties();
    setR("inletPressure", results.pressure);
    setR("inletTemperature", results.temperature);
    setR("inletSpecificEnthalpy", results.specificEnthalpy);
    setR("inletSpecificEntropy", results.specificEntropy);
    setR("inletQuality", results.quality);
    setR("inletEnergyFlow", t->getInletEnergyFlow());

    results = t->getOutletProperties();
    setR("outletPressure", results.pressure);
    setR("outletTemperature", results.temperature);
    setR("outletSpecificEnthalpy", results.specificEnthalpy);
    setR("outletSpecificEntropy", results.specificEntropy);
    setR("outletQuality", results.quality);
    setR("outletEnergyFlow", t->getOutletEnergyFlow());

    setR("massFlow", t->getMassFlow());
    setR("isentropicEfficiency", t->getIsentropicEfficiency());
    setR("energyOut", t->getEnergyOut());
    setR("powerOut", t->getPowerOut());
    setR("generatorEfficiency", t->getGeneratorEfficiency());
    info.GetReturnValue().Set(r);
}

NAN_METHOD(heatExchanger) {
    inp = info[0]->ToObject();
    r = Nan::New<Object>();

    const double hotInletMassFlow = getDouble("hotInletMassFlow");
    const double hotInletEnergyFlow = getDouble("hotInletEnergyFlow");
    const double hotInletTemperature = getDouble("hotInletTemperature");
    const double hotInletPressure = getDouble("hotInletPressure");
    const double hotInletQuality = getDouble("hotInletQuality");
    const double hotInletSpecificVolume = getDouble("hotInletSpecificVolume");
    const double hotInletDensity = getDouble("hotInletDensity");
    const double hotInletSpecificEnthalpy = getDouble("hotInletSpecificEnthalpy");
    const double hotInletSpecificEntropy = getDouble("hotInletSpecificEntropy");

    const double coldInletMassFlow = getDouble("coldInletMassFlow");
    const double coldInletEnergyFlow = getDouble("coldInletEnergyFlow");
    const double coldInletTemperature = getDouble("coldInletTemperature");
    const double coldInletPressure = getDouble("coldInletPressure");
    const double coldInletQuality = getDouble("coldInletQuality");
    const double coldInletSpecificVolume = getDouble("coldInletSpecificVolume");
    const double coldInletDensity = getDouble("coldInletDensity");
    const double coldInletSpecificEnthalpy = getDouble("coldInletSpecificEnthalpy");
    const double coldInletSpecificEntropy = getDouble("coldInletSpecificEntropy");

    const double approachTemp = getDouble("approachTemp");

    const SteamSystemModelerTool::FluidProperties hotInlet(hotInletMassFlow, hotInletEnergyFlow, hotInletTemperature,
    hotInletPressure, hotInletQuality, hotInletSpecificVolume, hotInletDensity, hotInletSpecificEnthalpy,
    hotInletSpecificEntropy);

    const SteamSystemModelerTool::FluidProperties coldInlet(coldInletMassFlow, coldInletEnergyFlow, coldInletTemperature,
    coldInletPressure, coldInletQuality, coldInletSpecificVolume, coldInletDensity, coldInletSpecificEnthalpy,
    coldInletSpecificEntropy);

    auto const output = HeatExchanger(hotInlet, coldInlet, approachTemp).calculate();

    setR("hotOutletMassFlow", output.hotOutlet.massFlow);
    setR("hotOutletEnergyFlow", output.hotOutlet.energyFlow);
    setR("hotOutletTemperature", output.hotOutlet.temperature);
    setR("hotOutletPressure", output.hotOutlet.pressure);
    setR("hotOutletQuality", output.hotOutlet.quality);
    setR("hotOutletSpecificVolume", output.hotOutlet.specificVolume);
    setR("hotOutletDensity", output.hotOutlet.density);
    setR("hotOutletSpecificEnthalpy", output.hotOutlet.specificEnthalpy);
    setR("hotOutletSpecificEntropy", output.hotOutlet.specificEntropy);

    setR("coldOutletMassFlow", output.coldOutlet.massFlow);
    setR("coldOutletEnergyFlow", output.coldOutlet.energyFlow);
    setR("coldOutletTemperature", output.coldOutlet.temperature);
    setR("coldOutletPressure", output.coldOutlet.pressure);
    setR("coldOutletQuality", output.coldOutlet.quality);
    setR("coldOutletSpecificVolume", output.coldOutlet.specificVolume);
    setR("coldOutletDensity", output.coldOutlet.density);
    setR("coldOutletSpecificEnthalpy", output.coldOutlet.specificEnthalpy);
    setR("coldOutletSpecificEntropy", output.coldOutlet.specificEntropy);

    info.GetReturnValue().Set(r);
}

#endif //AMO_TOOLS_SUITE_SSMT_H
