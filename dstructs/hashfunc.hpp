#ifndef _huptownfunc
#define _huptownfunc
#include "../geometry/vector.hpp"

class LocalityHashFamily{

    // This class will construct and provide a hash function family
    // to be used both by the LSH and hypercube projection
    // The family is created at random, but stays the same throughout the
    // length of the execution

    private:

        // Control variables
        int numFunctions; // How many hash functions are created
        int dimensions; // The size of the vectors used in the hash
        // NOTE: this should equal the size of the vectors to be hashed

        // Variables for storing the hash family data
        Vector ** vectors;
        float * tvalues;

        // Parameters set by the user
        int w; // The window that is used in calcuations


    public:

        // Con/destructors
        LocalityHashFamily(int, int,  int);
        ~LocalityHashFamily();

        // Caller for hash function
        int hash(int, Vector *);

};

#endif