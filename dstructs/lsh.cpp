#include "lsh.hpp"
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "pqueue.hpp"
#include <cstdio>

LSH::LSH(int _L, int _k,int _w, int buckets, int dim){

    // This will initialize the LSH, setting parameters and initiating data structures

    // Set the default parameters
    L = _L;
    k = _k;
    w = _w;
    numBuckets = buckets;
    dimension = dim;

    // Reset the inserted vectors counter
    elems = 0;

    // Set the randoms
    rands = new int[k];
    for(int i =0; i < k; i++) rands[i] = (1+rand()%3)*(-1+2*(rand()%2));

    // Create the hash tables
    tables = new HashTable*[L];
    for(int i = 0; i < L; i++){
        tables[i] = new HashTable(numBuckets);
    }

    // Initiate the hash function generator
    int maxhashes = 100;
    hashfunc = new LocalityHashFamily(maxhashes, dimension, w);

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

    // Delete the hash function generator
    delete hashfunc;

    // Delete the randoms
    delete[] rands;

}

long long LSH::getVectorMasterKey(int level, Vector * v){

    // This will generate the final key for the vector on the specified level

    // Calculate the M value (2^32-5)
    long long M = 4294967291;

    // Finally add the hash values together
    long long res = 0;
    for(int i = 0; i < k; i++){
        res = ((res % M)+((hashfunc->hash(hash_ids[level][i],v)*rands[i])%M))%M;
    }
    
    // Produce the key and return it
    return res;


}

void LSH::placeVectorToBucket(int level, Vector * v){

    // This will place a vector inside the appropriate buckets for the specified
    // level. It's separated from key generation because we don't always want to
    // insert to buckets when generating keys.


    // Generate the key and put to bucket
    long long finalkey = getVectorMasterKey(level, v);
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

    // Increment the inserted vectors counter
    elems += vectorlist->getElems();

}

Vector * LSH::approximateNN( Vector * q, Metric * metric){

    // This will look up the buckets, and return the kappa nearest neigbors

    // First create the variables that will hold the nn
    Vector * nn = nullptr;
    float mindist = 999999;


    // Then, search the correct bucket for each level
    for(int l = 0; l < L; l++){
        
        // Find the master key for vector q on that level
        long long master_key = getVectorMasterKey(l, q);

        // Then retrieve the bucket list for that key
        List * bucketlist = tables[l]->getChain(master_key);

        // Loop through the vectors and choose the ones with correct locality
        for(int i = 0; i < bucketlist->getElems(); i++){

            // Skip the ones with different keys
            HashElement * elem = (HashElement*)bucketlist->get(i);
            if(elem->key != master_key)
                continue;

            // Retrieve the vector and find the distance with the metric
            Vector * v = (Vector *) elem->data;
            float d = metric->dist(v,q);

            // Check if you need to replace the NN
            if ( mindist > d){
                mindist = d;
                nn = (Vector *) elem->data;
            }
        }

    }

    // Return the NN you found
    return nn;


}



PriorityQueue * LSH::approximatekNN(Vector * q, Metric * metric){

    // This will look up the buckets, and return the kappa nearest neigbors

    // First create a priority queue to hold the potentials
    PriorityQueue * queue = new PriorityQueue(10);

    // Then, search the correct bucket for each level
    for(int l = 0; l < L; l++){
        
        // Find the master key for vector q on that level
        long long  master_key = getVectorMasterKey(l, q);

        // Then retrieve the bucket list for that key
        List * bucketlist = tables[l]->getChain(master_key);

        // Loop through the vectors and choose the ones with correct locality
        for(int i = 0; i < bucketlist->getElems(); i++){

            // Skip the ones with different keys
            HashElement * elem = (HashElement*)bucketlist->get(i);
            if(elem->key != master_key)
                continue;

            // Retrieve the vector and find the distance with the metric
            Vector * v = (Vector *) elem->data;
            float dist = metric->dist(v,q);

            // Insert into the priority queue
            // dist is the priority (less is greater priority)
            queue->add((void *) v, dist);

        }

    }

    // Return the queue with all the elements
    // This must be deleted outside this function
    return queue;


}

PriorityQueue * LSH::approximateRange(float radius, Vector * q, Metric * metric){

    // This will look up the buckets, and return all the vectors at a range

    // First create a priority queue to hold the potentials
    // It must be able to hold as many elements as there are
    // This may change to put a limit and save ourselves
    PriorityQueue * queue = new PriorityQueue(30);

    // Then, search the correct bucket for each level
    for(int l = 0; l < L; l++){
        
        // Find the master key for vector q on that level
        long long master_key = getVectorMasterKey(l, q);

        // Then retrieve the bucket list for that key
        List * bucketlist = tables[l]->getChain(master_key);

        // Loop through the vectors and choose the ones with correct locality
        for(int i = 0; i < bucketlist->getElems(); i++){

            // Skip the ones with different keys
            HashElement * elem = (HashElement*)bucketlist->get(i);
            if(elem->key != master_key)
                continue;

            // Retrieve the vector
            Vector * v = (Vector *) elem->data;

            // Prevent clustered vectors from participating
            if ( v->getCluster() != -1)
                continue;

            // Calculate the distance
            float dist = metric->dist(v,q);

            // If the distance is within the radius insert at the priority queue
            // dist is the priority (less is greater priority)
            if (dist <= radius)
                queue->add((void *) v, dist);

        }

    }

    // Return the queue with all the elements
    // This must be deleted outside this function
    return queue;

}