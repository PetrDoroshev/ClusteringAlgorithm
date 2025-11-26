#ifndef LINKAGE_TRIPLET_H
#define LINKAGE_TRIPLET_H

struct LinkageRow {

    size_t left; 
    size_t right;
    double distance;
    int order;
    size_t size;
    
    LinkageRow(size_t left, size_t right, double distance, int order, size_t size): left(left), right(right), 
            distance(distance), order(order), size(size) {}
};

#endif