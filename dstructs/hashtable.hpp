#ifndef _htable
#define _htable
#include "list.hpp"

class HashTable{

    // This is a generic hashtable with integer keys
    // It accepts void * data

    private:
        
        // This will contain all the chains
        List ** array;

        // How many elements/chains are there
        int size;
        int elems;

    public:

        // Con/destructors
        HashTable(int);
        ~HashTable();

        // Operators
        void add(void *, int);
        List * getChain(int);
        List ** getAllChains();

        // Sizes and such
        int getElems();
        int getSize();

}


#endif