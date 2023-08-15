#include "../include/Customer.h"
#include "../include/Studio.h"
#include <iostream>
#include <algorithm>
#include <vector>

Customer::~Customer() =default ;
Customer::Customer(std::string c_name, int c_id) : workouts(),name(c_name), id(c_id) {
}


std::string Customer::getName() const {
    return name;
}
int Customer::getId() const {
    return id;
}



SweatyCustomer::SweatyCustomer(std::string name, int id): Customer(name, id) {
}


std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std:: vector<int> workout_ids;
    for(size_t i = 0; i < workout_options.size() ; i++){
        if(workout_options[i].getType() == CARDIO ){
            workout_ids.push_back(workout_options[i].getId());
            workouts.push_back(workout_options[i]);
        }
    }
    return workout_ids;
}

SweatyCustomer::~SweatyCustomer() =default ;

std::string SweatyCustomer::toString() const {
    return "swt";
}

SweatyCustomer *SweatyCustomer::clone() const {
    return new SweatyCustomer(*this);
}

CheapCustomer::CheapCustomer(std::string name, int id): Customer(name, id) {}

int Customer::getCheapestWorkout(const std::vector<Workout> &workout_options) {
    int min_price_id = workout_options[0].getId();
    int min_price = workout_options[0].getPrice();
    for(size_t i = 1; i < workout_options.size() ; i++){
        int this_price = workout_options[i].getPrice();
        int this_id = workout_options[i].getId();
        if(min_price > this_price){
            min_price = this_price;
            min_price_id = this_id;
        }
    }
    return min_price_id;
}


CheapCustomer::~CheapCustomer() =default ;

std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    std:: vector<int> workout_ids;
    int min_price_id = getCheapestWorkout(workout_options);
    workout_ids.push_back(min_price_id);
    workouts.push_back(workout_options[min_price_id]);
    return workout_ids;
}

std::string CheapCustomer::toString() const {
    return "chp";
}

CheapCustomer *CheapCustomer::clone() const {
    return new CheapCustomer(*this);
}

HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name, id) {}

HeavyMuscleCustomer::~HeavyMuscleCustomer() =default ;


std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std:: vector<int> workout_ids;
    std:: vector<int> workout_prices;

    for(size_t i = 0; i < workout_options.size(); i++){
        if(workout_options[i].getType() == ANAEROBIC){
            workout_ids.push_back(workout_options[i].getId());
            workout_prices.push_back(workout_options[i].getPrice());
        }
    }

    for(size_t i = 0; i < workout_ids.size(); i++)
        for(size_t j = 0; j < workout_ids.size(); j++){
            if(workout_prices[j] <  workout_prices[i]){
                int price = workout_prices[i];
                workout_prices[i] = workout_prices[j];
                workout_prices[j] = price;
                int id = workout_ids[i];
                workout_ids[i] = workout_ids[j];
                workout_ids[j] = id;
            }
        }
    return workout_ids;
}

std::string HeavyMuscleCustomer::toString() const {
    return "mcl";

}

HeavyMuscleCustomer *HeavyMuscleCustomer::clone() const {
    return new HeavyMuscleCustomer(*this);
}

FullBodyCustomer::FullBodyCustomer(std::string name, int id):Customer(name, id) {}


std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options) {
    std:: vector<int> workout_ids;
    std:: vector<Workout> cardios; // vector of the cardio types.
    std:: vector<Workout> mixed; // vector of the mixed types.
    std:: vector<Workout> anaerobics; // vector the anaerobic types.
    for(size_t i = 0; i < workout_options.size(); i++){
        WorkoutType type = workout_options[i].getType();
        if(type == CARDIO)
            cardios.push_back(workout_options[i]);
        else if(type == MIXED)
            mixed.push_back(workout_options[i]);
        else
            anaerobics.push_back(workout_options[i]);
    }
    workout_ids.push_back(getCheapestWorkout(cardios)); // first he takes the cheapest cardio.
    workout_ids.push_back(mostExpExr(mixed)); // second, he takes the most expensive mix.
    workout_ids.push_back(getCheapestWorkout(anaerobics)); // third, he takes the
    return workout_ids;

}

FullBodyCustomer::~FullBodyCustomer() =default ;

std::string FullBodyCustomer::toString() const {
    return "fbd";
}

FullBodyCustomer *FullBodyCustomer::clone() const {
    return new FullBodyCustomer(*this);
}

int Customer::mostExpExr(const std::vector<Workout> &workout_options) { // -----changed to static-----
    int max_price_id = workout_options[0].getId();
    int max_price = workout_options[0].getPrice();
    for(size_t i = 1; i < workout_options.size() ; i++){
        int this_price = workout_options[i].getPrice();
        int this_id = workout_options[i].getId();
        if(max_price < this_price){
            max_price = this_price;
            max_price_id = this_id;
        }
    }
    return max_price_id;
}