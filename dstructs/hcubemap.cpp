#include "hcubemap.hpp"

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

void HypercubeMapping::mapVectorToCube(Vector * v){

    // This will take a vector, pass it through the hash functions
    // perform additional mapping, and generate a bitkey for the table
    // representing its position on the hypercube
    // For this, I use f(x) = x mod 2 to map to a bit

    // Generate the key
    int key = 0;
    for(int i = 0; i < cubedims; i++){

        // Find the value for the i-th hash
        int i_value = hashfunc->hash(i,v);

        // Convert it to a bit
        int i_bit = i_value % 2 ==0;

        // Add it to the key and bitswift
        key = (key << 2)+i_bit;

    }

    // Finally insert the vector to the table
    table->add((void *) v,key);

}

void HypercubeMapping::addVectorList(List * vlist){

    // This will add all the vectors on the list to the table,
    // using the internal fuction for that purpose

    for(int i = 0; i < vlist->getElems(); i++){
        mapVectorToCube((Vector *)vlist->get(i));
    }

}