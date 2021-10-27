#include "hcubemap.hpp"
#include <cstdlib>

int f(int func_id, int value){
    // This is an implementation of the family of f functions
    // guarranteeing uniform zero/ones on each bucket, while
    // also staying the same when given the same id

    // Seed the rand with the number plus the id
    srand(func_id+value);
    
    // Generate and return the random bit
    return rand() % 2;

}


int * inclusiveHamming(int number, int bits, int dist, HashTable * ht = nullptr){

    // This function takes a number, and the number of viable bits,
    // and returns an array of numbers with less than or equal hamming
    // distance to it.
    // This is done through recursion
    // The elements are saved in a hashtable until returning to the first call


    //If the hashtable is a nullptr, this signifies the first call
    bool firstcall = ht == nullptr;
    if(firstcall){
        // Create the hash table
        ht = new HashTable(50);
    }

    if (dist <= 0){
        // If the distance is zero, only this number applies
        // Insert it to the hash table
        ht->add(nullptr,number);

    }else{
        // If the distance is more, find all the numbers with distance one,
        // and recursively fill the other distances
        
        // Find the numbers with distance == 1, by xoring and bitshifts
        int at = 1;
        for(int i = 0; i < bits; i++){

            // Find the number
            int num = number ^ at;

            // Insert it at the hash table
            ht->add(nullptr,num);

            // Recursively call the function for lower distances
            inclusiveHamming(num,bits,dist-1,ht);

            // Increment the at variable
            at = at << 1;
        }

        // Finally add the initial number to the hash table
        ht->add(nullptr,number);

    }

    if(firstcall){
        // If this is the first call, find all the elements on the table, and
        // return them in the form of the array stated above

        // Create the array to hold the elements
        int * ret = new int[ht->getElems()+1];

        // Find the contents of the hash table and add them to the array
        ret[0] = ht->getElems(); // This represents the number of elements
        int at = 1;
        List ** buckets = ht->getAllChains();
        for(int i = 0; i < ht->getSize(); i++){
            List * bucket = buckets[i];
            for(int j = 0; j < bucket->getElems(); j++){
                HashElement * he = (HashElement*)bucket->get(j);
                ret[at++] = he->key;
            }
        }

        // Delete the hash table and return the result
        delete ht;
        return ret;


    }else{
        // If it's not, the return value doesn't matter
        return nullptr;

    }


}


HypercubeMapping::HypercubeMapping(int cdims, int _w, int dims){

    // This will initiate all the parameters and the function generator
    // First initiate all the parameters
    cubedims = cdims;
    w = _w;
    dimension = dims;

    // Then initialize the hash function generation
    // We need as many functions as there are cube dimensions
    hashfunc = new LocalityHashFamily(cubedims,dims,w);

    // Finally initialize the hash table
    int tablesize = 1 << cubedims; //This may be overkill, we'll see
    table = new HashTable(tablesize);

}

HypercubeMapping::~HypercubeMapping(){

    // Delete the hashtable and the hash generator
    delete table;
    delete hashfunc;

}

int HypercubeMapping::findVectorCubeKey(Vector * v){
    // This will calculate and produce the key that is used to
    // store the vector into the hypercube
    // TODO: define better f(.)

    // Generate the key
    int key = 0;
    for(int i = 0; i < cubedims; i++){

        // Find the value for the i-th hash
        int i_value = hashfunc->hash(i,v);

        // Convert it to a bit using the f function
        int i_bit = f(i,i_value);

        // Add it to the key and bitswift
        key = (key << 2)+i_bit;

    }

    // Return the key you generated
    return key;


}

void HypercubeMapping::mapVectorToCube(Vector * v){

    // This will use the above function to get the key
    // and then proceed to store the vector in the appropriate
    // bucket on the table.

    // Insert the vector to the table
    table->add((void *) v,findVectorCubeKey(v));

}

void HypercubeMapping::addVectorList(List * vlist){

    // This will add all the vectors on the list to the table,
    // using the internal fuction for that purpose

    for(int i = 0; i < vlist->getElems(); i++){
        mapVectorToCube((Vector *)vlist->get(i));
    }

}

PriorityQueue * HypercubeMapping::approximatekNN(int kappa, Vector * q, Metric * metric, int hamming = 0){

    // First of all find the key of the vector to check
    int qkey = findVectorCubeKey(q);

    // Then retrieve the list of buckets to check according to hamming distance
    int * bucks = inclusiveHamming(qkey,cubedims, hamming);

    // After that create a priority queue to hold the nearest neighbors
    PriorityQueue * queue = new PriorityQueue(kappa);

    // Finally loop through those buckets to gather the nearest neighbors
    for(int i = 1; i <= bucks[0]; i++){

        // Retrieve the bucket we are searching in
        List * bucket = table->getChain(bucks[i]);

        // Loop through all the bucket elements
        for(int j = 0; j < bucket->getElems(); j++){

            // Find the element
            HashElement * he = (HashElement *) bucket->get(j);

            // Add it to the queue with the correct priority
            // No need to check locality since all elements on
            // the bucket have the same key
            queue->add((void*)he->data, metric->dist(q,(Vector *)he->data));
        }

    }

    // After you are done, return the priority queue with the elements
    return queue;

}


PriorityQueue * HypercubeMapping::approximateRange(double radius, Vector * q, Metric * metric, int hamming = 0){

    // First of all find the key of the vector to check
    int qkey = findVectorCubeKey(q);

    // Then retrieve the list of buckets to check according to hamming distance
    int * bucks = inclusiveHamming(qkey,cubedims, hamming);

    // After that create a priority queue to hold the nearest neighbors
    PriorityQueue * queue = new PriorityQueue(20);

    // Finally loop through those buckets to gather the nearest neighbors
    for(int i = 1; i <= bucks[0]; i++){

        // Retrieve the bucket we are searching in
        List * bucket = table->getChain(bucks[i]);

        // Loop through all the bucket elements
        for(int j = 0; j < bucket->getElems(); j++){

            // Find the element
            HashElement * he = (HashElement *) bucket->get(j);

            // Find the distance of the vectors
            double d = metric->dist(q, (Vector *) he->data);

            // If the distance is within radius, add the element to the
            // priority queue.
            if(d < radius)
                queue->add((void *) he->data, d);

        }

    }

    // After you are done, return the priority queue with the elements
    return queue;

}