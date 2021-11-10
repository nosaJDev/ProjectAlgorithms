// This is an abstract class that contains methods for querying points
// It will return ANN, kANN or Range query results
// Implementations of these are the LSH and Hypercube Mapping

#ifndef _pstruct
#define _pstruct
#include "pqueue.hpp"
#include "../geometry/vector.hpp"
#include "../geometry/metric.hpp"

class PointStruct{



    public:

        // Make the destructor virtual to avoid cataclysmic failure
        virtual ~PointStruct(){};

        // This is the preprocessing step
        virtual void addVectorList(List *) = 0;

        // These are methods for performing queries
        virtual Vector * approximateNN(Vector *,Metric *) = 0;
        virtual PriorityQueue * approximatekNN(Vector *, Metric *) = 0;
        virtual PriorityQueue * approximateRange(float, Vector *, Metric *) = 0;



};


#endif