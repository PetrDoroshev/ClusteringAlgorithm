#ifndef DENDROGRAM_H
#define DENDROGRAM_H

#include "Node.h"

class Dendrogram {

public:
    Dendrogram(std::shared_ptr<Node> root);

    std::shared_ptr<Node>& GetRoot() const;

private:
    std::shared_ptr<Node> root;

};

#endif