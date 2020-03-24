#include "Ant.h"

//member funciton definition

//constructor
Ant::Ant() {};
//Ant::Ant(std::vector<PathBlocker>* arg_pblock_system) 
//{
//	pblocker_systm_ptr = arg_pblock_system;
//};
Ant::Ant(const Ant& obj) {};
Ant::~Ant() {}; //destructor


//=======issue move command=====
void Ant::issue_move_command(sf::Vector2f coordinate)
{
	m_targetCoord = coordinate; //coordinate vector
	sf::Vector2f moveVector = getPosition() - coordinate; //move vector

	sf::Vector2f yUnitVector{ 0.0f, -1.0f };
	sf::Vector2f xUnitVector{ 1.0f, 0.0f };
	float faceAngle = vectorAngle(yUnitVector, moveVector) * 180.0 / constants::pi;

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
void Ant::updateMovement(float dt)
{
	// translation

	sf::Vector2f currentPosition = getPosition();
	sf::Vector2f moveVector = m_targetCoord - currentPosition;

	if (normVector(moveVector) >= 2) //ant move to precision of +-2 pixels
	{
		move(unitVector(moveVector) * m_movespeed * dt);
		//m_sensory_input.setPos(this->getPosition().x, this->getPosition().y);
	}

	//rotation
	float rotateDirection{ 1 }; //clockwise = 1, anticlockwise = -1
	float currentFaceAngle = getRotation();
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

		if (m_rotatespeed * dt > deltaAngle)
			rotate(deltaAngle * rotateDirection);
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

void Ant::secretPheromon2(float dt, PheroMatrix* pheroMat)
{
	m_internal_clock = m_internal_clock + dt;
	if (m_internal_clock >= m_pheromon_period) //secret pheromone at every fixed interval
	{
		switch (m_activity)
		{
		case Activity::GATHERING:
			pheroMat->addStrength(this->getPosition(), 50);
			break;

		default:
			pheroMat->addStrength(this->getPosition(), 50);
			break;
		}
		m_internal_clock = 0.0f; //restart clock
	}
}


void Ant::switchActivity(Activity new_activity)
{
	//std::cout << "Ant activity changed from " << printActivity(m_activity) << " to " << printActivity(new_activity) << "\n";
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
	for (int i = 0; i < (m_sensorNumPerSide * 2 + 1); i++)
	{
		window->draw(SensoryTracker[i]);
		SensoryTracker[i].setFillColor(sf::Color::Red);

	}
}

//Setter and Getters
Activity Ant::getActivity()
{
	//std::cout<< "Current activity: "<< printActivity(m_activity) <<"\n";//debug message
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

void Ant::rememberFoodLoc(sf::Vector2f food_loc)
{
	m_food_pos = food_loc;
}

sf::Vector2f Ant::recallFoodLoc()
{
	return m_food_pos;
}

void Ant::forgetFoodLoc()
{
	m_food_pos = {};
}

void Ant::initAnt(float size, sf::Vector2f init_pos, sf::Texture* skin, std::vector<PathBlocker>* arg_pblock_system,
	std::vector<Food>* arg_food_system, PheroMatrix* arg_pheromat_system)
{
	//sensor init
	m_sensorPosition.resize(m_sensorNumPerSide * 2 + 1); //include the middle sensor
	m_Ci.resize(m_sensorNumPerSide * 4 + 1);
	SensoryTracker.resize(m_sensorNumPerSide * 2 + 1);
	for (auto& s : SensoryTracker)
	{
		s.setFillColor(sf::Color::Red);
		s.setRadius(3);
		s.setOrigin(s.getRadius(), s.getRadius());
	}

	//food scrap init
	m_food_scrap.setFillColor(sf::Color::Transparent);
	m_food_scrap.setRadius(3);
	m_food_scrap.setOrigin(m_food_scrap.getRadius(), m_food_scrap.getRadius());



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


	//environment pointers
	pblocker_systm_ptr = arg_pblock_system; //path block system
	food_system_ptr = arg_food_system; //food system
	pheromat_system_ptr = arg_pheromat_system; //pheromone matrix

	//Animation
	m_currentState = State::FORAGING;
	ant_animation.InitAnimation(skin, sf::Vector2u(8, 8), 62, 0.01);
}

sf::Vector2f Ant::computeMovementMatrix(float dt)
{
	/////////////////////////////////
	/////////////////////////////////
	//=====define sensor area======//
	sf::Vector2u currentTilePos = pheromat_system_ptr->mapCoordsToPos(this->getPosition()); // compute ant current tile position

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
		m_sensorPosition[i + 1] = m_sensorPosition[0] + scalarProduct(-currentFaceVector, 0 * (i * i + 1)) + scalarProduct(normalFaceVector, (i + 1) * m_sensorSpacing);
	//left side
	for (int i = 0; i < m_sensorNumPerSide; i++)
		m_sensorPosition[m_sensorNumPerSide + 1 + i] = m_sensorPosition[0] + scalarProduct(-currentFaceVector, 0 * (i * i + 1)) + scalarProduct(-normalFaceVector, (i + 1) * m_sensorSpacing);

	//m_sensorPosition[m_sensorNumPerSide] = m_sensorPosition[m_sensorNumPerSide] + scalarProduct(-currentFaceVector, 7) + scalarProduct(-normalFaceVector,5);
	//m_sensorPosition[m_sensorNumPerSide * 2] = m_sensorPosition[m_sensorNumPerSide * 2] + scalarProduct(-currentFaceVector, 7) + scalarProduct(normalFaceVector, 5);

	for (int i = 0; i < sensorTotalNum; i++)
		SensoryTracker[i].setPosition(m_sensorPosition[i]);

	/////////////////////////////////////////////////
	//============COMPUTE PROBABILITY=============//
	//probability of moving to circle i, P(i) = Ci / sum(C)
	float sum_C{ 0.0 };
	//	for (int i = 0; i < sensorTotalNum; i++)
	//		m_Ci[i] = 1.0;

		// populate all Ci
	for (int i = 0; i < sensorTotalNum; i++)
	{
		m_Ci[i] = 2.0;
		//if ((i == m_sensorNumPerSide) | (i == sensorTotalNum - 1))
			//m_Ci[i] = 0.1;
		m_Ci[i] += pheromat_system_ptr->getStrengh(m_sensorPosition[i]) * 2;

		if (collision_check)
		{
			for (auto& pb : *pblocker_systm_ptr)
			{
				if (pb.getGlobalBounds().contains(m_sensorPosition[i]))
				{
					m_Ci[i] = 0;
					//std::cout << "collide!\n";

				}
			}

			//only check the one foraging
			if (m_activity == Activity::FORAGING)
			{
				for (auto& fs : *food_system_ptr)
					if (fs.getGlobalBounds().contains(m_sensorPosition[i]))
					{
						//m_state.CARRYING = true;
						m_activity = Activity::GATHERING;
						rememberFoodLoc(fs.getPosition());
						std::cout << "Food found!!\n";

					}
			}
		}

		sum_C += m_Ci[i];
	}

	//if no pheromone detected, make all circle equal chance
	if (sum_C <= constants::near_zero)
	{
		//std::cout << "path blocked!\n";
		return sf::Vector2f(500, 500);
		//return scalarProduct(-currentFaceVector, 15);
	}

	/////////////////////////////////////////////
	//-------------DECISION MAKING-------------//

	//create discrete distribution
	m_Ci[0] *= 5;
	std::discrete_distribution<> str_PDistrib(m_Ci.begin(), m_Ci.end());

	std::random_device rd;
	std::mt19937 gen(rd());
	int moveToIndex = str_PDistrib(gen);
	SensoryTracker[moveToIndex].setFillColor(sf::Color::Green);

	return m_sensorPosition[moveToIndex];
}

sf::Vector2f Ant::computeMovement_colony(float dt, sf::Vector2f colony_hole_pos)
{
	/////////////////////////////////
	/////////////////////////////////
	//=====define sensor area======//
	sf::Vector2u currentTilePos = pheromat_system_ptr->mapCoordsToPos(this->getPosition()); // compute ant current tile position

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
	sf::Vector2f vec2Chole = colony_hole_pos - this->getPosition(); //vector from ant to colony hole

	//sensors settings
	unsigned const int sensorTotalNum = m_sensorNumPerSide * 2 + 1; //total sensor number

	///////////////////////////////////
	//====define sensor positions===//

	//the middle sensor
	m_sensorPosition[0] = this->getPosition() + scalarProduct(currentFaceVector, 15);

	//right side
	for (int i = 0; i < m_sensorNumPerSide; i++)
		m_sensorPosition[i + 1] = m_sensorPosition[0] + scalarProduct(-currentFaceVector, 0 * (i * i + 1)) + scalarProduct(normalFaceVector, (i + 1) * m_sensorSpacing);
	//left side
	for (int i = 0; i < m_sensorNumPerSide; i++)
		m_sensorPosition[m_sensorNumPerSide + 1 + i] = m_sensorPosition[0] + scalarProduct(-currentFaceVector, 0 * (i * i + 1)) + scalarProduct(-normalFaceVector, (i + 1) * m_sensorSpacing);

	for (int i = 0; i < sensorTotalNum; i++)
		SensoryTracker[i].setPosition(m_sensorPosition[i]);

	/////////////////////////////////////////////////
	//============COMPUTE PROBABILITY=============//
	//probability of moving to circle i, P(i) = Ci / sum(C)
	float sum_C{ 0.0 };
	//	for (int i = 0; i < sensorTotalNum; i++)
	//		m_Ci[i] = 1.0;

		// populate all Ci
	for (int i = 0; i < sensorTotalNum; i++)
	{
		m_Ci[i] = 5.0;
		m_Ci[i] += pheromat_system_ptr->getStrengh(m_sensorPosition[i])*0.1;
		sf::Vector2f sensor_vec = m_sensorPosition[i] - this->getPosition();
		float angle2Chole = vectorAngle(sensor_vec, vec2Chole);
		angle2Chole = abs(angle2Chole / constants::pi * 180.0);
		float distanceCoeef = dist2Vec(m_chole_pos, this->getPosition()); //the closer to colony hole, the more accurate it is

		m_Ci[i] *= angle2Chole * distanceCoeef;

		if (collision_check)
		{
			for (auto& pb : *pblocker_systm_ptr)
			{
				if (pb.getGlobalBounds().contains(m_sensorPosition[i]))
				{
					m_Ci[i] = 0;
					//std::cout << "collide!\n";

				}
			}



			//only check the one foraging
			/*if (m_activity == Activity::FORAGING)
			{
				for (auto& fs : *food_system_ptr)
					if (fs.getGlobalBounds().contains(m_sensorPosition[i]))
					{
						m_state.CARRYING = true;
						m_activity = Activity::GATHERING;
						rememberFoodLoc(fs.getPosition());
						std::cout << "Food found!!\n";

					}
			}*/
		}

		for (auto& foodp : *food_system_ptr)
		{
			if (foodp.getGlobalBounds().contains(m_sensorPosition[i]))
			{
				m_Ci[i] = 0;
				//std::cout << "collide!\n";

			}
		}
		sum_C += m_Ci[i];
	}

	////if all route are blocked, turn back
	if (sum_C <= constants::near_zero)
		return scalarProduct(-currentFaceVector, 15);

	/////////////////////////////////////////////
	//-------------DECISION MAKING-------------//

	//create discrete distribution
	//m_Ci[0] *= 5;
	std::discrete_distribution<> str_PDistrib(m_Ci.begin(), m_Ci.end());

	std::random_device rd;
	std::mt19937 gen(rd());
	int moveToIndex = str_PDistrib(gen);
	SensoryTracker[moveToIndex].setFillColor(sf::Color::Green);

	return m_sensorPosition[moveToIndex];
}

sf::Vector2f Ant::getFaceVector()
{
	float currentFaceAngle = this->getRotation();

	//make angle system consistent 
	currentFaceAngle -= 90;
	if (currentFaceAngle < 0)
		currentFaceAngle = 360 + currentFaceAngle;

	// facing vector
	sf::Vector2f currentFaceVector;
	currentFaceVector.x = cos(currentFaceAngle * constants::pi / 180);
	currentFaceVector.y = sin(currentFaceAngle * constants::pi / 180);

	return currentFaceVector;
}


sf::Vector2f Ant::recallCholePos()
{
	return m_chole_pos;
}
void Ant::rememberCholePos(sf::Vector2f chole_pos)
{
	m_chole_pos = chole_pos;
}

// update with state machine
void Ant::Update(float dt)
{
	sf::Vector2f target_loc;
	sf::Time timer;

	switch (m_currentState)
	{
	case State::FORAGING:
	{
		this->updateMovement(dt);
		float smellMaxDist = 100.0f;
		for (auto& food_particle : *food_system_ptr)
		{
			float foodDist = dist2Vec(food_particle.getPosition(), this->getPosition());
			if (foodDist < smellMaxDist) //upon food detection
			{
				std::cout << "food found!\n";
				m_target_food = &food_particle;
				m_lock_position = food_particle.getPosition();
				m_currentState = State::GO2FOOD;
			}
		}
		target_loc = this->computeMovementMatrix(dt);
		this->issue_move_command(target_loc);
	}
	break;

	case State::GO2FOOD:
	{
		this->updateMovement(dt);
		float collectDist = 10.0f;
		float foodDist = dist2Vec(this->getPosition(), m_lock_position);
		if (foodDist < collectDist)
		{
			m_currentState = State::COLLECTING;
			m_foodCollectTimer.restart();
		}
		else
			issue_move_command(m_lock_position);
	}
	break;

	case State::COLLECTING:
	{
		float collectTime = 2.0f; //time taken to collect food
		timer = m_foodCollectTimer.getElapsedTime();
		if (m_target_food->depleted)
		{
			m_currentState = State::FORAGING;
			break;
		}
		if (timer.asSeconds() > collectTime)
		{
			int harvest_amount{1};
			gather_amount = m_target_food->Harvested(harvest_amount);
			m_currentState = State::GATHERING;

			m_food_scrap.setFillColor(sf::Color::Green); //make food scrap visible
			m_food_scrap.setOutlineColor(sf::Color::Black);
			sf::Vector2f currentFaceVector = this->getFaceVector();
			m_food_scrap.setPosition(this->getPosition() + scalarProduct(currentFaceVector, 12));

		}
	}
	break;


	case State::GATHERING:
	{
		float enterHoleDist = 2.0;
		float maxVisualDist = 100.0; //ant visual range
		float choleDist = dist2Vec(this->getPosition(), m_chole_pos);

		//check distance to hole
		if (choleDist < maxVisualDist) //check if colony hole is within visual range
		{
			if (choleDist > enterHoleDist) //check if ant can enter hole yet
				target_loc = m_chole_pos;
			else
			{
				m_currentState = State::ENTERHOLE;
				m_enterHoleTimer.restart();
			}
		}
		else
			target_loc = this->computeMovement_colony(dt, m_chole_pos);

		this->issue_move_command(target_loc);
		this->updateMovement(dt);
		this->secretPheromon2(dt, pheromat_system_ptr);

		//update food scrap
		m_food_scrap.setFillColor(sf::Color::Green);
		sf::Vector2f currentFaceVector = this->getFaceVector();
		sf::Vector2f scrap_pos = this->getPosition() + scalarProduct(currentFaceVector, 12);
		m_food_scrap.setPosition(scrap_pos);
	}
	break;

	case State::ENTERHOLE:
	{
		gather_amount = 0; //reset carry amount
		m_visible = false; //hide ant
		float holeTime{ 3.0 };
		timer = m_enterHoleTimer.getElapsedTime();
		if (timer.asSeconds() > holeTime)
		{
			float currentRotation = this->getRotation();
			this->setRotation(currentRotation - 180.0);
			m_visible = true;
			m_currentState = State::FORAGING;
			m_food_scrap.setFillColor(sf::Color::Transparent); //remove food scrap
		}
	}
	break;
	case State::IDLE:
		break;
	default:
		break;
	}
}