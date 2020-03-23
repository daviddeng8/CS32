#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>

using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return hash<string>()(g.latitudeText + g.longitudeText);
}

unsigned int hasher(const string& s) {
    hash<string> hash_string;
    return hash_string(s);
}

unsigned int hasher(const StreetSegment& s) {
    hash<string> hash_string;
    return hash_string(s.end.latitudeText + s.end.longitudeText + s.start.latitudeText + s.start.longitudeText + s.name);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
    
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment> > m_map;
};

StreetMapImpl::StreetMapImpl()
{
    
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream inf(mapFile);
    if (!inf) {
        cerr << "cannot open file!" << endl;
        return false;
    }
    
    string line;
    
    //getline reads individual lines and stores the name of the street in line
    while (getline(inf, line)) {
        //eats up the number of segments following
        int amt;
        inf >> amt;
        
        //jump to the next line
        inf.ignore(10000, '\n');
        
        //loops the number of segments that we have to store
        for (int i = 0; i < amt; ++i) {
            string coord1lat;
            string coord1long;
            string coord2lat;
            string coord2long;
            
            //read in coordinate values
            inf >> coord1lat;
            inf >> coord1long;
            inf >> coord2lat;
            inf >> coord2long;
            
            inf.ignore(10000, '\n');
            
            //converts coordinates to GeoCoords
            GeoCoord start(coord1lat, coord1long);
            GeoCoord end(coord2lat, coord2long);
            
            //converts GeoCoords to StreetSegments
            StreetSegment forwards(start, end, line);
            StreetSegment reverse(end, start, line);
            
            
            vector<StreetSegment>* temppointer = m_map.find(start);
            if (temppointer == nullptr) {
                //puts an association between a GeoCoord at a vector of one street segment
                m_map.associate(start, vector<StreetSegment>(1, forwards));
            }
            else {
                //push back a new vector that is the same as the previous one but with the next street segment pushed back on it
                vector<StreetSegment> tempvec = *temppointer;
                tempvec.push_back(forwards);
                m_map.associate(start, tempvec);
            }
            
            //same process as above for the reverse segments
            temppointer = m_map.find(end);
            if (temppointer == nullptr) {
                m_map.associate(end, vector<StreetSegment>(1, reverse));
            }
            else {
                vector<StreetSegment> tempvec = *temppointer;
                tempvec.push_back(reverse);
                m_map.associate(end, tempvec);
            }
        }
    }
    return true;  
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    //sets segs to the same values as the vector associated with gc in the hashmap
    const vector<StreetSegment>* segpointer = m_map.find(gc);
    if (segpointer == nullptr) {
        return false;
    }
    
    vector<StreetSegment> segments = *segpointer;
    segs = segments; 
    return true;  
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
