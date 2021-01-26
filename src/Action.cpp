#include <utility>

//
// Created by mahmoud on 04/11/18.
//
#include "../include/Action.h"
#include <string>
#include <iostream>
#include "../include/Restaurant.h"
#include <vector>
#include <typeinfo>
extern Restaurant* backup;


BaseAction::BaseAction():errorMsg(),status(PENDING) {

}
ActionStatus BaseAction:: getStatus()const {
    return status;
}

void BaseAction::complete() {
    status=COMPLETED;
}
void BaseAction::error(std::string errorMsg) {

    this->errorMsg= std::move(errorMsg);
    status=ERROR;
}
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}
BaseAction::~BaseAction() {}


///class opentable
OpenTable ::OpenTable(int id, std::vector<Customer *> &customersList):tableId(id),customers() {
     for (auto customer : customersList) {
         if(dynamic_cast<CheapCustomer*>(customer)) {
             this->customers.push_back(new CheapCustomer(customer->getName(),customer->getId()));
         }
         if(dynamic_cast<VegetarianCustomer*>(customer)) {
             this->customers.push_back(new VegetarianCustomer(customer->getName(),customer->getId()));
         }
         if(dynamic_cast<AlchoholicCustomer*>(customer)) {
             this->customers.push_back(new AlchoholicCustomer(customer->getName(),customer->getId()));
         }
         if(dynamic_cast<SpicyCustomer*>(customer)) {
             this->customers.push_back(new SpicyCustomer(customer->getName(),customer->getId()));
         }
    }
}
void OpenTable::act(Restaurant &restaurant) {
    if(restaurant.getNumOfTables() < tableId){
        this->error("Table does not exist or is already open");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;

    }
    if(restaurant.getTable(tableId)->isOpen()){
        this->error("Table does not exist or is already open");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;

    }

    else if(restaurant.getTable(tableId)->getCapacity() >= (int)customers.size()){
        this->complete();
        restaurant.getTable(tableId)->openTable();

    }
}
int OpenTable::getTableId() {
    return tableId;
}
std::vector<Customer *>& OpenTable::getCustomers() {
    return customers;
}
void OpenTable::setCustomers(std::vector<Customer *> &customersList) {
    this->customers=customersList;
}

std::string OpenTable::toString() const {
    std::string s;
    std::string s1;
    std::string s2;
    for (auto customer : customers) {
        if( dynamic_cast<CheapCustomer*>(customer)){
            s1=",chp";
        }
        else  if(dynamic_cast<VegetarianCustomer*>(customer)){
            s1=",veg";
        }
        else  if( dynamic_cast<SpicyCustomer*>(customer)){
            s1=",spc";
        }
        else{
            s1=",alc";
        }
       s2+=" "+ customer->getName()+s1;

    }
    s="Open "+std::to_string(tableId)+s2;
    return s;
}


OpenTable::~OpenTable() {

    if(customers.size()!=0)
    for (auto customer : customers) {
        delete customer;
    }
}


OpenTable::OpenTable(const OpenTable &other1):tableId(other1.tableId),customers() {
    if(other1.getStatus()==ERROR){
        this->error(other1.getErrorMsg());
    }
    for (auto customer : other1.customers) {
        if(dynamic_cast<CheapCustomer*>(customer)) {
            this->customers.push_back(new CheapCustomer(customer->getName(),customer->getId()));
        }
        if(dynamic_cast<VegetarianCustomer*>(customer)) {
            this->customers.push_back(new VegetarianCustomer(customer->getName(),customer->getId()));
        }
        if(dynamic_cast<AlchoholicCustomer*>(customer)) {
            this->customers.push_back(new AlchoholicCustomer(customer->getName(),customer->getId()));
        }
        if(dynamic_cast<SpicyCustomer*>(customer)) {
            this->customers.push_back(new SpicyCustomer(customer->getName(),customer->getId()));
        }
    }
}

OpenTable::OpenTable(OpenTable &&other):tableId(other.tableId),customers() {

    for (auto &customer : other.customers){
        if(dynamic_cast<CheapCustomer*>(customer)) {
            this->customers.push_back(new CheapCustomer(customer->getName(),customer->getId()));
        }
        if(dynamic_cast<VegetarianCustomer*>(customer)) {
            this->customers.push_back(new VegetarianCustomer(customer->getName(),customer->getId()));
        }
        if(dynamic_cast<AlchoholicCustomer*>(customer)) {
            this->customers.push_back(new AlchoholicCustomer(customer->getName(),customer->getId()));
        }
        if(dynamic_cast<SpicyCustomer*>(customer)) {
            this->customers.push_back(new SpicyCustomer(customer->getName(),customer->getId()));
        }
    }

    other.customers.clear();
    other.customers.shrink_to_fit();

}


