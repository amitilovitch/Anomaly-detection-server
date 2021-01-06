

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

public:virtual string read() = 0;

public:virtual void write(string text) = 0;

public:virtual void write(float f) = 0;

public:virtual void read(float* f) = 0;

public:virtual ~DefaultIO(){}

	// you may add additional methods here
};
/*
class StandardIO : public DefaultIO {
    ifstream in;
    ofstream out;
public:
    string read() {
        string s;
        in >> s;
        return s;
    }

    void write(string text) {
        out << text;
    }

    void write(float f) {
        out << f;
    }

    void read(float *f) {
        in >> *f;
    }
};
 */
// you may add here helper classes
struct command_data{
    HybridAnomalyDetector had;
    vector<AnomalyReport> ar;
    TimeSeries* ts_anomalyTest;
    TimeSeries* ts_anomalyTrain;
    long line_count_test;
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
        descrition = "1.upload a time series csv file\n";
    }

     static int write_to_csv(DefaultIO* dio,string filename) {
         int line_count = 0;
        ofstream out1(filename);
        string data_to_read = dio->read();
        while (data_to_read != "done") {
            out1 << data_to_read << endl;
           data_to_read =  dio->read();
           line_count++;
        }

         out1.close();
         return line_count - 1;
        //string head, parsed;
        //stringstream ss_learn_file(file_learn);
        //getline(ss_learn_file, head, '\n');

    }

    virtual void execute() {
      dio->write("Please upload your local train CSV file.\n");
      commandData->line_count_test = write_to_csv(dio,"trainFile.csv");
      //dio->read();
      dio->write("Upload complete.\n");
      this->commandData->ts_anomalyTrain = new TimeSeries("trainFile.csv");
      dio->write("Please upload your local test CSV file.\n");
        commandData->line_count_test = write_to_csv(dio,"testFile.csv");
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
        descrition = "2.algorithm settings\n";
    }
    // ask for the user for a new threshold until it gets a valid value.
    void execute_helper() {
        dio->write("The current correlation threshold is ");
        dio->write(commandData->had.getThreshold());
        dio->write("\n");
        dio->write( "Type a new threshold \n");

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
        descrition = "3.detect anomalies\n";
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
        descrition = "4.display results\n";
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
        dio->write("Done.\n");
    }
};
class Command5: public Command {

public:
    Command5(DefaultIO* dio,command_data *commandData1):Command(dio,commandData1){
        descrition = "5.upload anomalies and analyze results\n";
    }
//    static void write_to_csv(DefaultIO* dio,string filename) {
//        ofstream out1(filename);
//        string data_to_read = dio->read();
//        while (data_to_read != "done") {
//            out1 << data_to_read << endl;
//            data_to_read =  dio->read();
//        }
//        out1.close();
//        //string head, parsed;
//        //stringstream ss_learn_file(file_learn);
//        //getline(ss_learn_file, head, '\n');
//
//    }
    static vector<pair<long, long>> upload_to_vec(DefaultIO* dio) {
        dio->write("Please upload your local anomalies file.\n");
        //create vector hold the real anomaly data
        vector<pair<long, long>> anomaly_data_vec_temp;
        pair<long, long> anomaly_pair;
        string parsed1, parsed2;
        string data_to_read = dio->read();
        while (data_to_read != "done") {
            std::stringstream ss(data_to_read);
            getline(ss, parsed1, ',');
            getline(ss, parsed2, ',');
            //anomaly_pair(stol(parsed1),stol(parsed2));
            anomaly_data_vec_temp.push_back(make_pair(stol(parsed1), stol(parsed2)));
            data_to_read = dio->read();

        }
        dio->write("Upload complete.\n");
        return anomaly_data_vec_temp;
    }
    static vector<pair<long,long>> initialize_detect_vec(command_data* commandData) {
        vector<pair<long,long>> anomaly_detect_vec;
        long first_pair = -1;
        long second_pair;
        for (int i = 0; i < commandData->ar.size() ; ++i) {
            if (first_pair == -1) {
                first_pair = commandData->ar.at(i).timeStep;
                second_pair = first_pair;
                continue;
            }
            if (commandData->ar.at(i).timeStep == second_pair + 1) {
                second_pair = commandData->ar.at(i).timeStep;
            } else {
                anomaly_detect_vec.push_back(make_pair(first_pair, second_pair));
                first_pair = commandData->ar.at(i).timeStep;
                second_pair = first_pair;
//                if ( i ==  commandData->ar.size() - 1) {
//                    anomaly_detect_vec.push_back(make_pair(first_pair, second_pair));
//                }
            }
            if ( i ==  commandData->ar.size() - 1) {
                anomaly_detect_vec.push_back(make_pair(first_pair, second_pair));
            }
        }
        return anomaly_detect_vec;
    }
    static bool is_pair_intersect(pair<long, long> anomaly_detect_pair,pair<long, long> anomaly_real_pair) {
        if ((anomaly_real_pair.first <= anomaly_detect_pair.first)
        && (anomaly_real_pair.second >= anomaly_detect_pair.second)) {
            return true;
        } else if ((anomaly_real_pair.first >= anomaly_detect_pair.first)
                   && (anomaly_real_pair.second <= anomaly_detect_pair.second)) {
            return true;
        } else if ((anomaly_real_pair.second <= anomaly_detect_pair.second)
                   && (anomaly_real_pair.second >= anomaly_detect_pair.first)) {
            return true;
        }else if ((anomaly_real_pair.second >= anomaly_detect_pair.second)
                  && (anomaly_real_pair.first <= anomaly_detect_pair.second)) {
            return true;
        }
        return false;
    }
    void execute() {
        vector<pair<long, long>> anomaly_real_vec = upload_to_vec(this->dio);
        vector<pair<long,long>> anomaly_detect_vec = initialize_detect_vec(this->commandData);
        long P  = anomaly_real_vec.size();
        long N , sum_anomaly_real = 0, TP = 0, FP =0;
        int flag = 0;
        for (int i = 0; i < anomaly_real_vec.size(); i++) {
            sum_anomaly_real += anomaly_real_vec.at(i).second - anomaly_real_vec.at(i).first + 1;
        }

        N = commandData->line_count_test - sum_anomaly_real;

        for (int i = 0; i < anomaly_detect_vec.size(); i++) {
            flag = 0;
            for (int j =0; j < anomaly_real_vec.size(); j++) {

                if (is_pair_intersect(anomaly_detect_vec.at(i),anomaly_real_vec.at(j))) {
                    flag = 1;
                    TP++;
                    break;

                }
            }
            if (flag == 0) {
                FP++;
            }
        }

        float tpr_send;
        float far_send;
        if (TP == 0) {
            tpr_send = 0;
        } else {
            tpr_send = floor(((float)TP/(float)P) * 1000)/1000;
        }
        far_send = floor(((float)FP/(float)N)*1000)/1000;
        dio->write("True Positive Rate: ");
        dio->write(tpr_send);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        dio->write(far_send);
        dio->write("\n");
    }
};
class Command6: public Command {

public:
    Command6(DefaultIO* dio,command_data *commandData1):Command(dio,commandData1){
        descrition = "6.exit\n";
    }
    void execute() {
          delete this->commandData->ts_anomalyTest;
          delete this->commandData->ts_anomalyTrain;
    }
};


#endif /* COMMANDS_H_ */
