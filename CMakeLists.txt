cmake_minimum_required(VERSION 3.17)
project(EX3_advancsdProgramming)

set(CMAKE_CXX_STANDARD 14)

add_executable(EX3_advancsdProgramming main.cpp minCircle.h minCircle.cpp timeseries.h timeseries.cpp anomaly_detection_util.h
        anomaly_detection_util.cpp AnomalyDetector.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h SimpleAnomalyDetector.h
        SimpleAnomalyDetector.cpp)