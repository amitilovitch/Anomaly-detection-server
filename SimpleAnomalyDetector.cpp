/*
* Amit Ilovitch
* ID: 313581688
*/
#include "SimpleAnomalyDetector.h"
#include "anomaly_detection_util.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    threshold = 0.9;

}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    }
float SimpleAnomalyDetector::findThreshold(Point** ps,size_t len,correlatedFeatures cf) {
    Line lin_reg = cf.lin_reg;
    float max=0;
    for(size_t i=0;i<len;i++){
        float d=abs(ps[i]->y - lin_reg.f(ps[i]->x));
        if(d>max)
            max=d;
    }
    return max;
}
void SimpleAnomalyDetector::new_cf(float corallation, Point **points, size_t len, string f1, string f2) {
    if(corallation>threshold){
        correlatedFeatures c;
        c.feature1 = f1;
        c.feature2 = f2;
        c.corrlation = corallation;
        c.lin_reg=linear_reg(points,len);
        c.threshold=findThreshold(points,len,c)*1.1;
        cf.push_back(c);
    }
}

// learn the normal of features before flight
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    //initialize
    TimeSeries TimeS = ts;
    vector<float> checkedVector1;
    vector<float> checkedVector2;
    float pearsonReasult = 0;
    float tempPearsonReasult,tempThreshold , corrrlationBrink = 0.5;
    correlatedFeatures tempCorrelatedFeatures;
    vector<string> keyFeatuers = TimeS.getHeaders();
    int sizeOfVectors = TimeS.timeSeriesMap[keyFeatuers[0]].size();
    //Point* ps[sizeOfVectors];
    tempCorrelatedFeatures.corrlation = 0;
    tempCorrelatedFeatures.threshold = 0;

    int flag = 0;
    //run on the vectors and check corllation
    for (int i = 0; i < keyFeatuers.size(); i++) {
        checkedVector1 = TimeS.timeSeriesMap[keyFeatuers[i]];
        for (int j = i + 1; j < keyFeatuers.size(); j++) {
            checkedVector2 = TimeS.timeSeriesMap[keyFeatuers[j]];//לבדוק אם ההמרה טובה
            tempPearsonReasult = std::abs(pearson(checkedVector1.data(),checkedVector2.data(),
                                                  TimeS.timeSeriesMap[keyFeatuers[j]].size()));
            // if corelation and bigger then 0.9 keep them in cf
            if ((tempPearsonReasult > pearsonReasult) && (tempPearsonReasult > corrrlationBrink)) {
                tempCorrelatedFeatures.corrlation = tempPearsonReasult;
                tempCorrelatedFeatures.feature1 = keyFeatuers[i];
                tempCorrelatedFeatures.feature2 = keyFeatuers[j];
                flag = 1;
            }
        }
        //check if vector from corellation is bigger
        for (int j = 0; j < cf.size(); j++) {
            if((tempCorrelatedFeatures.feature1 == cf[j].feature2) &&
            (tempCorrelatedFeatures.corrlation < cf[j].corrlation)) {
                flag = 0;
            }
        }

        //initialize liniar reg.
        if (flag == 1) {

            Point* ps[sizeOfVectors];
            for (int k = 0; k < sizeOfVectors; k++) {
                ps[k] = new Point(TimeS.timeSeriesMap[tempCorrelatedFeatures.feature1][k],
                                  TimeS.timeSeriesMap[tempCorrelatedFeatures.feature2][k]);
            }
            new_cf(tempCorrelatedFeatures.corrlation,ps,sizeOfVectors,tempCorrelatedFeatures.feature1,tempCorrelatedFeatures.feature2);

            /*
            tempCorrelatedFeatures.lin_reg = linear_reg(ps, sizeOfVectors);

            //initialize threshold

            for (int l = 0; l < sizeOfVectors; l++) {
                Point point(ps[l]->x, ps[l]->y);
                tempThreshold = dev(point, tempCorrelatedFeatures.lin_reg);
                if (tempThreshold > tempCorrelatedFeatures.threshold) {
                    tempCorrelatedFeatures.threshold = tempThreshold;
                }
            }
             */
            //cf.push_back(tempCorrelatedFeatures);
            pearsonReasult = 0;
            flag = 0;
            for (int i = 0; i < sizeOfVectors; i++) {
                delete[] ps[i];
            }
        }
    }
}

bool SimpleAnomalyDetector::is_Anomaly_detect(Point point,correlatedFeatures c){
    float f = dev(point, c.lin_reg);
    return f > c.threshold;}

// detect exception during the flight
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    TimeSeries ts1 = ts;

    vector<AnomalyReport> arVector;
    float checkedFeaturesFloat1, checkedFeaturesFloat2, devOfPoint;
    vector<string> keyFeatuers = ts1.getHeaders();

// for each line check if there is exception
    for (int i = 0; i < ts1.timeSeriesMap[keyFeatuers[0]].size(); i++) {
        for (int j = 0; j < cf.size(); j++) {
            float maxDev = (float)1.1 * cf[j].threshold;
            checkedFeaturesFloat1 =   ts1.timeSeriesMap[cf[j].feature1][i];
            checkedFeaturesFloat2 =   ts1.timeSeriesMap[cf[j].feature2][i];
            Point pointForCheck(checkedFeaturesFloat1,checkedFeaturesFloat2);
            //devOfPoint = dev(pointForCheck,cf[j].lin_reg);
            if (is_Anomaly_detect(pointForCheck,cf[j])) {
                AnomalyReport anomalyReport(cf[j].feature1 + "-" +cf[j].feature2, i + 1);
                arVector.push_back(anomalyReport);
            }
        }
    }

    return arVector;
}

