// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.

#include "provided.h"
#include <vector>
#include <list>
#include <functional>


template<typename KeyType, typename ValueType>
class ExpandableHashMap
{
public:
	ExpandableHashMap(double maximumLoadFactor = 0.5);
	~ExpandableHashMap();
	void reset();
	int size() const;
	void associate(const KeyType& key, const ValueType& value);

	  // for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const;

	  // for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
	}

	  // C++11 syntax for preventing copying and assignment
	ExpandableHashMap(const ExpandableHashMap&) = delete;
	ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    struct T {
        KeyType key;
        ValueType value;
    };
    
    //creates a bucket number for any key type
    unsigned int getBucketNumber(const KeyType& key, int size) const {
        unsigned int hasher(const KeyType& key);
        unsigned int h = hasher(key);
        return h % size;
    }
    
    //changes the size of the hash map and rehashes it
    void resize() {
        std::vector<std::list<T> > newMap(m_buckets.size() * 2);
        for (int i = 0; i < m_buckets.size(); ++i) {
            auto it = m_buckets[i].begin();
            for (; it != m_buckets[i].end(); ++it) {
                int bucket = getBucketNumber(it->key, newMap.size());
                newMap[bucket].push_back(*it);
            }
        }
        m_buckets = newMap;
    }
    
    std::vector<std::list<T> > m_buckets;
    double m_loadfactor;
    int numEntries;
};

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor)
:m_buckets(8)
{
    //protect against negative load factor inputs
    if (maximumLoadFactor <= 0) {
        m_loadfactor = 0.5;
    }
    else {
        m_loadfactor = maximumLoadFactor;
    }
    numEntries = 0;
}

template<typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap()
{
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset()
{
    numEntries = 0;
    m_buckets = std::vector<std::list<T> >(8);
}

template<typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const
{
    return m_buckets.size();
}

template<typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    ValueType* valpointer = find(key);
    
    //overrides the previous value if the key already exists
    if (valpointer != nullptr) {
        *valpointer = value;
        return;
    }
    else {
        int bucket = getBucketNumber(key, m_buckets.size());
        T nextElem;
        nextElem.key = key;
        nextElem.value = value;
        m_buckets[bucket].push_back(nextElem);
        numEntries++;
    }
    
    //check if buckets need resizing, and if it does, resize it
    if (double (numEntries) / m_buckets.size() > m_loadfactor) {
        resize();
    }
}

template<typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const
{
    int bucket = getBucketNumber(key, m_buckets.size());
    auto it = m_buckets[bucket].begin();
    for (; it != m_buckets[bucket].end(); ++it) {
        if (it->key == key) {
            return &(it->value);
        }
    }
    return nullptr;  
}

