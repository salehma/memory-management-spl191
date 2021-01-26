//
// Created by mahmoud on 04/11/18.
//
#include <typeinfo>
#include "../include/Table.h"
#include <iostream>

Table::Table(int t_capacity): capacity(t_capacity),index(0),open(false),customersList(),orderList() {

}
int Table::getIndex() {
    return index;
}
void Table::setIndex(int i){
    index=i;
}
int Table::getCapacity() const {
    return capacity;
}
void Table::addCustomer(Customer *customer) {
    this->customersList.push_back(customer);
}

Table::~Table() {


    if(customersList.size()!=0)
    for (auto customer : customersList) {

       // std::cout<<"table d"<<std::endl;
        if(customer!= nullptr)
        delete customer;
    }
    customersList.clear();
    customersList.shrink_to_fit();
}

Table::Table(const Table &other):capacity(other.capacity),index(other.index),open(other.open),customersList(),orderList(other.orderList) {

    for (auto customer : other.customersList) {
        if(dynamic_cast<CheapCustomer*>(customer)) {
            this->customersList.push_back(new CheapCustomer(customer->getName(),customer->getId()));
        }
        if(dynamic_cast<VegetarianCustomer*>(customer)) {
            this->customersList.push_back(new VegetarianCustomer(customer->getName(),customer->getId()));
        }
        if(dynamic_cast<AlchoholicCustomer*>(customer)) {
            this->customersList.push_back(new AlchoholicCustomer(customer->getName(),customer->getId()));
        }
        if(dynamic_cast<SpicyCustomer*>(customer)) {
            this->customersList.push_back(new SpicyCustomer(customer->getName(),customer->getId()));
        }
    }
}

Table& Table::operator=(const Table &other) {
    if(this!= &other){
        index=other.index;
        capacity=other.capacity;
        open=other.open;
        for (auto &i : customersList) {
            delete i;
        }
        customersList.clear();
        for (auto &customer : other.customersList){
            if(dynamic_cast<CheapCustomer*>(customer)) {
                this->customersList.push_back(new CheapCustomer(customer->getName(),customer->getId()));
            }
            if(dynamic_cast<VegetarianCustomer*>(customer)) {
                this->customersList.push_back(new VegetarianCustomer(customer->getName(),customer->getId()));
            }
            if(dynamic_cast<AlchoholicCustomer*>(customer)) {
                this->customersList.push_back(new AlchoholicCustomer(customer->getName(),customer->getId()));
            }
            if(dynamic_cast<SpicyCustomer*>(customer)) {
                this->customersList.push_back(new SpicyCustomer(customer->getName(),customer->getId()));
            }
        }

    }
    return (*this);
}

Table::Table(Table &&other):capacity(other.capacity),index(other.index),open(other.open),customersList(),orderList(other.orderList) {

    for (auto &customer : other.customersList){
        if(dynamic_cast<CheapCustomer*>(customer)) {
            this->customersList.push_back(new CheapCustomer(customer->getName(),customer->getId()));
        }
        if(dynamic_cast<VegetarianCustomer*>(customer)) {
            this->customersList.push_back(new VegetarianCustomer(customer->getName(),customer->getId()));
        }
        if(dynamic_cast<AlchoholicCustomer*>(customer)) {
            this->customersList.push_back(new AlchoholicCustomer(customer->getName(),customer->getId()));
        }
        if(dynamic_cast<SpicyCustomer*>(customer)) {
            this->customersList.push_back(new SpicyCustomer(customer->getName(),customer->getId()));
        }
    }
    other.index=0;
    other.capacity=0;
    other.open=false;
    other.customersList.clear();
    other.customersList.shrink_to_fit();
    other.orderList.clear();
    other.orderList.shrink_to_fit();
}

Table& Table::operator=(Table &&other) {

    if(this!= &other){
        index=other.index;
        capacity=other.capacity;
        open=other.open;
        for (auto &customer : other.customersList){
            if(dynamic_cast<CheapCustomer*>(customer)) {
                this->customersList.push_back(new CheapCustomer(customer->getName(),customer->getId()));
            }
            if(dynamic_cast<VegetarianCustomer*>(customer)) {
                this->customersList.push_back(new VegetarianCustomer(customer->getName(),customer->getId()));
            }
            if(dynamic_cast<AlchoholicCustomer*>(customer)) {
                this->customersList.push_back(new AlchoholicCustomer(customer->getName(),customer->getId()));
            }
            if(dynamic_cast<SpicyCustomer*>(customer)) {
                this->customersList.push_back(new SpicyCustomer(customer->getName(),customer->getId()));
            }
        }
        for (auto &order : other.orderList){
            orderList.push_back(order);
        }
        other.index=0;
        other.capacity=0;
        other.open=false;
        other.customersList.clear();
        other.customersList.shrink_to_fit();
        other.orderList.clear();
        other.orderList.shrink_to_fit();
    }
    return (*this);
}

void Table::removeCustomer(int id) {
    auto customersList1=this->customersList;
    this->customersList.clear();
    for (const auto i : customersList1) {
        if(i->getId()!=id){
            this->customersList.push_back(i);
        }
    }
}

Customer* Table::getCustomer(int id) {
    for (const auto &i : customersList) {
        if(i->getId()==id){
            return i;
        }
    }
    return nullptr;
}

std::vector<Customer*>& Table::getCustomers() {
    return this-> customersList;
}

std::vector<OrderPair>& Table::getOrders() {
    return this->orderList;
}

void Table::order(const std::vector<Dish> &menu) {
    for (auto customer : customersList) {
        std::vector<int> orders=customer->order(menu);
        int a= static_cast<int>(orders.size());
        for(int i=0;i<a;i++){
            OrderPair op(customer->getId(),menu.at(static_cast<unsigned long>(orders.at(
                    static_cast<unsigned long>(i)))));
            if(auto ch= dynamic_cast<CheapCustomer*>(customer)) {
                if(!(ch->alreadyOrdered())){
                    orderList.push_back(op);
                }
                ch->Ordered();
            }
            else if(auto ch1= dynamic_cast<SpicyCustomer*>(customer)) {
                orderList.push_back(op);
                ch1->Ordered();
            }else
            orderList.push_back(op);
        }
    }
}

void Table::openTable() {
    this->open= true;
}

void Table::closeTable() {
    this->open= false;
}

int Table::getBill() {
    int sum=0;

    for (const auto &i : orderList) {
        sum+=i.second.getPrice();
    }

    return sum;

    }

bool Table::isOpen() {
    return this->open;
}

std::vector<OrderPair> Table::getOrdersofCustomer(int id) {
    std::vector<OrderPair> op;

    for (const auto &i : orderList) {
        if(i.first==id)
            op.push_back(i);
    }
    std::vector<OrderPair> op1=this->orderList;
    orderList.clear();
    for (const auto &i : op1){
        if(i.first!=id)
            orderList.push_back(i);
    }
    return op;
}


void Table::addOrders(std::vector<OrderPair> op) {
    for (const auto &i : op) {
        orderList.push_back(i);
    }
}