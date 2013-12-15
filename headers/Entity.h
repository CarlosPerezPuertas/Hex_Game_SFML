#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
	public:
		Entity();
		virtual ~Entity();

	private:
		virtual void draw(sf::RenderWindow &screen) const;
		virtual void update() const;

};

