#include "Ant.h"

//member funciton definition

//constructor
Ant::Ant() {};
//Ant::Ant(std::vector<PathBlocker>* arg_pblock_system) 
//{
//	pblocker_systm_ptr = arg_pblock_system;
//};
Ant::Ant(const Ant& obj){}

Ant::~Ant() {}; //destructor


//=======issue move command=====
void Ant::issue_move_command(sf::Vector2f coordinate)
{
	m_targetCoord = coordinate; //coordinate vector
	sf::Vector2f moveVector = getPosition()-coordinate; //move vector
	
	sf::Vector2f yUnitVector{ 0.0f, -1.0f };
	sf::Vector2f xUnitVector{ 1.0f, 0.0f };
	float faceAngle = vectorAngle(yUnitVector, moveVector)*180.0/constants::pi;

	float cosX = dotProduct(xUnitVector, moveVector) / (normVector(moveVector) * normVector(xUnitVector));

	if (cosX > 0)
		faceAngle = 360 - faceAngle;

	issue_face_command(faceAngle);
	//std::cout << "move command issued: [" << coordinate.x << "  " << coordinate.y << "], rotate " << faceAngle << " deg.\n";
}

//=====issue rotate command=====
void Ant::issue_face_command(float facingAngle)
{
	m_targetFaceAngle = facingAngle;
}


// frame update
void Ant::updateMovement(float dt, std::vector<PathBlocker>* path_blck_ptr)
{	
	// translation

	sf::Vector2f currentPosition = getPosition();
	sf::Vector2f moveVector = m_targetCoord- currentPosition;

	if (normVector(moveVector) >= 2) //ant move to precision of +-2 pixels
	{
		move(unitVector(moveVector) * m_movespeed * dt);
		//m_sensory_input.setPos(this->getPosition().x, this->getPosition().y);
	}
	

	//rotation
	float rotateDirection{1}; //clockwise = 1, anticlockwise = -1
	float currentFaceAngle= getRotation();
	float deltaAngle = fabsf(currentFaceAngle - m_targetFaceAngle);

	if (deltaAngle >= 2.0f) //ants rotate up to precision of +- 2degree
	{
		// decide to rotate clockwise or anticlockwise
		if (currentFaceAngle < m_targetFaceAngle)
			if (fabsf(currentFaceAngle - m_targetFaceAngle) < 180.0f)
				rotateDirection = 1; //clockwise
			else rotateDirection = -1;

		if (currentFaceAngle > m_targetFaceAngle)
			if (fabsf(currentFaceAngle - m_targetFaceAngle) < 180.0f)
				rotateDirection = -1; //clockwise
			else rotateDirection = 1;

		if(m_rotatespeed*dt > deltaAngle)
			rotate(deltaAngle*rotateDirection);
		else
			rotate(rotateDirection * m_rotatespeed * dt);
		//std::cout << getRotation() << "\n";
	}
	//sensory
	//m_sensory_input.updateFaceIndex(this->getRotation());
	//m_sensory_input.updateWeight(path_blck_ptr);

	//int target_circle_indx = computeMoveTarget();
	//sf::Vector2f targetLoc =  m_sensory_input.m_sensory_circle[target_circle_indx].getPosition();
	//issue_move_command(targetLoc);

	//update animation
	ant_animation.Update(dt);
	this->setTextureRect(ant_animation.m_uvRect);
}
void Ant::secretPheromon(float dt, PheromoneSystem* psystem)
{
	m_internal_clock = m_internal_clock + dt; 
	if (m_internal_clock >= m_pheromon_period) //secret pheromone at every fixed interval
	{
		psystem->createParticle(this->getPosition(), this->m_activity);
		m_internal_clock = 0.0f; //restart clock
	}
}

void Ant::secretPheromon2(float dt, PheroMatrix *pheroMat)
{
	m_internal_clock = m_internal_clock + dt;
	if (m_internal_clock >= m_pheromon_period) //secret pheromone at every fixed interval
	{
		pheroMat->addStrength(this->getPosition(), 50);
		m_internal_clock = 0.0f; //restart clock
	}
}


void Ant::switchActivity(Activity new_activity)
{
	std::cout << "Ant activity changed from " << printActivity(m_activity) << " to " << printActivity(new_activity) << "\n";
	m_activity = new_activity;
}
void Ant::sensePheromone()
{

}
void Ant::drawSensoryCircle(sf::RenderWindow* window)
{
	//for (int i = 0; i < m_sensory_input.m_count; i++)
	//{
		//std::cout << "draw circle" << "\n";
	//	window->draw(m_sensory_input.m_sensory_circle[i]);
	//}
	window->draw(SensoryTracker);
}

//Setter and Getters
Activity Ant::getActivity()
{
	std::cout<< "Current activity: "<< printActivity(m_activity) <<"\n";//debug message
	return m_activity;
}
float Ant::getMoveSpeed()
{
	return this->m_movespeed;
}
void Ant::setMoveSpeed(float movespeed)
{
	this->m_movespeed = movespeed;
}

