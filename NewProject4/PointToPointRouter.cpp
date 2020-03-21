//
//  PointToPointRouter.cpp
//  NewProject4
//
//  Created by Jiamin Xu on 3/7/20.
//  Copyright © 2020 Jiamin Xu. All rights reserved.
//

#include "provided.h"
#include <list>
#include "ExpandableHashMap.h"
#include <functional>
#include <queue>
#include <climits>
#include <stack>

using namespace std;

unsigned int hasher(const StreetSegment& g)
{
    return std::hash<string>()(g.start.latitudeText + g.start.longitudeText + g.end.latitudeText + g.end.longitudeText + g.name);
}
struct PQEntry //a struct to keep track of a street segment and its respective f and g values for A* algorithm
        {
        double f;
        double g;
         StreetSegment segment;
         PQEntry(StreetSegment segment1, double f1, double g1)
            {
                segment = segment1;
                f = f1;
                g = g1;
            }
        };
        
bool operator<(const PQEntry& lhs, const PQEntry& rhs)
       {
        return lhs.f > rhs.f;
       }


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
    const StreetMap* streetmap;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    streetmap = sm;
}
PointToPointRouterImpl::~PointToPointRouterImpl()
{
}
//double heuristic(GeoCoord q, GeoCoord successor, GeoCoord end)
//{
   // return distanceEarthMiles(q, successor) + distanceEarthMiles(successor, end);
//}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    //testing for bad coordinates
    totalDistanceTravelled = 0;
    vector<StreetSegment> useless;
    if(!(streetmap->getSegmentsThatStartWith(start, useless)))
       {
           //cout << "bad" <<endl;
           return BAD_COORD;
       }
    
    vector<StreetSegment> useless1;
    if(!(streetmap->getSegmentsThatStartWith(end, useless1)))
          {
             // cout << "bad" <<endl;
              return BAD_COORD;
          }

    //is the delivery end the same point as start
    if(start == end)
    {
        //cout << "bad" <<endl;
        route.clear();
        totalDistanceTravelled = 0;
        return DELIVERY_SUCCESS;
    }
    
    bool found = false;
    //A* SEARCH ALGORITHM

   // priority_queue<PQEntry> openSegments; // open list
    list<PQEntry> openSegmentsUnordered; // open list
    list<PQEntry> closedSegments; // closed list
    
    //generic starting point pushed onto the open segments
    PQEntry a = PQEntry(StreetSegment(start,start,"PLACEHOLDER"), 0, 0);
   //openSegments.push(a);
    openSegmentsUnordered.push_back(a);
    
    ExpandableHashMap<StreetSegment, StreetSegment> connections; //map all the children segments to their parent, keep track of route
    StreetSegment endingSegment;

    while(!openSegmentsUnordered.empty())
    {
       // PQEntry* q; // get the current node, least f
        PQEntry q = PQEntry(StreetSegment(start,start,"PLACEHOLDER"), 0, 0);
        double f = INT_MAX;
        for(list<PQEntry>::iterator a = openSegmentsUnordered.begin(); a != openSegmentsUnordered.end(); a++)
           {
               if((*a).f < f) //compares the F COST TO DECIDE WHICH CHILD TO PICK
               {
                   f = (*a).f;
                  // q = &(*a);
                   q.segment = (*a).segment;
                   q.f = (*a).f;
                   q.g = (*a).g;
               }
               //cout <<"F COST: " << f << endl;
           }

   // cout<< "PARENT: " << (q).segment.start.latitudeText <<" " << (q).segment.start.longitudeText << " "<< (q).segment.end.latitudeText << " " << (q).segment.end.longitudeText << " " << (q).segment.name << endl;
        
        // remove thecurrent node from the list
        for(list<PQEntry>::iterator a = openSegmentsUnordered.begin(); a != openSegmentsUnordered.end(); a++)
        {
            //add node to closed list
            closedSegments.push_back((q));
            if((q).segment == (*a).segment)
            {
                list<PQEntry>::iterator temp = a;
                openSegmentsUnordered.erase(temp);
              //closedSegments.push_back(PQEntry(StreetSegment((*q).segment.end, (*q).segment.start, (*q).segment.name), (*q).f, (*q).g));
                break;
            }
        }
        
        //is this the ending segment
        if(((q)).segment.end == end)
            {
                 found = true;
                 endingSegment = (q).segment;
                 break;
            }
        
        vector<StreetSegment> segments; //CREATE CHILDREN SEGMENTS
        streetmap->getSegmentsThatStartWith((q).segment.end, segments);
        for (vector<StreetSegment>::iterator it = segments.begin(); it != segments.end(); it++)
           {
             if((*it).end == (q).segment.start)
               {
                  // cout<<"MONKAAAA"<<endl;
                   continue;
               }
                      //cout<< (*it).start.latitudeText <<" " << (*it).start.longitudeText << " "<< (*it).end.latitudeText << " " <<(*it).end.longitudeText << " " << (*it).name << '\n';
              /* if((*it).end == end)
                 {
                      found = true;
                      endingSegment = (*it);
                     connections.associate((*it), (*q).segment);

                      break;
                 }
               */
               //is it on the closed list, if so CONTINUE
               bool x = true;
               for(list<PQEntry>::iterator a = closedSegments.begin(); a != closedSegments.end(); a++)
                  {
                      if((*a).segment == (*it))
                      {
                          x = false;
                          break;
                      }
                  }
               if(!x) {continue;}
                   
               //is it on the open list
               bool b = false;
               double childg = (q).g + distanceEarthMiles((q).segment.end, (*it).end);
                double childh = distanceEarthMiles((*it).end, end);
               double childf = childg + childh;
              for(list<PQEntry>::iterator a = openSegmentsUnordered.begin(); a!= openSegmentsUnordered.end(); a++)
                    {
                        if((*a).segment == (*it)) // it is on the open list, compare the g values of the child and decide whether to remap it to a different parent, if the path is better
                        {
                            double g = (*a).g;
                            b = true;
                            if(childg < g)
                             {
                                // cout << "YOINKED" <<endl;
                                 connections.associate((*a).segment, (q).segment);
                                 list<PQEntry>::iterator temp = a;
                                 openSegmentsUnordered.push_back(PQEntry((*a).segment, childf, childg)); //new f and g values
                                 openSegmentsUnordered.erase(temp);
                                break;
                             }
                        }
                    }
             
               
           if(!b) // if its not on the open list, push it in to open list
           {
              // cout << "PUSHED" << endl;
               connections.associate((*it), (q).segment);
               openSegmentsUnordered.push_back(PQEntry((*it), childf, childg));
            //   openSegments.push(PQEntry((*it),childf, childg));
           }
        
    }
}
    if(found)
    {
       // cout << "FOUND" << endl;
        route.push_back(endingSegment);
        //RE ROUTES BACK TO GET THE PATH FROM END TO START
        while(!(endingSegment == a.segment))
        {
            totalDistanceTravelled+= distanceEarthMiles((endingSegment).start,(endingSegment).end);
            StreetSegment prev = *(connections.find(endingSegment));
            endingSegment = prev;
            if(!(endingSegment == a.segment))
            {
                route.push_back(prev);
            }
        }

     // totalDistanceTravelled+= distanceEarthMiles((endingSegment).start,(endingSegment).end);
        //GET THE ACTUAL NON BACKTRACKED ROUTE
        route.reverse();
        return DELIVERY_SUCCESS;
    }
    
    //cout << "Not FOUND" << endl;

    return NO_ROUTE;
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

