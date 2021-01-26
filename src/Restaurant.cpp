//
// Created by mahmoud on 04/11/18.
//

#include "../include/Restaurant.h"
#include <fstream>
#include <iostream>


Restaurant::Restaurant():open(false),tables(),menu(),actionsLog() {}
Restaurant::~Restaurant() {
    for (auto table : tables) {
        delete table;
    }
    for(auto action:actionsLog){
        delete action;
    }
}

Restaurant::Restaurant(const Restaurant &other):open(other.open),tables(),menu(other.menu),actionsLog() {
    for (auto table : other.tables) {
        tables.push_back(new Table(*table));
    }
    for(auto &action:other.actionsLog){
        if(auto ch= dynamic_cast<OpenTable*>(action)){
          ch=new OpenTable(*ch);
            actionsLog.push_back(ch);
        }
        if(auto ch= dynamic_cast<Order*>(action)){
            actionsLog.push_back(ch->makeNew1(*ch));
        }
        if(auto ch= dynamic_cast<MoveCustomer*>(action)){
            actionsLog.push_back(ch->makeNew1(*ch));
        }
        if(auto ch= dynamic_cast<PrintActionsLog*>(action)){
            actionsLog.push_back(ch->makeNew1(*ch));
        }
        if(auto ch= dynamic_cast<PrintTableStatus*>(action)){
            actionsLog.push_back(ch->makeNew1(*ch));
        }
        if(auto ch= dynamic_cast<Close*>(action)){
            actionsLog.push_back(ch->makeNew1(*ch));
        }
        if(auto ch= dynamic_cast<CloseAll*>(action)){
            actionsLog.push_back(ch->makeNew1(*ch));
        }
        if(auto ch= dynamic_cast<BackupRestaurant*>(action)){
            actionsLog.push_back(ch->makeNew1(*ch));
        }
        if(auto ch= dynamic_cast<RestoreResturant*>(action)){
            actionsLog.push_back(ch->makeNew1(*ch));
        }
        if(auto ch= dynamic_cast<PrintMenu*>(action)){
            actionsLog.push_back(ch->makeNew1(*ch));
        }
    }
}

Restaurant& Restaurant::operator=(const Restaurant& other) {
    if(this!= &other){
        open=other.open;
        menu.clear();
        menu.shrink_to_fit();
        for (auto &dish : other.menu) {
            menu.push_back(dish);
        }
        for (auto &table : tables) {
            delete table;
        }

        tables.clear();
        for (auto table : other.tables) {
            tables.push_back(new Table(*table));

        }
        for(auto &action:actionsLog){
            delete action;
        }
        actionsLog.clear();
        for(auto action:other.actionsLog){
            if(auto ch= dynamic_cast<OpenTable*>(action)){
                ch=new OpenTable(*ch);
                actionsLog.push_back(ch);
            }
            if(auto ch= dynamic_cast<Order*>(action)){
                actionsLog.push_back(ch->makeNew1(*ch));
            }
            if(auto ch= dynamic_cast<MoveCustomer*>(action)){
                actionsLog.push_back(ch->makeNew1(*ch));
            }
            if(auto ch= dynamic_cast<PrintActionsLog*>(action)){
                actionsLog.push_back(ch->makeNew1(*ch));
            }
            if(auto ch= dynamic_cast<PrintTableStatus*>(action)){
                actionsLog.push_back(ch->makeNew1(*ch));
            }
            if(auto ch= dynamic_cast<Close*>(action)){
                actionsLog.push_back(ch->makeNew1(*ch));
            }
            if(auto ch= dynamic_cast<CloseAll*>(action)){
                actionsLog.push_back(ch->makeNew1(*ch));
            }
            if(auto ch= dynamic_cast<BackupRestaurant*>(action)){
                actionsLog.push_back(ch->makeNew1(*ch));
            }
            if(auto ch= dynamic_cast<RestoreResturant*>(action)){
                actionsLog.push_back(ch->makeNew1(*ch));
            }
            if(auto ch= dynamic_cast<PrintMenu*>(action)){
                actionsLog.push_back(ch->makeNew1(*ch));
            }
        }

    }
    return (*this);
}

