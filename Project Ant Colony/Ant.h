#pragma once

#include"AntState.h"
#include"SensoryInput.h"
#include"PheromoneSystem.h"
#include "VectorFunction.h"
#include "PathBlocker.h"
#include "Animation.h"

#include<random>
#include<math.h>
#include<SFML/Graphics.hpp>

// Activity and State determine the basic behaviour of ants. An ant can have multiple states but only 1 activity at a time (no multitasking yet).
// For simple analogy, brushing teeth is an activity while state of the person is "hand-moving" and "stationery".

class Ant: public sf::Sprite
{
public:

	//constant states
	float m_movespeed{ 120.0f }; //movespeed in pixel per second
	float m_rotatespeed{ 530.0f }; //degree per sec;
	float m_HP{}; //hitpoint
	float m_attack{}; //attack
	float m_dmgtaken{}; // dmg taken per timestep
	sf::Vector2f m_size; //ant size
	float m_age{}; //age [days]
	float m_pheromon_period{0.05f}; //pheromon secreting  period [s]
	float m_internal_clock{0.0f}; // for various counting

	//sensory inputs 
	float m_r1{20.0f}; //distance of sensory circles from ant origin
	int m_scircle_count{ 32 }; //sensory circle counts
	float m_active_num{ 5 };//number of neighbour circles from either side of face_indx

	SensoryInput m_sensory_input{m_r1,m_scircle_count, this->getPosition(), m_active_num}; //create sensory circles


	sf::Vector2f m_targetCoord;  //absolute coordinate vector in map
	float m_targetFaceAngle; //absolute angle [0, 360)

	std::vector<PathBlocker>* pblocker_systm_ptr{nullptr};
	
public:

	//constructor, destructor
	Ant();
	//Ant(std::vector<PathBlocker> *pblock_systm); //constructor
	Ant(const Ant& obj);
	~Ant(); //destructor

	void initAnt(float size, sf::Vector2f init_pos, sf::Texture *texture, std::vector<PathBlocker>* arg_pblock_system);
	void updateMovement(float dt, std::vector<PathBlocker>* path_blck_ptr);
	void secretPheromon(float dt, PheromoneSystem *psystem); //secret pheromone on spot
	void sensePheromone(); //sense pheromones around the ant
	void drawSensoryCircle(sf::RenderWindow* window);

	//getting and setter
	float getMoveSpeed();
	void setMoveSpeed(float movespeed);
	Activity getActivity(); //return current activity
	void switchActivity(Activity new_activity); //switch to new activity
	State getState(); //return all current states
	void switchState(State target_state, bool new_state); //turning a state on and off 

	// Command function
	void issue_move_command(sf::Vector2f coordinate); //issue move command. Issuing this command
	void issue_face_command(float facingAngle); //rotate ants (in degree)

	//======FORAGING=======//
	int computeMoveTarget(); //compute
private:
	State m_state{}; //reflects the states of ants
	Activity m_activity{}; //ant can only engage in one activity at one time
	Animation ant_animation;
	

};
