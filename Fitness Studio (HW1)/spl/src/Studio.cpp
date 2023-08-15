#include "../include/Studio.h"
#include <iostream>
#include "string.h"
#include <fstream>
#include <vector>

using namespace std;

Studio::Studio(): open(false), trainers(), workout_options(),actionsLog() {

};

Studio::Studio(const std::string &configFilePath):open(false), trainers(), workout_options(),actionsLog(){
    ifstream input;
    input.open(configFilePath);
    string line;
    int num_of_trainers = 0, counter = 0;
    bool got_trainers = false;
    while (getline(input,line)) {
        if(line[0] == '#'||line[0] == '\n')
            continue;
        else{
            if(num_of_trainers == 0) {
                num_of_trainers = std::stoi(line); // how many "push.back"s. (Vector size)
                getline(input,line);
            }
            else if(!got_trainers){
                size_t i = 0;
                while(i < line.length()){
                    string trainerName = "";
                    while((line[i] != ',') && (i != line.size())){
                        trainerName += line[i];
                        i++;
                    }
                    int trainerCapacity = std::stoi(trainerName);
                    Trainer* tr1 = new Trainer(trainerCapacity);
                    Studio::trainers.push_back(tr1);
                    i++;
                }
                got_trainers = true;
                getline(input,line);
            }
            else{
                if(line != "") {
                    string w_name = "";
                    int w_price = 0;
                    int firstComma = line.find(',');
                    int secondComma = line.find(',', firstComma + 1);
                    WorkoutType workoutType;
                    w_name = line.substr(0, firstComma);
                    if (line[firstComma + 2] == 'A')
                        workoutType = ANAEROBIC;
                    else if (line[firstComma + 2] == 'M')
                        workoutType = MIXED;
                    else
                        workoutType = CARDIO;
                    w_price = stoi(line.substr(secondComma + 1, line.length()));
                    Workout workout = Workout(counter, w_name, w_price, workoutType);
                    workout_options.push_back(workout);
                    counter++;
                }
            }
        }
    }
    input.close();
}

void Studio::start(){
    open = true;
    std::cout << "Studio is now open!"<<endl;
    string input = "";
    int trId = 0;
    int customerCounter = 0;
    while (input != "closeall") {
        input = "";
        getline(cin, input); 
        if (input.substr(0, 4) == "open") {
            vector<Customer *> custList;
            trId = stoi(input.substr(5, 1));
            size_t index = 5;
            while (index < input.length() - 2) {
                index += 2;
                string cus_name = "";
                while (index <= input.length() && input.substr(index, 1) != ",") {
                    cus_name += input.substr(index, 1);
                    index++;
                }
                string this_order = input.substr(index + 1, 3);
                index += 3;
                if (this_order == "swt") {
                    Customer *customer = new SweatyCustomer(cus_name, customerCounter);
                    custList.push_back(customer);
                    customer = nullptr;
                } else if (this_order == "chp") {
                    Customer *customer = new CheapCustomer(cus_name, customerCounter);
                    custList.push_back(customer);
                    customer = nullptr;
                    
                } else if (this_order == "mcl") {
                    Customer *customer = new HeavyMuscleCustomer(cus_name, customerCounter);
                    custList.push_back(customer);
                    customer = nullptr;
                } else if (this_order == "fbd") {
                    Customer *customer = new FullBodyCustomer(cus_name, customerCounter);
                    custList.push_back(customer);
                    customer = nullptr;
                }
                customerCounter++;

            }
            OpenTrainer *open_input = new OpenTrainer(trId, custList);
            open_input->act(*this);
            actionsLog.push_back(open_input);
            custList.clear();
            open_input = nullptr;

        }
        else if (input.substr(0, 5) == "order") {
            Order *order = new Order(stoi(input.substr(6, 1)));
            order->act(*this);
            actionsLog.push_back(order);
            order = nullptr;
        }
        else if (input.substr(0, 4) == "move") {
            int from = stoi(input.substr(5, 1));
            int to = stoi(input.substr(7, 1));
            int customer = stoi(input.substr(9, 1));
            MoveCustomer *movecustomer = new MoveCustomer(from, to, customer);
            movecustomer->act(*this);
            actionsLog.push_back(movecustomer);
            movecustomer = nullptr;
        }
        else if (input.substr(0, 6) == "status") {
            int trId = stoi(input.substr(7, 1));
            PrintTrainerStatus *status = new PrintTrainerStatus(trId);
            status->act(*this);
            actionsLog.push_back(status);
            status = nullptr;
        }
        else if ((input.substr(0, 5) == "close") && input.length() < 8) {
            int customerId = stoi(input.substr(6, 1));
            Close *close = new Close(customerId);
            close->act(*this);
            actionsLog.push_back(close);
            close = nullptr;
        }
        else if (input == "closeall") {
            CloseAll *closeall = new CloseAll();
            closeall->act(*this);
            actionsLog.push_back(closeall); 
            closeall = nullptr;
            break;   
        }
        else if (input == "workout_options") {
            PrintWorkoutOptions *workoutoptions = new PrintWorkoutOptions();
            workoutoptions->act(*this);
            actionsLog.push_back(workoutoptions);
            workoutoptions = nullptr;
        }
        else if (input == "log") {
            PrintActionsLog *log = new PrintActionsLog();
            log->act(*this);
            actionsLog.push_back(log);
            log = nullptr;
        }

        else if (input == "backup") {
            if(backup != nullptr)
                delete backup;
            BackupStudio *backup_temp = new BackupStudio();
            backup_temp->act(*this);
            
            delete backup_temp;
            backup_temp = nullptr;
        }
        else if (input == "restore") {
            RestoreStudio *restore = new RestoreStudio();
            restore->act(*this);
            actionsLog.push_back(restore);
            restore = nullptr;
        }
    }
}



