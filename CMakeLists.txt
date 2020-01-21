cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(amo_tools_suite)

set(CMAKE_MACOSX_RPATH 1)

set( CMAKE_VERSION_MAJOR 0 )
set( CMAKE_VERSION_MINOR 3 )
set( CMAKE_VERSION_PATCH 1 )

set(AMO_SUITE_VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}")
INCLUDE_DIRECTORIES (node_modules/nan)

# Build package
# Requires:
option( BUILD_PACKAGE "Build package" ON )

# Build testing
# Requires:
option( BUILD_TESTING "Build testing targets" ON )

# Build C++ documentation using Doxygen
# Requires: doxygen
# option( BUILD_DOCUMENTATION "Build Documentation" ON )

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(cmake/ProjectMacros.cmake)
include(cmake/CompilerFlags.cmake)

# On Windows, things end up under Debug/ or Release/.
if (WIN32)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
#  set(CMAKE_DATABASE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
else (WIN32)
  # If not on Windows, put them under more standard UNIX-like places.  This is
  # necessary, otherwise they would all end up in
  # ${CMAKE_BINARY_DIR}/src/... or somewhere else random like that.
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#  set(CMAKE_DATABASE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/db)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif (WIN32)

# set (CMAKE_CXX_FLAGS "-Wall")

#file(MAKE_DIRECTORY ${CMAKE_DATABASE_OUTPUT_DIRECTORY})

set(SOURCE_FILES
        src/ssmt/SaturatedProperties.cpp
        src/ssmt/SteamProperties.cpp
        src/ssmt/SteamSystemModelerTool.cpp
        src/ssmt/Boiler.cpp
        src/ssmt/HeatLoss.cpp
        src/ssmt/FlashTank.cpp
        src/ssmt/PRV.cpp
        src/ssmt/Deaerator.cpp
        src/ssmt/Header.cpp
        src/ssmt/Turbine.cpp
        src/ssmt/api/BoilerInput.cpp
        src/ssmt/api/HeaderInput.cpp
        src/ssmt/api/OperationsInput.cpp
        src/ssmt/api/SteamModeler.cpp
        src/ssmt/api/SteamModelerInput.cpp
        src/ssmt/api/SteamModelerOutput.cpp
        src/ssmt/api/TurbineInput.cpp
        src/ssmt/domain/BoilerFactory.cpp
        src/ssmt/domain/DeaeratorFactory.cpp
        src/ssmt/domain/FlashTankFactory.cpp
        src/ssmt/domain/FluidPropertiesFactory.cpp
        src/ssmt/domain/HeaderFactory.cpp
        src/ssmt/domain/HeatLossFactory.cpp
        src/ssmt/domain/InletFactory.cpp
        src/ssmt/domain/PrvWithDesuperheatingFactory.cpp
        src/ssmt/domain/PrvWithoutDesuperheatingFactory.cpp
        src/ssmt/domain/SteamModelerOutputFactory.cpp
        src/ssmt/domain/TurbineFactory.cpp
        src/ssmt/service/DeaeratorModeler.cpp
        src/ssmt/service/EnergyFlowCalculator.cpp
        src/ssmt/service/energy_and_cost/EnergyAndCostCalculator.cpp
        src/ssmt/service/high_pressure_header/CondensingTurbineCalculator.cpp
        src/ssmt/service/high_pressure_header/HighPressureCondensateCalculator.cpp
        src/ssmt/service/high_pressure_header/HighPressureFlashTankCalculator.cpp
        src/ssmt/service/high_pressure_header/HighPressureHeaderCalculator.cpp
        src/ssmt/service/high_pressure_header/HighPressureHeaderModeler.cpp
        src/ssmt/service/high_pressure_header/HighToLowSteamTurbineCalculator.cpp
        src/ssmt/service/high_pressure_header/HighToMediumSteamTurbineCalculator.cpp
        src/ssmt/service/low_pressure_header/LowPressureCondensateCalculator.cpp
        src/ssmt/service/low_pressure_header/LowPressureFlashedSteamIntoHeaderCalculator.cpp
        src/ssmt/service/low_pressure_header/LowPressureHeaderCalculator.cpp
        src/ssmt/service/low_pressure_header/LowPressureHeaderModeler.cpp
        src/ssmt/service/low_pressure_header/LowPressurePrvCalculator.cpp
        src/ssmt/service/MassFlowCalculator.cpp
        src/ssmt/service/medium_pressure_header/HighToMediumPrvCalculator.cpp
        src/ssmt/service/medium_pressure_header/MediumPressureCondensateCalculator.cpp
        src/ssmt/service/medium_pressure_header/MediumPressureHeaderCalculator.cpp
        src/ssmt/service/medium_pressure_header/MediumPressureHeaderModeler.cpp
        src/ssmt/service/medium_pressure_header/MediumToLowPressureTurbineCalculator.cpp
        src/ssmt/service/medium_pressure_header/ReducedSteamException.cpp
        src/ssmt/service/medium_pressure_header/SteamBalanceCheckerService.cpp
        src/ssmt/service/medium_pressure_header/UnableToBalanceException.cpp
        src/ssmt/service/power_balance/LowPressureVentedSteamCalculator.cpp
        src/ssmt/service/power_balance/PowerBalanceChecker.cpp
        src/ssmt/service/power_balance/SteamBalanceCalculator.cpp
        src/ssmt/service/power_balance/SteamProductionCalculator.cpp
        src/ssmt/service/power_balance/SteamUseCalculator.cpp
        src/ssmt/service/process_steam_usage/ProcessSteamUsageModeler.cpp
        src/ssmt/service/PrvCalculator.cpp
        src/ssmt/service/RestarterService.cpp
        src/ssmt/service/SteamBalanceException.cpp
        src/ssmt/service/SteamModelCalculator.cpp
        src/ssmt/service/SteamModelRunner.cpp
        src/ssmt/service/SteamReducer.cpp
        src/ssmt/service/TurbineCalculator.cpp
        src/ssmt/service/water_and_condensate/CombinedCondensateCalculator.cpp
        src/ssmt/service/water_and_condensate/HeatExchangerCalculator.cpp
        src/ssmt/service/water_and_condensate/MakeupWaterAndCondensateHeaderCalculator.cpp
        src/ssmt/service/water_and_condensate/MakeupWaterAndCondensateHeaderModeler.cpp
        src/ssmt/service/water_and_condensate/MakeupWaterCalculator.cpp
        src/ssmt/service/water_and_condensate/MakeupWaterMassFlowCalculator.cpp
        src/ssmt/service/water_and_condensate/MakeupWaterVolumeFlowCalculator.cpp
        src/ssmt/service/water_and_condensate/ReturnCondensateCalculator.cpp)

