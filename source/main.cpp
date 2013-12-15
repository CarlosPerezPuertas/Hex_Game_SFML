#include <ctime>
#include <chrono>
#include <random>
#include "Board.h"
#include "GameApp.h"
#include <string>



int main()
{
	srand((unsigned int)time(0));

	GameApp hex_game;
	hex_game.Run();

	return 0;
}
