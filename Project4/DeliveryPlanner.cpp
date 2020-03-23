#include "provided.h"
#include <vector>
using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
    
private:
    const StreetMap* m_streetpointer;
    void findDirectionName(double angle, string& name) const {
        if ((angle >= 0 && angle < 22.5) || angle >= 337.5) {
            name = "east";
        }
        else if (angle >= 22.5 && angle < 67.5) {
            name = "northeast";
        }
        else if (angle >= 67.5 && angle < 112.5) {
            name = "north";
        }
        else if (angle >= 112.5 && angle < 157.5) {
            name = "northwest";
        }
        else if (angle >= 157.5 && angle < 202.5) {
            name = "west";
        }
        else if (angle >= 202.5 && angle < 247.5) {
            name = "southwest";
        }
        else if (angle >= 247.5 && angle < 292.5) {
            name = "south";
        }
        else if (angle >= 292.5 && angle < 337.5) {
            name = "southeast";
        }
    }
    
    void findTurnDirectionName(double turnAngle, string& turnDir) const {
        if (turnAngle >= 1 && turnAngle < 180) {
            turnDir = "left";
        }
        else if (turnAngle >= 180 && turnAngle < 359) {
            turnDir = "right";
        }
    }
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    m_streetpointer = sm;
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    
    double oldCrowDistance;
    double newCrowDistance;
    totalDistanceTravelled = 0;
    
    //creates a modifiable vector with the same contents as the const DeliveryRequest vector passed as constant reference
    vector<DeliveryRequest> optimizedDeliveries = deliveries;
    
    //order deliveries based on efficiency
    DeliveryOptimizer optimizer(m_streetpointer);
    optimizer.optimizeDeliveryOrder(depot, optimizedDeliveries, oldCrowDistance, newCrowDistance);
    
    //router that finds each path
    PointToPointRouter pathFinder(m_streetpointer);
            
    //set the initial previous geo coordinate to the depot, since that's the default starting place
    GeoCoord previous = depot;
    
    //loops through each delivery
    for (int i = 0; i < optimizedDeliveries.size(); ++i) {
        double dist = 0;
        
        //container for the most optimal path to take
        list<StreetSegment> path;
        
        //creates the best route to the destination for delivery
        DeliveryResult result = pathFinder.generatePointToPointRoute(previous, optimizedDeliveries[i].location, path, dist);
        
        //protects against invalid paths
        if (result == BAD_COORD) {
            return BAD_COORD;
        }
        
        if (result == NO_ROUTE) {
            return NO_ROUTE;
        }
        
        totalDistanceTravelled += dist;
        
        //segment that marks the first segment of the current street
        StreetSegment curSegment = *path.begin();
        
        //segment that denotes the last segment of the street
        StreetSegment lastSegmentOfStreet = *path.begin();
        
        double miles = distanceEarthMiles(curSegment.start, curSegment.end);
        
        //stores the angle to keep moving in
        double angleProceed = angleOfLine(curSegment);
        
        //direction string to store conversions from angle to direction
        string dir;
        
        auto it = path.begin();
        it++;
        
        //this variable will keep updating and be pushed back into the commands vector at the end
        DeliveryCommand command;
        
        while (it != path.end()) {
            //if we continue on the same street
            if (it->name == curSegment.name) {
                
                //add distance travelled
                miles += distanceEarthMiles(it->start, it->end);
                
                //keep updating the latest segment of this street
                lastSegmentOfStreet = *it;
            }
            else {
                
                //make proceed command for previous segment
                findDirectionName(angleProceed, dir);
                
                //reset angle to current segment
                angleProceed = angleOfLine(*it);
                
                //add the command to the vector
                command.initAsProceedCommand(dir, curSegment.name, miles);
                commands.push_back(command);
                
                //make turn command onto the new one
                double turnAngle = angleBetween2Lines(lastSegmentOfStreet, *it);
                
                //segments get reset so that we would be comparing the current segment to its next turn, not the previous current segment
                curSegment = *it;
                lastSegmentOfStreet = *it;
                string turnDir;
                
                miles = distanceEarthMiles(it->start, it->end);
                
                //if the angle isn't significant, however, then it should just continue until another proceed command rather than generating a turn command
                if (!(turnAngle < 1 || turnAngle > 359)) {
                    findTurnDirectionName(turnAngle, turnDir);
                    command.initAsTurnCommand(turnDir, it->name);
                    commands.push_back(command);
                }
                
            }
            ++it;
        }
        
        //make proceed statement unless there isn't a path to take, implying that we are already at the delivery destination
        if (path.size() != 0) {
            findDirectionName(angleProceed, dir);
            command.initAsProceedCommand(dir, curSegment.name, miles);
            commands.push_back(command);
        }
        
        //does the deliver command UNLESS it is the path back to the depot, where nothing should be delivered
        if (i != optimizedDeliveries.size() - 1) {
            command.initAsDeliverCommand(optimizedDeliveries[i].item);
            commands.push_back(command);
        }
        
        previous = optimizedDeliveries[i].location;
    }
    
    return DELIVERY_SUCCESS;
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
