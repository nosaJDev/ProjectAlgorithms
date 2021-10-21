#include "vectorfile.hpp"
#include <cstdio>


VectorFile::VectorFile(const char * filename){
    // This will parse the specified file and save the info on
    // the lists. It is assumed that the file is in the correct
    // format following the assignment instructions.
    // Names for vectors are allowed up to 49 characters

    // Initialize the lists
    name_list = new List();
    vector_list = new List();

    // Open the file for reading
    FILE * file = fopen(filename,"r");
    if (file == nullptr){
        printf("Could not open file");
    }

    // Read the file line by line
    char line[500];
    while(fgets(line,sizeof(line), file)){

        //Parse the line for the name
        char * name = new char[50];
        sscanf(line,"%s",)

    }





}