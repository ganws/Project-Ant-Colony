#pragma once
#include"PathBlocker.h"
#include"SensoryCircle.h"
#include "PheromoneSystem.h"
#include <math.h>
#include"constant.h"
#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>

class PheromonSystem; //forward declaration

class SensoryInput
{
public:
	int m_count; //sensory circle counts
	std::vector<SensoryCircle> m_sensory_circle; // grid
	int m_facing_index{}; //index of nearest circle that the ant face.
	float m_active_num{};// number of neighbour circles from either side of face_indx
	std::vector<int> active_circle_indx; //list of active circle indexfor sensing pheromones;

public:
	//setter getters
	SensoryInput();
	SensoryInput(float r1, int count, sf::Vector2f ant_init, int active_circle_num); //constructor
	~SensoryInput();

	void resetClvlSum();
	void setPos(float dx, float dy);
	void detectPheromone(PheromoneSystem* psystem);
	void updateVisual();
	void updateFaceIndex(float ant_face_angle); //update the weight of sensory circles depending on the face angle of ants
	void updateWeight(std::vector<PathBlocker> *path_blck_ptr);
	void lightUpCircle(int selected_circle_indx);
};

