/**
 * @file
 * @brief Contains the implementation of the saturated properties calculator for steam systems.
 *
 * @author Autumn Ferree (ferreeak)
 * @bug No known bugs.
 *
 */

#include "ssmt/SaturatedProperties.h"
#include "ssmt/SteamSystemModelerTool.h"

double SaturatedTemperature::calculate() const {
    const double C1 = 0.11670521452767E+04, C2 = -0.72421316703206E+06, C3 = -0.17073846940092E+02;
    const double C4 = 0.12020824702470E+05, C5 = -0.32325550322333E+07, C6 = 0.14915108613530E+02;
    const double C7 = -0.48232657361591E+04, C8 = 0.40511340542057E+06, C9 = -0.23855557567849E+00;
    const double C10 = 0.65017534844798E+03;

    const double SS = std::pow(saturatedPressure_, 0.25);
    const double E = SS * SS + C3 * SS + C6;
    const double F = C1 * SS * SS + C4 * SS + C7;
    const double G = C2 * SS * SS + C5 * SS + C8;
    const double D = 2 * G / (-F - std::sqrt(pow(F, 2) - 4 * E * G));

    return (C10 + D - std::sqrt(std::pow((C10 + D), 2) - 4 * (C9 + C10 * D))) / 2;
}

double SaturatedPressure::calculate() const {
    const double C1 = 0.11670521452767E+04, C2 = -0.72421316703206E+06, C3 = -0.17073846940092E+02;
    const double C4 = 0.12020824702470E+05, C5 = -0.32325550322333E+07, C6 = 0.14915108613530E+02;
    const double C7 = -0.48232657361591E+04, C8 = 0.40511340542057E+06, C9 = -0.23855557567849E+00;
    const double C10 = 0.65017534844798E+03;

    const double v = saturatedTemperature_ + (C9 / (saturatedTemperature_ - C10));
    const double vSquared = std::pow(v, 2);

    const double A = vSquared + (C1 * v) + C2;
    const double B = (C3 * vSquared) + (C4 * v) + C5;
    const double C = (C6 * vSquared) + (C7 * v) + C8;

    return std::pow((2 * C) / (-B + std::sqrt(std::pow(B, 2) - 4 * A * C)), 4);
}

std::unordered_map<std::string, double> SaturatedProperties::calculate() {
    auto const gasProperties = SteamSystemModelerTool::region2(saturatedTemperature_, saturatedPressure_);
    std::unordered_map<std::string, double> liquidProperties;

    if ((saturatedTemperature_ >= SteamSystemModelerTool::TEMPERATURE_MIN)
        && (saturatedTemperature_ <= SteamSystemModelerTool::TEMPERATURE_Tp)) {
        liquidProperties = SteamSystemModelerTool::region1(saturatedTemperature_, saturatedPressure_);
    }

    if ((saturatedTemperature_ > SteamSystemModelerTool::TEMPERATURE_Tp)
        && (saturatedTemperature_ <= SteamSystemModelerTool::TEMPERATURE_CRIT)) {
        liquidProperties = SteamSystemModelerTool::region3(saturatedTemperature_, saturatedPressure_);
    }

    const double evaporationEnthalpy = gasProperties.at("specificEnthalpy") - liquidProperties.at("specificEnthalpy");
    const double evaporationEntropy = gasProperties.at("specificEntropy") - liquidProperties.at("specificEntropy");
    const double evaporationVolume = gasProperties.at("specificVolume") - liquidProperties.at("specificVolume");

    return {
            {"pressure", saturatedPressure_}, //pressure in MPa
            {"temperature", saturatedTemperature_}, // temperature in Kelvin
            {"gasSpecificEnthalpy", gasProperties.at("specificEnthalpy")}, //enthalpy in kJ/kg
            {"gasSpecificEntropy", gasProperties.at("specificEntropy")}, // entropy in kJ/kg/K
            {"gasSpecificVolume", gasProperties.at("specificVolume")}, // volume in m³/kg
            {"liquidSpecificEnthalpy", liquidProperties.at("specificEnthalpy")}, // enthalpy in kJ/kg
            {"liquidSpecificEntropy", liquidProperties.at("specificEntropy")}, // entropy in kJ/kg/K
            {"liquidSpecificVolume", liquidProperties.at("specificVolume")}, // volume in m³/kg
            {"evaporationSpecificEnthalpy", evaporationEnthalpy}, // enthalpy in kJ/kg
            {"evaporationSpecificEntropy", evaporationEntropy}, // entropy in kJ/kg/K
            {"evaporationSpecificVolume", evaporationVolume}, // volume in m³/kg
    };
}