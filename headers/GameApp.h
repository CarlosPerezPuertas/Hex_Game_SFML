#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <string>
#include "HexGame.h"

const int MAX_FRAME_RATE = 60;

class GameApp
{
	public:
		GameApp();
		virtual ~GameApp();

		void Run();

		void Events();
		void Update();
		void Render();

		bool ShutDown();
		
	private:
		sf::RenderWindow screen;
		sf::Event event;	
		HexGame hex_game;
};

#endif

