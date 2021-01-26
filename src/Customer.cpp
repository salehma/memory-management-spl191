//
// Created by mahmoud on 04/11/18.
//
#include <iostream>
#include "../include/Customer.h"

Customer::Customer(std::string c_name, int c_id):  name(c_name),id(c_id) {}
std::string Customer::getName() const {
    return name;
}
int Customer::getId() const {
    return id;
}
Customer::~Customer() {}


////Vegetarian
VegetarianCustomer ::VegetarianCustomer(std::string name, int id):Customer(name, id) {}
std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> a;
    int MaxCost=-1;
    int MaxCostId=-2;
    int VEGId=-1;
    bool b1= false,b2= false;
    for (const auto &i : menu) {
        if(i.getType()==VEG && !b1) {
            VEGId=i.getId();
            b1= true;
        }
        if(i.getType()==BVG){
            b2= true;
            if(MaxCost==-1) {
                MaxCost = i.getPrice();
                MaxCostId= i.getId();
            }
            else if(MaxCost< i.getPrice()) {
                MaxCost = i.getPrice();
                MaxCostId= i.getId();
            }
            else if(MaxCost==i.getPrice()){
                if(i.getId()<MaxCostId)
                    MaxCostId=i.getId();
            }
        }
    }
    if(b1 && b2) {
        a.push_back(VEGId);
        a.push_back(MaxCostId);
    }
    return a;
}
std::string VegetarianCustomer::toString() const {
    std::string s=getName()+",veg";
    return s;
}
Customer* VegetarianCustomer::makeNew(const Customer& other) {

    return new VegetarianCustomer(other.getName(),other.getId());
}

VegetarianCustomer::~VegetarianCustomer() {}


/////Cheap
CheapCustomer ::CheapCustomer(std::string name, int id):Customer(name,id),ordered(false) {}
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> a;
    int MinCost=menu.at(0).getPrice();
    int MinCostId=-2;
    for (const auto &i : menu) {
             if(MinCost> i.getPrice()) {
                 MinCost = i.getPrice();
                 MinCostId = i.getId();
             }
             if(MinCost==i.getPrice()){
                 if(i.getId()<MinCostId)
                     MinCostId=i.getId();
             }
    }
    if(MinCostId!=-2)
        a.push_back(MinCostId);
    return a;
}
std::string CheapCustomer::toString() const {
    std::string s=getName()+",chp";
    return s;}
bool CheapCustomer::alreadyOrdered() {
    return ordered;
}
void CheapCustomer::Ordered() {
    ordered= true;
}
Customer* CheapCustomer::makeNew(const Customer& other) {

    return new CheapCustomer(other.getName(),other.getId());
}

CheapCustomer::~CheapCustomer() {}
////Spicy
SpicyCustomer ::SpicyCustomer(std::string name, int id):Customer(name,id),ordered(false) {

}
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> a;
    int MaxCost=-1;
    int MaxCostId=-2;
    int MinCost=-1;
    int MinCostId=-2;


    for (const auto &i : menu) {
        if(i.getType()==SPC && !ordered){


            if(MaxCost==-1) {
                MaxCost = i.getPrice();
                MaxCostId= i.getId();
            }
            else if(MaxCost< i.getPrice()) {
                MaxCost = i.getPrice();
                MaxCostId= i.getId();
            }
            else if (MaxCost==i.getPrice()){
                if(i.getId()<MaxCostId)
                    MaxCostId=i.getId();
            }
        }
        if(i.getType()==BVG && ordered){
            if(MinCost==-1) {
                MinCost = i.getPrice();
                MinCostId= i.getId();
            }
            else if(MinCost> i.getPrice()) {
                MinCost = i.getPrice();
                MinCostId = i.getId();
            }
            else if(MinCost==i.getPrice()){
                if(i.getId()<MinCostId)
                    MinCostId=i.getId();
            }
        }
    }
   if(MinCostId !=-2) {
       a.push_back(MinCostId);
   }
   if(MaxCostId !=-2){
       a.push_back(MaxCostId);
   }
    return a;
}
std::string SpicyCustomer::toString() const {
    std::string s=getName()+",spc";
    return s;
}
bool SpicyCustomer::alreadyOrdered() {
    return ordered;
}
void SpicyCustomer::Ordered() {
    ordered= true;
}
Customer* SpicyCustomer::makeNew(const Customer& other) {

    return new SpicyCustomer(other.getName(),other.getId());
}


SpicyCustomer::~SpicyCustomer() {}

////Alcoholic
AlchoholicCustomer ::AlchoholicCustomer(std::string name, int id):Customer(name,id) ,orders(){}
std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> a;
    int MinCost=-1;
    int MinCostId=-2;
    //int j=0;
    bool b= false;
    for (const auto &i : menu) {
        for (int order : orders) {
            if(order ==i.getId()) {

                b = true;
            }
        }
        if(i.getType()==ALC && b== false) {
            if(MinCost==-1) {
                MinCost = i.getPrice();
                MinCostId= i.getId();
            }
           else if (MinCost > i.getPrice()) {
                MinCost = i.getPrice();
                MinCostId = i.getId();
            }
           else if (MinCost == i.getPrice()) {
                if (i.getId() < MinCostId)
                    MinCostId = i.getId();
            }
        }
       b= false;
    }
    if(MinCostId!=-2) {
        a.push_back(MinCostId);
        orders.push_back(MinCostId);
    }
    return a;
}
std::string AlchoholicCustomer::toString() const {
    std::string s=getName()+",alc";
    return s;
}

Customer* AlchoholicCustomer::makeNew(const Customer& other) {
    return new AlchoholicCustomer(other.getName(),other.getId());
}
AlchoholicCustomer::~AlchoholicCustomer() {

}