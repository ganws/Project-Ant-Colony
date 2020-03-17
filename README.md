# Project-Ant-Colony

Project to simulate ant colony with hormone system.

=====ROUGH IDEAS=====

Ant movement system:
  - An ant's movement is defined by an probability equation which takes pheromone strength (p-strength) as input parameter as follows,
  
		P(i) = Ci / sum(Ci) ,
	where Ci is the p-strength detected by 'sensor' i. 
	*The equation is highly simplified by ommiting all coefficients.
  - Each ant has multiple pheromone 'sensors' spread across the front area.
  - pathing coefficient is 0 when the grid is occupied by a path blocker.
  - Factors that affects the movement pattern (that I can think of right now)
	- Base value of all sensors (the higher it is relative to p-strength, the less sensitive sensors are)
	- Sensitivity factor of front sensor (tendency to follow pheromone) 
	- Coefficient of side sensors (tendency to diviate)
	- resolution of pheromone matrix grid

Pheromone system:
  - ant leaves behind a trail of pheromone particles at fixed interval
  - pheromone particle decays with time
  - each particle contains a set of chemicals with varying levels which reflect theant's behaviour at the moment of release
  
Ant sensory system
  - An ant's sensory region is divided into a number of circles(sensory circle) around it, checking
    pheromone levels constantly.
  - Sensory circle also acts of target for movement
  - A set of sensory circles (adjustable) in front of ants have higher movement probability

Path blocking system
  - An invisible square object is the basic building block for path blocking, known as path blocker.
  - the pathing of every object constitutes smaller path blockers. 

Ant State and Activity
 - Ant's current state can be explained as the action of the ant at any given momemnt. 
 - Activity is the defining behaviour of an ant. Ant can only be in ONE activity at any given moment.
 - eg. FORAGING activity put ants into 'walking' and 'carrying food' states.


Ant swarm system
- each Ant is an object of Ant Class
- Colony Class is a container class for a ants instances, with methods including 'addAnts','removeAnts' etc. 