/*//
//  CarMap.cpp
//  hw1
//
//  Created by David Deng on 1/16/20.
//  Copyright © 2020 David Deng. All rights reserved.
//


#include <iostream>
#include "CarMap.h"

CarMap::CarMap() {
    m_fleetSize = 0;
}

bool CarMap::addCar(std::string license) {
    if (m_map.insert(license, 0)) {
        m_fleetSize++;
        return true;
    }
    return false;
}

double CarMap::miles(std::string license) const {
    double miles = -1;
    m_map.get(license, miles);
    return miles;
}

bool CarMap::drive(std::string license, double distance) {
    if (distance < 0) {
        return false;
    }
    
    //if the license is in the map, store the current mileage in added_val
    double added_val;
    if (!m_map.get(license, added_val)) {
        return false;
    }
    
    //update license's miles
    return m_map.update(license, distance + added_val);
}

int CarMap::fleetSize() const {
    return m_fleetSize;
}

void CarMap::print() const {
    for (int i = 0; i < m_fleetSize; i++) {
        std::string tempKey = "";
        double tempVal = 0;
        m_map.get(i, tempKey, tempVal);
        std::cout << tempKey << " " << tempVal << std::endl;
    }
}



*/
