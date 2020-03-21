
//  main.cpp
//  Project4
//
//  Created by Jiamin Xu on 3/4/20.
//  Copyright © 2020 Jiamin Xu. All rights reserved.
//



#include <list>
#include "provided.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

//bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v);
//bool parseDelivery(string line, string& lat, string& lon, string& item)

/*
int main()
{

    StreetMap sm;
   StreetMap* smptr = &sm;
    
    if (!sm.load("MapData.txt"))
       {
           cout << "Unable to load map data file " <<  endl;
           return 1;
       }
    else
    {
        cout<<"GOOD" << endl;;
    }
    vector<StreetSegment> segs;
    sm.getSegmentsThatStartWith(GeoCoord("34.0687443","-118.4449195"), segs);
        
 //for (vector<StreetSegment>::iterator it = segs.begin(); it != segs.end(); it++)
// {
 //        cout << (*it).start.latitudeText << " " <<  (*it).start.longitudeText << " " <<  (*it).end.latitudeText << " " << //(*it).end.longitudeText << " " << (*it).name <<"\n" << endl;
 //}
  list<StreetSegment> a;
    list<StreetSegment> b;
    list<StreetSegment> c;
    list<StreetSegment> d;
    double distance;
    double distance1;
    double distance2;
    double distance3;
   PointToPointRouter apple(smptr);
    PointToPointRouter x(smptr);
    PointToPointRouter y(smptr);
    PointToPointRouter z(smptr);

    
    GeoCoord depot("34.0625329", "-118.4470263");
    vector<DeliveryRequest> stonks;
   // stonks.push_back(DeliveryRequest("B-Plate salmon (Eng IV)", GeoCoord("34.0727330", "-118.4810096")));
  //  stonks.push_back(DeliveryRequest("Pabst Blue Ribbon beer (Beta Theta Pi)", GeoCoord("34.0685657", "-118.4489289")));
   //  stonks.push_back(DeliveryRequest("Chicken tenders (Sproul Landing)", GeoCoord("34.0712323", "-118.4505969")));


    DeliveryOptimizer yeet(smptr);
    double crow;
    double newcrow;
    yeet.optimizeDeliveryOrder(depot, stonks, crow, newcrow);
    
    for(vector<DeliveryRequest>::iterator it = stonks.begin(); it!= stonks.end(); it++)
    {
        cout << (*it).location.latitudeText << " " << (*it).location.longitudeText << " " <<(*it).item << endl;
    }
    
    cout << crow << endl;
    cout << newcrow << endl;

    
    apple.generatePointToPointRoute(GeoCoord("34.0727330", "-118.4810096"), GeoCoord("34.0625329", "-118.4470263"), a, distance);

    cout << a.size() << endl;

     cout << "DELIVER" << endl;
     
     for (list<StreetSegment>::iterator it = a.begin(); it!= a.end(); it++)
       {
          cout << (*it).start.latitudeText << " " << (*it).start.longitudeText << " " << (*it).end.latitudeText << " " << (*it).end.longitudeText << " " <<(*it).name << " " << distanceEarthMiles((*it).start,(*it).end)<<"\n";

       }
     
        /*

        x.generatePointToPointRoute(GeoCoord("34.0685657", "-118.4489289"), GeoCoord("34.0712323", "-118.4505969"), b, distance1);
        
            y.generatePointToPointRoute(GeoCoord("34.0712323", "-118.4505969"), GeoCoord("34.0687443" , "-118.4449195"), c, distance2);
            z.generatePointToPointRoute(GeoCoord("34.0687443", "-118.4449195"), GeoCoord("34.0625329", "-118.4470263"), d, distance3);
        /Users/jiaminxu/Library/Developer/Xcode/DerivedData/NewProject4-amediadiilfxutfbwkyuyzkbaxii/Build/Products/Debug/NewProject4 mapdata.txt deliveries.txt

    for (list<StreetSegment>::iterator it = a.begin(); it!= a.end(); it++)
    {
       cout << (*it).start.latitudeText << " " << (*it).start.longitudeText << " " << (*it).end.latitudeText << " " << (*it).end.longitudeText << " " <<(*it).name << " " << distanceEarthMiles((*it).start,(*it).end)<<"\n";

    }
    /*
    cout << a.size() << endl;

    cout << "DELIVER" << endl;
    
    for (list<StreetSegment>::iterator it = b.begin(); it!= b.end(); it++)
      {
         cout << (*it).start.latitudeText << " " << (*it).start.longitudeText << " " << (*it).end.latitudeText << " " << (*it).end.longitudeText << " " <<(*it).name << " " << distanceEarthMiles((*it).start,(*it).end)<<"\n";

      }
    cout << b.size() << endl;

    cout << "DELIVER" << endl;

    for (list<StreetSegment>::iterator it = c.begin(); it!= c.end(); it++)
      {
         cout << (*it).start.latitudeText << " " << (*it).start.longitudeText << " " << (*it).end.latitudeText << " " << (*it).end.longitudeText << " " <<(*it).name << " " << distanceEarthMiles((*it).start,(*it).end)<<"\n";

      }
    cout << c.size() << endl;

    cout << "DELIVER" << endl;

    for (list<StreetSegment>::iterator it = d.begin(); it!= d.end(); it++)
         {
         cout << (*it).start.latitudeText << " " << (*it).start.longitudeText << " " << (*it).end.latitudeText << " " << (*it).end.longitudeText << " " <<(*it).name << " " << distanceEarthMiles((*it).start,(*it).end)<<"\n";
         }
    cout << d.size() << endl;

         cout << distance + distance1 + distance2 + distance3
                << endl;
     */







