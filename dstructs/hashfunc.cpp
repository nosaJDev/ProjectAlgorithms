#include "hashfunc.hpp"
#include <cstdlib>
#include <cmath>

double boxMulerNormal(double * n1, double * n2){
    // This helper function will generate two variables following
    // the standard normal distribution by using the box-muller method

    // First, generate the two uniform distributed variables
    double u1 = 1.0*rand()/RAND_MAX;
    double u2 = 1.0*rand()/RAND_MAX;

    // Then, calculate the normal variables
    double common = -2*sqrt(log(u1));
    *n1 = common*cos(2*M_PI*u2);
    *n2 = common*sin(2*M_PI*u2);

}


LocalityHashFamily::LocalityHashFamily(int number, int dim, int _w){

    // This will construct all the vectors and random values used
    // in the hash functions. You need to provide the number of different
    // hash functions that you are going to use.

    // Each function is associated with its id, ashending from and including zero.

    // Set the number of hash functions and window
    numFunctions = number;
    w = _w;

    // Initialize the vector array
    vectors = new Vector*[number];
    for(int i = 0; i < number; i++){

        // Initialize the vector
        vectors[i] = new Vector(dim,nullptr);

        // Fill the vector with random coords (following standard normal distrib.)
        for(int j = 0; j < vectors[i]->getSize()/2; j++){
            // Fetch two random coordinates
            double c1, c2;
            boxMulerNormal(&c1, &c2);
            vectors[i]->setCoord(2*j,c1);
            vectors[i]->setCoord(2*j+1,c2);
        }

        // Check if one coord is missing
        if (vectors[i]->getSize() % 2){
            // Fetch this one as well
            double c;
            boxMulerNormal(&c,&c);
            vectors[i]->setCoord(vectors[i]->getSize()-1,c);
        }

    }

    // Initialize the random t values
    tvalues = new double[number];
    for(int i = 0; i < number; i++){
        tvalues[i] = 1.0*rand()/RAND_MAX;
    }

    // Done

}

LocalityHashFamily::~LocalityHashFamily(){

    // Delete all the vectors
    for(int i = 0; i < numFunctions; i++){
        delete vectors[i];
    }
    delete[] vectors;

    // Delete the random t values
    delete[] tvalues;

}

int LocalityHashFamily::hash(int hash_id, Vector * v){

    // This will perform the hashing of the vector provided
    // hash_id will determine which parameters are used

    return floor(1.0*(vectors[hash_id]->dotProduct(v)+tvalues[hash_id])/w);

}