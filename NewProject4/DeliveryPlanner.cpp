//
//  DeliveryPlanner.cpp
//  NewProject4
//
//  Created by Jiamin Xu on 3/7/20.
//  Copyright © 2020 Jiamin Xu. All rights reserved.
//

#include "provided.h"
#include <vector>
#include <list>
#include <iostream>
#include <string>
#include "ExpandableHashMap.h"

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
        const StreetMap* map;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    map = sm;
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
    totalDistanceTravelled = 0;
    double oldcrow;
    double newcrow;
    DeliveryOptimizer optimal(map);
    
    vector<DeliveryRequest> optimalDeliveries = deliveries;
    optimal.optimizeDeliveryOrder(depot, optimalDeliveries, oldcrow, newcrow);
    
    if(optimalDeliveries.empty())
    {
        return DELIVERY_SUCCESS;
    }
    
    for(auto it = optimalDeliveries.begin(); it!= optimalDeliveries.end(); it++) // all deliveries
    {

        if(it == optimalDeliveries.begin()) //depot to the first delivery
           {
                list<StreetSegment> temp;
                vector<StreetSegment> temptemp;
               list<StreetSegment> temptemptemp;
               double distanceTraveledtemp;

                       double distanceTraveled;
                       PointToPointRouter router(map);
               
               //is the generated route possible?
                       if(router.generatePointToPointRoute(depot, (*(it)).location, temptemptemp, distanceTraveledtemp) == BAD_COORD)
                       {
                           return BAD_COORD;
                       }
                    else if(router.generatePointToPointRoute(depot, (*(it)).location, temptemptemp, distanceTraveledtemp) == NO_ROUTE)
                    {
                        return NO_ROUTE;
                    }
                    else
                    {
                        router.generatePointToPointRoute(depot, (*(it)).location, temp, distanceTraveled);
                    }
                    if(distanceTraveled == 0)
                    {
                        DeliveryCommand e = DeliveryCommand();
                         e.initAsDeliverCommand((*(it)).item);
                         commands.push_back(e);
                    }
                     totalDistanceTravelled += distanceTraveled;
                
                     for(list<StreetSegment>::iterator a = temp.begin(); a!= temp.end(); a++) //convert list to vector
                     {
                         temptemp.push_back((*a));
                     }
                   // cout << temptemp.size()<<endl;

                    StreetSegment lastUnique; //saves the last unique segment
                         string direction;
                        double distanceBetween = 0;

                for(vector<StreetSegment>::iterator a = temptemp.begin(); a!= temptemp.end(); a++)
                       {
                         //if its all other street segments but the last
                           if(a == temptemp.begin())
                           {
                               distanceBetween += distanceEarthMiles((*a).start, (*(a)).end);
                               lastUnique = (*a);
                           }
                           double angle = angleOfLine((lastUnique));
                         
                         if(0 <= angle && angle < 22.5)
                        {direction = "east";}
                        else if(22.5 <= angle && angle< 67.5)
                        {direction = "northeast";}
                         else if(67.5 <= angle && angle< 112.5)
                         {direction = "north";}
                        else if(112.5 <= angle&& angle < 157.5)
                        { direction = "northwest";}
                         else if(157.5 <= angle && angle< 202.5)
                        { direction = "west";}
                        else if(202.5 <= angle&& angle < 247.5)
                        {direction = "southwest";}
                         else if(247.5 <= angle&& angle < 292.5)
                        {direction = "south";}
                        else if(292.5 <= angle&& angle < 337.5)
                        {direction = "southeast";}
                        else if(angle >= 337.5)
                        {direction = "east";}

                             if(a!=temptemp.end()-1)
                                  {
                                     if((*a).name == (*(a+1)).name)
                                          {
                                              if(a != temptemp.begin())
                                                   {
                                                         distanceBetween += distanceEarthMiles((*a).start, (*(a)).end);
                                                   }
                                              continue;
                                          }
                                      else if(a!=temptemp.end() && (*a).name != (*(a+1)).name)
                                        {
                                        if(a != temptemp.begin())
                                          {
                                                distanceBetween += distanceEarthMiles((*a).start, (*(a)).end);
                                          }
                                         //   cout << "Angle: " << angle << endl;
                                            DeliveryCommand c = DeliveryCommand();
                                         c.initAsProceedCommand(direction, ((lastUnique).name), distanceBetween);
                                         commands.push_back(c);
                                            distanceBetween = 0;
                                         if(a!=temptemp.end()-1)
                                         {
                                           lastUnique = (*(a+1));
                                         }
                                     }
                                  }
                                 
                                  else
                                  {
                                      if(a != temptemp.begin())
                                       {
                                             distanceBetween += distanceEarthMiles((*a).start, (*(a)).end);
                                       }
                                      //   cout << "Angle: " << angle << endl;
                                         DeliveryCommand c = DeliveryCommand();
                                      c.initAsProceedCommand(direction, ((lastUnique).name), distanceBetween);
                                      commands.push_back(c);
                                      distanceBetween = 0;
                                  }
                           //get the proceed commands and the turn command  after
                           if(a == temptemp.end()-1) //if its at the final street segment to traverse
                              {
                                  DeliveryCommand e = DeliveryCommand();
                                  e.initAsDeliverCommand((*(it)).item);
                                  commands.push_back(e);
                                  break;
                              }
                                                  
                           DeliveryCommand d = DeliveryCommand();
                           double turnAngle = angleBetween2Lines((*a), (*(a+1)));
                            
                            if(turnAngle < 1 || turnAngle  > 359)
                            {
                                //if the angle between streets is minimal, change the streets but adjust the angle
                                lastUnique = (*(a+1));
                                continue;
                              /*
                                  string newdirection;
                                  double angleA = angleOfLine((*a));
                                   if(0 <= angleA && angleA < 22.5)
                                  {
                                      newdirection = "east";
                                  }
                                  else if(22.5 <= angleA && angleA< 67.5)
                                  {newdirection = "northeast";}
                                   else if(67.5 <= angleA && angleA< 112.5)
                                   {newdirection = "north";}
                                  else if(112.5 <= angleA&& angleA < 157.5)
                                  { newdirection = "northwest";}
                                   else if(157.5 <= angleA && angleA< 202.5)
                                  { newdirection = "west";}
                                  else if(202.5 <= angleA&& angleA < 247.5)
                                  {newdirection = "southwest";}
                                   else if(247.5 <= angleA&& angleA < 292.5)
                                  {newdirection = "south";}
                                  else if(292.5 <= angleA&& angleA < 337.5)
                                  {newdirection = "southeast";}
                                  else if(angle >= 337.5)
                                  {newdirection = "east";}
                                //  cout << "Angle SMALL: " << angle << endl;

                                  d.initAsProceedCommand(newdirection, (*(a+1)).name, distanceEarthMiles((*(a+1)).start, (*(a+1)).end));
                                  a++;
                                if(a == temptemp.end()-1) //if its at the final street segment to traverse
                                    {
                                        commands.push_back(d);
                                        DeliveryCommand e = DeliveryCommand();
                                        e.initAsDeliverCommand((*(it+1)).item);
                                        commands.push_back(e);
                                        break;
                                    }
                                */
                            }
                           else //create a turn command
                           {
                                if(turnAngle >= 1 && turnAngle < 180)
                               {
                                   d.initAsTurnCommand("left", (*(a+1)).name);
                               }
                               else if(turnAngle>= 180  && turnAngle <= 359 )
                                {
                                    d.initAsTurnCommand("right", (*(a+1)).name);
                                }
                           }
                           commands.push_back(d);
                       }
           }
              
        if(it == optimalDeliveries.end()-1) // last delivery point, then return to depot
           {

                 list<StreetSegment> temp;
                               vector<StreetSegment> temptemp;
                              list<StreetSegment> temptemptemp;

               double distanceTraveledtemp;
                      double distanceTraveled;
                      PointToPointRouter router(map);
              //is the generated route possible?
                      if(router.generatePointToPointRoute((*(it)).location,depot, temptemptemp, distanceTraveledtemp) == BAD_COORD)
                      {
                          return BAD_COORD;
                      }
                   else if(router.generatePointToPointRoute((*(it)).location, depot, temptemptemp, distanceTraveledtemp) == NO_ROUTE)
                   {
                       return NO_ROUTE;
                   }
                   else
                   {
                       router.generatePointToPointRoute((*(it)).location,depot, temp, distanceTraveled);
                   }
            
               totalDistanceTravelled += distanceTraveled;
               
               for(list<StreetSegment>::iterator a = temp.begin(); a!= temp.end(); a++)
               {
                   temptemp.push_back((*a));
               }
              // cout << temptemp.size()<<endl;

               StreetSegment lastUnique;
               string direction;
               double distanceBetween = 0;
                 for(vector<StreetSegment>::iterator a = temptemp.begin(); a!= temptemp.end(); a++)
                 {
                     if(a == temptemp.begin())
                     {
                         distanceBetween += distanceEarthMiles((*a).start, (*(a)).end);
                         lastUnique = (*a);
                     }
                     //get the angle of the last unique street

                    double angle = angleOfLine(lastUnique);
                   if(0 <= angle && angle < 22.5)
                  {
                      direction = "east";
                  }
                  else if(22.5 <= angle && angle< 67.5)
                  {direction = "northeast";}
                   else if(67.5 <= angle && angle< 112.5)
                   {direction = "north";}
                  else if(112.5 <= angle&& angle < 157.5)
                  { direction = "northwest";}
                   else if(157.5 <= angle && angle< 202.5)
                  { direction = "west";}
                  else if(202.5 <= angle&& angle < 247.5)
                  {direction = "southwest";}
                   else if(247.5 <= angle&& angle < 292.5)
                  {direction = "south";}
                  else if(292.5 <= angle&& angle < 337.5)
                  {direction = "southeast";}
                  else if(angle >= 337.5)
                  {direction = "east";}
                  
                         if(a!=temptemp.end()-1)
                             {
                                if((*a).name == (*(a+1)).name)
                                     {
                                         if(a != temptemp.begin())
                                              {
                                                    distanceBetween += distanceEarthMiles((*a).start, (*(a)).end);
                                              }
                                         continue;
                                     }
                                 else if(a!=temptemp.end() && (*a).name != (*(a+1)).name)
                                   {
                                   if(a != temptemp.begin())
                                     {
                                           distanceBetween += distanceEarthMiles((*a).start, (*(a)).end);
                                     }
                                    //   cout << "Angle: " << angle << endl;
                                       DeliveryCommand c = DeliveryCommand();
                                    c.initAsProceedCommand(direction, ((lastUnique).name), distanceBetween);
                                    commands.push_back(c);
                                       distanceBetween = 0;
                                    if(a!=temptemp.end()-1)
                                    {
                                      lastUnique = (*(a+1));
                                    }
                                }
                             }
                            
                             else
                             {
                                 if(a != temptemp.begin())
                                  {
                                        distanceBetween += distanceEarthMiles((*a).start, (*(a)).end);
                                  }
                                 //   cout << "Angle: " << angle << endl;
                                    DeliveryCommand c = DeliveryCommand();
                                 c.initAsProceedCommand(direction, ((lastUnique).name), distanceBetween);
                                 commands.push_back(c);
                                 distanceBetween = 0;
                             }
                     //get the proceed commands and the turn command after
            
                     if(a == temptemp.end()-1) //if its at the final street segment to traverse, only difference is no deliver command is used
                        {
                           //DeliveryCommand e = DeliveryCommand();
                           // e.initAsDeliverCommand((*(it+1)).item);
                          // commands.push_back(e);
                            break;
                        }
                     DeliveryCommand d = DeliveryCommand();
                     double turnAngle = angleBetween2Lines((*a), (*(a+1)));
                      if(turnAngle < 1 || turnAngle  > 359)
                      {
                         lastUnique = (*(a+1));
                        continue;
                          /*
                          string newdirection;
                          double angleA = angleOfLine((*a));
                           if(0 <= angleA && angleA < 22.5)
                          {
                              newdirection = "east";
                          }
                          else if(22.5 <= angleA && angleA< 67.5)
                          {newdirection = "northeast";}
                           else if(67.5 <= angleA && angleA< 112.5)
                           {newdirection = "north";}
                          else if(112.5 <= angleA&& angleA < 157.5)
                          { newdirection = "northwest";}
                           else if(157.5 <= angleA && angleA< 202.5)
                          { newdirection = "west";}
                          else if(202.5 <= angleA&& angleA < 247.5)
                          {newdirection = "southwest";}
                           else if(247.5 <= angleA&& angleA < 292.5)
                          {newdirection = "south";}
                          else if(292.5 <= angleA&& angleA < 337.5)
                          {newdirection = "southeast";}
                          else if(angle >= 337.5)
                          {newdirection = "east";}
                         // cout << "Angle SMALL: " << angle << endl;
                          d.initAsProceedCommand(newdirection, (*(a+1)).name, distanceEarthMiles((*(a+1)).start, (*(a+1)).end));
                          a++;
                          if(a == temptemp.end()-1) //if its at the final street segment to traverse
                              {
                                  commands.push_back(d);
                                  break;
                              }
                           */
                      }
                     else
                     {
                         if(turnAngle >= 1 && turnAngle < 180)
                         {
                             d.initAsTurnCommand("left", (*(a+1)).name);
                         }
                        else if(turnAngle>= 180  && turnAngle <= 359 )
                          {
                              d.initAsTurnCommand("right", (*(a+1)).name);
                          }
                     }
                     commands.push_back(d);
                 }
               break;
           }

         //all deliveries in between, same as logic as above
        cout << "yeet" << endl;
          list<StreetSegment> temp;
            vector<StreetSegment> temptemp;
           list<StreetSegment> temptemptemp;

            double distanceTraveledtemp;
                   double distanceTraveled;
                   PointToPointRouter router(map);
           //is the generated route possible?
                   if(router.generatePointToPointRoute((*it).location, (*(it+1)).location, temptemptemp, distanceTraveledtemp) == BAD_COORD)
                   {
                       return BAD_COORD;
                   }
                else if(router.generatePointToPointRoute((*it).location, (*(it+1)).location, temptemptemp, distanceTraveledtemp) == NO_ROUTE)
                {
                    return NO_ROUTE;
                }
                else
                {
                    router.generatePointToPointRoute((*it).location, (*(it+1)).location, temp, distanceTraveled);
                }
        
        if(distanceTraveled == 0)
             {
                 DeliveryCommand e = DeliveryCommand();
                  e.initAsDeliverCommand((*(it)).item);
                  commands.push_back(e);
             }
        totalDistanceTravelled += distanceTraveled;
        
        for(list<StreetSegment>::iterator a = temp.begin(); a!= temp.end(); a++)
        {
            temptemp.push_back((*a));
        }
        //cout << temptemp.size()<<endl;
        
        StreetSegment lastUnique;
        string direction;
        double distanceBetween = 0;
        for(vector<StreetSegment>::iterator a = temptemp.begin(); a!= temptemp.end(); a++)
        {
          //if its all other street segments but the last
            if(a == temptemp.begin())
            {
                distanceBetween += distanceEarthMiles((*a).start, (*(a)).end);
                lastUnique = (*a);
            }
            //get the angle of the last unique street
            double angle = angleOfLine((*a));
              if(0 <= angle && angle < 22.5)
              { direction = "east";}
              else if(22.5 <= angle && angle< 67.5)
              {direction = "northeast";}
               else if(67.5 <= angle && angle< 112.5)
               {direction = "north";}
              else if(112.5 <= angle&& angle < 157.5)
              { direction = "northwest";}
               else if(157.5 <= angle && angle< 202.5)
              { direction = "west";}
              else if(202.5 <= angle&& angle < 247.5)
              {direction = "southwest";}
               else if(247.5 <= angle&& angle < 292.5)
              {direction = "south";}
              else if(292.5 <= angle&& angle < 337.5)
              {direction = "southeast";}
              else if(angle >= 337.5)
              {direction = "east";}
         
             if(a!=temptemp.end()-1)
             {
                if((*a).name == (*(a+1)).name)
                     {
                         if(a != temptemp.begin())
                              {
                                    distanceBetween += distanceEarthMiles((*a).start, (*(a)).end);
                              }
                         continue;
                     }
                 else if(a!=temptemp.end() && (*a).name != (*(a+1)).name)
                   {
                   if(a != temptemp.begin())
                     {
                           distanceBetween += distanceEarthMiles((*a).start, (*(a)).end);
                     }
                    //   cout << "Angle: " << angle << endl;
                       DeliveryCommand c = DeliveryCommand();
                    c.initAsProceedCommand(direction, ((lastUnique).name), distanceBetween);
                    commands.push_back(c);
                       distanceBetween = 0;
                    if(a!=temptemp.end()-1)
                    {
                      lastUnique = (*(a+1));
                    }
                }
             }
            
             else
             {
                 if(a != temptemp.begin())
                  {
                        distanceBetween += distanceEarthMiles((*a).start, (*(a)).end);
                  }
                 //   cout << "Angle: " << angle << endl;
                    DeliveryCommand c = DeliveryCommand();
                 c.initAsProceedCommand(direction, ((lastUnique).name), distanceBetween);
                 commands.push_back(c);
                 distanceBetween = 0;
             }
            
            //get the proceed commands and the turn command  after
            if(a == temptemp.end()-1) //if its at the final street segment to traverse
               {
                   DeliveryCommand e = DeliveryCommand();
                   e.initAsDeliverCommand((*(it+1)).item);
                   commands.push_back(e);
                   break;
               }

             DeliveryCommand d = DeliveryCommand();
             double turnAngle = angleBetween2Lines((*a), (*(a+1)));
             if(turnAngle < 1 || turnAngle  > 359)
                  {
                      lastUnique = (*(a+1));
                      continue;
                        /*
                      //if the angle between streets is minimal, change the streets but adjust the angle
                        string newdirection;
                        double angleA = angleOfLine((*a));
                         if(0 <= angleA && angleA < 22.5)
                        { newdirection = "east";}
                        else if(22.5 <= angleA && angleA< 67.5)
                        {newdirection = "northeast";}
                         else if(67.5 <= angleA && angleA< 112.5)
                         {newdirection = "north";}
                        else if(112.5 <= angleA&& angleA < 157.5)
                        { newdirection = "northwest";}
                         else if(157.5 <= angleA && angleA< 202.5)
                        { newdirection = "west";}
                        else if(202.5 <= angleA&& angleA < 247.5)
                        {newdirection = "southwest";}
                         else if(247.5 <= angleA&& angleA < 292.5)
                        {newdirection = "south";}
                        else if(292.5 <= angleA&& angleA < 337.5)
                        {newdirection = "southeast";}
                        else if(angle >= 337.5)
                        {newdirection = "east";}
                       // cout << "Angle SMALL: " << angle << endl;
                        d.initAsProceedCommand(newdirection, (*(a+1)).name, distanceEarthMiles((*(a+1)).start, (*(a+1)).end));
                        a++;
                      if(a == temptemp.end()-1) //if its at the final street segment to traverse
                           {
                               commands.push_back(d);
                               DeliveryCommand e = DeliveryCommand();
                               e.initAsDeliverCommand((*(it+1)).item);
                               commands.push_back(e);
                               break;
                           }
                         */
                        
                  }
              else
              {
                  if(turnAngle >= 1 && turnAngle < 180)
                  {
                      d.initAsTurnCommand("left", (*(a+1)).name);
                  }
                   if(turnAngle>= 180  && turnAngle <= 359 )
                   {
                       d.initAsTurnCommand("right", (*(a+1)).name);
                   }
              }
              commands.push_back(d);
          }
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
