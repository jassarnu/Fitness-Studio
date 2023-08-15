#include "../include/Action.h"
#include "../include/Trainer.h"
#include "../include/Studio.h"
#include <iostream>

BaseAction::~BaseAction() =default;
BaseAction::BaseAction() :errorMsg(),status(){};
BaseAction &BaseAction::operator=(const BaseAction &other) =default;

void BaseAction::complete() {
    this->status = COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    this->status = ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error: " + errorMsg <<std::endl;
}

ActionStatus BaseAction::getStatus() const {
    return status;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

BaseAction::BaseAction(const BaseAction &other): errorMsg(other.errorMsg), status(other.status){
}


//OpenTrainer class
OpenTrainer ::OpenTrainer(int id, std::vector<Customer *> &customersList):trainerId(id), customers() {
    size_t size = customersList.size();
    for(size_t i =0; i < size; i++){
        customers.push_back(customersList[i]);
    }
}

void OpenTrainer::act(Studio &studio) {
    Trainer *tr =studio.getTrainer(trainerId);
    if(tr == nullptr || tr->isOpen()){
        error("Trainer does not exist or is already open");
    }
    else{
        tr->openTrainer();
        size_t trcap =tr->getCapacity();
        size_t size = customers.size();
        for(size_t i =0 ;(i < size) && (trcap > 0);i++){
            tr->addCustomer(customers[i]);
            trcap--;
        }
        complete();
    }
}

OpenTrainer::~OpenTrainer(){
    size_t size = customers.size();
    for(size_t i = 0; i < size; i++){
        delete customers[i];
        customers[i] = nullptr;
    }
    customers.clear();
}

OpenTrainer::OpenTrainer(const OpenTrainer &other):BaseAction(other), trainerId(other.trainerId), customers(){//nuulptr  delete customer
    size_t size = other.customers.size();
    std::vector<Customer*> customersOther = other.customers;
    for(size_t i = 0; i < size; i++){
        std::string type = customersOther[i]->toString();
        std::string cus_name = customersOther[i]->getName();
        int id = customersOther[i]->getId();
        if (type == "swt") {
            Customer *customer = new SweatyCustomer(cus_name, id);
            customers.push_back(customer);
            customer=nullptr;
        }
        else if (type == "chp") {
            Customer *customer = new CheapCustomer(cus_name, id);
            customers.push_back(customer);
            customer=nullptr;
        } 
        else if (type == "mcl") {
            Customer *customer = new HeavyMuscleCustomer(cus_name, id);
            customers.push_back(customer);
            customer=nullptr;
        } 
        else if (type == "fbd") {
            Customer *customer = new FullBodyCustomer(cus_name, id);
            customers.push_back(customer);
            customer=nullptr;
        }
    }

}
std::string OpenTrainer::toString() const {
    std ::string str = "open " + std::to_string(trainerId) + " ";
    size_t size = customers.size();
    for(size_t i =0 ; i<size ;i++){
        str += customers[i]->getName()+","+customers[i]->toString() + " ";
    }
    return str;
}
std::string OpenTrainer::getType(){
    return type;
}

// BaseAction *OpenTrainer::clone() const {
//     return new OpenTrainer(*this);
// }

std::string OpenTrainer::getErrortoString() const {
    return getErrorMsg();
}


//order class
Order::Order(int id) : trainerId(id){
};

void Order::act(Studio &studio) {
    Trainer *tr = studio.getTrainer(trainerId);
    if(tr == nullptr || !tr->isOpen()) {
        error("Trainer does not exist or is already open");
    }
    else{
        std::vector<Workout> &workoutopt = studio.getWorkoutOptions();
        std::vector<Customer *> customers = tr->getCustomers();
        for (size_t i = 0; i < customers.size(); i++) {
            Customer *cust = customers[i];
            std::vector<int> workoutids = cust->order(workoutopt);
            tr->order(cust->getId(), workoutids, workoutopt);
        }

        std::string outp = "";
            for (size_t i = 0; i < customers.size(); i++) {
                std::vector<int> workoutids = customers[i]->order(workoutopt);
                for (size_t j = 0; j < workoutids.size(); j++) {
                    outp = outp + customers[i]->getName() + " Is Doing " + workoutopt[workoutids[j]].getName();
                    if ((j < workoutids.size()-1 ) || (i < customers.size()-1))
                        outp = outp + "\n";
                }
            }

        std::cout << outp << std::endl;
        complete();

    }
}

Order::Order(const Order &other):BaseAction(other), trainerId(other.trainerId){
}


// BaseAction *Order::clone() const {
//     return new Order(*this);
// }

std::string Order::getErrortoString() const {
    return getErrorMsg();
}


std::string Order::toString() const {
    std::string str ="order ";
    return str + std::to_string(trainerId);
}

std::string Order::getType(){
    return type;
}

//movecustomer
MoveCustomer::MoveCustomer(int src, int dst, int customerId) :srcTrainer(src),dstTrainer(dst),id(customerId){};

MoveCustomer::MoveCustomer(const MoveCustomer &other): BaseAction(other),srcTrainer(other.srcTrainer), dstTrainer(other.dstTrainer), id(other.id){}

void MoveCustomer::act(Studio &studio) {
    Trainer *trainerSrc = studio.getTrainer(srcTrainer);
    Trainer *trainerDst = studio.getTrainer(dstTrainer);
    if (trainerSrc == nullptr || trainerSrc->getCustomer(id) == nullptr|| trainerDst == nullptr || (!trainerSrc->isOpen()) || (!trainerDst->isOpen())) {
        error("cannot move customer");
    }
    else if (trainerSrc->getCustomer(id) == nullptr || (size_t)trainerDst->getCapacity() == trainerDst->getCustomers().size()) {
        error("cannot move customer");
    }
    else {
        int salarySrc = trainerSrc->getSalary();
        Customer *customer = trainerSrc->getCustomer(id);
        std::vector<OrderPair>& orderCustomerSrc = trainerSrc->getOrders();
        std::vector<OrderPair> newOrderPairs;
        Customer *cpy = customer->clone();
        std::vector<int> workout_ids;
        for (size_t i = 0; i < orderCustomerSrc.size(); i++) {
            if (orderCustomerSrc[i].first == id) {
                workout_ids.push_back(orderCustomerSrc[i].second.getId());
                salarySrc  -= orderCustomerSrc[i].second.getPrice();
            }
            else
                newOrderPairs.push_back(orderCustomerSrc[i]);
        }
        trainerDst->addCustomer(cpy);
        trainerSrc->updateOrderList(newOrderPairs);
        trainerDst->order(customer->getId(), workout_ids, studio.getWorkoutOptions());
        trainerSrc->removeCustomer(id);
        trainerSrc->updateSalary(salarySrc);

        if (trainerSrc->getCustomers().size() == 0) { // end of the session.
            Close *closeTr = new Close(this->srcTrainer);
            closeTr->act(studio);
            delete closeTr;
        }
        customer = nullptr;
        delete customer;
        delete cpy;
        complete();
    }
    trainerSrc = nullptr;
    trainerDst = nullptr;
}

// BaseAction *MoveCustomer::clone() const {
//     return new MoveCustomer(*this);
// }

std::string MoveCustomer::getErrortoString() const {
    return getErrorMsg();
}



std::string MoveCustomer::toString() const {
    std::string str ="move ";
    return str + std::to_string(srcTrainer) + " " +std::to_string(dstTrainer) + " "+ std::to_string(id);
};

std::string MoveCustomer::getType(){
    return type;
}

//close class
Close::Close(int id) :trainerId(id){};
void Close::act(Studio &studio) {
    Trainer *tr =studio.getTrainer(trainerId);
    std::string str = "Trainer " + std::to_string(trainerId) + " " +"closed. "+"Salary" +" "; ;
    if(tr == nullptr||!tr->isOpen())
        error("Trainer does not exist or is not open");
    else{
        std::cout << str + std::to_string(tr->getSalary()) + "NIS\n";
        tr->closeTrainer();
        complete();
    }
}

Close::Close(const Close &other): BaseAction(other),trainerId(other.trainerId){}

std::string Close::toString() const {
    std::string str ="close " +std::to_string(trainerId);
    return str;

}
std::string Close::getType(){
    return type;
}

// BaseAction *Close::clone() const {
//     return new Close(*this);
// }

std::string Close::getErrortoString() const {
    return getErrorMsg();
}



//close all class
CloseAll::CloseAll() {};

void CloseAll::act(Studio &studio) {
    size_t size = studio.getTrainersList().size();
    for(size_t i = 0 ; i < size ;i++){
        if(studio.getTrainersList()[i]->isOpen()){
            Close *closeTr = new Close(i);
            closeTr->act(studio);
            delete closeTr;
        }
    }
    complete();
}

CloseAll::CloseAll(const CloseAll &other) :BaseAction(other){};

std::string CloseAll::toString() const {
    std::string str ="closeall";
    return str;
}
std::string CloseAll::getType(){
    return type;
}

std::string CloseAll::getErrortoString() const {
    return getErrorMsg();
}

// BaseAction *CloseAll::clone() const {
//     return new CloseAll(*this);
// }



//PrintWorkotoption
PrintWorkoutOptions::PrintWorkoutOptions() {
};

void PrintWorkoutOptions::act(Studio &studio) {
    std::string str = "";
    std::vector<Workout> works = studio.getWorkoutOptions();
    for(size_t i = 0; i < works.size(); i++){
        Workout work = works[i];
        str+= work.getName() + ", " + std::to_string(work.getType()) +", " + std::to_string(work.getPrice());
        //if(i != works.size())
        str+= "\n";
    }
    complete();
    std::cout << str;
}
PrintWorkoutOptions::PrintWorkoutOptions(const PrintWorkoutOptions &other):BaseAction(other){};

std::string PrintWorkoutOptions::toString() const { // check
    std::string str ="workout_options";
    return str;
};
std::string PrintWorkoutOptions::getType(){
    return type;
}

std::string PrintWorkoutOptions::getErrortoString() const {
    return getErrorMsg();
}
// BaseAction *PrintWorkoutOptions::clone() const {
//     return new PrintWorkoutOptions(*this);
// }

//Print Trainerstatus
PrintTrainerStatus::PrintTrainerStatus(int id) :trainerId(id){};

void PrintTrainerStatus::act(Studio &studio) {
    Trainer *tr = studio.getTrainer(trainerId);
    std::string str = "Trainer " + std::to_string(trainerId)+" status: ";
    if(tr->isOpen()) {
        std::vector<Customer*> customers = tr->getCustomers();
        str += "open\nCustomers:\n";
        for(size_t i = 0; i < customers.size(); i++) {
            str += std::to_string(customers[i]->getId()) + " " + customers[i]->getName() + "\n";
        }
        str += "Orders:\n";
        std::vector<OrderPair>&  trOrders = tr->getOrders();

        for(size_t i = 0; i < trOrders.size(); i++){
            Workout work = trOrders[i].second;
            str+= work.getName() + " " + std::to_string(work.getPrice()) +"NIS " + std::to_string(trOrders[i].first) +"\n";
        }
        str+="Current Trainer's Salary: "+ std::to_string(tr->getSalary()) + "NIS\n";
    }
    else {
        str += "closed\n";
    }
    std::cout << str;
    complete();
};
PrintTrainerStatus::PrintTrainerStatus(const PrintTrainerStatus &other): BaseAction(other),trainerId(other.trainerId){}
std::string PrintTrainerStatus::toString() const {
    std::string str ="status " + std::to_string(trainerId);
    return str;
}

std::string PrintTrainerStatus::getType(){
    return type;
}
std::string PrintTrainerStatus::getErrortoString() const {
    return getErrorMsg();
}
// BaseAction *PrintTrainerStatus::clone() const {
//     return new PrintTrainerStatus(*this);
// }

//print action log

PrintActionsLog::PrintActionsLog() = default;

PrintActionsLog::PrintActionsLog(const PrintActionsLog &other) :BaseAction(other){};

void PrintActionsLog::act(Studio &studio) {
    std::string str = "";
    const std::vector<BaseAction*> logs= studio.getActionsLog();
    size_t s = logs.size();
    for(size_t i =0 ; i< s;i++){
        str = "";
        str+=logs[i]->toString();
        if(logs[i]->getStatus() == COMPLETED) {
            str += "Completed";
        }
        else{
            str += " Error: " + logs[i]->getErrortoString();
        }
            std::cout << str << std::endl;
    }
}


std::string PrintActionsLog::toString() const {
    std::string str ="log";
    return str;
}
std::string PrintActionsLog::getType(){
    return type;
}

std::string PrintActionsLog::getErrortoString() const {
    return getErrorMsg();
}

// BaseAction *PrintActionsLog::clone() const {
//     return new PrintActionsLog(*this);
// }


//backupstudio
BackupStudio::BackupStudio() {};
BackupStudio::~BackupStudio() {};

void BackupStudio::act(Studio &studio) {
    backup = new Studio(studio);
}

BackupStudio::BackupStudio(const BackupStudio &other):BaseAction(other){};

std::string BackupStudio::toString() const {
    std::string str ="backup";
    return str;
}

std::string BackupStudio::getType(){
    return type;
}
std::string BackupStudio::getErrortoString() const {
    return getErrorMsg();
}

// BaseAction *BackupStudio::clone() const {
//     return new BackupStudio(*this);
// }


//restore studio
RestoreStudio::RestoreStudio() {
};

RestoreStudio::RestoreStudio(const RestoreStudio &other) :BaseAction(other){};

void RestoreStudio::act(Studio &studio) {
    if (backup == nullptr){
        BaseAction::error("No backup available");
    }
    else{
        studio = *backup;
    }
}

std::string RestoreStudio::toString() const {
    std::string str ="restore";
    return str;
}

std::string RestoreStudio::getType(){
    return type;
}
std::string RestoreStudio::getErrortoString() const {
    return getErrorMsg();
}
// BaseAction *RestoreStudio::clone() const {
//     return new RestoreStudio(*this);
// }
