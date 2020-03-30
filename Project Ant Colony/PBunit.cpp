#include "PBunit.h"

PBunit::PBunit(sf::VertexArray vertexArray, int PBsize)
{
	pbunit.resize(4);
	// CONSTRUCT SQUARE PBunit
	if (vertexArray.getPrimitiveType() == sf::Quads &&
		vertexArray.getVertexCount() == 4)
	{

		pbunit.setPrimitiveType(vertexArray.getPrimitiveType());

		//define 4 corners
		pbunit[0].position = sf::Vector2f(.0f, 0.f);
		pbunit[1].position = sf::Vector2f(PBsize, .0f);
		pbunit[2].position = sf::Vector2f(PBsize, PBsize);
		pbunit[3].position = sf::Vector2f(0.f, PBsize);

		this->setOrigin(PBsize / 2.0f, PBsize / 2.0f);
		this->setColor(sf::Color::Blue);
	}

	this->m_size = sf::Vector2f(PBsize, PBsize);

}

PBunit::PBunit(const PBunit& copyPB)
{
	sf::PrimitiveType copyType = copyPB.pbunit.getPrimitiveType();
	sf::Color copyColor = copyPB.pbunit[0].color;
	int vertexCountCopy = copyPB.pbunit.getVertexCount();

	pbunit.setPrimitiveType(copyType);
	pbunit.resize(vertexCountCopy);

	if (copyType == sf::PrimitiveType::Quads)
	{
		//define 4 corners
		for (int i = 0; i < 4; i++)
		{
			this->pbunit[i].position.x = copyPB.pbunit[i].position.x;
			this->pbunit[i].position.y = copyPB.pbunit[i].position.y;
			this->pbunit[i].color = copyColor;
		}
	}
	this->setOrigin(copyPB.getOrigin());
	this->setPosition(copyPB.getPosition());
	this->setColor(copyColor);
	this->m_size = copyPB.m_size;
}

PBunit::~PBunit() {};

void PBunit::setColor(sf::Color color)
{
	for (int i = 0; i < pbunit.getVertexCount(); i++)
		pbunit[i].color = color;
}

sf::Vector2f PBunit::getSize()
{
	return this->m_size;
}