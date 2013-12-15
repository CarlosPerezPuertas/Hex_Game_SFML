#include "Hex.h"


Hex::Hex() : id(0), center(0, 0), state(EMPTY), sprite()
{

}

Hex::Hex(sf::Vector2f position, int id, TextureContainer &texture_manager) : 
sprite(texture_manager.get(Textures::EmptyHex))
{
	state = EMPTY;
	this->id = id;
	center.x = position.x + 28;
	center.y = position.y + 32;
	sprite.setPosition(position);
}

Hex::~Hex()
{

}



void Hex::draw(sf::RenderWindow &screen) const
{
	screen.draw(sprite);
}

void Hex::update() const
{

}

std::pair<bool, int> Hex::IsClicked(sf::Vector2f click_pos)
{
	click_pos.x = abs(click_pos.x - center.x); //We pass to local coordenates taking the center of the hex as origin
	click_pos.y = abs(click_pos.y - center.y);


	if (click_pos.x > h || click_pos.y > v * 2) return std::make_pair(false, id);
	if (2 * v*h - v*click_pos.x - h*click_pos.y >= 0) return std::make_pair(true, id);
}

void Hex::ChangeState(HEX_STATE state, TextureContainer &texture_manager)
{
	switch (state)
	{
		case EMPTY:
			sprite.setTexture(texture_manager.get(Textures::EmptyHex));
			break;
		case RED:
			sprite.setTexture(texture_manager.get(Textures::RedHex));
			break;
		case BLUE:
			sprite.setTexture(texture_manager.get(Textures::BlueHex));
			break;
	}

	this->state = state;
}
