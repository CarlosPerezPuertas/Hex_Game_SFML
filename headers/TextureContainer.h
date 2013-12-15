#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>


namespace Textures
{
	enum ID
	{
		EmptyHex,
		BlueHex,
		RedHex,
		Board
	};
}

class TextureContainer
{
	public:
		TextureContainer();
		virtual ~TextureContainer();

		void load(int id);
		void load(int id, const std::string &file_name);
		void insert_resource(int id, std::unique_ptr<sf::Texture> resource);

		sf::Texture& get(int id);
		const sf::Texture& get(int id) const;

	private:
		std::map<int, std::unique_ptr<sf::Texture>> textures;
		std::map<int, std::string> textures_direction;
};

