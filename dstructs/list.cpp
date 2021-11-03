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
    head = nullptr;
    elems = 0;

    // Reset the iterator
    iter_at = -1;
    iter_node = nullptr;

}

List::~List(){

    // Resets the list before deletion
    reset();

}

void List::add(void * elem){

    // Adds a new element to front of the list
    
    // Create the new node and set it as the new head
    head = new ListNode(head, elem);

    // Increase the element counter
    elems++;

    // Move the iteration forward since you moved the head
    // one position to the right
    if(iter_at != -1){
        iter_at++;
    }

}

void * List::get(int pos){

    // Finds and returns the element at the specified position
    // If out of range it returns nullptr

    // Check for negatives
    if(pos < 0){
        return nullptr;
    }

    // Check if the iterator must be reset
    if(iter_at < 0 || iter_at > pos){
        // Reset the iterator to look at the head
        iter_at = 0;
        iter_node = head;
    }

    // Move the iterator until you reach the node you want
    for(;iter_node != nullptr && iter_at != pos; iter_at++,iter_node = iter_node->getNext()){}

    // Return the result of the iterator
    return iter_node->getData();


}

void List::reset(){
    
    // This will empty the list of nodes but leave the rest intact for further use
    
    // Deletes all the list nodes iteratively
    for(ListNode * at = head; at != nullptr;){

        // Get the node you are about to delete
        ListNode * todelete = at;

        // Seek the next node before the detatchment
        at = at->getNext();

        // Detatch and delete the node
        todelete->setNext(nullptr);
        delete todelete;

    }

    // Set the head and iterator appropriately
    head = nullptr;
    iter_at = -1;
    iter_node = nullptr;

    // Reset the size back to zero
    elems = 0;

    

}

int List::getElems(){
    // Returns the number of elements in the list
    return elems;

}