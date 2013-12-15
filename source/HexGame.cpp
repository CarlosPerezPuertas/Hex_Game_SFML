#include "HexGame.h"


HexGame::HexGame(sf::RenderWindow &window, sf::Event &events) :
screen(window), 
event(events),
RefreshState(false),
exit(false), game_state(GAME_INIT),
texture_manager()
{
	texture_manager.load(Textures::EmptyHex);
	texture_manager.load(Textures::RedHex);
	texture_manager.load(Textures::BlueHex);

	texture_manager.load(Textures::Board);
	s_board.setTexture(texture_manager.get(Textures::Board));
	s_board.setPosition(sf::Vector2f(40, 100));

	RestartBoard();
}


HexGame::~HexGame()
{

}

void HexGame::Init()
{
	board.Init(BOARD_SIZE);
	game_state = PLAYING;
}

void HexGame::MakeMovement()
{
	// 1 When there is a left click iterate trough the board to know the cell clicked
	// 2 We check if it's empty
	// 3 We put the red hex piece on the board

	if (board.CurrentPlayer() == PLAYER)
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2f mouse_click = static_cast<sf::Vector2f>(sf::Mouse::getPosition(screen));

				for (auto itr = board_sfml.begin(); itr != board_sfml.end(); itr++)
				{
					if (itr->IsClicked(mouse_click).first && itr->GetState() == EMPTY)
					{
						itr->ChangeState(BLUE, texture_manager);
						board.InputPlayer(std::distance(board_sfml.begin(), itr));
						RefreshState = true;
					}
				}
			}
		}
	}
	else if (board.CurrentPlayer() == COMPUTER)
	{
		int pos = board.InputComputer();
		board_sfml[pos].ChangeState(RED, texture_manager);
		RefreshState = true;
	}
}


void HexGame::EventsPlaying()
{
	MakeMovement();
}

void HexGame::UpdatePlaying()
{
	//When there is a movement we check for the winner and change player
	if (RefreshState == true)
	{
		if (board.IsWinner() == false) board.ChangePlayer();
		else game_state = WAITING_FOR_CONTINUE;
	}

	for (auto &itr : board_sfml)
	{
		itr.update();
	}

}

void HexGame::RenderPlaying()
{
	if (RefreshState == true) RefreshState = false;

	//Show the background
	screen.draw(s_board);

	//Show all hexs (board)
	for (auto &itr : board_sfml)
	{
		itr.draw(screen);
	}
}

// Continue or no?
void HexGame::EventsContinue()
{
	//CreateButton(Button::Yes);
	//CreateButton(Button::No);

	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2i mouse_click = sf::Mouse::getPosition(screen);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			// Yes is clicked
			if (mouse_click.x > 370 && mouse_click.x < 370 + 70 && mouse_click.y > 20 && mouse_click.y < 20 + 70)
			{
				RestartBoard();
			}
			//No is clicked
			else if (mouse_click.x > 537 && mouse_click.x < 537 + 70 && mouse_click.y > 20 && mouse_click.y < 20 + 70)
			{
				game_state = EXIT;
			}
		}
	}

}

void HexGame::UpdateContinue()
{

}

void HexGame::RenderContinue()
{
	sf::RectangleShape yes_rectangle(sf::Vector2f(70, 70));
	yes_rectangle.setFillColor(sf::Color(0, 170, 0, 140));
	yes_rectangle.setOutlineThickness(5);
	yes_rectangle.setOutlineColor(sf::Color(0, 130, 0));
	yes_rectangle.move(370, 20);

	sf::RectangleShape no_rectangle(sf::Vector2f(70, 70));
	no_rectangle.setFillColor(sf::Color(220, 16, 16, 140));
	no_rectangle.setOutlineThickness(5);
	no_rectangle.setOutlineColor(sf::Color(163, 12, 12));
	no_rectangle.move(537, 20);

	sf::Font font_futurist;
	font_futurist.loadFromFile("fonts/ace_futurism.ttf");

	sf::Text continue_text;
	continue_text.setFont(font_futurist);
	continue_text.setString("Play again?       Yes         No");
	continue_text.setCharacterSize(40);
	continue_text.setColor(sf::Color(204, 204, 204));
	continue_text.setPosition(sf::Vector2f(100, 20));

	screen.draw(yes_rectangle);
	screen.draw(no_rectangle);
	screen.draw(continue_text);
}

void HexGame::RestartBoard()
{
	board.Restart();
	game_state = PLAYING;
	board_sfml.clear();

	for (int i = 0; i < BOARD_SIZE; i++)
	{
		for (int j = 0; j < BOARD_SIZE; j++)
		{
			board_sfml.push_back(Hex(sf::Vector2f(static_cast<float>(97 + 56 * j + i * 28),
				static_cast<float>(139 + 47 * i)), i * BOARD_SIZE + j, texture_manager));
		}
	}
}

