# simulARM
very basic ARM visualizer using C++ and Verilog

## Dependencies
* Icarus Verilog
* GNU Assembler

## Installation

### Windows

#### Icarus Verilog
1. Download the installer from the [Icarus Verilog website](http://iverilog.icarus.com/).
2. Run the installer and follow the instructions.

#### GNU Assembler
GNU Assembler (as) is part of the GNU Binutils package. For Windows, it's typically provided through MinGW or Cygwin.
1. Download and install [MinGW](http://www.mingw.org/).
2. During the installation, make sure to select the `mingw32-base` package.
3. Add the MinGW bin directory to your PATH environment variable.

### Linux

#### Icarus Verilog
1. Open a terminal.
2. Update your package list: `sudo apt update`.
3. Install Icarus Verilog: `sudo apt install iverilog`.

#### GNU Assembler
GNU Assembler (as) is typically pre-installed on most Linux distributions. If not, it's part of the binutils package.
1. Open a terminal.
2. Update your package list: `sudo apt update`.
3. Install binutils: `sudo apt install binutils`.

## Build
