#ifndef _htable
#define _htable
#include "list.hpp"


struct HashElement{

    // This is a struct that will hold the data of the element
    // as well as the key with which it was inserted
    // These structs are created and deleted automatically by the HashTable

    void * data;
    int key;

    HashElement(void * d, int k){
        // Add the data to the struct
        data = d;
        key = k;
    };

};

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
        bool exists(void *, int);
        List * getChain(int);
        List ** getAllChains();
        void reset();

        // Sizes and such
        int getElems();
        int getSize();



};


#endif