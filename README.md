# UnrealEngine-FMIPlugin

![FMUinUE4](docs/fmuUEBP.PNG)

- A MVP (minimally viable product) of simulating FMUs within the Unreal Engine environment.
- This MVP relies on the [FMIKit repositorty](https://github.com/CATIA-Systems/FMIKit-Simulink) (non-simulink portion)
- Special thanks to Torsten Sommer for his work on FMIKit!

# Prerequistes

This work was tested using the following. It may work on something else but no gurantees.
- Windows 10
- Unreal Engine 4.25
- Visual Studio 2019

FMU
- The included example FMU, `test.fmu`, and is a LorenzSystem model. The Modelica file is included in the `Modelica` folder.
- This FMU is generated using Dymola 2021 with Binary Model Export License (i.e., license free FMU)

# Regarding `A_FMU`

In this current version, the `A_FMU` under `FMIKit C++ Classes/FMIKit/Public` contains the magic to make the FMU run. Users are highly encouraged to look at `A_FMU.cpp` if they need to understand more intimately the implementation.
- `mResults` returns the results requested by the user `mStoredVariables`.
  - `mResults` only returns values when `mAutoSimulateTick` = True. Else it is empty and variables can be retrieved using the `GetReal()` function.
- `mModelVariables` are the names of all the variables found in the model.

# Installation

Follow the following steps to get this project up and running on your own computer.

- Download and unzip the folder into your desired location **OR** clone the repository.
- Open the project `FMIPlugin.uproject` by double-clicking the file.
- When prompte, rebuild the project click `Yes`.
  - The project will build and then launch Unreal Engine.

# Test Installation

These examples use the `test.fmu` included in the repo.

- `Level_0`
  - This level provides example blueprints (`BP_FMU`) demonstrating the automatic and manual options for simulating an FMU.
  - `BP_FMU` implements the `A_FMU` class
- `Level_1`
  - Simple use of  `A_FMU` and printing a variable to the screen via the level blueprint.
  - [Short Demo video](https://youtu.be/_S_oY2bdlZM)

# Known Issues

- Currently only floats and booleans (i.e., 0/1) are supported variables in `A_FMU`.
