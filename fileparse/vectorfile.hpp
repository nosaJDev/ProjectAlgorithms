#ifndef _vecfile
#define _vecfile
#include "../geometry/vector.hpp"
#include "../dstructs/list.hpp"

class VectorFile{

    // This will parse input files in the format of the 
    // assignment and put all the vectors in the list

    private:

        // Lists for storing the vector and naming
        List * name_list;
        List * vector_list;

    public:

        VectorFile(const char *,int);
        ~VectorFile();

        // Getters
        List * getNameList();
        List * getVectorList();


}

#endif