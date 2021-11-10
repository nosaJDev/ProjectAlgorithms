#ifndef _vec
#define _vec

class Vector{

    // This will represent a mathematical point in n dimensions
    // The dimensions will be given in the constructor, as well
    // as any initial points in an array

    private:

        // Identification for caching distances
        // This may increase the speed by much
        int id = -1;

        // Coordinates and dimensions
        double * coords;
        int size;
        char * label;

        // These are used in clustering
        int cluster;
        double clusterdist;
    
    public:

        // Con/destructor
        Vector(int, double * = nullptr, char * = nullptr,int = -1);
        ~Vector();

        // Getters and setters
        int getSize();
        double getCoord(int);
        void setCoord(int, double);
        char * getLabel();

        // Math operations
        double dotProduct(Vector *);

        // Other operations
        void paste(Vector *);

        // Operations used in range clustering
        bool addPossibleCluster(int, double);
        int getCluster();
        double getClusterDist();
        void resetCluster();

        // Identification getters
        int getId();

        // Used for debugging
        void print();


};

#endif