//
// Created by mahmoud on 04/11/18.
//

#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    virtual ~Customer();
    std::string getName() const;

    virtual Customer* makeNew(const Customer& other)=0;
    int getId() const;
private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;

    Customer* makeNew(const Customer& other);
    ~VegetarianCustomer();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;

    void Ordered();
    bool alreadyOrdered();
    Customer* makeNew(const Customer& other);
    ~CheapCustomer();

private:
    bool ordered;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    void Ordered();
    Customer* makeNew(const Customer& other);

    bool alreadyOrdered();
    ~SpicyCustomer();
private:
    bool ordered;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    Customer* makeNew(const Customer& other);
    ~AlchoholicCustomer();
private:
    std::vector<int> orders;
};


#endif
