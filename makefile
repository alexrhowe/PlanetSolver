# ---------------------------------------------------------------------
# PlanetSolver makefile
# Author: Alex Howe
# ---------------------------------------------------------------------

# Macros
SRC = src

# Variables

executables = PlanetSolver EqGen
library = src/stdafx.h src/files.h src/constants.h src/constants.cpp src/EOS.h src/EOS.cpp src/Planet.h src/Planet.cpp src/HHeComp.h src/HHeComp.cpp src/EqGen.cpp

# Non File Targets

all: $(executables)

clean: 
	rm -f $(executables)

# File Targets

EqGen: src/EqGen.cpp $(library)
	g++ -O3 src/EqGen.cpp src/constants.cpp src/EOS.cpp src/HHeComp.cpp -o EqGen

PlanetSolver: src/PlanetSolver.cpp $(library)
	g++ -O3 src/PlanetSolver.cpp src/constants.cpp src/EOS.cpp src/Planet.cpp src/HHeComp.cpp -o PlanetSolver