set(INCLUDE_FILES
        include/ssmt/SaturatedProperties.h
        include/ssmt/SteamProperties.h
        include/ssmt/SteamSystemModelerTool.h
        include/ssmt/Boiler.h
        include/ssmt/HeatLoss.h
        include/ssmt/FlashTank.h
        include/ssmt/PRV.h
        include/ssmt/FlashTank.h
        include/ssmt/Deaerator.h
        include/ssmt/Header.h
        include/ssmt/api/BoilerInput.h
        include/ssmt/api/HeaderInput.h
        include/ssmt/api/OperationsInput.h
        include/ssmt/api/SteamModeler.h
        include/ssmt/api/SteamModelerInput.h
        include/ssmt/api/SteamModelerOutput.h
        include/ssmt/api/TurbineInput.h
        include/ssmt/domain/BoilerFactory.h
        include/ssmt/domain/DeaeratorFactory.h
        include/ssmt/domain/EnergyAndCostCalculationsDomain.h
        include/ssmt/domain/FlashTankFactory.h
        include/ssmt/domain/FluidPropertiesFactory.h
        include/ssmt/domain/HeaderFactory.h
        include/ssmt/domain/HeatLossFactory.h
        include/ssmt/domain/HighPressureHeaderCalculationsDomain.h
        include/ssmt/domain/HighToMediumSteamTurbineCalculationsDomain.h
        include/ssmt/domain/InletFactory.h
        include/ssmt/domain/LowPressureFlashedSteamIntoHeaderCalculatorDomain.h
        include/ssmt/domain/LowPressureHeaderCalculationsDomain.h
        include/ssmt/domain/LowPressureVentedSteamCalculationsDomain.h
        include/ssmt/domain/MakeupWaterAndCondensateHeaderCalculationsDomain.h
        include/ssmt/domain/MakeupWaterVolumeFlowCalculationsDomain.h
        include/ssmt/domain/MediumPressureHeaderCalculationsDomain.h
        include/ssmt/domain/PowerBalanceCheckerCalculationsDomain.h
        include/ssmt/domain/ProcessSteamUsageCalculationsDomain.h
        include/ssmt/domain/PrvWithDesuperheatingFactory.h
        include/ssmt/domain/PrvWithoutDesuperheatingFactory.h
        include/ssmt/domain/ReturnCondensateCalculationsDomain.h
        include/ssmt/domain/SteamModelCalculationsDomain.h
        include/ssmt/domain/SteamModelerOutputFactory.h
        include/ssmt/domain/TurbineFactory.h
        include/ssmt/service/DeaeratorModeler.h
        include/ssmt/service/EnergyFlowCalculator.h
        include/ssmt/service/energy_and_cost/EnergyAndCostCalculator.h
        include/ssmt/service/high_pressure_header/CondensingTurbineCalculator.h
        include/ssmt/service/high_pressure_header/HighPressureCondensateCalculator.h
        include/ssmt/service/high_pressure_header/HighPressureFlashTankCalculator.h
        include/ssmt/service/high_pressure_header/HighPressureHeaderCalculator.h
        include/ssmt/service/high_pressure_header/HighPressureHeaderModeler.h
        include/ssmt/service/high_pressure_header/HighToLowSteamTurbineCalculator.h
        include/ssmt/service/high_pressure_header/HighToMediumSteamTurbineCalculator.h
        include/ssmt/service/low_pressure_header/LowPressureCondensateCalculator.h
        include/ssmt/service/low_pressure_header/LowPressureFlashedSteamIntoHeaderCalculator.h
        include/ssmt/service/low_pressure_header/LowPressureHeaderCalculator.h
        include/ssmt/service/low_pressure_header/LowPressureHeaderModeler.h
        include/ssmt/service/low_pressure_header/LowPressurePrvCalculator.h
        include/ssmt/service/MassFlowCalculator.h
        include/ssmt/service/medium_pressure_header/HighToMediumPrvCalculator.h
        include/ssmt/service/medium_pressure_header/MediumPressureCondensateCalculator.h
        include/ssmt/service/medium_pressure_header/MediumPressureHeaderCalculator.h
        include/ssmt/service/medium_pressure_header/MediumPressureHeaderModeler.h
        include/ssmt/service/medium_pressure_header/MediumToLowPressureTurbineCalculator.h
        include/ssmt/service/medium_pressure_header/ReducedSteamException.h
        include/ssmt/service/medium_pressure_header/SteamBalanceCheckerService.h
        include/ssmt/service/medium_pressure_header/UnableToBalanceException.h
        include/ssmt/service/power_balance/LowPressureVentedSteamCalculator.h
        include/ssmt/service/power_balance/PowerBalanceChecker.h
        include/ssmt/service/power_balance/SteamBalanceCalculator.h
        include/ssmt/service/power_balance/SteamProductionCalculator.h
        include/ssmt/service/power_balance/SteamUseCalculator.h
        include/ssmt/service/process_steam_usage/ProcessSteamUsageModeler.h
        include/ssmt/service/PrvCalculator.h
        include/ssmt/service/RestarterService.h
        include/ssmt/service/SteamBalanceException.h
        include/ssmt/service/SteamModelCalculator.h
        include/ssmt/service/SteamModelRunner.h
        include/ssmt/service/SteamReducer.h
        include/ssmt/service/TurbineCalculator.h
        include/ssmt/service/water_and_condensate/CombinedCondensateCalculator.h
        include/ssmt/service/water_and_condensate/HeatExchangerCalculator.h
        include/ssmt/service/water_and_condensate/MakeupWaterAndCondensateHeaderCalculator.h
        include/ssmt/service/water_and_condensate/MakeupWaterAndCondensateHeaderModeler.h
        include/ssmt/service/water_and_condensate/MakeupWaterCalculator.h
        include/ssmt/service/water_and_condensate/MakeupWaterMassFlowCalculator.h
        include/ssmt/service/water-and-condensate/MakeupWaterVolumeFlowCalculator.h
        include/ssmt/service/water_and_condensate/ReturnCondensateCalculator.h
        include/ssmt/HeatExchanger.h
        )

