#include "provided.h"
#include <vector>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
    
private:
    const StreetMap* m_streetpointer;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
    m_streetpointer = sm;
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{
}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    oldCrowDistance = 0;
    newCrowDistance = 0;
    
    //calculate the old crow distance in the current order
    
    GeoCoord currentCoord = depot;
    //add distance from each coordinate to the next
    for (int i = 0; i < deliveries.size(); ++i) {
        oldCrowDistance += distanceEarthMiles(currentCoord, deliveries[i].location);
        currentCoord = deliveries[i].location;
    }
    //add the distance back to the depot
    oldCrowDistance += distanceEarthMiles(currentCoord, depot);

    //optimize the delivery order
    
    //1. set the current coordinate to be the starting position
    //2. find the closest delivery to the current coordinate
    //3. move that delivery to the i position of the vector so that once i increments, that delivery is no longer recalculated
    //4. set the current coordinate to the coordinate of the closest delivery
    
    currentCoord = depot;
    for (int i = 0; i < deliveries.size(); ++i) {
        DeliveryRequest curDelivery = deliveries[i];
        int pos = i;
        for (int k = i + 1; k < deliveries.size(); ++k) {
            if (distanceEarthMiles(deliveries[k].location, currentCoord) < distanceEarthMiles(curDelivery.location, currentCoord)) {
                curDelivery = deliveries[k];
                pos = k;
            }
        }
        
        //swaps the order of the deliveries into the right positions
        DeliveryRequest temp = deliveries[i];
        deliveries[i] = curDelivery;
        deliveries[pos] = temp;
        
        //adds to the new distances
        newCrowDistance += distanceEarthMiles(curDelivery.location, currentCoord);
        currentCoord = curDelivery.location;
    }
    newCrowDistance += distanceEarthMiles(currentCoord, depot);
    
    //add a request back to the depot into the vector 
    DeliveryRequest backToDepot("", depot);
    deliveries.push_back(backToDepot);

}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}
