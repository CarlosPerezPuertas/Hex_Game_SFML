#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "TextureContainer.h"

enum HEX_STATE{ EMPTY, RED, BLUE };


class Hex : public Entity
{
	private:
		static const int v = 16;
		static const int h = 28;

		int id;
		sf::Vector2f center;
		HEX_STATE state;

		sf::Sprite sprite;

	public:
		Hex();
		Hex(sf::Vector2f position, int id, TextureContainer &texture_manager);
		~Hex();

		virtual void draw(sf::RenderWindow &screen) const;
		virtual void update() const;

		std::pair<bool, int> IsClicked(sf::Vector2f click_pos);
		inline int Id(){ return id; }
		void ChangeState(HEX_STATE state, TextureContainer &texture_manager);
		inline HEX_STATE GetState() { return state; }
	
};