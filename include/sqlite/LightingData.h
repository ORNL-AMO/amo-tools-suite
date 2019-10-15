#ifndef AMO_TOOLS_SUITE_LIGHTINGDATA_H
#define AMO_TOOLS_SUITE_LIGHTINGDATA_H
#include <string>

namespace LightingDataCategory
{
enum class Category
{
    NONE
};
}

class LightingData
{
public:
    LightingData(
        const LightingDataCategory::Category category,
        const double lampsPerFixture,
        const double lampWatage,
        const double lampOutput,
        const double lampLife,
        const double lampCRI,
        const double coefficientOfUtilization,
        const double ballastFactor,
        const double lumenDegredationFactor
        ) : category (category),
            lampsPerFixture (lampsPerFixture),
            lampWatage (lampWatage),
            lampOutput (lampOutput),
            lampLife (lampLife),
            lampCRI (lampCRI),
            coefficientOfUtilization (coefficientOfUtilization),
            ballastFactor (ballastFactor),
            lumenDegredationFactor (lumenDegredationFactor)
    {
    }

private:
    LightingDataCategory::Category category = LightingDataCategory::Category::NONE;
    double lampsPerFixture = 0.0;
    double lampWatage = 0.0;
    double lampOutput = 0.0;
    double lampLife = 0.0;
    double lampCRI = 0.0;
    double coefficientOfUtilization = 0.0;
    double ballastFactor = 0.0;
    double reactionHeat = 0.0;
    double lumenDegredationFactor = 0.0;
    std::string type = "Unknown";
    int id = 0;

    friend class SQLite;

}

// std::vector<LightingData>
// SQLite::get_default_lighting_data()
// {
//     return {
//         {"Water vapor - Near Atm. Pressure", 0.47},
//         {"Steam - 50 PSIG, 400 degrees F", 0.4901},
//         {"Steam - 150 PSIG, 500 degrees F", 0.5099},
//         {"Steam - 600 PSIG, 700 degrees F", 0.5899},
//         {"Air - Low Pressure", 0.245},
//         {"Nitrogen - Low Pressure", 0.2501},
//         {"Oxygen - Low Pressure", 0.23},
//         {"Carbon Dioxide - Low Pressure", 0.24},
//         {"Carbon Monoxide - Low Pressure", 0.2501},
//         {"Hydrogen - Low Pressure", 3.45}};
// }

#endif //AMO_TOOLS_SUITE_LIGHTINGDATA_H