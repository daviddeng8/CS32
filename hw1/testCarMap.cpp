/*//
//  testCar.cpp
//  hw1
//
//  Created by David Deng on 1/17/20.
//  Copyright © 2020 David Deng. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <cassert>
#include "CarMap.h"
#include "Map.h"


int main() {
    CarMap car;
    car.print();
    car.addCar("ABCDE");
    car.addCar("1BCDE");
    car.addCar("12CDE");
    assert(!car.addCar("ABCDE") && car.fleetSize() == 3);
    car.print();
    assert(!car.drive("FGHIJK", 2) && !car.drive("ABCDE", -5) & !car.drive("FGHIJK", -6));
    assert(car.drive("ABCDE", 5));
    assert(car.miles("ABCDE") == 5);
    assert(car.drive("ABCDE", 3));
    assert(car.miles("ABCDE") == 8);
    car.print();
    std::cout << "Passed!" << std::endl;
}

*/
