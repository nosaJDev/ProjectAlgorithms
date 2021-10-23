#include "metric.hpp"
#include <cmath>
#define ABS(x) (((x)>0)?(x):(-(x)))

double L2Metric::dist(Vector * a, Vector *b){
    // This will calculate the distance using the eucledian formula

    // Add all the squared differences
    double ret = 0.0;
    for(int i = 0; i < a->getSize(); i++){
        double diff = ABS(a->getCoord(i)-b->getCoord(i));
        ret += diff*diff;
    }

    // Finally return the square root of the result
    return sqrt(ret);

}