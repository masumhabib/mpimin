MPIMIN
========
A minimalistic project that demonstrates the usage of
[Boost.MPI](http://www.boost.org/doc/libs/1_56_0/doc/html/mpi.html) and cmake.

Usage
======
The configure script finds the Boost.MPI library, compiles the 
program and links it automatically: 
```bash
./build config # configures project using cmake
./build # compile and link
./build install # install
```
The executable file will be in the bin folder.
