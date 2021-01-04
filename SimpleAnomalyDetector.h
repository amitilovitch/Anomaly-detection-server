
/*
* Amit Ilovitch
* ID: 313581688
*/
#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
	string feature1,feature2;  // names of the correlated features
	float corrlation; //מחזירה מרחק נקודה גדול ביותר מהקו
	Line lin_reg;
	float threshold;//מחזירה את הסף שנותן קורלציה
	float x,y; // center of the circle
	float radius;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    protected:vector<correlatedFeatures> cf;
	float threshold;
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();
    virtual float findThreshold(Point** ps,size_t len,correlatedFeatures cf);
	virtual void learnNormal(const TimeSeries& ts);
	virtual void new_cf(float corallation, Point** points, size_t len, string f1, string f2);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    virtual bool is_Anomaly_detect(Point point,correlatedFeatures c);
	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
