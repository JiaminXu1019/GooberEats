//
//  StreetMap.cpp
//  Project4
//
//  Created by Jiamin Xu on 3/6/20.
//  Copyright © 2020 Jiamin Xu. All rights reserved.
//

#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <string.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <functional>
using namespace std;

unsigned int hasher(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
    
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment>> hashMap;
  //  vector<StreetSegment>uniqueSegments;
   // vector<GeoCoord>startingGeoCoords;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
       hashMap.reset();
      ifstream infile(mapFile);
           if (!infile)
           {
               cerr << "Can't open!" << endl;
               return false;
           }

       string savedStreet;
       string line;
       int k;
      string latitude1;
      string longitude1;
      string latitude2;
      string longitude2;
      bool firstStart = true;
    bool found = true;

       while (getline(infile, line))
       {
           if (!isdigit(line.back())) //if a street is read
                    {
                        savedStreet = line;
                        firstStart = true;
                        found = false;
                        continue;
                    }
         
                 if (!found)  //if the int after the street is read
                  {
                      found = true;
                      istringstream iss(line);
                      iss >> k;
                      continue;
                  }
                 
                 istringstream iss(line); //reads the geocoordinates of every street segment
                 if(iss>>latitude1>>longitude1>>latitude2>>longitude2)
                 {
                 }
           
               GeoCoord start = GeoCoord((latitude1), (longitude1));
                GeoCoord end = GeoCoord((latitude2), (longitude2));
               if(hashMap.find(start)== nullptr) //if the start geocoord is not mapped to anything
               {
                   vector<StreetSegment> correspondingSegments;
                   correspondingSegments.push_back(StreetSegment(start, end, savedStreet)); //map this one segment
                   hashMap.associate(start, correspondingSegments);
               }
               else //otherwise if it is mapped add it to the old  vector of segments
               {
                   vector<StreetSegment> correspondingSegments;
                   getSegmentsThatStartWith(start, correspondingSegments);
                   correspondingSegments.push_back(StreetSegment(start, end, savedStreet));
                   hashMap.associate(start, correspondingSegments);
               }
        
                   if(hashMap.find(end)== nullptr) // map end geocoord, reverse
                     {
                         vector<StreetSegment> correspondingSegments;
                         correspondingSegments.push_back(StreetSegment(end, start, savedStreet));
                         //cerr << start.latitude << " " << start.longitude << endl;
                         hashMap.associate(end, correspondingSegments);
                     }
                     else
                     {
                         vector<StreetSegment> correspondingSegments;
                         getSegmentsThatStartWith(end, correspondingSegments);
                         correspondingSegments.push_back(StreetSegment(end, start, savedStreet));
                         hashMap.associate(end, correspondingSegments);
                   }
           
           
        /*
                        GeoCoord start = GeoCoord(latitude1, longitude1);
                        // startingGeoCoords.push_back(start);
                         GeoCoord end = GeoCoord((latitude2), (longitude2));
                       //  uniqueSegments.push_back(StreetSegment(start, end, savedStreet));

                         if(hashMap.find(start)== nullptr) //already has a hashmap value?
                          {
                              x++;
                              vector<StreetSegment> correspondingSegments; //maps to the front and the reverse
                              correspondingSegments.push_back(StreetSegment(start, end, savedStreet));
                              correspondingSegments.push_back(StreetSegment(start, previousStart, savedStreet));
                              previousStart = start;
                              //cerr << start.latitude << " " << start.longitude << endl;
                              hashMap.associate(start, correspondingSegments);
                          }
                          else
                          {
                              x++;
                              vector<StreetSegment> correspondingSegments;
                              getSegmentsThatStartWith(start, correspondingSegments);//maps to the front and the reverse
                              correspondingSegments.push_back(StreetSegment(start, end, savedStreet));
                              correspondingSegments.push_back(StreetSegment(start, previousStart, savedStreet));
                              hashMap.associate(start, correspondingSegments);
                          }
                         
                             if(x==k)
                               {
                                   if(hashMap.find(end)== nullptr)
                                     {
                                         x++;
                                         vector<StreetSegment> correspondingSegments;
                                         correspondingSegments.push_back(StreetSegment(end, start, savedStreet));
                                         //cerr << start.latitude << " " << start.longitude << endl;
                                         hashMap.associate(end, correspondingSegments);
                                     }
                                     else
                                     {
                                         x++;
                                         vector<StreetSegment> correspondingSegments;
                                         getSegmentsThatStartWith(end, correspondingSegments);
                                         correspondingSegments.push_back(StreetSegment(end, start, savedStreet));
                                         previousStart = start;
                                         hashMap.associate(end, correspondingSegments);
                                     }
                               }
                    
           */
           
         
        }
    /*
      for (vector<GeoCoord>::iterator it = startingGeoCoords.begin(); it != startingGeoCoords.end(); it++)
      {
          vector<StreetSegment> correspondingSegments;
          
          for (vector<StreetSegment>::iterator it2 = uniqueSegments.begin(); it2 != uniqueSegments.end(); it2++)
          {
              if((*it).latitude == (*it2).start.latitude && (*it).longitude == (*it2).start.longitude)
              {
                  correspondingSegments.push_back((*it2));
              }
                 
                else if((*it).latitude == (*it2).end.latitude && (*it).longitude == (*it2).end.longitude)
                {
                    correspondingSegments.push_back(StreetSegment((*it2).end, (*it2).start, (*it2).name));
                }
           }
        
          hashMap.associate(*it, correspondingSegments);
      }
     */


    return true;

}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, std::vector<StreetSegment>& segs) const
{
    segs.clear();
    if(hashMap.find(gc)!= nullptr) //get the value, or the vectors of street segments correspoinding to this GC
    {
        segs = *(hashMap.find(gc));
    }
    /*
    if(startingGeoCoords.size() == 0 || uniqueSegments.size() == 0)
    {
        return false;
    }

        for (vector<StreetSegment>::iterator it2 = unique.begin(); it2 != unique.end(); it2++)
        {
            if((gc).latitude == (*it2).start.latitude && (gc).longitude == (*it2).start.longitude)
            {
                segs.push_back((*it2));
            }
               
              else if((gc).latitude == (*it2).end.latitude && (gc).longitude == (*it2).end.longitude)
              {
                  segs.push_back(StreetSegment((*it2).end, (*it2).start, (*it2).name));
              }
         }
    */
    
    if(segs.size()==0) //if no values were found
    {
        return false;
    }
    
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
