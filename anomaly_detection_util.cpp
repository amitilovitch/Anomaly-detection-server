/*
 * animaly_detection_util.cpp
 *
 * Author: write your ID and name here
 * Amit Ilovitch
 * ID: 313581688
 */
#include <cmath>
#include <iostream>
#include <cstdlib>
#include "anomaly_detection_util.h"

float avg(float* x, int size){
    if (size == 0) {
        return 0;
    }
    float sum = 0;
    float average = 0;
    for (int i = 0; i < size; i++) {
      sum = sum + x[i];
    }
    average = sum / size;
	return average;
}

// returns the variance of X and Y
float var(float* x, int size) {
    if (size == 0){
        return 0;
    }

    float sumVal = 0;
    float tempCalculate = 0;
    float aveg = avg(x, size);

    if (size == 0) {
        return 0;
    }
    for (int i = 0; i < size ; i++) {
        tempCalculate = std::pow(x[i] - aveg,2);
        sumVal = tempCalculate + sumVal;
    }

	return sumVal/size;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float xyArr[size];
    float* xy;
    float averageXY = 0;
    float averageX = 0;
    float averageY = 0;
    for (int i = 0; i < size; i++) {
        xyArr[i] = x[i] * y[i];
    }
    xy = xyArr;
    averageXY = avg(xy, size);
    averageX = avg(x, size);
    averageY = avg(y, size);
	return averageXY - (averageX * averageY);
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){

    if (size == 0) {
        return 0;
    }
    float tempCov = cov(x,y,size);
    float sqrtt = std::sqrt(var(x,size)) * std::sqrt(var(y,size));

	return tempCov / sqrtt;
}



// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){

    float a = 0;
    float b = 0;
    float xArr[size];
    float yArr[size];

    for (int i = 0; i < size; i++) {
        xArr[i] = points[i]->x;
        yArr[i] = points[i]->y;
    }

    a = cov(xArr,yArr,size) / var(xArr,size);
    b = avg(yArr,size) - (avg(xArr,size) * a);

	return Line(a,b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    if (size == 0) {
        return 0;
    }
    Line lineEqu = linear_reg(points,size);

	return dev(p,lineEqu);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    float exeptedPnty = 0;
    exeptedPnty = l.f(p.x);
	return std::abs(exeptedPnty - p.y);
}




