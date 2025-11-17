#ifndef NODE_H
#define NODE_H

#include <cstdint>
#include <vector>
#include <memory>
#include <optional>
#include "../DataPoint.h"

class Node {

public:

    Node(size_t id, std::shared_ptr<DataPoint> point);
    Node(size_t id, std::shared_ptr<Node> left, std::shared_ptr<Node> right, size_t level);

    void SetID(size_t id);
    size_t GetID() const;

    void SetLevel(size_t level);
    size_t GetLevel() const;

    void SetMergeValue(double mergeValue);
    std::optional<double> GetMergeValue() const;


private:

    std::optional<double> merge_value;
    size_t id;
    size_t level;

    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;
    std::shared_ptr<DataPoint> point = nullptr;
};

#endif