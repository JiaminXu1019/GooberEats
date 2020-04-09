# GooberEats
Project as created in CS32

Delivery logistics system that loads and indexes a bunch of Open Street Map geospatial data (which contains latitude and
longitude data for thousands of streets) and then use this data to produce delivery plans and
navigation instructions for GooberEats delivery robots.

Optimizes the delivery order (to reduce driving time and
ensure the food doesn’t cool too much) and generates turn-by-turn navigation and delivery
instructions for a delivery robot so it can deliver all of its food efficiently. 
(Greedy Algorithm and A* Search Algorithm)

The input to the delivery logistics system consists of:
1. A bunch of Open Street Map data that contains details on thousands of streets in
Westwood, and lists the latitude and longitude of each street segment block by block.
2. The latitude and longitude of the central meal prep depot.
3. The latitude and longitude of one or more delivery stops (always on street corners),
along with what food item must be delivered at each such street corner.
The output of your delivery logistics system is a set of driving directions for a GooberEats
delivery robot to guide it from the central depot, where it picks up the food, to each of its delivery
stops, then back to the central depot.

![Example of turn by turn instructions generated](https://user-images.githubusercontent.com/37451521/77239123-36fb6b80-6b94-11ea-92e2-bfd4175045ee.png)

