## Building.

Stardust can be built with [CMake](https://cmake.org/).

First, clone the repository if you haven't done so already.  
* `git clone https://github.com/LucidSigma/stardust`.

Change to the build directory.  
* `cd build` (or `Set-Location -Path ".\build"` on PowerShell).

Build the project with CMake.  
* `cmake ..`

CMake options include:
* `STARDUST_BUILD_SANDBOX` - whether to build the Sandbox project or not (default: `OFF`).
* `STARDUST_BUILD_TESTS` - whether to build the unit tests or not (default: `OFF`).
* `STARDUST_BUILD_BENCHMARKS` - whether to build the benchmarks or not (default: `OFF`).

This will create a project file (Visual Studio, XCode, Makefile, et cetera). Open this project and build it.  
Building has only been tested on Windows. Some dependencies may not link correctly on MacOS/Linux. Please inform me of any issues regarding this.