

#ifndef COMMANDS_H_
#define COMMANDS_H_
#include <sstream>

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	// you may add additional methods here
};

// you may add here helper classes
struct command_data{
    HybridAnomalyDetector had;
    vector<AnomalyReport> ar;
    TimeSeries* ts_anomalyTest;
    TimeSeries* ts_anomalyTrain;
    //int line_count_test;
};

// you may edit this class
class Command{
public:DefaultIO* dio;
public:string descrition;
//protected:HybridAnomalyDetector *had;
//protected:TimeSeries** ts1_p;
//protected:TimeSeries** ts2_p;
public:command_data* commandData;

public:
	Command(DefaultIO* dio, command_data *commandData1):dio(dio),commandData(commandData1){}
	virtual void execute()=0;
	virtual string getDescription() {
        return descrition;
	}
	virtual ~Command(){}
};

// implement here your command classes
class Command1: public Command {
      //DefaultIO* dio;
      TimeSeries *file_learn;
      string file_detect;
public:

    Command1(DefaultIO* dio,command_data *commandData1):Command(dio,commandData1){
        descrition = "1. upload a time series csv file\n";
    }

     static void write_to_csv(DefaultIO* dio,string filename) {
        ofstream out1(filename);
        string data_to_read = dio->read();
        while (data_to_read != "done") {
            out1 << data_to_read << endl;
           data_to_read =  dio->read();
        }
         out1.close();
        //string head, parsed;
        //stringstream ss_learn_file(file_learn);
        //getline(ss_learn_file, head, '\n');

    }

    virtual void execute() {
      dio->write("Please upload your local train CSV file.\n");
      write_to_csv(dio,"trainFile.csv");
      //dio->read();
      dio->write("Upload complete.\n");
      this->commandData->ts_anomalyTrain = new TimeSeries("trainFile.csv");
      dio->write("Please upload your local test CSV file.\n");
      write_to_csv(dio,"testFile.csv");
     // dio->read();
      dio->write("Upload complete.\n");
      this->commandData->ts_anomalyTest = new TimeSeries("testFile.csv");
    }
};
class Command2: public Command {
    //DefaultIO* dio;
    float new_threshold;
public:
    Command2(DefaultIO* dio,command_data *commandData1):Command(dio,commandData1) {
        descrition = "2. algorithm settings\n";
    }
    // ask for the user for a new threshold until it gets a valid value.
    void execute_helper() {
        dio->write("The current correlation threshold is" + to_string(commandData->had.getThreshold()) + "\n"
                   "Type a new threshold\n");

        new_threshold = stof(dio->read());
        if (new_threshold > 1 || new_threshold < 0) {
            dio->write("please choose a value between 0 and 1.");
            execute_helper();
        }
    }
    virtual void execute() {
        execute_helper();
        commandData->had.setThreshold(new_threshold);
    }
};
class Command3: public Command {

public:
    Command3(DefaultIO* dio, command_data *commandData1):Command(dio,commandData1){
        descrition = "3. detect anomalies\n";
    }
    void execute() {
         commandData->had.learnNormal(*commandData->ts_anomalyTrain);
         commandData->ar = commandData->had.detect(*commandData->ts_anomalyTest);
         dio->write("anomaly detection complete.\n");
    }
};
class Command4: public Command {

public:
    Command4(DefaultIO* dio,command_data *commandData1):Command(dio,commandData1) {
        descrition = "4. display results\n";
    }
    void execute() {
        int length_of_anomaly_report = commandData->ar.size();
        string timestep;
        string description_ar;
        for (int i=0;i < length_of_anomaly_report; i++) {
            timestep = to_string(commandData->ar[i].timeStep);
            description_ar = commandData->ar[i].description;
            dio->write(timestep + "\t" + description_ar + "\n");
        }
        dio->write("Done.");
    }
};
class Command5: public Command {

public:
    Command5(DefaultIO* dio,command_data *commandData1):Command(dio,commandData1){
        descrition = "5. upload anomalies and analyze results\n";
    }
    static void write_to_csv(DefaultIO* dio,string filename) {
        ofstream out1(filename);
        string data_to_read = dio->read();
        while (data_to_read != "done") {
            out1 << data_to_read << endl;
            data_to_read =  dio->read();
        }
        out1.close();
        //string head, parsed;
        //stringstream ss_learn_file(file_learn);
        //getline(ss_learn_file, head, '\n');

    }
    void execute() {

    }
};
class Command6: public Command {

public:
    Command6(DefaultIO* dio,command_data *commandData1):Command(dio,commandData1){
        descrition = "6. exit\n";
    }
    void execute() {
          delete this->commandData->ts_anomalyTest;
          delete this->commandData->ts_anomalyTrain;
    }
};

#endif /* COMMANDS_H_ */
