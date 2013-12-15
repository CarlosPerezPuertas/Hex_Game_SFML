#include "TextureContainer.h"


TextureContainer::TextureContainer()
{
	textures_direction[Textures::EmptyHex] = "textures/empty_hex_2.png";
	textures_direction[Textures::BlueHex] = "textures/blue_hex_2.png";
	textures_direction[Textures::RedHex] = "textures/red_hex_2.png";
	textures_direction[Textures::Board] = "textures/board_background.png";
}


TextureContainer::~TextureContainer()
{
}

//LOAD TEXTURE WITHOUT USING FILENAME. THE ID IS MAPPED
void TextureContainer::load(int id)
{
	// Create and load resource
	std::unique_ptr<sf::Texture> resource(new sf::Texture());

	if (!resource->loadFromFile(textures_direction[id]))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + textures_direction[id]);

	// insert resource to map
	insert_resource(id, std::move(resource));
}

//LOAD TEXTURE USING FILENAME
void TextureContainer::load(int id, const std::string &file_name)
{
	// Create and load resource
	std::unique_ptr<sf::Texture> resource(new sf::Texture());

	if (!resource->loadFromFile(file_name))
		throw std::runtime_error("ResourceHolder::load - Failed to load " + file_name);

	// insert resource to map
	insert_resource(id, std::move(resource));
}


void TextureContainer::insert_resource(int id, std::unique_ptr<sf::Texture> resource)
{
	auto inserted = textures.insert(std::make_pair(id, std::move(resource)));
	assert(inserted.second);
}

sf::Texture& TextureContainer::get(int id)
{
	auto found = textures.find(id);
	assert(found != textures.end());

	return *found->second;
}

const sf::Texture& TextureContainer::get(int id) const
{ 
	auto found = textures.find(id);
	assert(found != textures.end());

	return *found->second;
}