#include "provided.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v);
bool parseDelivery(string line, string& lat, string& lon, string& item);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " mapdata.txt deliveries.txt" << endl;
        return 1;
    }

    StreetMap sm;
        
    if (!sm.load(argv[1]))
    {
        cout << "Unable to load map data file " << argv[1] << endl;
        return 1;
    }

    GeoCoord depot;
    vector<DeliveryRequest> deliveries;
    if (!loadDeliveryRequests(argv[2], depot, deliveries))
    {
        cout << "Unable to load delivery request file " << argv[2] << endl;
        return 1;
    }

    cout << "Generating route...\n\n";

    DeliveryPlanner dp(&sm);
    vector<DeliveryCommand> dcs;
    double totalMiles;
    DeliveryResult result = dp.generateDeliveryPlan(depot, deliveries, dcs, totalMiles);
    if (result == BAD_COORD)
    {
        cout << "One or more depot or delivery coordinates are invalid." << endl;
        return 1;
    }
    if (result == NO_ROUTE)
    {
        cout << "No route can be found to deliver all items." << endl;
        return 1;
    }
    cout << "Starting at the depot...\n";
    for (const auto& dc : dcs)
        cout << dc.description() << endl;
    cout << "You are back at the depot and your deliveries are done!\n";
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << totalMiles << " miles travelled for all deliveries." << endl;
}

bool loadDeliveryRequests(string deliveriesFile, GeoCoord& depot, vector<DeliveryRequest>& v)
{
    ifstream inf(deliveriesFile);
    if (!inf)
        return false;
    string lat;
    string lon;
    inf >> lat >> lon;
    inf.ignore(10000, '\n');
    depot = GeoCoord(lat, lon);
    string line;
    while (getline(inf, line))
    {
        string item;
        if (parseDelivery(line, lat, lon, item))
            v.push_back(DeliveryRequest(item, GeoCoord(lat, lon)));
    }
    return true;
}

bool parseDelivery(string line, string& lat, string& lon, string& item)
{
    const size_t colon = line.find(':');
    if (colon == string::npos)
    {
        cout << "Missing colon in deliveries file line: " << line << endl;
        return false;
    }
    istringstream iss(line.substr(0, colon));
    if (!(iss >> lat >> lon))
    {
        cout << "Bad format in deliveries file line: " << line << endl;
        return false;
    }
    item = line.substr(colon + 1);
    if (item.empty())
    {
        cout << "Missing item in deliveries file line: " << line << endl;
        return false;
    }
    return true;
}
 






