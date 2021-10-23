#ifndef _lsh
#define _lsh
#include "../geometry/metric.hpp"
#include "../geometry/vector.hpp"
#include "../dstructs/list.hpp"
#include "../dstructs/hashtable.hpp"

class LSH{

    // This class will classify and distribute vectors to buckets
    // After that, it will support Aproximate kNN and range queries

    private:

        // Parameters
        int L; // This will determine how may times will the process occur
        int k; // This will determine how many h() functions will be used
        int w; // This is the window that is used throughout the algorithm
        int numBuckets; // This will determine how many buckets per hash table

        // The hash function family used for generating the "small" keys
        int h(int, Vector *);

        // The array of the hashtables that store the points
        HashTable ** tables;

        // The hash functions that are used at each level (through ids)
        int ** hash_ids;

        // Internal functions for handling vectors
        void placeVectorToBucket(int, Vector *);

        // A random offset for controlled random values
        int rand_offset;

        // Cashing of vectors and t values to avoid recalculation
        int maxhashes = 50; // How many different hash functions are used
        Vector ** hashvectors;
        double * hasht;


    public:

        LSH(int, int,int,int);
        ~LSH();

        void addVectorList(List *);


};


#endif