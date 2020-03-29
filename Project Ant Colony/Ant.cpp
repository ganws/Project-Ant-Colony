#include "Ant.h"

Ant::Ant() {};
Ant::Ant(const Ant& obj) {};
Ant::~Ant() {};

void Ant::initAnt(float size, sf::Vector2f init_pos, sf::Texture* skin, std::vector<PathBlocker>* arg_pblock_system,
	std::vector<Food>* arg_food_system, PheroMatrix* arg_pheromat_system)
{
	//sensor init
	m_sensorNumTotal = m_sensorNumPerSide * 2 + 1; //total sensor numbers, include the middle sensor
	m_sensorPosition.resize(m_sensorNumTotal);//sensor position array
	m_Ci.resize(m_sensorNumTotal); //weight array
	SensoryTracker.resize(m_sensorNumTotal); //sensor ball array
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
	setOrigin(static_cast<float> (skin_length / 2), static_cast<float> (skin_height / 2 ) );
	setScale(size, size);
	setPosition(init_pos);
	switchActivity(Activity::FORAGING);
	m_size.x = this->getTextureRect().width * size;
	m_size.y = this->getTextureRect().height * size;


	//environment pointers
	pblocker_systm_ptr = arg_pblock_system; //path block system
	food_system_ptr = arg_food_system; //food system
	pheromat_system_ptr = arg_pheromat_system; //pheromone matrix

	//Animation
	m_currentState = State::FORAGING;
	ant_animation.InitAnimation(skin, sf::Vector2u(8, 8), 62, 0.01f);
}

void Ant::issue_move_command(sf::Vector2f coordinate)
{
	m_targetCoord = coordinate; //coordinate vector
	sf::Vector2f moveVector = getPosition() - coordinate; //move vector

	sf::Vector2f yUnitVector{ 0.0f, -1.0f };
	sf::Vector2f xUnitVector{ 1.0f, 0.0f };
	float faceAngle = vectorAngle(yUnitVector, moveVector) * 180.0f / constants::pi;

	float cosX = dotProduct(xUnitVector, moveVector) / (normVector(moveVector) * normVector(xUnitVector));

	if (cosX > 0.0f)
		faceAngle = 360 - faceAngle;

	issue_rotate_command(faceAngle);
}
void Ant::issue_rotate_command(float facingAngle)
{
	m_targetFaceAngle = facingAngle;
}

