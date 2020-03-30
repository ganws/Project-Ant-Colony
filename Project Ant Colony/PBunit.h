#pragma once
#include <SFML/Graphics.hpp>

class PBunit : public sf::Drawable, public sf::Transformable
{
public:
	//basic unit of PathBlocker (PBunit) 

	sf::VertexArray pbunit;

public:

	PBunit(sf::VertexArray vertexArray, int PBsize);
	PBunit(const PBunit& copyPB);
	~PBunit();

	void setColor(sf::Color color);
	sf::Vector2f getSize();

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(pbunit, states);
	}

	sf::Vector2f m_size;

};

