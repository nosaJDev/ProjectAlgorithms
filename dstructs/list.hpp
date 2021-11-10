#ifndef _list
#define _list

class ListNode{

    // This is a generic list node used in the list
    // It accepts void * data

    private:

        // The information
        void * data;

        // The pointer to the next node
        ListNode * next;
    
    public:

        // Con/destructors
        ListNode(ListNode *, void *);
        ~ListNode();

        // Getters and setters
        void * getData();
        void setData(void *);
        ListNode * getNext();
        void setNext(ListNode *);

};

class List{

    // This is a generic list that accepts void * data
    // No option for deleting elements
    // ListNode proved poor decision, switching to array

    private:

        // The data of the list
        void ** array;
        int size;

        // How many elements are in the list
        int elems;



    public:

        // Con/destructors
        List();
        ~List();

        // Operations of the list
        void add(void *);
        void * get(int);
        void reset();

        // Getters/Setters
        int getElems();



};


#endif