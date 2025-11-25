#include "DataPoint.h"

size_t DataPoint::GetDim() const {

    return data.size();
};

DataPoint DataPoint::operator+ (const DataPoint& rhs) const {

    std::vector<double> d(data.begin(), data.end());

    for (size_t i = 0; i < data.size(); ++i) {
        d[i] += rhs.data[i];
    }
        
    return DataPoint(d);
};

DataPoint DataPoint::operator- (const DataPoint& rhs) const {

    std::vector<double> d(data.begin(), data.end());

    for (size_t i = 0; i < data.size(); ++i) {
        d[i] -= rhs.data[i];
    }
        
    return DataPoint(d);
    
}

DataPoint DataPoint::operator* (int number) const {
    
    std::vector<double> d(data.begin(), data.end());

    for (size_t i = 0; i < data.size(); ++i) {
        d[i] *= number;
    }
        
    return DataPoint(d);
}

double DataPoint::operator[] (size_t index) const {

    return data[index];
}

