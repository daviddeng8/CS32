//
//  Map.h
//  Project2
//
//  Created by David Deng on 1/22/20.
//  Copyright © 2020 David Deng. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <iostream>
#include <string>

using KeyType = std::string;
using ValueType = double;

class Map
{
  public:
    Map();
    Map(const Map& other);
    Map& operator= (const Map& other);
    ~Map();
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    void dump() const;
    
  private:
    struct MapType {
      public:
        KeyType key;
        ValueType value;
        MapType* next;
        MapType* prev;
    };

    MapType* dummy;
    int m_nPairs;
    
    MapType* inLinkedList(const KeyType& key) const;
    
};

bool combine(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);


#endif /* Map_h */
