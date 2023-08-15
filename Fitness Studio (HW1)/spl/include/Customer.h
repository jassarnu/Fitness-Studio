#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"
class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    virtual Customer* clone() const = 0;
    std::string getName() const;
    int getId() const;
    virtual ~Customer() ;

protected:
    int getCheapestWorkout(const std::vector<Workout> &workout_options);
    int mostExpExr(const std::vector<Workout> &workout_options);
    std::vector<Workout> workouts;
private:

    const std::string name;
    const int id;
};


class SweatyCustomer : public Customer {
public:
    SweatyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    SweatyCustomer* clone() const;
    virtual ~SweatyCustomer() ;
private:

};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    CheapCustomer* clone() const;
    virtual ~CheapCustomer() ;

private:
};


class HeavyMuscleCustomer : public Customer {
public:
    HeavyMuscleCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    HeavyMuscleCustomer* clone() const;
    virtual ~HeavyMuscleCustomer() ;
    

private:
};


class FullBodyCustomer : public Customer {
public:
    FullBodyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    FullBodyCustomer* clone() const;
    virtual ~FullBodyCustomer() ;
private:
};


#endif