set(TEST_FILES
        tests/OptimalSpecificSpeedCorrection.unit.cpp
        tests/MotorEfficiency.unit.cpp
        tests/Results.unit.cpp
        tests/SolidLoadChargeMaterial.unit.cpp
        tests/LiquidLoadChargeMaterial.unit.cpp
        tests/Atmosphere.unit.cpp
        tests/FixtureLosses.unit.cpp
        tests/GasLoadChargeMaterial.unit.cpp
        tests/GasCoolingLosses.unit.cpp
        tests/LiquidCoolingLosses.unit.cpp
        tests/WaterCoolingLosses.unit.cpp
        tests/LeakageLosses.unit.cpp
        tests/WallLosses.unit.cpp
        tests/OpeningLosses.unit.cpp
        tests/GasFlueGasMaterial.unit.cpp
        tests/HeadTool.unit.cpp
        tests/EnergyInputEAF.unit.cpp
        tests/SQLite.unit.cpp
        tests/SolidLiquidFlueGasMaterial.unit.cpp
        tests/AuxiliaryPower.unit.cpp
        tests/SlagOtherMaterialLosses.unit.cpp
        tests/ExhaustGasEAF.unit.cpp
        tests/EnergyEquivalency.unit.cpp
        tests/EfficiencyImprovement.unit.cpp
        tests/ElectricityReduction.unit.cpp
        tests/NaturalGasReduction.unit.cpp
        tests/CompressedAirReduction.unit.cpp
        tests/CompressedAirPressureReduction.unit.cpp
        tests/WaterReduction.unit.cpp
        tests/InsulatedPipeReduction.unit.cpp
        tests/InsulatedTankReduction.unit.cpp
        tests/InsulationServices.unit.cpp
        tests/SteamReduction.unit.cpp
        tests/O2Enrichment.unit.cpp
        tests/FlowCalculationsEnergyUse.unit.cpp
        tests/EnergyInputExhaustGasLosses.unit.cpp
        tests/HumidityRatio.unit.cpp
        tests/SaturatedProperties.unit.cpp
        tests/SteamProperties.unit.cpp
        tests/Boiler.unit.cpp
        tests/HeatLoss.unit.cpp
        tests/FlashTank.unit.cpp
        tests/PRV.unit.cpp
        tests/Deaerator.unit.cpp
        tests/Header.unit.cpp
        tests/Turbine.unit.cpp
        tests/Fan.unit.cpp
        tests/CHP.unit.cpp
        tests/CurveFitVal.unit.cpp
        tests/MotorCurrent.unit.cpp
        tests/CompressedAir.unit.cpp
        tests/FluidPower.unit.cpp
        tests/CompressibilityFactor.unit.cpp
        tests/OptimalFanEfficiency.unit.cpp
        tests/HeatExchanger.unit.cpp
        tests/steamapi/BoilerInput.unit.cpp
        tests/steamapi/HeaderInput.unit.cpp
        tests/steamapi/OperationsInput.unit.cpp
        tests/steamapi/SteamModeler.unit.cpp
        tests/steamapi/TurbineInput.unit.cpp)

