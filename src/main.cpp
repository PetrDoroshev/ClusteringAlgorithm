#include <iostream>
#include "DistanceMatrix.h"
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "DataPoint.h"
#include "GenerateData.h"
#include "clustering/HierarchicalClustering.h"
#include "linkage/WPGMA.h"
#include "metrics/EuclideanDistance.h"

namespace py = pybind11;


void bind_datapoint(py::module_& m) {

    py::class_<DataPoint>(m, "DataPoint")
       
        .def(py::init<const std::vector<double>&>(), py::arg("data"))

       
        .def("get_dim", &DataPoint::GetDim)
        .def("get_data", &DataPoint::GetData)

        .def("__add__", &DataPoint::operator+)
        .def("__sub__", &DataPoint::operator-)
        .def("__mul__", &DataPoint::operator*)
        .def("__rmul__", [](int n, const DataPoint& p) {
            return p * n;  
        })

        .def("__getitem__", [](const DataPoint& p, size_t i) {
            if (i >= p.GetDim())
                throw py::index_error("index out of range");
            return p[i];      // вызывает operator[] уже существующий в .cpp
        });
};

class PyDistanceMetric : public DistanceMetric {
public:
    using DistanceMetric::DistanceMetric;

    double operator()(const DataPoint& p1, const DataPoint& p2) const override {
        PYBIND11_OVERRIDE_PURE(
            double,
            DistanceMetric,
            operator(),
            p1, p2
        );
    }
};

void bind_distance_metric(py::module_& m) {
    py::class_<DistanceMetric, PyDistanceMetric, std::shared_ptr<DistanceMetric>>(m, "DistanceMetric")
        .def(py::init<>())
        .def("__call__", &DistanceMetric::operator());
};

void bind_euclidean_metric(py::module_& m) {
    py::class_<EuclideanDistance, DistanceMetric, std::shared_ptr<EuclideanDistance>>(m, "EuclideanDistance")
        .def(py::init<>())
        .def("__call__", &EuclideanDistance::operator());
};

class PyLinkageStrategy : public LinkageStrategy {
public:
    using LinkageStrategy::LinkageStrategy;

    void UpdateDistance(size_t new_node, size_t m1, size_t m2, size_t i, DistanceMatrix& DM) const override {
        PYBIND11_OVERRIDE_PURE(
            void,
            LinkageStrategy,
            UpdateDistance,
            new_node, m1, m2, i, DM
        );
    }
};

void bind_linkage_strategy(py::module_& m) {
    py::class_<LinkageStrategy, PyLinkageStrategy, std::shared_ptr<LinkageStrategy>>(m, "LinkageStrategy")
        .def(py::init<>())
        .def("update_distance", &LinkageStrategy::UpdateDistance);
};

void bind_wpgma(py::module_& m) {
    py::class_<WPGMA, LinkageStrategy, std::shared_ptr<WPGMA>>(m, "WPGMA")
        .def(py::init<>())
        .def("update_distance", &WPGMA::UpdateDistance);
};

void bind_distance_matrix(py::module_& m) {
    py::class_<DistanceMatrix>(m, "DistanceMatrix")
        .def(py::init<size_t>())
        .def(py::init<const std::vector<std::vector<double>>&>())
        .def(py::init<>())
        
        .def("get", &DistanceMatrix::Get)
        .def("set", &DistanceMatrix::Set)
        .def("get_dim", &DistanceMatrix::GetDim)
        .def("get_data", &DistanceMatrix::GetData)
        .def("print", &DistanceMatrix::Print);
};

void bind_hclustering(py::module_& m) {

    // --- Enum MetricType ---
    py::enum_<MetricType>(m, "MetricType")
        .value("EUCLIDEAN", MetricType::EUCLIDEAN);

    // --- Enum LinkageType ---
    py::enum_<LinkageType>(m, "LinkageType")
        .value("WPGMA", LinkageType::WPGMA);

    // --- Основной класс ---
    py::class_<HClustering>(m, "HClustering")
        .def(
            py::init<
                const std::vector<DataPoint>&,
                MetricType,
                LinkageType
            >(),
            py::arg("dataset"),
            py::arg("metric"),
            py::arg("linkage")
        )

        // Fit() возвращает vector<tuple> → python list[list]
        .def("fit", [](HClustering& self) {
            auto result = self.Fit();

            // Преобразуем tuple → python list
            py::list py_result;
            for (const auto& t : result) {
                auto [a, b, c, d] = t;
                py_result.append(py::make_tuple(a, b, c, d));
            }
            return py_result;
        })

        .def("set_dataset", &HClustering::setDataset)
        .def("get_dm", &HClustering::getDM);

}

PYBIND11_MODULE(clustering, m) {
    
    bind_datapoint(m);
    bind_distance_metric(m);
    bind_euclidean_metric(m);
    bind_linkage_strategy(m);
    bind_wpgma(m);
    bind_distance_matrix(m);
    bind_hclustering(m);

    m.def(
        "generate_data",
        &generateData,                       
        py::arg("dimensions"),
        py::arg("count"),
        py::arg("spread")
    );

};

