#pragma once
#include "Pheromone.h"
#include "AntState.h"

#include<SFML/Graphics.hpp>
#include<SFML/Window.hpp>

class PheromoneSystem:public sf::Drawable//,public sf::Transformable
{
public:
	PheromoneSystem(unsigned int count); //constructr
	void decay(float dt);
	void createParticle(sf::Vector2f spawn_pos, Activity ant_activity);
	void destroyParticle(std::size_t index);
   // void draw(sf::RenderWindow *window);
    friend class SensoryInput;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        //states.transform *= getTransform();

        // our particles don't use a texture
        states.texture = NULL;

        // draw the vertex array
        for (auto i:m_active_index)
            target.draw(m_pvisual[i], states);
    }

private:
    float m_initial_strength{ 100.0f }; //initial strength of newly created particle
    std::vector<Pheromone> m_particle; //pheromone particle container
    std::vector<sf::CircleShape> m_pvisual; // visual for paticle
    std::vector<std::size_t> m_empty_indx; //list of unassigned index in m_vertices and m_particles
    std::vector<std::size_t> m_active_index; //list of active index of pheromone particles
    
};
