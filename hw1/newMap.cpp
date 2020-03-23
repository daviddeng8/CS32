//
//  newMap.cpp
//  hw1
//
//  Created by David Deng on 1/17/20.
//  Copyright © 2020 David Deng. All rights reserved.
//


#include <iostream>
#include "newMap.h"

Map::Map() {
    m_dictionary = new MapType[DEFAULT_MAX_ITEMS];
    m_maxPairs = DEFAULT_MAX_ITEMS;
    m_nPairs = 0;
}

Map::Map(int pairs) {
    if (pairs < 1) {
        std::cout << "The number of pairs in the Map must be positive!";
        exit(1);
    }
    m_dictionary = new MapType[pairs];
    m_maxPairs = pairs;
    m_nPairs = 0;
}

Map::Map(const Map& other) {
    m_dictionary = new MapType[other.m_nPairs];
    for (int i = 0; i < other.m_nPairs; i++) {
        m_dictionary[i] = other.m_dictionary[i];
    }
    m_maxPairs = other.m_maxPairs;
    m_nPairs = other.m_nPairs;
}

Map& Map::operator= (const Map& other) {
    if (this == &other) {
        return *this;
    }
    m_nPairs = other.m_nPairs;
    m_maxPairs = other.m_maxPairs;
    delete [] m_dictionary;
    m_dictionary = new MapType[other.m_maxPairs];
    for (int i = 0; i < other.m_nPairs; i++) {
        m_dictionary[i] = other.m_dictionary[i];
    }
    return *this;
}

Map::~Map() {
    delete [] m_dictionary;
    m_dictionary = nullptr;
}
    
bool Map::empty() const {
    return m_nPairs == 0;
}

int Map::size() const {
    return m_nPairs;
}

bool Map::insert(const KeyType &key, const ValueType &value) {
    if (m_nPairs == m_maxPairs) {
        return false;
    }
    for (int i = 0; i < m_nPairs; i++) {
        if (m_dictionary[i].key == key) {
            return false;
        }
    }
    m_dictionary[m_nPairs].key = key;
    m_dictionary[m_nPairs].value = value;
    m_nPairs++;
    return true;
}

bool Map::update(const KeyType &key, const ValueType &value) {
    for (int i = 0; i < m_nPairs; i++) {
        if (m_dictionary[i].key == key) {
            m_dictionary[i].value = value;
            return true;
        }
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType &key, const ValueType &value) {
    if (m_nPairs == m_maxPairs) {
        return false;
    }
    if (update(key, value)) {
        return true;
    }
    m_dictionary[m_nPairs].key = key;
    m_dictionary[m_nPairs].value = value;
    m_nPairs++;
    return true;
}

bool Map::erase(const KeyType &key) {
    for (int i = 0; i < m_nPairs; i++) {
        if (m_dictionary[i].key == key) {
            m_dictionary[i] = m_dictionary[m_nPairs - 1];
            m_nPairs--;
            return true;
        }
    }
    return false;
}

bool Map::contains(const KeyType &key) const {
    for (int i = 0; i < m_nPairs; i++) {
        if (m_dictionary[i].key == key) {
            return true;
        }
    }
    return false;
}

bool Map::get(const KeyType &key, ValueType &value) const {
    for (int i = 0; i < m_nPairs; i++) {
        if (m_dictionary[i].key == key) {
            value = m_dictionary[i].value;
            return true;
        }
    }
    return false;
}

bool Map::get(int i, KeyType &key, ValueType &value) const {
    if (i < 0 || i >= m_nPairs) {
        return false;
    }
    key = m_dictionary[i].key;
    value = m_dictionary[i].value;
    return true;
}


void Map::swap(Map &other) {
    MapType* tempPointer = other.m_dictionary;
    other.m_dictionary = m_dictionary;
    m_dictionary = tempPointer;
   
    int tempPairs = other.m_nPairs;
    other.m_nPairs = m_nPairs;
    m_nPairs = tempPairs;
    
    int tempMax = other.m_maxPairs;
    other.m_maxPairs = m_maxPairs;
    m_maxPairs = tempMax;
}



