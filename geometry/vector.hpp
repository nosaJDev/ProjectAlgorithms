#ifndef _vec
#define _vec

class Vector{

    // This will represent a mathematical point in n dimensions
    // The dimensions will be given in the constructor, as well
    // as any initial points in an array

    private:

        // Coordinates and dimensions
        double * coords;
        int size;
    
    public:

        // Con/destructor
        Vector(int, double *);
        ~Vector();

        // Getters and setters
        int getSize();
        double getCoord(int);
        void setCoord(int, double);

        // Math operations
        double dotProduct(Vector *);


};

#endif