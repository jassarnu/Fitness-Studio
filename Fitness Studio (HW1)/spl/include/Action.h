#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"




enum ActionStatus{
    COMPLETED, ERROR
};

//Forward declaration
class Studio;
extern Studio* backup;


class BaseAction{
public:
    virtual std::string getType()=0;
    BaseAction();
    ActionStatus getStatus() const; // done
    BaseAction(const BaseAction &other);
    virtual void act(Studio& studio)=0;
    virtual std::string getErrortoString() const=0;
    virtual std::string toString() const=0;
    virtual ~BaseAction();
    BaseAction& operator=(const BaseAction &other);
    // houn ghayyaret
    // virtual BaseAction* clone() const = 0;


protected:
    void setType(std::string);
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;

private:
    std::string errorMsg;
    ActionStatus status;
    std::string type;
};


class OpenTrainer : public BaseAction {
public:
    OpenTrainer(int id, std::vector<Customer *> &customersList);
    std::string getType();
    void act(Studio &studio);
    std::string toString() const;
    OpenTrainer(const OpenTrainer &other);
    //OpenTrainer* clone() const;
    // BaseAction* clone() const;
    std::string getErrortoString() const;
    virtual ~OpenTrainer();

private:
    const int trainerId;
    std::vector<Customer *> customers;
    std::string type = "open";
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Studio &studio);
    std::string getType();
    std::string toString() const;
    //Order* clone() const;
    Order(const Order &other);
    // BaseAction* clone() const;
    std::string getErrortoString() const;

private:
    const int trainerId;
    std::string type = "order";
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Studio &studio);
    std::string getType();
    //MoveCustomer* clone() const;
    // BaseAction* clone() const;
    std::string toString() const;
    std::string getErrortoString() const;
    MoveCustomer(const MoveCustomer &other);

private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
    std::string type = "move";
};


class Close : public BaseAction {
public:
    Close(int id);
    std::string getType();
    void act(Studio &studio);
    std::string toString() const;
    //Close* clone() const;
    // BaseAction* clone() const;
    std::string getErrortoString() const;
    Close(const Close &other);

private:
    const int trainerId;
    std::string type = "close";
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Studio &studio);
    std::string getType();
    std::string toString() const;
    std::string getErrortoString() const;
//    CloseAll* clone() const;
    // BaseAction* clone() const;
    CloseAll(const CloseAll &other);

private:
    std::string type = "closeall";
};


class PrintWorkoutOptions : public BaseAction {
public:
    PrintWorkoutOptions();
    void act(Studio &studio);
    std::string getType();
    std::string toString() const;
    std::string getErrortoString() const;
    // BaseAction* clone() const;
//    PrintWorkoutOptions* clone() const;
    PrintWorkoutOptions(const PrintWorkoutOptions &other);

private:
    std::string type = "workout_options";
};


class PrintTrainerStatus : public BaseAction {
public:
    PrintTrainerStatus(int id);
    std::string getType();
    void act(Studio &studio);
    std::string toString() const;
    std::string getErrortoString() const;
    // BaseAction* clone() const;
    PrintTrainerStatus(const PrintTrainerStatus &other);
//    PrintTrainerStatus* clone() const;

private:
    const int trainerId;
    std::string type = "status";
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Studio &studio);
    std::string getType();
    std::string toString() const;
    std::string getErrortoString() const;
    // BaseAction* clone() const;
    PrintActionsLog(const PrintActionsLog &other);


private:
    std::string type = "log";
};


class BackupStudio : public BaseAction {
public:
    BackupStudio() ;
    virtual ~BackupStudio();
    BackupStudio(const BackupStudio& other);
    std::string getType();
    void act(Studio &studio);
    std::string toString() const;
    std::string getErrortoString() const;
    // BaseAction* clone() const;
//    BackupStudio* clone() const;
    // BackupStudio(const BackupStudio &other);

private:
    std::string type = "backup";
};


class RestoreStudio : public BaseAction {
public:
    RestoreStudio() ;
    RestoreStudio(const RestoreStudio& other);
    void act(Studio &studio);
    std::string getType();
    std::string toString() const;
    std::string getErrortoString() const;
    // BaseAction* clone() const;
    // RestoreStudio(const RestoreStudio &other);

//    RestoreStudio* clone() const;
private:
    std::string type = "restore";

};


#endif