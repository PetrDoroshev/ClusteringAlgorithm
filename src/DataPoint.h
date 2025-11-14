#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <vector>
#include <cstdint>

class DataPoint {

public:
    DataPoint(const std::vector<double>& data): data(data) {};
    
private:
    std::vector<double> data;
};

#endif