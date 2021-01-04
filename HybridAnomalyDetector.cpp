
#include "HybridAnomalyDetector.h"
#include "minCircle.h"
#include "SimpleAnomalyDetector.h"
HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

float HybridAnomalyDetector::findThreshold(Point **ps, size_t len, correlatedFeatures cfe) {
    if (cfe.corrlation >= 0.9){
        return SimpleAnomalyDetector::findThreshold(ps, len, cfe);
    } else {
        return cfe.radius;
    }
}
void HybridAnomalyDetector::new_cf(float corallation, Point** points, size_t len, string f1, string f2) {
    if (corallation >= 0.9){
        SimpleAnomalyDetector::new_cf(corallation,points,len,f1,f2);
    } else if ( corallation > 0.5) {
            correlatedFeatures c;
            Circle min_circ = findMinCircle(points,len);
            c.feature1 = f1;
            c.feature2 = f2;
            c.corrlation = corallation;
            c.x = min_circ.center.x;
            c.y = min_circ.center.y;
            //c.lin_reg = findMinCircle(points,len).;
            c.threshold = min_circ.radius * 1.1;
            cf.push_back(c);
    }
}
bool HybridAnomalyDetector::is_Anomaly_detect(Point point, correlatedFeatures c) {
    if (c.corrlation > 0.9){
        return SimpleAnomalyDetector::is_Anomaly_detect(point, c);
    } else if ( c.corrlation > 0.5) {
        Point center(c.x,c.y);
        return dist(point, center) > c.threshold;
    }
}

