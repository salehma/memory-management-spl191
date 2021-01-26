//
// Created by mahmoud on 04/11/18.
//

#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    //rull of 5
    virtual ~Table();   //destructor
    Table(const Table& other);///copy constructor
    Table& operator=(const Table& other);  //copy assignment operator
    Table(Table&& other);  //move constructor
    Table& operator=(Table&& other);  //move assignment operator

    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    std::vector<OrderPair> getOrdersofCustomer(int id);
    void addOrders(std::vector<OrderPair> op);
    void order(const std::vector<Dish> &menu); //
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    int getIndex();
    void setIndex(int i);
private:

    int capacity;
    int index;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif