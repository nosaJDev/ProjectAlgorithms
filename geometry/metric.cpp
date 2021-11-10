#include "metric.hpp"
#include <cmath>
#define ABS(x) (((x)>0)?(x):(-(x)))



float L2Metric::dist(Vector * a, Vector *b){
    // This will calculate the distance using the eucledian formula

    // Add all the squared differences
    float ret = 0.0;
    for(int i = 0; i < a->getSize(); i++){
        float diff = ABS(a->getCoord(i)-b->getCoord(i));
        ret += diff*diff;
    }
    float res = sqrt(ret);

    // Finally return the square root of the result
    return res;

}