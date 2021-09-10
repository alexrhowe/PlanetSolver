USER GUIDE TO THE PLANETSOLVER STRUCTURE AND MASS LOSS CODE

PlanetSolver v0.5 beta
10 September 2021

Developers:
Alex R. Howe, NASA Goddard Space Flight Center
Wesley Verne, Princetion University Class of 2013

Included files
README.txt
maketar.sh
makefile
PlanetSolver source code
Equation of state tables
H-He pressure and entropy tables

CONTENTS
1. Description
2. Installation
3. Running
4. Example Commands
5. Options and Limitations
6. Output Format
7. Error Messages

1. DESCRIPTION

PlanetSolver is a planetary structure evolution code designed to compute bulk
planetary structures over time incorporating radiative cooling and mass loss
for a broad range of compositions. The code can compute mass-radius relations
and temperature-pressure-density profiles for a given composition and estimate
the radius and mass evolution of the planet in a given stellar environment.

Also include is the EqGen code to generate custom equation of state files.

NOTE: the options for EqGen are not fully documented. This document covers the
main use case of creating EOS's for mixed compositions.

PlanetSolver computes planetary structures by solving the structure equations
using a 4th-order Runge-Kutta method. A wide range of equation of state tables
are provided for these models.

Mass loss rates are computed for multiple contributing processes including
photoevaporation, stellar wind ablation, and core-powered mass loss. Cooling
incorporates Kelvin-Helmholtz cooling, radioactive decay, and simple radiative
cooling of the core. The evolution of the planet due to both cooling and mass
loss is integrated forward up to 14 Gyr to show the mass-radius relation over
time.

2. INSTALLATION

Download the repository, and compile the code with the included Makefile.

3. RUNNING

PlanetSolver is run from the command line with arguments describing the planet
to be modeled. These arguments may appear in any order expect for the ones
describing the planet's structure, as discussed below. The currently-supported
arguments are as follows:

-help/-h
Prints a summary of the command line options.

-writeto/-wr <output file>
This is the output file for the evolution calculation. Default: "Evolve.dat".

Arguments for the planetary structure. PlanetSolver builds a planet structured
in layers from the center outward, with the mass fractions for each layer
given as a decimal fraction of total mass. The "-c" or "-cn" argument must be
listed first, followed by any "-e" or "-en" arguments. The "-a" argument (if
used) must be last.

-central/-c     <central EOS file>   <layer mass fraction>
-centralnum/-cn <central EOS number> <layer mass fraction>
-eos/-e         <next EOS file>      <layer mass fraction>
-eosnum-en      <next EOS number>    <layer mass fraction>
-atmosphere     <entropy> <metallicity> <layer mass fraction>

The atmosphere is assumed to be a hydrogen-helium envelope, but may be omitted.
Only the convective region of the envelope is modeled. (A radiative upper
atmosphere component is in development.)

Entropy is given in units of Boltzmann's constant per baryon. Typical values
for mature planets are ~6.0, while young planets may be as high as ~10.0.
Allowed range: 4.0 to 17.0.

The metallicity is given in multiples of  Solar. Typical values for giant
planets are a little above 1, while mini-Neptunes may be in excess of 100.
Allowed range 0.0 to 100.0.

Any number of layers may be included, and a range of equation of state files
are available in the EOS_Tables subdirectory for specific layer compositions.
Available numbered tables are:

1. "Fe" (Iron, the standard core material)
2. "pv" (Perovskite, a major mantle constituent)
3. "MgO" (Periclase, the other major mantle constituent)
4. "SiC" (the expected mantle material for carbon-rich planets)
5. "Diamond" (included as a carbon-rich limit)
6. "Platinum" (included as a high-density limit)
7. "H2OVII" (Ice VII, the expected form of an ice mantle)

Negative numbers are used for testing purposes:
-1. White dwarf polytrope
-2. Platinum (alternate EOS)
-3. H2O VII (alternate EOS)
-4. Fe (alternate EOS)
-5. pv (alternate EOS)
-6. Fe (another alternate EOS)
-7. n=1 polytrope

NOTE: If an atmosphere is specified, the code currently computes the structure
based on the mass fraction of the atmosphere, while the mass fraction of the
last layer below the atmosphere is compute as 1 minus the other layers
regardless of the number given at the command line. This is also cause an error
if the mass fractions sum to more than 1.

Other arguments:

-pressure/-p <surface_pressure>
This sets a "surface pressure" in bars, defined as the pressure at the
radiative-convective boundary of the atmosphere. This will be important for the
eventual radiative atmosphere calculations. Default: 0.

