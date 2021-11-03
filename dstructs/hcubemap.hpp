#ifndef _hcubemap
#define _hcubemap
#include "../geometry/vector.hpp"
#include "hashfunc.hpp"
#include "hashtable.hpp"
#include "metric.hpp"
#include "pqueue.hpp"
#include "pointstruct.hpp"

class HypercubeMapping: public PointStruct{

    // This class will sort the points according to the theory of hypercube
    // mapping.

    private:

        // Parameters
        int cubedims; // How many dimensions will the mapping project to
        // Typically this is chosen to be ln(n)-3 to ln(n)-1 where n is the
        // number of projected vectors.
        int w; // Window used by the hash functions
        int dimension; // The dimension of the vectors that are projected

        // The hash function generator
        LocalityHashFamily * hashfunc;

        // The hash table to store all the points
        HashTable * table;

        // Internal operations
        int findVectorCubeKey(Vector *);
        void mapVectorToCube(Vector *);

    public:

        // Con/destructors
        HypercubeMapping(int, int, int);
        ~HypercubeMapping();

        // This is the preprocessing step
        void addVectorList(List *);

        // These are methods for performing queries
        Vector * approximateNN(Vector *,Metric *);
        PriorityQueue * approximatekNN(int, Vector *, Metric *);
        PriorityQueue * approximateRange(double, Vector *, Metric *);


};

#endif