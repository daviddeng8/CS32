//
//  main.cpp
//  Project2
//
//  Created by David Deng on 1/22/20.
//  Copyright © 2020 David Deng. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "Map.h"

Map::Map() {
    dummy = new MapType;
    dummy->next = dummy;
    dummy->prev = dummy;
    m_nPairs = 0;
}

Map::~Map() {
    for (MapType* p = dummy->next; p != dummy;) {
        MapType* temp = p->next;
        delete p;
        p = temp;
    }
    delete dummy;
}

Map::Map(const Map& other) {
    dummy = new MapType;
    dummy->next = dummy;
    dummy->prev = dummy;
    
    for (MapType* p = other.dummy; p->next != other.dummy; p = p->next) {
        MapType* newNode = new MapType;
        newNode->key = p->next->key;
        newNode->value = p->next->value;
        
        //adds new node to the back of the linkedlist
        newNode->next = dummy;
        newNode->prev = dummy->prev;
        dummy->prev->next = newNode;
        dummy->prev = newNode;
    }
    
    m_nPairs = other.m_nPairs;
}

Map& Map::operator= (const Map& other) {
    if (this == &other) {
        return *this;
    }
    
    //clears the memory from the Map's current linkedlist
    for (MapType* traverse = dummy->next; traverse != dummy;) {
        MapType* temp = traverse->next;
        delete traverse;
        traverse = temp;
    }
    
    delete dummy;
    
    dummy = new MapType;
    dummy->next = dummy;
    dummy->prev = dummy;
    
    for (MapType* p = other.dummy; p->next != other.dummy; p = p->next) {
        MapType* newNode = new MapType;
        newNode->key = p->next->key;
        newNode->value = p->next->value;
        
        //adds the new node to the back of the linkedlist
        newNode->next = dummy;
        newNode->prev = dummy->prev;
        dummy->prev->next = newNode;
        dummy->prev = newNode;
    }
    
    m_nPairs = other.m_nPairs;
    return *this;
}

bool Map::empty() const {
    return m_nPairs == 0;
}

int Map::size() const {
    return m_nPairs;
}

bool Map::insert(const KeyType &key, const ValueType &value) {
    MapType* pointerToKey = inLinkedList(key);
    if (pointerToKey != nullptr) {
        return false;
    }
    
    //creates pointer to a new
    MapType* toAdd = new MapType;
    toAdd->key = key;
    toAdd->value = value;
    
    //add the pointer to the back of the linkedlist
    toAdd->next = dummy;
    toAdd->prev = dummy->prev;
    dummy->prev->next = toAdd;
    dummy->prev = toAdd;
    
    m_nPairs++;
    return true;
}

bool Map::update(const KeyType &key, const ValueType &value) {
    MapType* pointerToKey = inLinkedList(key);
    if (pointerToKey == nullptr) {
        return false;
    }
    
    pointerToKey->value = value;
    return true;
}

bool Map::insertOrUpdate(const KeyType &key, const ValueType &value) {
    if (update(key, value)) {
        return true;
    }
    else {
        insert(key, value);
    }
    return true;
}

bool Map::erase(const KeyType &key) {
    MapType* pointerToKey = inLinkedList(key);
    if (pointerToKey == nullptr) {
        return false;
    }
    
    //resets next and previous for the elements currently linking to the element to delete
    pointerToKey->prev->next = pointerToKey->next;
    pointerToKey->next->prev = pointerToKey->prev;
    
    delete pointerToKey;
    m_nPairs--;
    return true;
}

bool Map::contains(const KeyType &key) const {
    MapType* pointerToKey = inLinkedList(key);
    return pointerToKey != nullptr;
}

bool Map::get(const KeyType &key, ValueType &value) const {
    MapType* pointerToKey = inLinkedList(key);
    if (pointerToKey == nullptr) {
        return false;
    }
    
    value = pointerToKey->value;
    return true;
}

bool Map::get(int i, KeyType &key, ValueType &value) const {
    
    //i can go from 0 to size() - 1
    if (i >= size() || i < 0) {
        return false;
    }
    
    MapType* p = dummy->next;
    for (int k = 0; k < i; k++) {
        p = p->next;
    }
    key = p->key;
    value = p->value;
    return true;
}

