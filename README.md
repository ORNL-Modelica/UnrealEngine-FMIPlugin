# UnrealEngine-FMIPlugin

![FMUinUE4](docs/fmuUEBP.PNG)

- A MVP (minimally viable product) of simulating FMUs within the Unreal Engine environment.
- This MVP relies on the [FMIKit repository] (https://github.com/CATIA-Systems/FMIKit-Simulink) (non-simulink portion)
- Special thanks to Torsten Sommer for his work on FMIKit!
- [Short Demo video] (https://youtu.be/r3NeJKJt4Z8)
  
# Prerequisites

This work was tested using the following. It may work on something else but no guarantees.
- Windows 10
- Unreal Engine 4.25+
- Visual Studio 2019
- **An unzip utility on path, currently supported are unzip, 7z, tar**

FMU
- The included example FMU, `test.fmu`, and is a [Lorenz System model] (https://en.wikipedia.org/wiki/Lorenz_system).
  - The Modelica file is included in the `Modelica` folder.
- This FMU is generated using Dymola 2021 with Binary Model Export License (i.e., license free FMU)

# Regarding `A_FMU`

In this current version, the `A_FMU` under `FMIKit C++ Classes/FMIKit/Public` contains the magic to make the FMU run. Users are highly encouraged to look at `A_FMU.cpp` if they need to understand more intimately the implementation.
- `mResults` returns the results requested by the user `mStoredVariables`.
  - `mResults` only returns values when `mAutoSimulateTick` = True. Else it is empty and variables can be retrieved using the `GetReal()` function.
- `mModelVariables` are the names of all the variables found in the model.

# Installation

Follow the following steps to get this project up and running on your own computer. This project uses symbolic links (Method 2) but those can be tricky. So if you are not familiar with them it is recommended to use Method 1.

**Method 1**
1. Clone the repositoy with FMIKit submodule
   - `git clone https://github.com/ORNL-Modelica/UnrealEngine-FMIPlugin.git`
   - `cd` to repo
   - `git submodule init`
   - `git submodule update`
1. Fix the symbolic links
   - From `UnrealEngine-FMIPlugin/FMIPlugin/Plugins/FMIKit/ThirdParty/fmikit/src` copy the files `FMU.cpp`, `FMU1.cpp`, and `FMU2.cpp` and replace the same names files in `UnrealEngine-FMIPlugin/FMIPlugin/Plugins/FMIKit/Source/FMIKit/ThirdParty/fmikit/src`
1. Launch the `FMIPlugin.uproject` rebuilding the project when prompted.
1. Have fun!
  
**Method 2**
1. Clone the repository
  - "git submodule update --init"
  - FMU[1/2].cpp are symlinks 
    - copy the submodule files over the links (text files) **OR**
    - it may be necessary to turn symlinks = true in .git/config
    - it may be necessary to have installed git with enable symlinks.
    - it may be necessary to clone or run "git reset HEAD --hard" as administrator 
- Open the project `FMIPlugin.uproject` by double-clicking the file.
- When prompted, rebuild the project click `Yes`.
  - The project will build and then launch Unreal Engine.

# Test Installation

These examples use the `test.fmu` included in the repo.

> For both levels, the user must replace the `M Path` variable of the actor component in the details panel with the path on your system to the `test.fmu` located at the root of the repo.
> 
- `Level_0`
  - This level provides example blueprints (`BP_FMU`) demonstrating the automatic and manual options for simulating an FMU.
  - `BP_FMU` implements the `A_FMU` class
- `Level_1`
  - Simple use of  `A_FMU` and printing a variable to the screen via the level blueprint.

# Known Issues

- Currently only floats and booleans (i.e., 0/1) are supported variables in `A_FMU`.

# License

Copyright 2019 UT-Battelle. The code is released under the [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0).