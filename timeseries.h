/*
* Amit Ilovitch
* ID: 313581688
*/

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <string>
#include <map>
#include <list>
#include <vector>
#include <map>


using namespace std;

class TimeSeries{

public:

    const char* CSVfileName;
     std::map<string, vector<float>> timeSeriesMap;
     std::vector<string> headersVec;

    TimeSeries(const char* CSVfileName);
    TimeSeries(const TimeSeries &ts2) {
        CSVfileName = ts2.CSVfileName;
        this->read();
    }

private:
    void read();

public:
    virtual std::vector<std::string>  getHeaders();

    const map<string, vector<float>> &getTimeSeriesMap() const;
    //void setHeaders();
    //void  addRow(string a, float a1,string b, float b1,string c, float c1,string d, float d1);
   // list<string> findValueInTime(string valueName);
};


#endif /* TIMESERIES_H_ */
