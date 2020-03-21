//
//  DeliveryOptimizer.cpp
//  NewProject4
//
//  Created by Jiamin Xu on 3/7/20.
//  Copyright © 2020 Jiamin Xu. All rights reserved.
//
#include <climits>
#include "provided.h"
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

bool operator<(const DeliveryRequest& lhs, const DeliveryRequest& rhs)
{
 return (lhs.location < rhs.location);
}

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
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
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
   //get old crow distance
    oldCrowDistance = 0;
    for(vector<DeliveryRequest>::iterator it = deliveries.begin(); it!= deliveries.end(); it++)
    {
        if(it == deliveries.begin())
        {
            oldCrowDistance+= distanceEarthMiles(depot, (*it).location);
        }
        if(it == deliveries.end()-1)
        {
            oldCrowDistance+= distanceEarthMiles((*it).location, depot);
            break;
        }
        oldCrowDistance += distanceEarthMiles((*it).location, ((*(it+1)).location));
    }
    
    /*
    if(!deliveries.empty())
    {
        sort(deliveries.begin(), deliveries.end());
    }
    */
    //vector<DeliveryRequest> permutationVector = deliveries;
    
    
    //NEAREST neighbor algorithm
    
    vector<DeliveryRequest> newDeliveries = deliveries;
    newCrowDistance = oldCrowDistance;
    double generatedcrowdistance = 0;
    vector<DeliveryRequest> newVector;

while(!newDeliveries.empty())
{
    if(newVector.empty()) //find the first delivery location closest to the depot and push it into the newvector, erase it from the old one
               {
                   double distance = INT_MAX;
                   vector<DeliveryRequest>::iterator temp;
                   DeliveryRequest a("placeholder", GeoCoord("0", "0"));
                    for(vector<DeliveryRequest>::iterator it = newDeliveries.begin(); it!= newDeliveries.end(); it++)
                    {
                                if(distanceEarthMiles(depot, (*it).location) < distance)
                                {
                                       distance = distanceEarthMiles(depot, (*it).location);
                                        a = (*it);
                                        temp = it;
                                }
                    }
                   generatedcrowdistance += distance;
                   newVector.push_back(a);
                   newDeliveries.erase(temp);
                   if(newDeliveries.size() == 0) // if no more deliveries
                            {
                                //newVector.push_back(newVector[newVector.size()-1]);
                                generatedcrowdistance += distanceEarthMiles(newVector[newVector.size()-1].location, depot);
                                break;
                            }
               }

    //find the first delivery location closest to the last location and push it into the newvector, erase it from the old one
    double dist = INT_MAX;
    DeliveryRequest a("placeholder", GeoCoord("0", "0"));
    vector<DeliveryRequest>::iterator temp;
    for(vector<DeliveryRequest>::iterator it = newDeliveries.begin(); it!= newDeliveries.end(); it++)
             {
                 
                    if(distanceEarthMiles(newVector[newVector.size()-1].location, (*(it)).location) < dist)
                    {
                        dist = distanceEarthMiles(newVector[newVector.size()-1].location, (*(it)).location);
                         a = (*it);
                         temp = it;
                    }
             }
                generatedcrowdistance += dist;
                newVector.push_back(a);
                newDeliveries.erase(temp);
    
    if(newDeliveries.size() == 0) // if no more deliveries
               {
                   //newVector.push_back(newVector[newVector.size()-1]);
                   generatedcrowdistance += distanceEarthMiles(newVector[newVector.size()-1].location, depot);
                   break;
               }

}
    newCrowDistance = generatedcrowdistance;
    deliveries = newVector;
    
    /*
    do{
      
        for(vector<DeliveryRequest>::iterator it = permutationVector.begin(); it!= permutationVector.end(); it++)
       {
          cout << "Perm: " << (*it).location.latitudeText << " " <<
           " " << (*it).location.longitudeText << " " << (*it).item << endl;
       }
       
   
       for(vector<DeliveryRequest>::iterator it = permutationVector.begin(); it!= permutationVector.end(); it++)
          {
              generatedcrowdistance = 0;
              if(it == permutationVector.begin())
              {
                  generatedcrowdistance+= distanceEarthMiles(depot, (*it).location);
              }
              if(it == permutationVector.end()-1)
              {
                  generatedcrowdistance+= distanceEarthMiles((*it).location, depot);
                  break;
              }
              generatedcrowdistance += distanceEarthMiles((*it).location, ((*(it+1)).location));
          }

            if(generatedcrowdistance < newCrowDistance)
            {
                newCrowDistance = generatedcrowdistance;
                deliveries = permutationVector;
            }
        }
    
    while(next_permutation(permutationVector.begin(), permutationVector.end())); //Generate next permutation till it is not lexicographically largest
    */
    
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
