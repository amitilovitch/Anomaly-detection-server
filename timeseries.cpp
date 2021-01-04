/*
* Amit Ilovitch
* ID: 313581688
*/
#include <vector>
#include <fstream>
#include "timeseries.h"
#include <map>
#include <sstream>

using namespace std;

TimeSeries::TimeSeries(const char *CSVfileName) {
    this->CSVfileName = CSVfileName;
    this->read();
}
/*
 * read the csv file and put the rows in vectors.
 */
 void TimeSeries::read() {
    string head, parsed;
    vector<string> parsedHeader;
    std::ifstream myFile;
    myFile.open(this->CSVfileName);
    if (!myFile.is_open()){
        exit(EXIT_FAILURE);
    }
    getline(myFile, head, '\n');
    std::stringstream ss(head);
    while (ss.good()){
        getline(ss, parsed, ',');
        this->headersVec.push_back(parsed);
    }
   // this->headersVec.push_back(ss.str());

    while (myFile.good()){
        for (int i = 0; i < this->headersVec.size() - 1; i++){
            getline(myFile, parsed, ',');
            if (!myFile.good()) {
                break;
            }
            this->timeSeriesMap[this->headersVec[i]].push_back(std::stof(parsed));
        }
        if (!myFile.good()) {
            break;
        }
        getline(myFile, parsed, '\n');
        this->timeSeriesMap[this->headersVec[this->headersVec.size() - 1]].push_back(std::stof(parsed));
    }
    myFile.close();
}
   //return vector of headers
 std::vector<std::string>  TimeSeries::getHeaders() {
     vector<string> returnVec;
     for(int i = 0 ; i <headersVec.size(); i++) {
         returnVec.push_back(headersVec[i]);
     }
     return returnVec;
}

const map<string, vector<float>> &TimeSeries::getTimeSeriesMap() const {
    return timeSeriesMap;
}









