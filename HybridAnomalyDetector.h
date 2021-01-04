

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    virtual void new_cf(float corallation, Point** points, size_t len, string f1, string f2);
    virtual bool is_Anomaly_detect(Point point,correlatedFeatures c);
    virtual float findThreshold(Point** ps,size_t len,correlatedFeatures cf);

};


#endif /* HYBRIDANOMALYDETECTOR_H_ */
