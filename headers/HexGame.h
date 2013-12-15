#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <iterator> 

#include "Board.h"
#include "Hex.h"
#include "TextureContainer.h"
#include "Entity.h"


enum GAME_STATES{ GAME_INIT, PLAYING, WAITING_FOR_CONTINUE, EXIT };

const int BOARD_SIZE = 11;

class HexGame
{
	private:
		sf::RenderWindow &screen;
		sf::Event &event;
		TextureContainer texture_manager;

		std::vector<Hex> board_sfml;
		Board board;
		std::map<std::string, sf::Sprite> sprites;
		sf::Sprite s_board;

		bool RefreshState;
		GAME_STATES game_state;
		bool exit;

	public:
		HexGame(sf::RenderWindow &window, sf::Event &events);
		virtual ~HexGame();

		void Init();

		//Game states
		void EventsPlaying();
		void UpdatePlaying();
		void RenderPlaying();

		void EventsContinue();
		void UpdateContinue();
		void RenderContinue();

		void RestartBoard();
		void MakeMovement();

		inline GAME_STATES GameState(){ return game_state;}


};

