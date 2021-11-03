#ifndef _lsh
#define _lsh
#include "../geometry/metric.hpp"
#include "../geometry/vector.hpp"
#include "../dstructs/list.hpp"
#include "../dstructs/hashtable.hpp"
#include "hashfunc.hpp"
#include "pointstruct.hpp"

class LSH: public PointStruct{

    // This class will classify and distribute vectors to buckets
    // After that, it will support Aproximate kNN and range queries

    private:

        // Parameters
        int L; // This will determine how may times will the process occur
        int k; // This will determine how many h() functions will be used
        int w; // This is the window that is used throughout the algorithm
        int numBuckets; // This will determine how many buckets per hash table
        int dimension; // This will determine the size of the vectors used

        // The hash function generator
        LocalityHashFamily * hashfunc;

        // The array of the hashtables that store the points
        HashTable ** tables;

        // The hash functions that are used at each level (through ids)
        int ** hash_ids;

        // Internal functions for handling vectors
        int getVectorMasterKey(int, Vector *);
        void placeVectorToBucket(int, Vector *);

        // A random offset for controlled random values
        int rand_offset;

        // Bookkeeping for inserted vectors
        int elems;


    public:

        LSH(int, int,int,int,int);
        ~LSH();

        // This is the preprocessing step
        void addVectorList(List *);

        // These are methods for performing queries
        Vector * approximateNN(Vector *,Metric *);
        PriorityQueue * approximatekNN(int, Vector *, Metric *);
        PriorityQueue * approximateRange(double, Vector *, Metric *);


};


#endif