#include "vector.hpp"


Vector::Vector(int _size, double * values = nullptr){
    // Creates a vector, sets the size and adds initial values

    size = _size;

    // Initialize the coordinate array
    coords = new double[size];
    for(int i = 0; i < size; i++){
        // If the values is defined, use those, else zeros
        if(values != nullptr){
            coords[i] = values[i];
        }else{
            coords[i] = 0.0;
        }
    }

}

Vector::~Vector(){
    // Deletes the coords array
    delete[] coords;

}

int Vector::getSize(){
    // Returns the number of dimensions of the vector
    return size;
}

double Vector::getCoord(int pos){
    // Returns the coordinate number at position
    
    // If out of bounds it returns zero
    if(pos < 0 || pos >= size){
        return 0.0;
    }

    // If not, return the value at this position
    return coords[pos];

}

void Vector::setCoord(int pos, double value){
    // Sets a specific coord of the vector
    
    // If out of bounds, do nothing
    // This may change to increase the size of the vector
    if(pos < 0 || pos > size){
        return;
    }

    // If within bounds, set the appropriate coord
    coords[pos] = value;

}