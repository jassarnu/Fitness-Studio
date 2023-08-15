#include "../include/Trainer.h"
#include <algorithm>
#include <iostream>

Trainer::Trainer(int t_capacity):salary(0), capacity(t_capacity),open(false),customersList(),orderList(){
}


int Trainer::getCapacity() const {
    return capacity;
}


void Trainer::addCustomer(Customer *customer) {
    size_t t = capacity;
    std::string type = customer->toString();
    int id = customer->getId();
    std::string name = customer->getName();
    if(customersList.size() < t) {
        if(type == "swt"){
            SweatyCustomer *cus = new SweatyCustomer(name, id);
            customersList.push_back(cus);
            cus = nullptr;

        }       
        else if(type == "fbd"){
            FullBodyCustomer *cus = new FullBodyCustomer(name, id);
            customersList.push_back(cus);
            cus = nullptr;
        }

        else if(type == "mcl"){
            HeavyMuscleCustomer *cus = new HeavyMuscleCustomer(name, id);
            customersList.push_back(cus);
            cus = nullptr;
        }
        else{
            CheapCustomer *cus = new CheapCustomer(name, id);
            customersList.push_back(cus);
            cus = nullptr;
        }              
    }
}

void Trainer::updateSalary(int sal) {
    salary = sal;
}

void Trainer::removeCustomer(int id) {
    std::vector<OrderPair> tempo_orders;
    for(size_t i = 0; i < customersList.size(); i++){
        if(customersList[i]->getId() == id){
            delete customersList[i];
            customersList[i] = nullptr;
            customersList.erase(customersList.begin() + i);
            break;
        }
    }
    for(size_t i = 0; i < orderList.size(); i++){
        if(orderList[i].first != id)
            tempo_orders.push_back(orderList[i]);
    }
    updateOrderList(tempo_orders);
}

Customer *Trainer::getCustomer(int id) {
    for(size_t i = 0; i < customersList.size(); i++) {
        if (customersList[i]->getId() == id)
            return customersList[i];
    }
    return nullptr;
}

std::vector<Customer *> &Trainer::getCustomers() {
    return customersList;
}
std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;
}

void
Trainer::order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout> &workout_options) {
        for (size_t i = 0; i < workout_ids.size(); i++) {
            orderList.push_back(std::make_pair(customer_id, workout_options[workout_ids[i]])); // <customer_id, Workout>
            salary = salary + workout_options[workout_ids[i]].getPrice();
        }
    
}

void Trainer::openTrainer() { //check
    open =true;
}

void Trainer::closeTrainer() {//check
    for(size_t i =0; i < customersList.size(); i++)
        delete customersList[i];
    customersList.clear();
    orderList.clear();
    open = false;
}

void Trainer::updateOrderList(std::vector<OrderPair>& list) {
    orderList.clear();
    for(OrderPair pair: list){
        orderList.push_back(pair);
    }
}

int Trainer::getSalary() {
    return salary;
}

bool Trainer::isOpen() {
    return open;
}


Trainer::~Trainer() {//des
    clean();
}

Trainer::Trainer(const Trainer &other) :salary(),capacity(),open(),customersList(),orderList() {//copy constructor
    deepcopy(other);
}

Trainer::Trainer(Trainer &&other):salary(),capacity(),open(),customersList(),orderList() { // move constructor
    steal(other);
}

Trainer &Trainer::operator=(const Trainer &other) {//copy ass opertaor
    if(this!=&other){
        clean();
        deepcopy(other);
    }
    return (*this);
}

Trainer &Trainer::operator=(Trainer &&other) {//move ass operator
    if(this!= &other){
        clean();
        steal(other);
    }
    return (*this);

}

void Trainer::clean() {
    size_t sizecu =customersList.size();
    if(sizecu != 0){
    for(size_t i =0 ;i<sizecu;i++){
        delete customersList[i];
        customersList[i]= nullptr;
    }
    }
    this->salary=0;
    this->capacity=0;
    this->open=false;
    orderList.clear();
    customersList.clear();
    }

void Trainer::deepcopy(const Trainer &other) {
    this->salary=other.salary;
    this->capacity=other.capacity;
    this->open=other.open;
    for(Customer* cus: other.customersList){
        Customer* m =cus->clone();
        customersList.push_back(m);
        m=nullptr;
    }
    size_t sizepa =orderList.size();
    for(size_t i =0 ;i<sizepa;i++){
        OrderPair pair ={ other.orderList[i].first, other.orderList[i].second};
        this->orderList.push_back(pair);
    }
}

//     size_t sizecu =customersList.size();
// //     for(size_t i =0 ;i<sizecu;i++){
// //         Customer* cusr =other.customersList[i]->clone();
// //         this->customersList.push_back(cusr);
// //         cusr= nullptr;
// //     }
// //     size_t sizepa =orderList.size();
// //     for(size_t i =0 ;i<sizepa;i++){
// //         OrderPair pair =other.orderList[i];
// //         this->orderList.push_back(pair);
// //     }
// // }

Trainer *Trainer::clone() const {
    return new Trainer(*this);
}
void Trainer::steal(Trainer &other) {
    this->salary=other.salary;
    this->capacity=other.capacity;
    this->open=other.open;
    size_t os =orderList.size();
    size_t cus =other.customersList.size();
    for(size_t i =0 ; i<cus;i++) {
     this->customersList.push_back(other.customersList[i]);
    other.customersList[i]= nullptr;
    }
    for(size_t i =0;i<os;i++){
        OrderPair pair =other.orderList[i];
        this->orderList.push_back(pair);
    }
    other.customersList.clear();
    other.orderList.clear();
}