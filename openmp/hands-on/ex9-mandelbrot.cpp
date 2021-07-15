#include <iostream>
#include <fstream>
#include <iterator>
#include <complex>
#include <cmath>
#include <random>
#include <vector>
#include <array> // This is interesting...
#include <algorithm>

#include <unistd.h>
#define INFO_BUFFER_SIZE 1024

#include "defs.h"

using namespace std;

/*
Compile:
g++ -std=c++11 -Wall -Wextra ex9-mandelbrot.cpp -o exe -DNUM_POINTS=1000 -Wno-unknown-pragmas && ./exe
OpenMP Compile:
g++ -std=c++11 -Wall -Wextra -fopenmp ex9-mandelbrot.cpp -o exe -DNUM_POINTS=1000 && ./exe
*/

#define R_minX -2
#define R_maxX 1
#define R_minY -1
#define R_maxY 1
#ifndef NUM_POINTS
    #define NUM_POINTS 1000
#endif
#ifndef LIMIT
    #define LIMIT 100
#endif
#ifndef UPPER_BOUND
    #define UPPER_BOUND 5 // for the norm squared 
#endif

typedef double myprec;
#define Comp std::complex<myprec>

int Mandelbrot(Comp z0){
    // Comp z = z0;
    Comp z(0,0);
    for (int i = 0; i < LIMIT; ++i) {
        if (norm(z) > UPPER_BOUND)
            return i;
        z = z*z + z0;
    }
    return LIMIT;
}

int main(void){
    InfoOpenMP();

    int j,k;
    SimpleTimer _t;

    cout<<"Part 1: fill the Mandelbrot Matrix"<<endl;

    Comp z0(R_minX,R_minY);
    Comp dz_re((myprec)(R_maxX-R_minX)/NUM_POINTS,0);
    Comp dz_im(0,(myprec)(R_maxY-R_minY)/NUM_POINTS);
    z0 = z0+(dz_re+dz_im)*Comp(0.5,0);

    cout<<"z0="<<z0<<", |z0|^2="<<norm(z0)<<endl;
    cout<<"dz_re="<<dz_re<<", |dz_re|^2="<<norm(dz_re)<<endl;
    cout<<"dz_im="<<dz_im<<", |dz_im|^2="<<norm(dz_im)<<endl;

    int MandelMat[NUM_POINTS][NUM_POINTS];
    VectorMemUsage(NUM_POINTS*NUM_POINTS*sizeof(int),"MandelMat");

    _t.start("Compute mandelbrot region");
    for(j = 0; j < NUM_POINTS; j++)
        for(k = 0; k < NUM_POINTS; k++){
            MandelMat[j][k] = Mandelbrot(
                z0+Comp(j,0)*dz_re+Comp(k,0)*dz_im
            );
        }
    _t.stop(); _t.print();

    ofstream output_file("./mandelbrot.dat");
    for(j = 0; j < NUM_POINTS; j++){
        for(k = 0; k < NUM_POINTS; k++)
            output_file<<MandelMat[j][k]<<"  ";
        output_file<<"\n";
    }
    return 0;
}
