Slicer-CleaverExtension
=======================

This extension packages Cleaver, a multimaterial tetrahedral meshing library.

Cleaver is a C++ library for generating guaranteed-quality, tetrahedral meshes
of multimaterial volumetric data. It is based on the 'Lattice Cleaving' algor-
ithm:

Bronson J., Levine, J., Whitaker R., "Lattice Cleaving: Conforming Tetrahedral
Meshes of Multimaterial Domains with Bounded Quality." Proceedings of the 21st
International Meshing Roundtable (San Jose, CA, Oct 7-10, 2012)

The method is theoretically guaranteed to produce valid meshes with bounded
dihedral angles, while still conforming to multimaterial surfaces. Empirically
these bounds have been shown to be significant.


Acknowledgement: 
Cleaver is an Open Source software project that is principally funded through
the SCI Institute's NIH/NIGMS CIBC Center. Please use the following acknowledg-
ment and send us references to any publications, presentations, or successful
funding applications that make use of NIH/NIGMS CIBC software or data sets.

"This project was supported by grants from the National Center for Research
Resources (5P41RR012553-15) and the National Institute of General Medical 
Sciences (8 P41 GM103545-15) from the National Institutes of Health.

Dependencies
=======================

There a number of dependencies required.

1. Cmake  (www.cmake.org)
2. Git    (git-scm.com)
3. Slicer (www.slicer.org)
4. tclap, ModuleDescriptionParser, GenerateCLP :

	Check out the required directories from the Slicer3 repository.

	svn co http://svn.slicer.org/Slicer3/trunk/Libs/SlicerExecutionModel/tclap
	svn co http://svn.slicer.org/Slicer3/trunk/Libs/SlicerExecutionModel/ModuleDescriptionParser
	svn co http://svn.slicer.org/Slicer3/trunk/Libs/SlicerExecutionModel/GenerateCLP

	Run cmake on tclap
	Run cmake on ModuleDescriptionParser
	make ModuleDescriptionParser
	Run cmake on GenerateCLP
	make GenerateCLP

Building
=======================

Once the dependencies are obtained, you can build the module.

1. mkdir build #in the Slicer-CleaverExtension directory
2. cd build
3. cmake ..
4. ccmake .. #in here, set the directory for your slicer build.
5. make

Running
=======================

Inside of Slicer, you can edit the program properties:

1. Edit->Application Settings->Modules
2. In "Additional module paths", add the directory where the Cleaver Module was built (../Slicer-CleaverExtension/build/CleaverCLI)
3. Click OK and restart Slicer.
4. Search for "Cleaver" in the Modules. The module should appear.
5. Load NRRD data and choose desired volumes.
6. In Output Model 1-10, create as many models as you desire. The program will automatically load the geometry for each model you specify. If more are generated than specified, the program will notify you (in the terminal) where the extra models are saved.
7. Set additional settings and click "Apply" at the bottom.
8. When the program is done, you can manually load the extra models into the Slicer Scenewith the ones automatically loaded.
