
#include "CLI.h"
#include "stdlib.h"
CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->commandData = new command_data();
    this->commandData->had = HybridAnomalyDetector();
    this->command1 = new Command1(dio, commandData);
    this->command2 = new Command2(dio, commandData);
    this->command3 = new Command3(dio, commandData);
    this->command4 = new Command4(dio, commandData);
    this->command5 = new Command5(dio, commandData);
    this->command6 = new Command6(dio, commandData);
}

void CLI::start(){
    vector<Command*> commands_vec;
    string user_choice_string;
    int user_choice = 0;
    commands_vec.push_back(this->command1);
    commands_vec.push_back(this->command2);
    commands_vec.push_back(this->command3);
    commands_vec.push_back(this->command4);
    commands_vec.push_back(this->command5);
    commands_vec.push_back(this->command6);
    while (user_choice != 5) {
//        dio->write("Welcome to the Anomaly Detection Server.\n");
//                   "Please choose an option:\n" + command1->descrition + command2->descrition
//                   + command3->descrition + command4->descrition + command5->descrition
//                   + command6->descrition);
        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        for (int i = 0; i < commands_vec.size(); ++i) {
            dio->write(commands_vec[i]->getDescription());
        }
        user_choice_string = dio->read();
        user_choice = atoi(user_choice_string.c_str());
        user_choice = user_choice - 1;
        commands_vec.at(user_choice)->execute();
    }
}


CLI::~CLI() {

    delete command1;
    delete command2;
    delete command3;
    delete command4;
    delete command5;
    delete command6;
    delete commandData;
    //delete dio;
}