void Ant::initAnt(float size, sf::Vector2f init_pos, sf::Texture *skin, std::vector<PathBlocker>* arg_pblock_system)
{
	SensoryTracker.setFillColor(sf::Color::Transparent);
	SensoryTracker.setRadius(5);
	SensoryTracker.setOrigin(2.5, 2.5);

	m_sensorPosition.resize(m_sensorNumPerSide * 2 + 1); //include the middle sensor
	m_Ci.resize(m_sensorNumPerSide*2 +1); //set size

	//Texture and size
	int skin_length = skin->getSize().x / 8; // length of single frame from animation
	int skin_height = skin->getSize().y / 8; // height of single frame from animation
	this->setTexture(*skin);
	setTextureRect(sf::IntRect(0, 0, skin_length, skin_height));
	setOrigin(skin_length / 2, skin_height / 2);
	setScale(size, size);
	setPosition(init_pos);
	switchActivity(Activity::FORAGING);
	m_size.x = getLocalBounds().height;
	m_size.y = getLocalBounds().width;

	pblocker_systm_ptr = arg_pblock_system; //path block system

	//state init
	//m_activity = Activity::IDLE; //init
	//m_sensory_input.setPos(init_pos.x , init_pos.y);
	this->issue_move_command(init_pos);

	//Animation
	ant_animation.InitAnimation(skin, sf::Vector2u(8, 8), 62, 0.01);
	std::cout << "Ant initialized, pblock ptr=" << pblocker_systm_ptr << "\n";
}

//int Ant::computeMoveTarget()
//{
//	//probability of moving to circle i, P(i) = Ci / sum(C)
//	double sum_C{0};
//	
//	std::vector<double> Ci_vector;
//	for (int n = 0; n < m_scircle_count; n++)
//	{
//		Ci_vector.push_back(m_sensory_input.m_sensory_circle[n].Ci);
//		sum_C += m_sensory_input.m_sensory_circle[n].Ci;
//	}
//
//	//if no pheromone detected, make all circle equal chance
//	if (sum_C <= constants::near_zero)
//		for (int n = 0; n < m_scircle_count; n++)
//			Ci_vector[n] = 1.0;
//	Ci_vector[m_sensory_input.m_facing_index] = 3;
//
//	//create discrete distribution
//	std::discrete_distribution<int> target_circle_PDistrib (Ci_vector.begin(), Ci_vector.end());
//
//	//displaying probabilities
//	/*for (double x : target_circle_PDistrib.probabilities())
//		std::cout << x << " ";
//	std::cout << std::endl;*/
//
//	//decision making
//	std::random_device rd;
//	std::mt19937 gen(rd());
//	int target_circle_index = target_circle_PDistrib(gen);
//
//	//visual update to sensory circle
//	m_sensory_input.lightUpCircle(target_circle_index);
//
//	return target_circle_index;
//	
//}

// matrix version
sf::Vector2f Ant::computeMovementMatrix(float dt, PheroMatrix* pheroMat)
{
	/////////////////////////////////
	//=====define sensor area======//
	sf::Vector2u currentTilePos = pheroMat->mapCoordsToPos(this->getPosition()); // compute ant current tile position

	float currentFaceAngle = this->getRotation();

	//make angle system consistent 
	currentFaceAngle -= 90;
	if (currentFaceAngle < 0)
		currentFaceAngle = 360 + currentFaceAngle;

	// facing vector
	sf::Vector2f currentFaceVector;
	currentFaceVector.x = cos(currentFaceAngle * constants::pi / 180);
	currentFaceVector.y = sin(currentFaceAngle * constants::pi / 180);
	sf::Vector2f normalFaceVector(sf::Vector2f(-currentFaceVector.y, currentFaceVector.x));  	//normal vector

	//sensors settings
	unsigned const int sensorTotalNum = m_sensorNumPerSide * 2 + 1; //total sensor number

	///////////////////////////////////
	//====define sensor positions===//

	//the middle sensor
	m_sensorPosition[0] = this->getPosition() + scalarProduct(currentFaceVector, 15); 

	//right side
	for (int i = 0; i < m_sensorNumPerSide; i++) 
		m_sensorPosition[i+1] = m_sensorPosition[0] + scalarProduct(normalFaceVector, (i+1)*m_sensorSpacing);
	//left side
	for (int i = 0; i < m_sensorNumPerSide; i++)
		m_sensorPosition[m_sensorNumPerSide+1+i] = m_sensorPosition[0] + scalarProduct(-normalFaceVector, (i+1)*m_sensorSpacing);

	SensoryTracker.setPosition(m_sensorPosition[0]);

	/////////////////////////////////////////////////
	//============COMPUTE PROBABILITY=============//
	//probability of moving to circle i, P(i) = Ci / sum(C)
	float sum_C{ 0.0 };
//	for (int i = 0; i < sensorTotalNum; i++)
//		m_Ci[i] = 1.0;

	// populate all Ci
	for (int i = 0;i < sensorTotalNum; i++)
	{
		m_Ci[i] = 1.0;
		m_Ci[i] += pheroMat->getStrengh(m_sensorPosition[i]);

		/*if (collision_check)
		{
			for (auto& pb : *pblocker_systm_ptr)
			{
				if (pb.getGlobalBounds().contains(m_sensorPosition[i]))
					m_Ci[i] = 0;
			}
		}*/
		sum_C += m_Ci[i];
	}

	//if no pheromone detected, make all circle equal chance
	if (sum_C <= constants::near_zero)
		return sf::Vector2f(500, 500);

	//create discrete distribution
	std::discrete_distribution<int> str_PDistrib(m_Ci.begin(), m_Ci.end());

	//decision making
	std::random_device rd;
	std::mt19937 gen(rd());
	int moveToIndex = str_PDistrib(gen);

	return m_sensorPosition[moveToIndex];
}





