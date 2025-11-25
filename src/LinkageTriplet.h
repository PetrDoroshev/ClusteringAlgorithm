#ifndef LINKAGE_TRIPLET_H
#define LINKAGE_TRIPLET_H

struct LinkageTriplet {

    size_t left; 
    size_t right;
    double distance;
    
    LinkageTriplet(size_t left, size_t right, double distance): left(left), right(right), distance(distance) {}
};

#endif