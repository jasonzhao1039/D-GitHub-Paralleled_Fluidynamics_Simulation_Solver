SOURCES = LidDrivenCavity.cpp LidDrivenCavitySolver.cpp SolverCG.cpp
OBJECTS = LidDrivenCavity.o LidDrivenCavitySolver.o SolverCG.o

testSOURCES = unittests.cpp SolverCG.cpp LidDrivenCavity.cpp
testOBJECTS = unittests.o SolverCG.o LidDrivenCavity.o

export OMP_NUM_THREADS = 2
export OMPI_CXX = g++-10 

checkVersion:
	mpicxx -v

solver: $(OBJECTS)
	mpicxx -fopenmp $(OBJECTS) -g -O0 -o solver -lblas -lboost_program_options

unittests: $(testOBJECTS)
	mpicxx -fopenmp $(testOBJECTS) -g -O0 -o unittests -lblas -lboost_unit_test_framework



LidDrivenCavitySolver.o: LidDrivenCavitySolver.cpp LidDrivenCavity.h
	mpicxx -fopenmp -g -O0 -c LidDrivenCavitySolver.cpp

SolverCG.o: SolverCG.cpp SolverCG.h
	mpicxx -fopenmp -g -O0 -c SolverCG.cpp

LidDrivenCavity.o: LidDrivenCavity.cpp LidDrivenCavity.h SolverCG.h
	mpicxx -fopenmp -g -O0 -c LidDrivenCavity.cpp

unittests.o: unittests.cpp SolverCG.h LidDrivenCavity.h
	mpicxx -fopenmp -g -O0 -c unittests.cpp



run: solver
	mpiexec -np 9 ./solver 

test: unittests
	mpiexec -np 16 ./unittests
	


doc: *.cpp
	doxygen Doxyfile



clean:
	rm -f $(OBJECTS) solver