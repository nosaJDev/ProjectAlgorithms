#include "lsh.hpp"
#include <cstdlib>
#include <cmath>
#include <ctime>
#include "pqueue.hpp"

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

LSH::LSH(int _L, int _k,int _w, int buckets, Metric * _met){

    // This will initialize the LSH, setting parameters and initiating data structures

    // Set the default parameters
    L = _L;
    k = _k;
    w = _w;
    numBuckets = buckets;
    metric = _met;

    // Reset the inserted vectors counter
    elems = 0;

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

int LSH::getVectorMasterKey(int level, Vector * v){

    // This will generate the final key for the vector on the specified level

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
    
    // Produce the key and return it
    return res % M;


}

void LSH::placeVectorToBucket(int level, Vector * v){

    // This will place a vector inside the appropriate buckets for the specified
    // level. It's separated from key generation because we don't always want to
    // insert to buckets when generating keys.


    // Generate the key and put to bucket
    int finalkey = getVectorMasterKey(level, v);
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

Vector ** LSH::approximatekNN(int kappa, Vector * q){

    // This will look up the buckets, and return the kappa nearest neigbors

    // First create a priority queue to hold the potentials
    PriorityQueue * queue = new PriorityQueue(kappa);

    // Then, search the correct bucket for each level
    for(int l = 0; l < L; l++){
        
        // Find the master key for vector q on that level
        int master_key = getVectorMasterKey(l, q);

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
            double dist = metric->dist(v,q);

            // Insert into the priority queue
            // dist is the priority (less is greater priority)
            queue->add((void *) v, dist);

        }

    }

    // After you are done, the queue will contain at most kappa instances
    // Copy them back on a new array
    Vector ** ret_vecs = new Vector*[queue->getElems()];
    QueueElement ** queue_elems = queue->getArray();
    for(int i = 0; i < queue->getElems();i++){
        ret_vecs[i] = (Vector*) queue_elems[i]->data;
    }

    // Delete the priority queue (no longer needed)
    delete queue;

    // Return the retrieved vectors
    return ret_vecs;

}


Vector ** LSH::approximateRange(double radius, Vector * q){

    // This will look up the buckets, and return all the vectors at a range

    // First create a priority queue to hold the potentials
    // It must be able to hold as many elements as there are
    // This may change to put a limit and save ourselves
    PriorityQueue * queue = new PriorityQueue(elems);

    // Then, search the correct bucket for each level
    for(int l = 0; l < L; l++){
        
        // Find the master key for vector q on that level
        int master_key = getVectorMasterKey(l, q);

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
            double dist = metric->dist(v,q);

            // If the distance is within the radius insert at the priority queue
            // dist is the priority (less is greater priority)
            if (dist <= radius)
                queue->add((void *) v, dist);

        }

    }

    // After you are done, the queue will contain at most kappa instances
    // Copy them back on a new array
    Vector ** ret_vecs = new Vector*[queue->getElems()];
    QueueElement ** queue_elems = queue->getArray();
    for(int i = 0; i < queue->getElems();i++){
        ret_vecs[i] = (Vector*) queue_elems[i]->data;
    }

    // Delete the priority queue (no longer needed)
    delete queue;

    // Return the retrieved vectors
    return ret_vecs;

}