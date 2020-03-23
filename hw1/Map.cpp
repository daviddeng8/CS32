/*//
//  main.cpp
//  hw1
//
//  Created by David Deng on 1/16/20.
//  Copyright © 2020 David Deng. All rights reserved.
//


#include <iostream>
#include "Map.h"


Map::Map() {
    m_nPairs = 0;
}

bool Map::empty() const {
    return m_nPairs == 0;
}

int Map::size() const {
    return m_nPairs;
}

bool Map::insert(const KeyType &key, const ValueType &value) {
    if (m_nPairs == DEFAULT_MAX_ITEMS) {
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
    if (m_nPairs == DEFAULT_MAX_ITEMS) {
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
    if (i < 0 || i >= DEFAULT_MAX_ITEMS) {
        return false;
    }
    key = m_dictionary[i].key;
    value = m_dictionary[i].value;
    return true;
}

void Map::swap(Map &other) {
    //largest of two pairs needed to know how many elements to swap
    int greater = (m_nPairs > other.m_nPairs) ? m_nPairs : other.m_nPairs;
    
    //it's ok to swap uninitialized elements since we are keeping track of the number of pairs
    for (int i = 0; i < greater; i++) {
        MapType temp = m_dictionary[i];
        m_dictionary[i] = other.m_dictionary[i];
        other.m_dictionary[i] = temp;
    }
    
    //swaps the number of pairs for each Map
    int temp = m_nPairs;
    m_nPairs = other.m_nPairs;
    other.m_nPairs = temp;
}


*/
