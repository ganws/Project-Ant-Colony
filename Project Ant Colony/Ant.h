#pragma once

#include"AntState.h"
#include "VectorFunction.h"
#include "PathBlocker.h"
#include "Animation.h"
#include "PheroMatrix.h"
#include "constant.h"	
#include "Food.h"
#include "Colony.h"
#include "Terrain.h"

#include<random>
#include<math.h>
#include<SFML/Graphics.hpp>

// Activity and State determine the basic behaviour of ants. An ant can have multiple states but only 1 activity at a time (no multitasking yet).
// For simple analogy, brushing teeth is an activity while state of the person is "hand-moving" and "stationery".

class Colony;
class Ant;

class Ant : public sf::Sprite
{
public:

	//constant stats
	float m_movespeed{ 120.0f }; //movespeed in pixel per second
	float m_rotatespeed{ 530.0f }; //degree per sec;
	float m_HP{}; //hitpoint
	float m_attack{}; //attack
	float m_dmgtaken{}; // dmg taken per timestep
	sf::Vector2f m_size; //ant size
	float m_age{}; //age [days]
	float m_pheromon_period{ 0.03f }; //pheromon secreting  period [s]
	float m_internal_clock{ 0.0f }; // for various counting

	bool collision_check{ false }; //true:check collision

	std::vector<sf::CircleShape> SensoryTracker; //for debug

	sf::Vector2f m_targetCoord{ 0.0,0.0 };  //absolute coordinate vector in map
	float m_targetFaceAngle{}; //absolute angle [0, 360)


public:

	//constructor, destructor
	Ant();
	Ant(const Ant& obj);
	~Ant();

	void Update(float dt);

	void initAnt(float size, sf::Vector2f init_pos, sf::Texture* texture, std::vector<PathBlocker>* arg_pblock_system, std::vector<Food>* arg_food_system, PheroMatrix* pheromat_sytm);
	void updateMovement(float dt);

	// pheromones interaction
	void secretPheromon2(float dt, PheroMatrix* phermatrix, float str); //secret pheromone on spot (matrix version)
	void drawSensoryCircle(sf::RenderWindow &window);

	//getting and setter
	void setMoveSpeed(float movespeed);
	float getMoveSpeed();

	void switchActivity(Activity new_activity); //switch to new activity
	Activity getActivity(); //return current activity

	State getState(); //return all current states
	void switchState(State state_target, bool new_state); //turning a state on and off 

	sf::Vector2f getLastPos();

	sf::Vector2f recallFoodLoc(); //recall food location
	void rememberFoodLoc(sf::Vector2f food_loc); //remember food location
	void forgetFoodLoc();

	sf::Vector2f recallCholePos();
	void rememberCholePos(sf::Vector2f chole_pos);

	// Command function
	void issue_move_command(sf::Vector2f coordinate); //issue move command. Issuing this command
	void issue_face_command(float facingAngle); //rotate ants (in degree)

	// compute movement
	sf::Vector2f computeMovementMatrix(float dt); //foraging movement
	sf::Vector2f computeMovement_colony(float dt, sf::Vector2f colony_hole_pos); //find way back to colony

private:

	// boolean
	bool m_visible{ true };

	// ant state
	enum class State
	{
		FORAGING,
		GO2FOOD,
		COLLECTING,
		GATHERING,
		ENTERHOLE,
		IDLE
	};
	State m_currentState{State::FORAGING}; //Ant current state
	Activity m_activity{}; //ant can only engage in one activity at one time
	Animation ant_animation;

	//timer
	sf::Clock m_foodCollectTimer;
	sf::Clock m_enterHoleTimer;

	//gather
	int gather_amount;
	sf::CircleShape m_food_scrap;

	//sensors
	std::vector<float> m_Ci; //strength of sensor
	unsigned int m_sensorNumPerSide{ 3 }; //sensors per side
	float m_sensorSpacing{ 5 }; // space between sensor
	std::vector<sf::Vector2f> m_sensorPosition; // sensor positions

	// ant memory
	sf::Vector2f m_chole_pos{}; //colony hole location
	sf::Vector2f m_food_pos{}; //temporary food location in ant small memory
	sf::Vector2f m_lastPos; //position of previous update
	sf::Vector2f m_lock_position; //fixed target location to move to
	std::list <sf::Vector2f> m_MoveQueue;

	//environment pointers
	std::vector<PathBlocker>* pblocker_systm_ptr{ nullptr };
	std::vector<Food>* food_system_ptr{ nullptr };
	PheroMatrix* pheromat_system_ptr{ nullptr };
	Food* m_target_food{ nullptr };
	Terrain* m_terrain_system_ptr{ nullptr };

	friend class Colony;
	Colony* m_colony;
	
private:
	sf::Vector2f getFaceVector();
};