// frame update
void Ant::Update(float dt)
{
	sf::Vector2f target_loc;
	sf::Time timer;

	switch (m_currentState)
	{
	case State::FORAGING:
	{
		this->transformAnt(dt);
		float smellMaxDist = 100.0f; //how far can the ant smell

		for (auto& food_particle : *food_system_ptr)
		{
			float foodDist = dist2Vec(food_particle.getPosition(), this->getPosition());
			if (foodDist < smellMaxDist) //upon food detection
			{
				m_target_food = &food_particle;
				m_lock_position = food_particle.getPosition();
				m_currentState = State::GO2FOOD;
			}
		}

		target_loc = this->computeMovement(dt);
		this->issue_move_command(target_loc);
		this->secretPheromon(dt, pheromat_system_ptr, -2.0f);

	}
	break;

	case State::GO2FOOD:
	{
		this->transformAnt(dt);
		float collectDist = 20.0f;
		float foodDist = dist2Vec(this->getPosition(), m_lock_position);

		if (foodDist < collectDist)
		{
			m_currentState = State::COLLECTING;
			m_foodCollectTimer.restart();
		}
		else
			issue_move_command(m_lock_position);

		break;
	}
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
			int harvest_amount{ 1 };
			m_gather_amount = m_target_food->Harvested(harvest_amount);
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
		float enterHoleDist = 3.0;
		float maxVisualDist = 80.0; //ant visual range
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
			target_loc = this->computeMovement(dt);

		this->issue_move_command(target_loc);
		this->transformAnt(dt);
		this->secretPheromon(dt, pheromat_system_ptr, 50);

		//update food scrap
		m_food_scrap.setFillColor(sf::Color::Green);
		sf::Vector2f currentFaceVector = this->getFaceVector();
		sf::Vector2f scrap_pos = this->getPosition() + scalarProduct(currentFaceVector, 12);
		m_food_scrap.setPosition(scrap_pos);
	}
	break;

	case State::ENTERHOLE:
	{
		m_gather_amount = 0; //reset carry amount
		m_colony->addResourceAmount(m_gather_amount);
		m_visible = false; //hide ant
		float holeTime{ 3.0 };
		timer = m_enterHoleTimer.getElapsedTime();
		if (timer.asSeconds() > holeTime)
		{
			float currentRotation = this->getRotation();
			this->setRotation(currentRotation - 180.0f);
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
void Ant::secretPheromon(float dt, PheroMatrix* pheroMat, float add_strength)
{
	m_internal_clock = m_internal_clock + dt;
	if (m_internal_clock >= m_pheromon_period) //secret pheromone at every fixed interval
	{
		pheroMat->addStrength(this->getPosition(), add_strength);
		m_internal_clock = 0.0f; //restart clock
	}
}
void Ant::drawSensoryCircle(sf::RenderWindow &window)
{
	for (unsigned int i = 0; i < (m_sensorNumPerSide * 2 + 1); i++)
	{
		window.draw(SensoryTracker[i]);
		SensoryTracker[i].setFillColor(sf::Color::Red);
	}
}

void Ant::transformAnt(float dt)
{
	// translation

	sf::Vector2f currentPosition = getPosition();
	m_lastPos = currentPosition;

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

	//update animation
	ant_animation.Update(dt);
	this->setTextureRect(ant_animation.m_uvRect);
}
sf::Vector2f Ant::computeMovement(float dt)
{

	// Ant has a number of "sensors" at the front of the head.
	// Each sensor "detects" pheromone level and objects, stored as sensor "weight"
	// At each time step, ant will select 1 sensor to move to based on "weight"
	// The higher the weight, the higher the probabilty.
	// Hence, the ant movement is probabilistic in nature

	// Probability of moving to sensor[i], 
	//         P(i) = C[i] / sum(C)  ,
	// where C:weight.


	//////////////////////////////////////
	//==========INITILIZATION===========//

	sf::Vector2u currentTilePos = pheromat_system_ptr->mapCoordsToPos(this->getPosition()); // compute ant current tile position
	sf::Vector2f currentFaceVector = this->getFaceVector();
	sf::Vector2f normalFaceVector(sf::Vector2f(-currentFaceVector.y, currentFaceVector.x));  	//normal vector
	sf::Vector2f vec2Chole = m_chole_pos - this->getPosition(); //vector from ant to colony hole

	//====calculate sensor positions===//

	m_sensorPosition[0] = this->getPosition() + scalarProduct(currentFaceVector, 15); //the middle sensor

	//right side
	for (unsigned int i = 0; i < m_sensorNumPerSide; i++)
		m_sensorPosition[i + 1] = m_sensorPosition[0] + scalarProduct(normalFaceVector, (i + 1) * m_sensorSpacing);

	//left side
	for (unsigned int i = 0; i < m_sensorNumPerSide; i++)
		m_sensorPosition[m_sensorNumPerSide + 1 + i] = m_sensorPosition[0] + scalarProduct(-normalFaceVector, (i + 1) * m_sensorSpacing);

	//m_sensorPosition[m_sensorNumPerSide] = m_sensorPosition[m_sensorNumPerSide] + scalarProduct(-currentFaceVector, 7) + scalarProduct(-normalFaceVector,5);
	//m_sensorPosition[m_sensorNumPerSide * 2] = m_sensorPosition[m_sensorNumPerSide * 2] + scalarProduct(-currentFaceVector, 7) + scalarProduct(normalFaceVector, 5);


	///////////////////////////////////////////////////
	//============COMPUTE SENSOR WEIGHT =============//

	float sum_C{ 0.0f };

	for (unsigned int i = 0; i < m_sensorNumTotal; i++)
	{
		SensoryTracker[i].setPosition(m_sensorPosition[i]);
		
		// all the terms and coefficients

		float Ci = 0.0f; // sensor weight
		float baseWeight = 5.0f; // base weight
		float choleCoeff = 1.0f; // angle to colony hole
		float pheroStr = pheromat_system_ptr->getStrengh(m_sensorPosition[i]); //pheromone strength
		float pheroCoeff; // sensitivity towards pheromone
		float terrainCoeff = m_terrain_system_ptr->getTerrainCoeff(m_sensorPosition[i]); // 1 = moveable space, 0 = blocked by terrain
		float collisionCoeff = m_terrain_system_ptr->getCollisionCoeff(m_sensorPosition[i]); // 1 = moveable space, 0 = blocked by other ants
		float priorityCoeff = 1.0f; // fine tune sensor weight

		//--------------------WEIGHT EQUATION (FORAGING) ------------------------

		if (m_currentState == State::FORAGING)
		{
			pheroCoeff = 2.0f;
			baseWeight = 5.0f;
			if (i == 0)
				priorityCoeff = 5.0f; // increase middle sensor weight

			Ci = priorityCoeff * collisionCoeff * terrainCoeff *(baseWeight + pheroCoeff * pheroStr );

		}
		//---------------------------------------------------------------------------
		//--------------------WEIGHT EQUATION ( GATHERING ) -------------------------

		if (m_currentState == State::GATHERING)
		{
			pheroCoeff = 1.0f;
			baseWeight = 2.0f;
			sf::Vector2f sensor_vec = m_sensorPosition[i] - this->getPosition(); // vector of ant to sensor
			choleCoeff = abs(vectorAngle(sensor_vec, vec2Chole) / constants::pi * 180.0f);


			Ci = 5* choleCoeff + priorityCoeff * collisionCoeff * terrainCoeff * (baseWeight + pheroCoeff * pheroStr);

		}

		//--------------------------------------------------------------------------------

		m_Ci[i] = static_cast<int>(Ci);
		sum_C += m_Ci[i];
	}


	///////////////////////////////////////////////////
	///-------------COMPUTE MOVEMENT-------------//////

	//if front are unmovable, ant turns back
	if (sum_C <= 0.1f)
		return this->getPosition() + scalarProduct(-currentFaceVector, 20);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::discrete_distribution<> weight_PDistrib(m_Ci.begin(), m_Ci.end()); // create discrete probability distribution
	int moveToIndex = weight_PDistrib(gen); // decide next move based on probability distribution

	SensoryTracker[moveToIndex].setFillColor(sf::Color::Green); // selected sensor lights up

	return m_sensorPosition[moveToIndex];
}

//Setter and Getters
void Ant::switchActivity(Activity new_activity)
{
	m_activity = new_activity;
}
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

sf::Vector2f Ant::getLastPos()
{
	return m_lastPos;
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

