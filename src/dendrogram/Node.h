#ifndef NODE_H
#define NODE_H

#include <cstdint>
#include <vector>
#include <memory>
#include "../DataPoint.h"

class Node {

public:

    Node(size_t id, std::shared_ptr<DataPoint> point);
    Node(std::shared_ptr<Node> left, std::shared_ptr<Node> right, size_t level);


private:

    double merge_value;
    size_t id;
    size_t level;

    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;
    std::shared_ptr<DataPoint> point = nullptr;
};

#endif