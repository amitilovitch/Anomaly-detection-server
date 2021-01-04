//
// Created by Amit on 03/01/2021.
//

#include "check.h"

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include "HybridAnomalyDetector.h"
//
// Created by Lee on 01/01/2021.
//

#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}

float HybridAnomalyDetector::find_threshold(Point** ps,size_t len,correlatedFeatures cof){
    if (cof.corrlation > 0.9){
        return SimpleAnomalyDetector::find_threshold(ps, len, cof);
    }
    return cof.min_circle.radius;
}

void HybridAnomalyDetector::create_correlated_features(string f1, string f2, float cor, Point** points, size_t len){
    if (cor > threshold) {
        SimpleAnomalyDetector::create_correlated_features(f1,f2,cor,points,len);
    }
    else if (cor > 0.5){
        correlatedFeatures cofeat;
        cofeat.feature1 = f1;
        cofeat.feature2 = f2;
        cofeat.corrlation = cor;
        cofeat.min_circle = findMinCircle(points, len);
        float th = find_threshold(points, len, cofeat);
        cofeat.threshold = th * 1.1;
        cf.push_back(cofeat);
    }
}


bool HybridAnomalyDetector::is_anomaly(Point p, correlatedFeatures c){
    if (c.corrlation > 0.9){
        return SimpleAnomalyDetector::is_anomaly(p, c);
    }
    return distance(p, c.min_circle.center) > c.threshold;
}

#include "SimpleAnomalyDetector.h"
#include <iostream>
using namespace std;
SimpleAnomalyDetector::SimpleAnomalyDetector() {
    threshold = 0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    vector<pair<string, vector<float>>> data = ts.dataAsVector();
    int size = data.size();
    int sizeOfVector = data.at(0).second.size();
    for (int i = 0; i < size - 1; i++) {
        float maxCor = -1;
        float *x = new float[sizeOfVector];
        for (int k = 0; k < sizeOfVector; k++){
            x[k] = data.at(i).second.data()[k];
        }
        string bestCorrelated;
        for (int j = i + 1; j < size; j++) {
            float *y = new float[sizeOfVector];
            for (int k = 0; k < sizeOfVector; k++){
                y[k] = data.at(j).second.data()[k];
            }
            float cor = fabs(pearson(x, y, sizeOfVector));
            if (cor > maxCor) {
                maxCor = cor;
                bestCorrelated = data.at(j).first;
            }
            delete y;
        }
        string firstFeature = data.at(i).first;
        Point **points = ts.getAllPoints(firstFeature, bestCorrelated);
        size_t len = data.at(i).second.size();
        create_correlated_features(firstFeature, bestCorrelated, maxCor, points, len);
        // Delete points
        for (int i = 0; i < size; i++) {
            delete points[i];
        }
        delete[] points;
        delete[] x;
    }
}
void SimpleAnomalyDetector::create_correlated_features(string f1, string f2, float cor, Point** points, size_t len){
    if (cor > threshold) {
        correlatedFeatures cofeat;
        cofeat.feature1 = f1;
        cofeat.feature2 = f2;
        cofeat.corrlation = cor;
        cofeat.lin_reg = linear_reg(points, len);
        float th = find_threshold(points, len, cofeat);
        cofeat.threshold = th * 1.1;
        cf.push_back(cofeat);
    }
}

float SimpleAnomalyDetector::find_threshold(Point** ps,size_t len, correlatedFeatures cof){
    float max=0;
    Line rl = cof.lin_reg;
    for(size_t i=0;i<len;i++){
        float d=abs(ps[i]->y - rl.f(ps[i]->x));
        if(d>max)
            max=d;
    }
    return max;
}

bool SimpleAnomalyDetector::is_anomaly(Point p, correlatedFeatures c){
    float d = dev(p, c.lin_reg);
    return d > c.threshold;
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> reports;
    for (int i = 0; i < cf.size(); i++){
        int size = ts.getData().at(cf[i].feature1).size();
        for (int j = 0; j < size; j++){
            Point point = ts.getTimeStepPoint(cf[i].feature1, cf[i].feature2, j);
            if (is_anomaly(point,cf[i])){
                string description = cf[i].feature1 + "-" + cf[i].feature2;
                AnomalyReport report(description, j+1);
                reports.push_back(report);
            }
        }
    }
    return reports;
}
