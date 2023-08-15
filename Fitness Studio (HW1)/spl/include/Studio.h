#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio{
public:
    Studio();
    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    std::vector<Trainer*> getTrainersList();

    //rule of five
    virtual ~Studio(); // destructor
    Studio(const Studio &other); // copy constructor
    Studio& operator=(const Studio &other);
    Studio(Studio &&other);
    Studio& operator=(Studio &&other);

private:
    void clean();
    void deepcopy(const Studio &other);
    void steal(Studio &other);
    bool open;
    std::vector<Trainer*> trainers; // done
    std::vector<Workout> workout_options; // done
    std::vector<BaseAction*> actionsLog; // done
};

#endif