#ifndef _metric
#define _metric
#include "vector.hpp"

class Metric{

    // This is an abstract class containing a method for comparing
    // distances between Vectors, it is extended by the classes below

    protected:
        double ** cache;
        int cache_no;

    public:
        Metric(int=100);
        virtual ~Metric();
        virtual double dist(Vector *, Vector *) = 0;

};


class L2Metric: public Metric{


    public:

        L2Metric(int=100);
        double dist(Vector *, Vector *);


};



#endif