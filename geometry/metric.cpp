#include "metric.hpp"
#include <cmath>
#define ABS(x) (((x)>0)?(x):(-(x)))

Metric::Metric(int c){

    // Initialize the cache
    cache_no = c;
    cache = new double*[c];
    for(int i = 0; i < c; i++){
        cache[i] = new double[i+1];
        for(int j = 0; j < i+1;j++)
            cache[i][j] = -1;
    }

}

Metric::~Metric(){

    // Delete the cache
    for(int i = 0; i < cache_no;i++)
        delete[] cache[i];
    delete[] cache;

}

L2Metric::L2Metric(int c):Metric(c){
};

double L2Metric::dist(Vector * a, Vector *b){
    // This will calculate the distance using the eucledian formula

    // Check the ids
    int id_M = (a->getId()>=b->getId())?a->getId():b->getId();
    int id_m = (a->getId()<b->getId())?a->getId():b->getId();
    
    // Check if the cache has the value
    if(id_M < cache_no && id_m > -1 && cache[id_M][id_m] >= 0)
        return cache[id_M][id_m];


    // Add all the squared differences
    double ret = 0.0;
    for(int i = 0; i < a->getSize(); i++){
        double diff = ABS(a->getCoord(i)-b->getCoord(i));
        ret += diff*diff;
    }
    double res = sqrt(ret);

    // Write the result to cache if possible
    if(id_M < cache_no && id_m > -1)
        cache[id_M][id_m] = res; 

    // Finally return the square root of the result
    return res;

}