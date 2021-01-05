

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	float *choice;
    command_data* commandData;
    Command1* command1;
    Command2* command2;
    Command3* command3;
    Command4* command4;
    Command5* command5;
    Command6* command6;


    // you can add data members
public:
	CLI(DefaultIO* dio) ;
	void start();
	virtual ~CLI();
//	virtual void set_ts1() {
//	    ts1_p = new TimeSeries("anomalyTest.csv");
//	}
//    virtual void set_ts2() {
//        ts2_p = new TimeSeries("anomalyTrain.csv");
//    }
};

#endif /* CLI_H_ */
