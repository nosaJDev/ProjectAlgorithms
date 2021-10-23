#ifndef _metric
#define _metric
#include "vector.hpp"

class Metric{

    // This is an abstract class containing a method for comparing
    // distances between Vectors, it is extended by the classes below

    public:
        virtual double dist(Vector *, Vector *) = 0;

};


class L2Metric: public Metric{


    double dist(Vector *, Vector *);

};



#endif