-mass/-m  <total_mass>
Total mass of the planet in Earth Masses.

-star/-s <mass> <luminosity> <spectral_type> <orbital_distance>

The properties of the host star. Mass and luminosity are given in solar units,
and orbital distance is given in AU.

The available spectral types are:

1. Solar (recommended for F and G stars)
2. K-dwarfs
3. Early M (recommended for M1-M2)
4. Mid M (recommended for M3-M5)
5. Late M (recommended for M6-M9)

-integration/-i <step_size> <write_interval>

The step size is given in meters and applies to the integration over the
planet's radius. Default: 100.

The write interval indicates the level of detail in the structure output file.
For interval n, every nth data point will be written to the file. Default: 100.

-struct <output_file>
This sets the output file that contains the planet's structural profile. If
evolution is on, it will be the structure at the final timestep. Default:
"Planet.dat".

-evolve <mass_loss_flag>
Sets the evolution calculation flag to true. Default: false. Use 1 for mass
loss and 0 for no mass loss.

New EOS files may be created using the EqGen code. This is of particular
interest for mixed equations of state. This  This may be done with the following
command:
./EqGen -mix <eos_num1> <eos_num2> <eos1_mass_frac>

NOTE: not all mixtures will produce a working EOS file. This appears to be due
to a failure to converge in EOS::getPMix(). Always check new EOS files before
using them.

4. EXAMPLE COMMANDS

PlanetSolver command:

Mass: 5 M_Earth
Composition: 30% iron, 65% silicates, 5% hydrogen-helium
Entropy: 6.0 k_B/B
Metallicity: 10x solar
Star: solar-type
Orbital distance: 0.1 AU
Evolution: yes
Mass loss: yes

Command line:
./PlanetSolver -cn 1 0.3 -en 2 0.65 -a 6.0 10.0 0.05 -s 1.0 1.0 1 0.1 -m 5.0 -evolve 1

NOTE: For v0.5, running multiple models will require a separate script.

EqGen command:

First EOS: silicate
Second EOS: ice VII
silicate fraction: 40%

Command line:
./EqGen -mix 2 7 0.4

5. OPTIONS AND LIMITATIONS

In addition to the numbered tables, a variety of specific EOS tables are
provided. These include "+5" and "-5" tables, with densities 5% higher
or lower than the published EOS. There are also several tables for
undifferentiated planet compositions, and legacy tables for hydrogen-helium
envelopes with specific entropy values.

The limits on entropy and metallicity, are hard-coded to halt execution.
However, the code will sometimes continue to give usable results if they are
bypassed, and may occasionally fail within the allowed range.

The main problem for entropies outside these limits is that the equation of
state fails to converge. ("Envelope iteration limit exceeded.") However, it
may still converge well enough to continue execution, especially if the
convergence criteria are changed.

For high metallicities, the mean molecular weight is larger, and the
atmosphere must be hotter to follow the same pressure-density curve. Thus, the
entropy must be lower for the equation of state to converge. In testing, Z=300x
required s=2.0, and Z=1000x failed to converge at all.

In the mass loss model, the cooling rate is dependent on computed temperatures.
This often produces temperatures near 0 K in the uppermost layers of the
atmosphere. In order to avoid this problem and speed up calculation with fewer
iterative steps, a minimum temperature cutoff of 30 K is used.

6. OUTPUT FORMAT

The planet structure output file has five columns:

1. Radius in R_Earth.
2. Central pressure guess in Mbar.
3. Central pressure step size in Mbar.
4. Temperature in K.
5. Cumulative mass in M_Earth.

In addition, the final line of this output is printed to the command line,
giving a direct view of the computed mass and radius of the planet.

NOTE: The "core" temperature is set to a constant at the outer boundary of the
solid core. Accurate calculations of the internal temperature profile of the
solid core are not yet implemented. The temperature column should be correct
for the hydrogen-helium atmosphere.

The planet evolution file has a header describing the input parameters and
any issues with the model setup. After this, the tabular part of the file has
seven columns:

1. Age in Myr (automatic cutoff at 14000).
2. Atmosphere entropy in k_B/B.
3. Radius in R_Earth.
4. Mass in M_Earth.
5. Change in mass in M_Earth.
6. Atmosphere mass fraction.
7. Effective temperature in K.

7. ERROR MESSAGES

NOTE: These warnings do not necessarily mean that the code has failed. They may
indicate that the tolerances need to be relaxed or that a more sophisticated
step size control scheme is needed for higher precision. For many applications,
the results will be "good enough" despite the warnings. Comparison of results
with models that converged without warnings should be used to determine the
quality of results.

