#ifndef _cluster
#define _cluster
#include "pointstruct.hpp"
#include "hashtable.hpp"
#include "pqueue.hpp"

class LloydClusterer{

    // This class will perform clustering of points into k clusters
    // using the k-means++ initialization and then proceeding with
    // either direct assignment or Range Assignment
    // Both LSH and Hypercube Mapping can be used for the Range Assignment

    private:

        // Parameters
        int k; // How many clusters will it assign to
        bool rangeAssign; // Determines the algorithm for the assignment
        int dimension; // The number of coords per vector

        // Internal working variables
        Vector ** centroids; // Vectors that represent the centroids
        List * points; // A list with all the points that need to be clustered
        HashTable * clusters; // A hashtable that contains the clustered points
        PointStruct * query; // The datastructure that allows for range queries
        bool queryDone; // Indicating if the query object has been initialized

        // Sillouette variables
        float * sillouete;
        float globalSillouete;

        // Internal working functions
        void initialization(Metric *);
        void assignment(Metric *);
        void rangeAssignment(Metric *);
        void directAssignment(Metric *);
        bool update();



    public:

        // Con/destructors
        LloydClusterer(int,List * p,int,bool, PointStruct * = nullptr);
        ~LloydClusterer();

        // Operations
        int performClustering(Metric *,int = -1);

        // Result getters
        List * getCluster(int);
        Vector * getCentroid(int);

        // Sillouete calculation
        void calculateSillouete(Metric *);
        float getSillouete(int);
        float getGlobalSillouete();




};


#endif