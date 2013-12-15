#include "GameApp.h"



GameApp::GameApp() :
screen(sf::VideoMode(1100, 800), "HEX GAME"),
hex_game(screen, event)
{
	screen.setFramerateLimit(MAX_FRAME_RATE);
}


GameApp::~GameApp()
{

}

void GameApp::Run()
{
	hex_game.Init();

	while (ShutDown() == false )
	{
		Events();
		Update();
		Render();
	}
}

bool GameApp::ShutDown()
{
	return !screen.isOpen() || hex_game.GameState() == EXIT;
}


void GameApp::Events()
{
	if (screen.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			screen.close();

		switch (hex_game.GameState())
		{
			case PLAYING:
				hex_game.EventsPlaying();
				break;
			case WAITING_FOR_CONTINUE:
				hex_game.EventsContinue();
				break;
			default:
				std::cout << "Error in void GameApp::Events()" << std::endl;
		}
	}
}


void GameApp::Update()
{
	switch (hex_game.GameState())
	{
		case PLAYING:
			hex_game.UpdatePlaying();
			break;
		case WAITING_FOR_CONTINUE:
			hex_game.UpdateContinue();
			break;
		default:
			std::cout << "Error in void GameApp::Events()" << std::endl;
	}	
}


void GameApp::Render()
{
	screen.clear();

	switch (hex_game.GameState())
	{
		case PLAYING:
			hex_game.RenderPlaying();
			break;
		case WAITING_FOR_CONTINUE:
			hex_game.RenderPlaying();
			hex_game.RenderContinue();
			break;
	}

	screen.display();
}


