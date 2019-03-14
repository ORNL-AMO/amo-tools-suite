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
include(CheckCXXCompilerFlag)
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
        src/results/Results.cpp
        src/calculator/util/AnnualCost.cpp
        src/calculator/util/AnnualEnergy.cpp
        src/calculator/util/CurveFitVal.cpp
        src/calculator/motor/EstimateFLA.cpp
        src/calculator/motor/MotorCurrent.cpp
        src/calculator/motor/MotorEfficiency.cpp
        src/calculator/motor/MotorPower.cpp
        src/calculator/motor/MotorPowerFactor.cpp
        src/calculator/motor/MotorShaftPower.cpp
        src/calculator/pump/OptimalDeviationFactor.cpp
        src/calculator/motor/OptimalMotorPower.cpp
        src/calculator/motor/OptimalMotorShaftPower.cpp
        src/calculator/motor/OptimalMotorSize.cpp
        src/calculator/pump/OptimalPrePumpEff.cpp
        src/calculator/pump/OptimalPumpEfficiency.cpp
        src/calculator/pump/OptimalPumpShaftPower.cpp
        src/calculator/pump/OptimalSpecificSpeed.cpp
        src/calculator/pump/OptimalSpecificSpeedCorrection.cpp
        src/calculator/motor/Poles.cpp
        src/calculator/pump/MoverEfficiency.cpp
        src/calculator/pump/PumpShaftPower.cpp
        src/calculator/losses/Atmosphere.cpp
        src/calculator/losses/FixtureLosses.cpp
        src/calculator/losses/GasCoolingLosses.cpp
        src/calculator/losses/WaterCoolingLosses.cpp
        src/calculator/losses/LiquidCoolingLosses.cpp
        src/calculator/losses/LeakageLosses.cpp
        src/calculator/losses/WallLosses.cpp
        src/calculator/losses/OpeningLosses.cpp
        src/sqlite/SQLite.cpp
        src/calculator/losses/GasFlueGasMaterial.cpp
        src/calculator/pump/HeadTool.cpp
        src/calculator/losses/EnergyInputEAF.cpp
        src/calculator/losses/SlagOtherMaterialLosses.cpp
        src/calculator/losses/SolidLiquidFlueGasMaterial.cpp
        src/calculator/losses/AuxiliaryPower.cpp
        src/calculator/losses/ExhaustGasEAF.cpp
        src/calculator/furnace/EnergyEquivalency.cpp
        src/calculator/furnace/EfficiencyImprovement.cpp
        src/calculator/furnace/O2Enrichment.cpp
        src/calculator/furnace/FlowCalculationsEnergyUse.cpp
        src/calculator/furnace/HumidityRatio.cpp
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
        src/fans/FanCurve.cpp
        src/calculator/util/CompressedAir.cpp
        src/calculator/util/CHP.cpp
        src/calculator/motor/MotorData.cpp)

set(INCLUDE_FILES
        include/results/Results.h
        include/calculator/util/AnnualCost.h
        include/calculator/util/AnnualEnergy.h
        include/calculator/util/CurveFitVal.h
        include/calculator/motor/EstimateFLA.h
        include/calculator/pump/FluidPower.h
        include/calculator/motor/MotorCurrent.h
        include/calculator/motor/MotorEfficiency.h
        include/calculator/motor/MotorPower.h
        include/calculator/motor/MotorPowerFactor.h
        include/calculator/motor/MotorShaftPower.h
        include/calculator/pump/OptimalDeviationFactor.h
        include/calculator/motor/OptimalMotorPower.h
        include/calculator/motor/OptimalMotorShaftPower.h
        include/calculator/motor/OptimalMotorSize.h
        include/calculator/pump/OptimalPrePumpEff.h
        include/calculator/pump/OptimalPumpEfficiency.h
        include/calculator/pump/OptimalPumpShaftPower.h
        include/calculator/pump/OptimalSpecificSpeed.h
        include/calculator/pump/OptimalSpecificSpeedCorrection.h
        include/calculator/motor/Poles.h
        include/calculator/pump/MoverEfficiency.h
        include/calculator/pump/PumpShaftPower.h
        include/calculator/losses/LoadChargeMaterial.h
        include/calculator/losses/SolidLoadChargeMaterial.h
        include/calculator/losses/LiquidLoadChargeMaterial.h
        include/calculator/losses/GasLoadChargeMaterial.h
        include/calculator/losses/Atmosphere.h
        include/calculator/losses/GasCoolingLosses.h
        include/calculator/losses/FixtureLosses.h
        include/calculator/losses/WaterCoolingLosses.h
        include/calculator/losses/LiquidCoolingLosses.h
        include/calculator/losses/LeakageLosses.h
        include/calculator/losses/WallLosses.h
        include/calculator/losses/OpeningLosses.h
        include/sqlite/SQLite.h
        include/calculator/losses/GasFlueGasMaterial.h
        include/calculator/pump/HeadTool.h
        include/calculator/pump/HeadTool.h
        include/sqlite/SolidLoadChargeMaterialData.h
        include/sqlite/GasLoadChargeMaterialData.h
        include/sqlite/LiquidLoadChargeMaterialData.h
        include/sqlite/SolidLiquidFlueGasMaterialData.h
        include/sqlite/GasFlueGasMaterialData.h
        include/calculator/losses/EnergyInputEAF.h
        include/calculator/losses/SlagOtherMaterialLosses.h
        include/calculator/losses/SolidLiquidFlueGasMaterial.h
        include/calculator/losses/AuxiliaryPower.h
        include/calculator/losses/ExhaustGasEAF.h
        include/calculator/furnace/EnergyEquivalency.h
        include/calculator/furnace/EfficiencyImprovement.h
        include/calculator/furnace/O2Enrichment.h
        include/calculator/furnace/FlowCalculationsEnergyUse.h
        include/calculator/losses/EnergyInputExhaustGasLosses.h
        include/sqlite/AtmosphereSpecificHeatData.h
        include/sqlite/WallLossesSurfaceData.h
        include/calculator/furnace/HumidityRatio.h
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
        include/fans/Fan203.h
        include/fans/FanShaftPower.h
        include/fans/Planar.h
        include/ssmt/Turbine.h
        include/calculator/util/CHP.h
        include/fans/FanCurve.h
        include/calculator/util/CompressedAir.h
        include/calculator/motor/MotorData.h
        include/sqlite/MotorData.h
        include/fans/CompressibilityFactor.h
        include/results/InputData.h
        include/fans/FanEnergyIndex.h
        include/fans/OptimalFanEfficiency.h
        include/ssmt/HeatExchanger.h)

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
        tests/FlashTank.unit.cpp
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
        tests/OptimalFanEfficiency.unit.cpp tests/HeatExchanger.unit.cpp)

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
target_link_libraries( amo_tools_suite_main amo_tools_suite "--bind")
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
