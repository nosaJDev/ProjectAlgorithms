#include "vectorfile.hpp"
#include <cstdio>
#include <cstring>


VectorFile::VectorFile(const char * filename, int dimension){
    // This will parse the specified file and save the info on
    // the lists. It is assumed that the file is in the correct
    // format following the assignment instructions.

    // Initialize the list
    vector_list = new List();

    // Open the file for reading
    FILE * file = fopen(filename,"r");
    if (file == nullptr){
        printf("Could not open file");
    }

    // Find out the correct dimension by reading the first line
    dimension = 0;
    bool lws = false;
    while(true){
        char c = fgetc(file);
        if(c == '\t' || c == ' '){
            lws = true;
        }else{
            if(lws && c >= '0' && c <= '9') dimension += 1;
            lws = false;
        }

        if (c == '\n')
            break;
    }
    printf("The dimensions are %d\n",dimension);
    rewind(file);

    // Start reading the file
    int dims_read = 0; // How many dimensions have you read
    bool read_name = true; // Wether you are reading the label
    char name_buffer[100]; // Buffer for the label
    double * coord_buffer = new double[dimension];

    while(true){

        // Check if you are reading the name
        if (read_name){

            // Try to read the name
            if(fscanf(file,"%s",name_buffer) == EOF)
                break;

            read_name = false;
        }

        // Try to read the next coordinate
        if(fscanf(file,"%lf",coord_buffer+dims_read) == EOF)
            break;

        // Increment what you have read
        dims_read++;

        // Check if you read the whole vector and save it if you do
        if (dims_read == dimension){


            // Create and save the vector object aswell
            Vector * newvec = new Vector(dimension,coord_buffer,name_buffer);
            vector_list->add((void*)newvec);


            // Reset the counter and try to read the name on the next round
            dims_read = 0;
            read_name = true;

        }

    }

    // Tidy up the mess
    delete[] coord_buffer;
    fclose(file);

}

VectorFile::~VectorFile(){

    // Delete all the vectors and the names
    for(int i = 0; i < vector_list->getElems(); i++){
        delete (Vector *) vector_list->get(i);

    }
    delete vector_list;

}

List * VectorFile::getVectorList(){
    // Returns the list with all the parsed vectors
    return vector_list;
}