Restaurant::Restaurant(Restaurant &&other):open(other.open),tables(other.tables),menu(other.menu),actionsLog(other.actionsLog) {
    other.open= false;
    other.menu.clear();
    other.menu.shrink_to_fit();
    other.actionsLog.clear();
    other.actionsLog.shrink_to_fit();
    other.tables.clear();
    other.tables.shrink_to_fit();
}

Restaurant& Restaurant::operator=(Restaurant &&other) {
    if(this!= &other) {
        open=other.open;
        for (auto &dish : other.menu) {
            menu.push_back(dish);
        }
        tables=other.tables;
        actionsLog=other.actionsLog;
        other.open= false;
        other.menu.clear();
        other.menu.shrink_to_fit();
        other.actionsLog.clear();
        other.actionsLog.shrink_to_fit();
        other.tables.clear();
        other.tables.shrink_to_fit();
    }
    return (*this);
}





Restaurant::Restaurant(const std::string &configFilePath):open(false),tables(),menu(),actionsLog() {
    std::ifstream infile(configFilePath);
    std::string line;
    tables.push_back(new Table(0));
    while(infile>>line){
        if(line=="tables") {
            infile>>line;
        }
        if(line=="description") {
            infile>>line;
            for (int i=0; i<(int)line.length();i++){

                Table *t= new Table(std::stoi(line.substr(0,line.find(','))));
                tables.push_back(t);
                line=line.substr(line.find(',')+1,line.length()-1);
            }

            Table *t1= new Table(std::stoi(line.substr(0,line.find(','))));
            tables.push_back(t1);
        }
    }
    std::string str;
    std::string myFile;
    std::ifstream infile1(configFilePath);
    while (std::getline(infile1, str)) {
        myFile+=str+";";
    }

    std::vector<std::string> myv;
    std::string myMenu=myFile.substr(myFile.find("#Menu")+6,myFile.length()-1);
    int numOfLines=0;
    for(int i=0 ;i<(int)myMenu.length();i++){
        if(myMenu.at(i)==';')
            numOfLines++;
    }
    std::string s=myMenu;
    for(int i=0;i<numOfLines;i++){
        myv.push_back(s.substr(0, s.find(';')));
        s = s.substr(s.find(';') + 1, s.length() - 1);
    }
    int j=0;
    for (int i=1;i<(int)myv.size();i++) {
       DishType dt;
        std::string name;
        name=myv.at(i).substr(0, myv.at(i).find(','));
        myv.at(i) = myv.at(i).substr(myv.at(i).find(',') + 1, myv.at(i).length() - 1);
        if(myv.at(i).substr(0, myv.at(i).find(','))=="VEG")
            dt=VEG;
        if(myv.at(i).substr(0, myv.at(i).find(','))=="SPC")
            dt=SPC;
        if(myv.at(i).substr(0, myv.at(i).find(','))=="BVG")
            dt=BVG;
        if(myv.at(i).substr(0, myv.at(i).find(','))=="ALC")
            dt=ALC;
        myv.at(i) = myv.at(i).substr(myv.at(i).find(',') + 1, myv.at(i).length() - 1);
        Dish d(j,name,std::stoi(myv.at(i).substr(0, myv.at(i).find(','))),dt);
        menu.push_back(d);
        j++;
    }

}
void Restaurant::start() {

    std::cout<<"Restaurant is now open!"<<std::endl;
    std::string action;
    int id=0;
    bool b= false;
     while (!b){
         getline (std::cin, action);
        if(action.find("open")!=std::string::npos){
            std::string action1=action.substr(5);
            int tableId=std::stoi(action1.substr(0, action1.find(' ')));
            action1=action1.substr(action1.find(' ')+1);
            std::vector<Customer*> customersList;
            for(int i=0;i<(int)action1.length();i++){
                std::string customerName=action1.substr(0,action1.find(','));
                std::string customerType=action1.substr(action1.find(',')+1,3);
                if(action1.find(' ')<10000) {
                    action1 = action1.substr(action1.find(' ')+1);
                }
                else i= static_cast<int>(action1.length() - 1);
                if(customerType=="veg"){
                    VegetarianCustomer *vc=new VegetarianCustomer(customerName,id);
                    customersList.push_back(vc);
                }
                if(customerType=="chp"){
                    CheapCustomer *cc=new CheapCustomer(customerName,id);
                    customersList.push_back(cc);
                }
                if(customerType=="alc"){
                    AlchoholicCustomer *ac=new AlchoholicCustomer(customerName,id);
                    customersList.push_back(ac);
                }
                if(customerType=="spc"){
                    SpicyCustomer *sc=new SpicyCustomer(customerName,id);
                    customersList.push_back(sc);
                }

                id++;
            }
            OpenTable *ot=new OpenTable(tableId,customersList);
            ot->act(*this);
            if(ot->getStatus()==COMPLETED) {
                for (auto customer : customersList) {
                    this->getTable(tableId)->addCustomer(customer);
                }
            }
            else{
                for (auto customer : customersList) {
                    delete(customer);
                }
            }
            actionsLog.push_back(ot);

         }
        if(action.find("order")!=std::string::npos){

            int tableId=std::stoi(action.substr(6));
            BaseAction *order=new Order(tableId);
            order->act(*this);
            actionsLog.push_back(order);
        }

        if(action.find("move") != std::string::npos){
            std::string action1=action.substr(5);
            int src=std::stoi(action1.substr(0,action1.find(' ')));
            action1=action1.substr(action1.find(' ')+1);
            int dst=std::stoi(action1.substr(0,action1.find(' ')));
            action1=action1.substr(action1.find(' ')+1);
            int customerId=std::stoi(action1);
            BaseAction *mc=new MoveCustomer(src,dst,customerId);
            mc->act(*this);
            actionsLog.push_back(mc);
        }
        if(action.find("close ") != std::string::npos){
            std::string action1=action.substr(6);
            int tableId=std::stoi(action1);
            BaseAction *close=new Close(tableId);
            close->act(*this);
            actionsLog.push_back(close);
        }
        if(action.find("closeall") != std::string::npos){
            BaseAction *closeAll=new CloseAll();
            closeAll->act(*this);
            actionsLog.push_back(closeAll);
            b= true;
        }
        if(action.find("menu") != std::string::npos){
            BaseAction *pm=new PrintMenu();
            pm->act(*this);
            actionsLog.push_back(pm);
        }
        if(action.find("status") != std::string::npos){
            std::string action1=action.substr(7);
            int tableId=std::stoi(action1);
            BaseAction *status= new PrintTableStatus(tableId);
            status->act(*this);
            actionsLog.push_back(status);
        }
        if(action.find("log") != std::string::npos){
            BaseAction *pl=new PrintActionsLog();
            pl->act(*this);
            actionsLog.push_back(pl);
        }
        if(action.find("backup") != std::string::npos){

            BaseAction *backup=new BackupRestaurant();
            actionsLog.push_back(backup);
            backup->act(*this);

        }
        if(action.find("restore") != std::string::npos){
            BaseAction *restore=new RestoreResturant();
            restore->act(*this);
            actionsLog.push_back(restore);
        }



    }


}
int Restaurant::getNumOfTables() const {
    return (int)tables.size()-1;
}
Table* Restaurant::getTable(int ind) {
    return tables.at(static_cast<unsigned long>(ind));
}
const std::vector<BaseAction*>& Restaurant::getActionsLog() const {
    return actionsLog;
}

std::vector<Dish>& Restaurant::getMenu() {
    return menu;
}