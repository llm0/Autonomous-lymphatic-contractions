st:Evolute.cpp Common.cpp Lattice.cpp Vector.cpp kernel.cu AnimateData.cpp Boundary.cpp BoundaryLattice.cpp CUDA_Transfer.cpp LatticeAnimate.cpp Line.cpp Link.cpp LymphAnimate.cpp Membrane.cpp MemValve.cpp PermeatAnimate.cpp PermeatLine.cpp Petal.cpp PetalAnimate.cpp Point.cpp Segment.cpp SolftValve.cpp ValveAnimate.cpp ValveSegment.cpp VectorAnimate.cpp LymphaticVessle.cpp
	nvcc -c Common.cpp
	nvcc -c Lattice.cpp
	nvcc -c Vector.cpp
	nvcc -c kernel.cu
	nvcc -c AnimateData.cpp
	nvcc -c Boundary.cpp
	nvcc -c BoundaryLattice.cpp
	nvcc -c CUDA_Transfer.cpp
	nvcc -c LatticeAnimate.cpp
	nvcc -c Line.cpp
	nvcc -c Link.cpp
	nvcc -c LymphAnimate.cpp
	nvcc -c Membrane.cpp
	nvcc -c MemValve.cpp
	nvcc -c PermeatAnimate.cpp
	nvcc -c PermeatLine.cpp
	nvcc -c Petal.cpp
	nvcc -c PetalAnimate.cpp
	nvcc -c Point.cpp
	nvcc -c Segment.cpp
	nvcc -c SolftValve.cpp
	nvcc -c ValveAnimate.cpp
	nvcc -c ValveSegment.cpp
	nvcc -c VectorAnimate.cpp
	nvcc -c LymphaticVessle.cpp
	nvcc -c MPIComm.cpp 
	nvcc -c Evolute.cpp
	nvcc -O3 -o lymph.o Evolute.o Common.o Lattice.o Vector.o kernel.o AnimateData.o Boundary.o BoundaryLattice.o CUDA_Transfer.o LatticeAnimate.o Line.o Link.o LymphAnimate.o Membrane.o MemValve.o PermeatAnimate.o PermeatLine.o Petal.o PetalAnimate.o Point.o Segment.o SolftValve.o ValveAnimate.o ValveSegment.o VectorAnimate.o LymphaticVessle.o MPIComm.o /opt/mpich-3.2.1/lib/libmpi.a /opt/mpich-3.2.1/lib/libmpicxx.a /opt/mpich-3.2.1/lib/libmpifort.a /opt/zlib-1.2.11/lib/libz.a -lm

