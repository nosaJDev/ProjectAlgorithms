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

    // Delete all the HashElement structs and the lists
    for(int i = 0; i < size; i++){

        // Delete all of the HashElement structs
        for(int j = 0; j < array[i]->getElems(); j++){
            HashElement * he = (HashElement *) array[i]->get(j);
            delete he;
        }

        delete array[i];
    }

    delete[] array;

}

void HashTable::add(void * elem, int key){
    // Adds an element under a specific key

    // Ignore negative keys
    if(key < 0)
        return;

    // Find the actual position
    int pos = key % size;

    // Add the element to the approrpiate chain
    array[pos]->add(new HashElement(elem, key));

    // Increase the element counter
    elems++;

}

bool HashTable::exists(void * elem, int key){
    // Checks and return wether the element is already present on the
    // hash table under the bucket of the key provided

    // Find the bucket at the key position
    List * bucket = getChain(key);

    // Search to find the element
    for(int i = 0; i < bucket->getElems(); i++){
        HashElement * el_at = (HashElement*) bucket->get(i);

        // Check if it's the same element
        if (el_at->data == elem){
            // If it is, return true
            return true;
        }

    }

    // If you don't find it return false
    return false;

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