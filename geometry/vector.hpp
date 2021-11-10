#ifndef _vec
#define _vec
#include <cstdio>

class Vector{

    // This will represent a mathematical point in n dimensions
    // The dimensions will be given in the constructor, as well
    // as any initial points in an array

    private:

        // Identification for caching distances
        // This may increase the speed by much
        int id = -1;

        // Coordinates and dimensions
        float * coords;
        int size;
        char * label;

        // These are used in clustering
        int cluster;
        float clusterdist;
    
    public:

        // Con/destructor
        Vector(int, float * = nullptr, char * = nullptr,int = -1);
        ~Vector();

        // Getters and setters
        int getSize();
        float getCoord(int);
        void setCoord(int, float);
        char * getLabel();

        // Math operations
        float dotProduct(Vector *);

        // Other operations
        void paste(Vector *);

        // Operations used in range clustering
        bool addPossibleCluster(int, float);
        int getCluster();
        float getClusterDist();
        void resetCluster();

        // Identification getters
        int getId();

        // Used for debugging
        void print();
        void printFile(FILE *);


};

#endif