#find_package(PythonInterp 2.7 REQUIRED)
#execute_process( COMMAND ${PYTHON_EXECUTABLE} "${PROJECT_SOURCE_DIR}/scripts/seed_database.py" "${CMAKE_DATABASE_OUTPUT_DIRECTORY}/amo_tools_suite.db")

if (MINGW)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
  set(CMAKE_STATIC_LIBRARY_PREFIX "")
endif ()

include_directories(${CMAKE_SOURCE_DIR}/include)

# Create amo_tools_suite static library
add_library(amo_tools_suite STATIC ${SOURCE_FILES} ${INCLUDE_FILES})

if( UNIX AND NOT APPLE )
  target_link_libraries( amo_tools_suite dl )
endif()

# Add SQLite project
include_directories(${CMAKE_SOURCE_DIR}/third_party/sqlite/ SYSTEM)
add_subdirectory(third_party/sqlite)

# Create command line amo_tools_suite program
add_executable(amo_tools_suite_main main.cpp)
set_target_properties(amo_tools_suite_main PROPERTIES OUTPUT_NAME "amo_tools_suite")
target_link_libraries( amo_tools_suite_main amo_tools_suite )

if( BUILD_TESTING )
    add_library(Catch INTERFACE)
    target_include_directories(Catch INTERFACE third_party/catch/)

    # Create unit testing executable
    add_executable(amo_tools_suite_tests tests/main.unit.cpp ${TEST_FILES})
    target_link_libraries( amo_tools_suite_tests Catch amo_tools_suite )
endif()

#if(BUILD_DOCUMENTATION)
#    find_package(Doxygen REQUIRED)
#set(doxyfile_in ${CMAKE_SOURCE_DIR}/doxyfile.in)
#set(doxyfile ${CMAKE_BINARY_DIR}/doxyfile)
#message("${DOXYGEN_EXECUTABLE} ${doxyfile_in}")
#add_custom_target(
#            doc ALL
#        COMMAND "${DOXYGEN_EXECUTABLE} ${doxyfile_in}"
#            COMMAND
#            $<$<CONFIG:Release>:"${DOXYGEN_EXECUTABLE} ${doxyfile_in}">
#            $<$<NOT:$<CONFIG:Release>>:${CMAKE_COMMAND} -E echo "Only done in Release builds" >
#            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc
#            COMMENT "Generating API documentation with Doxygen"
#            VERBATIM
#    )
#endif()

if( BUILD_PACKAGE )
  set(CPACK_INSTALL_CMAKE_PROJECTS
    "${CMAKE_BINARY_DIR};amo_tools_suite;ALL;/"
  )

  include(cmake/Install.cmake)
endif()
