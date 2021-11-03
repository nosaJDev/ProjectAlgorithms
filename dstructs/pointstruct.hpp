// This is an abstract class that contains methods for querying points
// It will return ANN, kANN or Range query results
// Implementations of these are the LSH and Hypercube Mapping

#ifndef _pstruct
#define _pstruct
#include "pqueue.hpp"
#include "vector.hpp"
#include "metric.hpp"

class PointStruct{



    public:

        // This is the preprocessing step
        virtual void addVectorList(List *) = 0;

        // These are methods for performing queries
        virtual Vector * approximateNN(Vector *,Metric *) = 0;
        virtual PriorityQueue * approximatekNN(int, Vector *, Metric *) = 0;
        virtual PriorityQueue * approximateRange(double, Vector *, Metric *) = 0;



}


#endif