WARNINGS: indicate a potential problem with the model, but do not halt
execution. 

"Unrecognized argument."
This occurs in Main if one of the command line arguments is wrong. Depending
on the arguments given, this may or may not result in a crash. Prints to the
command line.

"Envelope iteration limit exceeded in temperature solution."
This occurs in HHeComp.cpp and indicates that the code was unable to converge
on a temperature solution. This does not affect the structure model calculation
directly, but may indicate a problem with the mass loss model. In testing, this
was caused by temperatures running too low in the upper atmosphere, so a cutoff
of 30 K was added. Prints to the command line.

"Planet integration failed."
This occurs in Planet.cpp and indicates that the code could not solve the
entire structure of the planet from center to surface. This can happen if the
atmosphere is "too fluffy," and the planet has an unphysically large radius.
Prints on the command line.

"Iteration limit exceeded for..."
This occurs in Main and indicates that the planet model failed to converge for
the input entropy. The code will automatically decrement the entropy by 0.1 and
try again. Prints to the output file.

"Roche lobe overflow encountered."
This occurs in Main if the planet's atmosphere heats up enough to overflow its
Roche lobe, perhaps due to high irradiation. Note that this result may be
physically justified.

ERRORS: serious problems that halt execution.

"Core fraction greater than 1."
"Envelope fraction too large."
This occurs in Main when mass fraction of the planet's layers add up to  >100%.
Print to the output file.

"Entropy out of range."
"Metallicity out of range."
Print to the output file.

"Error: negative pressure encountered."
This occurs in Planet.cpp and indicates something breaking in the structure
model.

"Initial model failed: low entropy."
This occurs in Main and indicates that no solution for the planet's structure
could be found. This will tend to occur for very low masses or high
metallicities. Prints to both the output file and the command line.

NORMAL END STATES:

"Atmosphere lost."
Normal end state for mass loss calculations if the mass loss rate is high
enough to strip the atmosphere within cosmis time.

CHANGE LOG:

v0.5
Implemented alternative mass loss prescriptions for K- and M-dwarfs. This adds
a new "startype" parameter to the "-star" flag.
Fixed Planet::printRecord() so that it still works with no atmosphere.
Added the EqGen program to generate new EOS files, including mixes, copied
from wemEG in the old code.
Added a "low entropy" alert on the command line.
Added spelled-out command line options.
Added "-help" command line option.
Added "-pressure" command line option for the "surface" pressure.
Added "unrecognized argument" check.
Added "-struct" output file argument.
Added "-integration" command line argument.
NOTE: "-rad" and "-verbose" options from the old code discarded.
Made "-struct" apply only to setting the output file name instead of
suppressing evolution.
Changed the "testout" variable to "evolOut" and removed the "append" setting.
Added a header to the evolution output file.
Updated the documentation.

v0.4
Implemented cooling and mass loss.
Probably some other stuff I should have kept track of.

v0.3
Vectorized all arrays.
EOS fails to converge if the entropy is s<5--COULD NOT REPRODUCE.
Changed the layer boundaries to instead inputting the mass fraction of each
layer (including of the core). (The old system was confusing.)
NOTE: the mass fraction for the top layer below the envelope is a dummy. The
model will automatically reset it to a total mass fraction of 1, although it
will return an error if the total mass fraction is greater than 1.
Set all step sizes, iteration cutoffs, required precisions, and deltas at the
top of their respective files.
Implemented temperature in PrintRecord and cleaned up the output.
Updated the documentation.

v0.2
Implemented the printRecord() subroutine.
Reduced the command line output to the final result.
NOTE: If convergence nominally fails, the code will still produce an output
that may be "close enough" to the correct values.
Implemented the setEntTab() subroutine and HHeComp class to handle arbitrary
entropy.
Tested the arbitrary entropy sections to make sure it was getting the EOS
tables right.
Added hard-coded limits on entropy and metallicity.
Clarified the warning messages.
Changed the output line to conform to Planet.dat.
Updated the documentation.

TO-DO LIST:

1. Better handling of layer fractions that do not sum to 1.
2. Run multiple models without needing a separate script.
3. Be more consistent with where and why the error messages appear.
4. Implement core temperature corrections.
5. Use a proper input file that is robust against missing arguments like in
   APOLLO.
6. Consider alternate integration limits than min_P and 1000 R_Earth.
7. Implement a radiative atmosphere.
8. Convert everything to cgs.
9. Investigate the convergence failure in EqGen.
10. Investigate the failure of temperature convergence.
11. Implement impact erosion.
12. Implement a Henyey solver.
