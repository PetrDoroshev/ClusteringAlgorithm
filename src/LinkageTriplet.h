#ifndef LINKAGE_TRIPLET_H
#define LINKAGE_TRIPLET_H

struct LinkageTriplet {

    size_t left; 
    size_t right;
    double distance;
    int order;
    size_t size;
    
    LinkageTriplet(size_t left, size_t right, double distance, int order, size_t size): left(left), right(right), 
            distance(distance), order(order), size(size) {}
};

#endif