void Map::swap(Map &other) {
    //switches the dummy nodes
    MapType* tempDummy = other.dummy;
    other.dummy = dummy;
    dummy = tempDummy;
    
    //swaps the number of pairs in both maps
    int tempNum = other.m_nPairs;
    other.m_nPairs = m_nPairs;
    m_nPairs = tempNum;
}

bool combine(const Map& m1, const Map& m2, Map& result) {
    //loop through the elements of one linked list
    KeyType tempKey;
    ValueType tempVal;
    
    Map emptyMap;
    bool returnValue = true;
    
    for (int i = 0; i < m1.size(); i++) {
        m1.get(i, tempKey, tempVal);
        ValueType m2tempVal;
        
        //look for duplicates in the other map
        if (m2.get(tempKey, m2tempVal) && tempVal != m2tempVal) {
            returnValue = false;
            continue;
        }
        emptyMap.insert(tempKey, tempVal);
    }
    
    //loops through the second map
    for (int i = 0; i < m2.size(); i++) {
        m2.get(i, tempKey, tempVal);
        
        //continues if the emptyMap already has a pair with the same key as in this m2 element
        if (emptyMap.contains(tempKey)) {
            continue;
        }
        
        //also continues if m1 already has the same key, since that means they both had the same key and value
        
        if (m1.contains(tempKey)) {
            continue;
        }
        emptyMap.insert(tempKey, tempVal);
    }
    
    //assignment operator accounts for the fact that result might not be empty
    result = emptyMap;
    
    return returnValue;
}

void reassign(const Map& m, Map& result) {
    Map emptyMap;
    
    //returns the empty map if m is empty
    if (m.size() <= 0) {
        result = emptyMap;
        return;
    }
    
    //store the key and value of the first element
    KeyType firstKey;
    ValueType firstVal;
    m.get(0, firstKey, firstVal);
    
    //if there's only one element in m, fill result with it and return – no reassigning needed
    if (m.size() == 1) {
        emptyMap.insert(firstKey, firstVal);
        result = emptyMap;
        return;
    }
    
    KeyType prevKey;
    ValueType prevVal;
    
    KeyType curKey;
    ValueType curVal;
    
    //iterates and assigns the previous element to the current value
    for (int i = 1; i < m.size(); i++) {
        m.get(i-1, prevKey, prevVal);
        m.get(i, curKey, curVal);
        emptyMap.insert(prevKey, curVal);
    }
    
    //makes sure that the last element gets swapped with stored value of the first element
    emptyMap.insert(curKey, firstVal);
    
    //assigment operator accounts for fact that result might not be empty
    result = emptyMap;
}

Map::MapType* Map::inLinkedList(const KeyType& key) const {
    //linear search through the linked list to find the key
    for (MapType* p = dummy; p->next != dummy; p = p->next) {
        if (p->next->key == key) {
            return p->next;
        }
    }
    return nullptr;
}

void Map::dump() const {
    KeyType tempKey;
    ValueType tempVal;
    
    for (int i = 0; i < size(); i++) {
        get(i, tempKey, tempVal);
        std::cerr << tempKey << " " << std::endl;
        std::cerr << tempVal << " " << std::endl;
    }
}