int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer *Studio::getTrainer(int tid) {
    if((tid < 0) || (size_t(tid) > trainers.size()))
        return nullptr;
    return trainers[tid];
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}

std::vector<Trainer *> Studio::getTrainersList() {
    return trainers;
}

Studio::~Studio() {//des
    clean();
}

Studio::Studio(const Studio &other): open(),trainers(),workout_options(),actionsLog() { //copy constructor
    deepcopy(other);
    
}


Studio::Studio(Studio &&other) : open(),trainers(),workout_options(),actionsLog()  { //move constructor
    steal(other);

}


Studio &Studio::operator=(const Studio &other) {//copy assignment opertaor
    if(this != &other){
        clean();
        deepcopy(other);
    }
    return (*this);
}

Studio &Studio::operator=(Studio &&other) {//move assignment operator
    if(this!= &other){
        clean();
        steal(other);
    }
    return (*this);

}

void Studio::clean() {
    size_t sizetr =trainers.size();
    for(size_t i =0 ;i<sizetr;i++){
        delete trainers[i];
        trainers[i]= nullptr;
    }
    size_t sizeba=actionsLog.size();
    if(sizeba != 0){
        for(size_t i =0 ;i<sizeba;i++){
            delete actionsLog[i];
            actionsLog[i]= nullptr;
        }
    }
    trainers.clear();
    actionsLog.clear();
    workout_options.clear();
    this->open=false;
}

void Studio::deepcopy(const Studio &other) {
    this->open=other.open;
    for(Trainer* tr: other.trainers){
        Trainer* m = new Trainer(*tr);
        this->trainers.push_back(m);
        m=nullptr;
    }


    size_t size = other.actionsLog.size();
        for(size_t i =0 ;i < size;i++){
        std::string input = other.getActionsLog()[i]->getType();
        if (input == "log") {
            PrintActionsLog *log = new PrintActionsLog(*((PrintActionsLog*)other.actionsLog[i]));
            actionsLog.push_back(log);        
        }
        else if (input == "open"){
            OpenTrainer *tr = new OpenTrainer(*((OpenTrainer*)other.actionsLog[i]));
            actionsLog.push_back(tr);
        }
        else if (input == "move") {
            MoveCustomer *move = new MoveCustomer(*((MoveCustomer*)other.actionsLog[i]));
            actionsLog.push_back(move);
        }
        else if (input == "order") {
            Order *order = new Order(*((Order*)other.actionsLog[i]));
            actionsLog.push_back(order);
        }
        else if ((input == "close") && input.length() < 8) {
            Close *close = new Close(*((Close*)other.actionsLog[i]));
            actionsLog.push_back(close);
        }
        else if (input == "status") {
            PrintTrainerStatus *status = new PrintTrainerStatus(*((PrintTrainerStatus*)other.actionsLog[i]));
            actionsLog.push_back(status);
        }
        else if (input == "closeall") {
            CloseAll *closeall = new CloseAll(*((CloseAll*)other.actionsLog[i]));
            actionsLog.push_back(closeall); 
        }
        else if (input == "workout_options") {
            PrintWorkoutOptions *workoutoptions = new PrintWorkoutOptions(*((PrintWorkoutOptions*)other.actionsLog[i]));
            actionsLog.push_back(workoutoptions);
        }
        else if (input == "backup") {
            BackupStudio *backup = new BackupStudio(*((BackupStudio*)other.actionsLog[i]));
            actionsLog.push_back(backup);
        }
        else if (input == "restore") {
            RestoreStudio *restore = new RestoreStudio(*((RestoreStudio*)other.actionsLog[i]));
            actionsLog.push_back(restore);
        }
    }

    size_t sizewk=workout_options.size();
    for(size_t i =0 ;i<sizewk;i++){
        Workout work =other.workout_options[i];
        this->workout_options.push_back(work);
    }
    


}

void Studio::steal(Studio &other) {
    this->open=other.open;
    size_t tr=trainers.size();
    size_t al =actionsLog.size();
    size_t wo =workout_options.size();
    for(size_t i =0;i<tr;i++){
        this->trainers.push_back(other.trainers[i]);
        other.trainers[i]= nullptr;
    }
    for(size_t i=0;i<al;i++){
        this->actionsLog.push_back(other.actionsLog[i]);
        other.actionsLog[i]= nullptr;
    }
    for(size_t i=0;i<wo;i++){
        Workout work =other.workout_options[i];
        this->workout_options.push_back(work);
    }
    other.trainers.clear();
    other.workout_options.clear();
    other.actionsLog.clear();


}