//class Order
Order::Order(int id):tableId(id) {
}
void Order::act(Restaurant &restaurant) {
    if(restaurant.getNumOfTables() < tableId){
        this->error("Table does not exist or is not open");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;
    }
    else  if(!(restaurant.getTable(tableId)->isOpen())){
        this->error("Table does not exist or is not open");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;
    }
   else {
        restaurant.getTable(tableId)->order(restaurant.getMenu());

        int i;
        for (i = restaurant.getTable(tableId)->getIndex(); i < (int)restaurant.getTable(tableId)->getOrders().size(); i++) {
            OrderPair op = restaurant.getTable(tableId)->getOrders().at(i);
            std::cout << restaurant.getTable(tableId)->getCustomer(op.first)->getName() + " ordered " +
                         op.second.getName() << std::endl;
        }
        restaurant.getTable(tableId)->setIndex(i);
        this->complete();
    }
}
std::string Order::toString() const {
    std::string s="order "+std::to_string(tableId);
    return s;
}

BaseAction* Order::makeNew1(const Order &other) {
    Order* order=new Order(tableId);
    if(other.getStatus()==ERROR){
        order->error(other.getErrorMsg());
    }
    return order;
}
Order::~Order() {}


//class MoveCustomer

MoveCustomer::MoveCustomer(int src, int dst, int customerId):srcTable(src),dstTable(dst),id(customerId) {}
void MoveCustomer::act(Restaurant &restaurant) {
    if(!(restaurant.getTable(srcTable)->isOpen())){
        this->error("Cannot move customer");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;
    }
    else if(!(restaurant.getTable(dstTable)->isOpen())){
        this->error("Cannot move customer");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;
    }
    else if(restaurant.getNumOfTables() < dstTable){
        this->error("Cannot move customer");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;
    }
    else if(restaurant.getNumOfTables() < srcTable){
        this->error("Cannot move customer");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;
    }
    else if(restaurant.getTable(srcTable)->getCustomer(id)== nullptr){
        this->error("Cannot move customer");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;
    }
    else if(restaurant.getTable(dstTable)->getCapacity()== (int)restaurant.getTable(dstTable)->getCustomers().size()+1 ){
        this->error("Cannot move customer");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;
    }
    else{
        restaurant.getTable(dstTable)->addCustomer(restaurant.getTable(srcTable)->getCustomer(id));
        restaurant.getTable(srcTable)->removeCustomer(id);
        std::vector<OrderPair> op=restaurant.getTable(srcTable)->getOrdersofCustomer(id);
        restaurant.getTable(dstTable)->addOrders(op);
        int index=restaurant.getTable(dstTable)->getIndex();
        restaurant.getTable(dstTable)->setIndex(index+op.size());
        int index1=restaurant.getTable(srcTable)->getIndex();
        restaurant.getTable(srcTable)->setIndex(index1-op.size());
        this->complete();
    }
}
std::string MoveCustomer::toString() const {
    std::string s="move "+std::to_string(srcTable)+" "+std::to_string(dstTable)+" "+std::to_string(id);
    return s;
}

BaseAction* MoveCustomer::makeNew1(const MoveCustomer &other) {
    MoveCustomer* move=new MoveCustomer(srcTable,dstTable,id);
    if(other.getStatus()==ERROR){
        move->error(other.getErrorMsg());
    }
    return move;
}
MoveCustomer::~MoveCustomer() {}


//class Close
Close::Close(int id):tableId(id) {}
void Close::act(Restaurant &restaurant) {
    int bill=restaurant.getTable(tableId)->getBill();
    restaurant.getTable(tableId)->closeTable();
    std::cout<<"Table ";
    std::cout<<tableId;
    std::cout<<" was closed. Bill ";
    std::cout<<bill;
    std::cout<<"NIS"<<std::endl;
    this->complete();
}
std::string Close::toString() const {
    std::string s="close "+std::to_string(tableId);
    return s;
}
BaseAction* Close::makeNew1(const Close &other) {
    Close* close=new Close(tableId);
    if(other.getStatus()==ERROR){
        close->error(other.getErrorMsg());
    }
    return close;
}
Close::~Close() {}



//class CloseAll
CloseAll::CloseAll() {}
void CloseAll::act(Restaurant &restaurant) {
    for(int i=1;i<restaurant.getNumOfTables()-1;i++){
        if(restaurant.getTable(i)->isOpen()){
        int bill=restaurant.getTable(i)->getBill();
        restaurant.getTable(i)->closeTable();
        std::cout<<"Table ";
        std::cout<<i;
        std::cout<<" was closed. Bill ";
        std::cout<<bill;
        std::cout<<"NIS"<<std::endl;
        }
    }
    this->complete();
}
std::string CloseAll::toString() const {
    std::string s="closeall";
    return s;
}

BaseAction* CloseAll::makeNew1(const CloseAll &other) {
    CloseAll* closeall=new CloseAll();
    if(other.getStatus()==ERROR){
        closeall->error(other.getErrorMsg());
    }
    return closeall;
}
CloseAll::~CloseAll() {

}

