#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"


typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity); // done
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    void updateOrderList(std::vector<OrderPair>& list);
    int getSalary();
    bool isOpen();
    void updateSalary(int sal);
    //rule of five
    virtual ~Trainer();
    Trainer(const Trainer &other);
    Trainer& operator=(const Trainer &other);
    Trainer(Trainer &&other);
    Trainer& operator=(Trainer &&other);
    virtual Trainer* clone() const;

private:
    void clean();
    void deepcopy(const Trainer &other);
    void steal(Trainer &other);
    int salary;
    int capacity;
    bool open; // if the trainer's session is in progress.
    //bool statused;
    std::vector<Customer*> customersList; //
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif