//
// Created by mahmoud on 04/11/18.
//

#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"
#include <iostream>


class Restaurant{

public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    //rull of 5
    virtual ~Restaurant();   //destructor
    Restaurant(const Restaurant& other);///copy constructor
    Restaurant& operator=(const Restaurant& other);  //copy assignment operator
    Restaurant(Restaurant&& other);  //move constructor
    Restaurant& operator=(Restaurant&& other);  //move assignment operator

    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();

private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif
