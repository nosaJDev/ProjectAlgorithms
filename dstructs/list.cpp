#include "list.hpp"

ListNode::ListNode(ListNode * _next = nullptr, void * _data = nullptr){

    //Set the variables of the node
    next = _next;
    data = _data;

}

ListNode::~ListNode(){

    // This will perform cascade delete of the nodes
    // Keep in mind that in large data this could cause
    // a stack overflow (yes I've seen it) so set the
    // next to null before deleting if you don't want this
    // behaviour

    if(next != nullptr)
        delete next;

}

void * ListNode::getData(){
    // Retruns the data held by the node
    return data;
}

void ListNode::setData(void * _data){
    // Sets the data field of the node
    data = _data;
}

ListNode * ListNode::getNext(){
    // Returns the following node
    return next;

}

void ListNode::setNext(ListNode * _next){
    // Sets the next node of this one
    next = _next;
}

// LIST METHODS FROM HERE BELOW

List::List(){
    // Constructs an empty list
    size = 50;
    array = new void*[size];
    elems = 0;


}

List::~List(){

    // Delete the array
    delete[] array;

}

void List::add(void * elem){

    // Adds a new element to front of the list
    
    // Double size if necessary
    if(elems == size){
        void ** newarray = new void*[size*2];
        for(int i = 0; i < size; i++)
            newarray[i] = array[i];
        size = 2*size;
        delete[] array;
        array = newarray;

    }

    // Add to the end of the list
    array[elems] = elem;

    // Increase the element counter
    elems++;


}

void * List::get(int pos){

    // Finds and returns the element at the specified position
    // If out of range it returns nullptr

    // Check for negatives
    if(pos < 0 || pos >= elems){
        return nullptr;
    }

    // Return the element
    return array[pos];


}

void List::reset(){
    
    // This will reset the size, essentially emptying the list
    elems = 0;

    

}

int List::getElems(){
    // Returns the number of elements in the list
    return elems;

}