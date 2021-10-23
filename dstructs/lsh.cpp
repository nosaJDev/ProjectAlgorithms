#include "lsh.hpp"
#include <cstdlib>
#include <cmath>
#include <ctime>

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


int LSH::h(int id, Vector * v){
    // This is a single hash function that will return an integer for
    // a vector. Argument id determines which one is used.

    // The function differenciates by seeding the rnJesus with the id
    srand(id);


    // Then, check if you already have a vector cached for this hfunction
    // Otherwise create it now, and do the same for the value
    Vector * randvec;
    double t;
    if (hashvectors[id] == nullptr){

        randvec = new Vector(v->getSize(),nullptr);
        for(int i = 0; i < randvec->getSize()/2; i++){
            // Fetch two random coordinates
            double c1, c2;
            boxMulerNormal(&c1, &c2);
            randvec->setCoord(2*i,c1);
            randvec->setCoord(2*i+1,c2);
        }

        // Check if one coord is missing
        if (randvec->getSize() % 2){
            // Fetch this one as well
            double c;
            boxMulerNormal(&c,&c);
            randvec->setCoord(randvec->getSize()-1,c);
        }

        // Also generate the value for t
        t = w*rand()/RAND_MAX;

        // Save both values to the cache
        hashvectors[id] = randvec;
        hasht[id] = t;

    }else{

        // If the values exist on the cache, just grab em
        randvec = hashvectors[id];
        t = hasht[id];

    }

    // Finally produce the key using the known formula
    int key = (int) floor(1.0*(randvec->dotProduct(v)+t)/w);

    // Return the key you produced
    return key;

}

LSH::LSH(int _L, int _k,int _w, int buckets){

    // This will initialize the LSH, setting parameters and initiating data structures

    // Set the default parameters
    L = _L;
    k = _k;
    w = _w;
    numBuckets = buckets;

    // Set the random offset
    srand(time(nullptr));
    rand_offset = rand() % 100000;

    // Create the hash tables
    tables = new HashTable*[L];
    for(int i = 0; i < L; i++){
        tables[i] = new HashTable(numBuckets);
    }

    // Choose which hash functions will accomodate each level
    hash_ids = new int*[L];
    for(int i = 0; i < L; i++){
        // Create the array of hash ids
        hash_ids[i] = new int[k];
        
        // Populate with random values
        for(int j = 0; j < k; j++){
            // Using 50 different functions
            hash_ids[i][j] = rand() % maxhashes;
        }
    }

    // Initialize the cached values for the hashes
    hashvectors = new Vector*[maxhashes];
    hasht = new double[maxhashes];
    for(int i = 0; i < maxhashes; i++){
        hashvectors[i] = nullptr;
    }


}

LSH::~LSH(){

    // Delete the hash tables
    for(int i = 0; i < L; i++){
        delete tables[i];
    }
    delete[] tables;

    // Delete the hash ids table
    for(int i = 0; i < L; i++){
        delete[] hash_ids[i];
    }
    delete[] hash_ids;

    // Delete the cached values for vectors and t.
    for(int i = 0; i < maxhashes; i++){
        if(hashvectors[i] != nullptr)
            delete hashvectors[i];
    }
    delete[] hashvectors;
    delete[] hasht;

}

void LSH::placeVectorToBucket(int level, Vector * v){

    // This will generate the final key for the vector on the specified level,
    // and will place the vector to the bucket accordingly

    // First generate random numbers using the offset and the level
    srand(rand_offset+level);
    int * rands = new int[k];
    for(int i = 0; i < k; i++){
        rands[i] = -1000+rand()%2001;
    }

    // Then calculate the M value (nineth fibonacci prime)
    int M = 514229;

    // Finally add the hash values together
    int res = 0;
    for(int i = 0; i < k; i++){
        res += (h(hash_ids[level][i],v)*rands[i])  % M;
    }
    
    // Produce the key and insert the vector to the hash table
    int finalkey = res % M;
    tables[level]->add((void*)v,finalkey);


}


void LSH::addVectorList(List * vectorlist){

    // This will take a list of vectors and will put each one at the various
    // hash tables at each level

    // For each level
    for (int l = 0; l < L; l++){
        // Put the vectors on the hash table
        for(int i = 0; i < vectorlist->getElems(); i++){
            placeVectorToBucket(l,(Vector*)vectorlist->get(i));
        }
    }

}