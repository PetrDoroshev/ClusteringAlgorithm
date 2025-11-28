#include "../src/GenerateData.h"
#include "../src/clustering/HierarchicalClustering.h"

int main() {

    auto data = std::vector<DataPoint> {
        DataPoint(std::vector<double>({0, 0})), 
        DataPoint(std::vector<double>({0, 1})), 
        DataPoint(std::vector<double>({1, 0})),
        DataPoint(std::vector<double>({0, 4})),
        DataPoint(std::vector<double>({0, 3})),
        DataPoint(std::vector<double>({1, 4})),
        DataPoint(std::vector<double>({4, 0})),
        DataPoint(std::vector<double>({3, 0})),
        DataPoint(std::vector<double>({4, 1})),
        DataPoint(std::vector<double>({4, 4})),
        DataPoint(std::vector<double>({3, 4})),
        DataPoint(std::vector<double>({4, 3}))};

    auto hc = HClustering(data, MetricType::EUCLIDEAN, LinkageType::WPGMA);
    hc.Fit();

    return 0;
}