#include "vector.hpp"
#include <cstring>
#include <cstdio>

Vector::Vector(int _size, float * values, char * _label,int _id){
    // Creates a vector, sets the size and adds initial values

    size = _size;
    id = _id;

    // Initialize the coordinate array
    coords = new float[size];
    for(int i = 0; i < size; i++){
        // If the values is defined, use those, else zeros
        if(values != nullptr){
            coords[i] = values[i];
        }else{
            coords[i] = 0.0;
        }
    }

    // Set the label if one is provided
    label = nullptr;
    if ( _label != nullptr){
        label = new char[strlen(_label)+1];
        strcpy(label,_label);
    }

    // Set variables used in clustering
    cluster = -1;
    clusterdist = 0;


}

Vector::~Vector(){
    // Deletes the coords array
    delete[] coords;

    // Delete the label if there was one
    if (label != nullptr)
        delete[] label;

}

int Vector::getSize(){
    // Returns the number of dimensions of the vector
    return size;
}

float Vector::getCoord(int pos){
    // Returns the coordinate number at position
    
    // If out of bounds it returns zero
    if(pos < 0 || pos >= size){
        return 0.0;
    }

    // If not, return the value at this position
    return coords[pos];

}

void Vector::setCoord(int pos, float value){
    // Sets a specific coord of the vector
    
    // If out of bounds, do nothing
    // This may change to increase the size of the vector
    if(pos < 0 || pos > size){
        return;
    }

    // If within bounds, set the appropriate coord
    coords[pos] = value;

}

char * Vector::getLabel(){
    // Returns the name of this specific vector
    return label;
}

float Vector::dotProduct(Vector * other){
    // Calculates and returns the dot product of this vector
    // with another one. Assumes that the sizes match

    float res = 0.0;
    for(int i = 0; i < size; i++){
        res += coords[i]*other->coords[i];
    }
    return res;

}

void Vector::paste(Vector * other){

    // This will copy the coords of the other vector into this one
    for(int i = 0; i < size; i++){
        coords[i] = other->coords[i];
    }

}

bool Vector::addPossibleCluster(int clust, float cdist){

    // Adds a possible cluster based on distance
    // Returns wether this was a first assignment

    bool ret = cluster == -1;
    if (ret || cdist < clusterdist){
        
        cluster = clust;
        clusterdist = cdist;
    }

    return ret;

}

int Vector::getCluster(){

    // Returns the cluster that has been set for the vector
    return cluster;

}

float Vector::getClusterDist(){
    // Returns the distance to the cluster centroid
    return clusterdist;

}

void Vector::resetCluster(){

    // Resets the placement on the cluster
    cluster = -1;

}

int Vector::getId(){

    // Returns the id of the vector
    return id;

}

void Vector::print(){

    // Print out the vector
    if(label != nullptr){
        printf("[%s]: (",label);
    }else{
        printf("[no label]: (");
    }
    
    //Print the dimensions
    for(int i = 0; i < size; i++){
        printf(" %.1lf%s",coords[i],(i == size-1)?"":",");
    }
    printf(")\n");

}

void Vector::printFile(FILE * thefile){

    // Similar to print but in a file
    fprintf(thefile,"(");
    for(int i = 0; i < size; i++){
        fprintf(thefile," %.1lf%s",coords[i],(i == size-1)?"":",");
    }
    fprintf(thefile,")\n");

}