#include "Node.h"

Node::Node(size_t id, std::shared_ptr<DataPoint> point): id(id), point(point), level(0) {}

Node::Node(size_t id, std::shared_ptr<Node> left, std::shared_ptr<Node> right, size_t level): id(id), 
                                                                                              left(left), 
                                                                                              right(right),
                                                                                              level(level) {}

void Node::SetID(size_t id) {
    this->id = id;
} 

size_t Node::GetID() const {
    return id;
}

void Node::SetLevel(size_t level)  {
    this->level = level;
}

size_t Node::GetLevel() const {
    return level;
}

void Node::SetMergeValue(double mergeValue) {
    merge_value = mergeValue; 
}

std::optional<double> Node::GetMergeValue() const {

    return merge_value;
}