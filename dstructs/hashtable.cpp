#include "hashtable.hpp"

HashTable::HashTable(int _size){
    // Constructs the hashtable of the specified size

    // Set the counting variables
    size = _size;
    elems = 0;

    // Initialize the list array
    array = new List*[size];
    for(int i = 0; i < size; i++){
        array[i] = new List();
    }

}

HashTable::~HashTable(){
    // Deletes all the chains and the array

    for(int i = 0; i < size; i++){
        delete array[i];
    }

    delete[] array;

}

HashTable::add(void * elem, int key){
    // Adds an element under a specific key

    // Ignore negative keys
    if(key < 0)
        return;

    // Find the actual position
    int pos = key % size;

    // Add the element to the approrpiate chain
    array[pos]->add(elem);

    // Increase the element counter
    elems++;


}

List * HashTable::getChain(int key){
    // Finds and return the chain of the corresponding key

    // Ignore negative keys
    if(key < 0)
        return nullptr;
    
    // Find the position
    int pos = key % size;

    // Return the appropriate chain
    return array[pos];

}

List ** HashTable::getAllChains(){
    // Returns a reference of the array holding all the chains
    // This array should be read only and not tampered on the
    // outside of the HashTable code
    return array;

}


int HashTable::getElems(){
    // Returns the number of elements in the chain
    return elems;

}

int HashTable::getSize(){
    // Returns the total number of chains
    return size;

}