#include "provided.h"
#include "ExpandableHashMap.h"
#include <list>
#include <cstdlib>
#include <float.h>
#include <queue>
using namespace std;

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
    
private:
    const StreetMap* m_streetpointer;
    double calculateF(const StreetSegment& seg, const GeoCoord& destination, double distanceTravelledSoFar) const {
        
        //g: the total distance travelled along this path plus the additional distance from this current line segment
        double g = distanceEarthMiles(seg.start, seg.end) + distanceTravelledSoFar;
        
        //h: the distance from the end of this line segment to the final destination
        double h = distanceEarthMiles(seg.end, destination);
    
        return g + h;
    }
    
    //segmentpair struct helps us keep track of the priorities and is the struct we push onto the priority queue
    struct segmentPair {
        StreetSegment segment;
        double priorityVal;
        double distanceTravelledSoFar;
    };
    
    //allows us to rank the values in the priority queue via a minheap
    struct myComparator {
        bool operator() (const segmentPair& lhs, const segmentPair& rhs) {
            return lhs.priorityVal > rhs.priorityVal;
        }
    };
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    m_streetpointer = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{
}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    
    vector<StreetSegment> segs;
    
    //makes sure that the coordinates are valid
    
    if (m_streetpointer->getSegmentsThatStartWith(end, segs) == false) {
        return BAD_COORD;
    }
        
    if (m_streetpointer->getSegmentsThatStartWith(start, segs) == false) {
        return BAD_COORD;
    }
    
    //priority queue is to hold all the possible paths to be taken at any given moment along the path
    priority_queue<segmentPair, vector<segmentPair>, myComparator > newlist;
    
    //meant to associate street segments to their previous segments in order to build the final path
    ExpandableHashMap<StreetSegment, StreetSegment> pathTracer;
    
    //stores the final route to return
    list <StreetSegment> path;
    totalDistanceTravelled = 0;
    
    //variable to store the last segment, necessary for tracing backwards
    StreetSegment finalSegment;
    
    bool foundEnd = false;
    
    //in the case that the start and end are the same position
    if (start == end) {
        route = path;
        return DELIVERY_SUCCESS;
    }
    
    //pushes back the initial segments onto priority queue
    for (int i = 0; i < segs.size(); ++i) {
        segmentPair temp;
        temp.segment = segs[i];
        temp.priorityVal = calculateF(segs[i], end, 0);
        temp.distanceTravelledSoFar = 0;
        newlist.push(temp);
    }
    
    //runs until it goes through all the possible segments and doesn't find a solution, or the end is found
    while (!newlist.empty() && !foundEnd) {
        
        //gets the highest priority element of the priority queue
        StreetSegment curStreetSeg = newlist.top().segment;
        segmentPair curStreetSegPair = newlist.top();
        newlist.pop();
        
        //check if end has been reached 
        if (curStreetSeg.end == end) {
            foundEnd = true;
            finalSegment = curStreetSeg;
            continue;
        }
        
        //generate q's successors
        vector<StreetSegment> qsuccessors;
        m_streetpointer->getSegmentsThatStartWith(curStreetSeg.end, qsuccessors);
        
        //sets the parents of the successors to q
        for (int i = 0; i < qsuccessors.size(); ++i) {
            
            if (pathTracer.find(qsuccessors[i]) != nullptr) {
                continue;
            }
            
            //protects against infinite loops due to mapping back to the start coordinate
            if (curStreetSeg.end == start) {
                continue;
            }
        
            pathTracer.associate(qsuccessors[i], curStreetSeg);
            
            //creates the new segmentPair to add to the priority queue
            segmentPair temp;
            temp.segment = qsuccessors[i];
            temp.priorityVal = calculateF(qsuccessors[i], end, curStreetSegPair.distanceTravelledSoFar);
            temp.distanceTravelledSoFar = curStreetSegPair.distanceTravelledSoFar + distanceEarthMiles(temp.segment.start, temp.segment.end);
            
            newlist.push(temp);
        }

    }
    
    if (foundEnd == false) {
        return NO_ROUTE;
    }
        
    //creates the path by linking the segment to its previous segment repeatedly
    while (pathTracer.find(finalSegment) != nullptr) {
        path.push_front(finalSegment);
        totalDistanceTravelled += distanceEarthMiles(finalSegment.end, finalSegment.start);
        finalSegment = *(pathTracer.find(finalSegment));
    }
    
    //adds the first original segment, which isn't a key in the hash map
    path.push_front(finalSegment);
    
    //reassign variables passed by reference
    totalDistanceTravelled += distanceEarthMiles(finalSegment.end, finalSegment.start);
    route = path;

    return DELIVERY_SUCCESS;
}

//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}

DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}