/*
int main() {
      // default constructor
    Map m;
      // For an empty map:
    assert(m.size() == 0);      // test size
    assert(m.empty());          // test empty
    assert(!m.erase("Ricky"));  // nothing to erase
    assert(combine(m,m,m));     //using combine with the same empty Map
    assert(m.empty());          //the map is still empty
    
    assert(m.insert("Fred", 123)); //insert function works properly
    assert(m.insert("Ethel", 456));
    
    Map d;
    Map temp;
    
    KeyType key = "Fred";
    ValueType value = 123;
    assert(!d.get(d.size(), key, value)); //3 parameter get works correctly with 0 i value
    assert(!m.get(3, key, value)); //3 parameter get works correctly with i greater than Map size
    assert(m.get(key, value)); //
    assert(value == 123);
    
    //erase function works when the element exists
    assert(m.erase("Ethel")); //checks that erase correctly erases
    assert(m.size() == 1);
    assert(!m.erase("Hello")); //checks that erase doesn't erase when the element doesn't exist
    assert(m.size() == 1);
    
    assert(m.insertOrUpdate("Ethel", 456)); //checks that this function inserts when it's supposed to
    assert(m.insertOrUpdate("Lucy", 123));
    assert(m.size() == 3);
    assert(m.update("Lucy", 456)); //checks that update will change the value properly
    assert(m.get("Lucy", value) && value == 456); //checks that the get function properly assigns value
    assert(m.size() == 3); //checks that updating doesn't somehow change the size
    assert(!m.update("Jonny", 256)); //checks that update is false if the key doesn't already exist
    assert(m.insertOrUpdate("Lucy", 123)); //checks that insertOrUpdate will correctly update
    
    d.insert("Jeff", 69);
    m.swap(d);
    assert(m.size() == 1 && m.get("Jeff", value) && value == 69 && d.size() == 3 && d.get("Fred", value) && value == 123 && d.get("Ethel", value) && value == 456 && d.get("Lucy", value) && value == 123); //checks that swap properly works
    
    Map empty;

    m.swap(empty);
    assert(m.size() == 0 && empty.size() == 1 && empty.get("Jeff", value) && value == 69); //checks that map works properly with one empty Map
    
    empty = m;
    assert(empty.size() == 0 && m.size() == 0); //checks that assignment operator works with an empty Map
    
    empty.swap(m);
    assert(empty.size() == 0 && m.size() == 0); //checks that swap works properly with both empty Maps
    
    Map a = d; //checks that the copy constructor works
    assert(a.size() == d.size() && a.size() == 3 && a.get("Fred", value) && value == 123 && a.get("Ethel", value) && value == 456 && a.get("Lucy", value) && value == 123 && d.get("Fred", value) && value == 123 && d.get("Ethel", value) && value == 456 && d.get("Lucy", value) && value == 123); //checks that the copy operator works
    
    assert(a.erase("Ethel"));
    assert(!a.get("Ethel", value) && d.get("Ethel", value) && value == 456); //checks that the copy constructor doesn't create an alias to the argument
    
    Map e = empty;
    assert(e.size() == 0 && empty.size() == 0); //checks that the copy constructor works with empty argument
    
    e = d;
    assert(e.size() == d.size() && e.size() == 3 && e.get("Fred", value) && value == 123 && e.get("Ethel", value) && value == 456 && e.get("Lucy", value) && value == 123 && d.get("Fred", value) && value == 123 && d.get("Ethel", value) && value == 456 && d.get("Lucy", value) && value == 123); //checks that the assignment operator works
    assert(e.erase("Lucy"));
    assert(!e.get("Lucy", value) && d.get("Lucy", value) && value == 123); // checks that the assignment operator doesn't create an alias to the argument
    
    
    assert(e.insertOrUpdate("Lucy", 789) && e.insert("Jonny", 100));
    assert(!combine(e, d, a) && a.size() == 3 && a.get("Jonny", value) && value == 100 && a.get("Fred", value) && value == 123 && a.get("Ethel", value) && value == 456);  //checks that the combine function correctly combines

    reassign(a, empty);
    assert(empty.size() == 3 && empty.get("Jonny", value) && value == 123 && empty.get("Fred", value) && value == 456 && empty.get("Ethel", value) && value == 100); //checks that the reassign function correctly works
    
    Map l;
    assert(l.insert("Jonny", 123) && l.insert("Sammy", 250) && l.insert("Jordan", 500));
    assert(combine(empty, l, l) && l.size() == 5 && l.get("Jonny", value) && value == 123 && l.get("Fred", value) && value == 456 && l.get("Ethel", value) && value == 100 && l.get("Sammy", value) && value == 250 && l.get("Jordan", value) && value == 500); //checking that combine works with aliasing
    
    reassign(l, l);
    assert(l.size() == 5 && l.get("Jonny", value) && value != 123 && l.get("Fred", value) && value != 456 && l.get("Ethel", value) && value != 100 && l.get("Sammy", value) && value != 250 && l.get("Jordan", value) && value != 500); //checks that reassign works with aliasing
    
    return 0;
}*/