//class PrintMenu
PrintMenu::PrintMenu(){}
void PrintMenu::act(Restaurant &restaurant) {
    for (auto &i : restaurant.getMenu()) {
        std::string name= i.getName();
        DishType dishTybe= i.getType();
        std::string tybe;
        if(dishTybe==VEG){
            tybe="VEG";
        }
        if(dishTybe==ALC){
            tybe="ALC";
        }
        if(dishTybe==SPC){
            tybe="SPC";
        }
        if(dishTybe==BVG){
            tybe="BVG";
        }
        int price= i.getPrice();
        std::cout<<name + " " + tybe+" ";
        std::cout<<price;
        std::cout<<"NIS"<<std::endl;
    }
    this->complete();
}
std::string PrintMenu::toString() const {
    std::string s="menu";
    return s;
}

BaseAction* PrintMenu::makeNew1(const PrintMenu &other) {
    PrintMenu* pm=new PrintMenu();
    if(other.getStatus()==ERROR){
        pm->error(other.getErrorMsg());
    }
    return pm;
}
PrintMenu::~PrintMenu() {}

//class PrintTableStatus
PrintTableStatus ::PrintTableStatus(int id):tableId(id) {}
void PrintTableStatus::act(Restaurant &restaurant) {

    bool b=restaurant.getTable(tableId)->isOpen();
    if(!b) {
        std::cout << "table ";
        std::cout<<tableId;
        std::cout<<" status: closed"<<std::endl;
    }
    else{
        std::cout << "table ";
        std::cout<<tableId;
        std::cout<<" status: open"<<std::endl;
        std::cout << "Customers:"<<std::endl;
        for (auto &customer : restaurant.getTable(tableId)->getCustomers()) {
            std::cout << customer->getId();
            std::cout<<" "+customer->getName()<<std::endl;
        }
        std::cout << "Orders:"<<std::endl;
        for (auto &order : restaurant.getTable(tableId)->getOrders()) {
            std::cout<<order.second.getName()+" ";
            std::cout<<order.second.getPrice();
            std::cout<<"NIS ";
            std::cout<<order.first<<std::endl;
        }
        std::cout<<"Current bill: ";
        std::cout<<restaurant.getTable(tableId)->getBill();
        std::cout<<"NIS"<<std::endl;
        }
    this->complete();
}
std::string PrintTableStatus::toString() const {
    std::string s="status "+std::to_string(tableId);
    return s;
}

BaseAction* PrintTableStatus::makeNew1(const PrintTableStatus &other) {
    PrintTableStatus* pts=new PrintTableStatus(tableId);
    if(other.getStatus()==ERROR){
        pts->error(other.getErrorMsg());
    }
    return pts;
}
PrintTableStatus::~PrintTableStatus() {}

//class PrintActionsLog
PrintActionsLog::PrintActionsLog() {}
void PrintActionsLog::act(Restaurant &restaurant) {
    for (auto i : restaurant.getActionsLog()) {

        if(i->getStatus()==ERROR){
            if( dynamic_cast<OpenTable*>(i)){
                std::cout<< i->toString()+" Error: Table does not exist or is already open"<<std::endl;

            }
            if( dynamic_cast<Order*>(i)){
                std::cout<< i->toString()+" Error: Table does not exist or is not open"<<std::endl;
            }
            if( dynamic_cast<MoveCustomer*>(i)){
                std::cout<< i->toString()+" Error: Cannot move customer"<<std::endl;
            }
            if( dynamic_cast<RestoreResturant*>(i)){
                std::cout<< i->toString()+" Error: No backup available"<<std::endl;
            }

        } else if(i->getStatus()!=ERROR)
        std::cout<< i->toString()+ " Completed "<< std::endl ;
    }
}
std::string PrintActionsLog::toString() const {
    std::string s="log";
    return s;
}

BaseAction* PrintActionsLog::makeNew1(const PrintActionsLog &other) {
    PrintActionsLog* pal=new PrintActionsLog();
    if(other.getStatus()==ERROR){
        pal->error(other.getErrorMsg());
    }
    return pal;
}

PrintActionsLog::~PrintActionsLog() {}

//class BackupRestaurant
BackupRestaurant::BackupRestaurant() {}
void BackupRestaurant::act(Restaurant &restaurant) {


    complete();
    backup=new Restaurant(restaurant);

}
std::string BackupRestaurant::toString() const {
    std::string s="backup";
    return s;
}

BaseAction* BackupRestaurant::makeNew1(const BackupRestaurant &other) {
    BackupRestaurant* br=new BackupRestaurant();
    if(other.getStatus()==ERROR){
        br->error(other.getErrorMsg());
    }
    return br;
}
BackupRestaurant::~BackupRestaurant() {}


//class RestoreResturant
RestoreResturant::RestoreResturant() {}

void RestoreResturant::act(Restaurant &restaurant) {
    if(backup== nullptr){
        this->error("No backup available");
        std::cout<<"Error: "+getErrorMsg()<<std::endl;
    }
    else{
        complete();
        restaurant=*backup;

    }

}
std::string RestoreResturant::toString() const {
    std::string s="restore";
    return s;
}

BaseAction* RestoreResturant::makeNew1(const RestoreResturant &other) {
    RestoreResturant* r=new RestoreResturant();
    if(other.getStatus()==ERROR){
        r->error(other.getErrorMsg());
    }
    return r;
}
RestoreResturant::~RestoreResturant() {}
