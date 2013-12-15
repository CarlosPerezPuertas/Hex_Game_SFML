#include "board.h"

#include <string>
#include <boost/lexical_cast.hpp>
#include "GameApp.h"



Board::Board() : m_shut_down(false), m_hex_to_check(0)
{
    m_player_hexes[PLAYER] = 'X' ;
    m_player_hexes[COMPUTER] = 'O';
    m_player_hexes[NOBODY] = '.';
    m_player = PLAYER;
}

//WE NEED TO MAKE COPIES OF THE BOARD SO WE USE THE COPY CONSTRUCTOR
Board::Board(const Board &b) : m_board_size(b.m_board_size), m_num_nodes(b.m_num_nodes), m_shut_down(b.m_shut_down),
                               m_hex_board(b.m_hex_board), m_graph_board(b.m_graph_board), m_player_hexes(b.m_player_hexes),
                               m_player(b.m_player) , m_hex_to_check(b.m_hex_to_check),
                               m_tree(b.m_tree), AI(b.AI),  empty_hex(b.empty_hex) ,adjacent_edges(b.adjacent_edges), posible_edges(b.posible_edges), nodes_to_visit(b.nodes_to_visit)
{

}

Board::~Board()
{

}

//MAIN LOOP
void Board::Run()
{
    while(Running())
    {
        //Input();

        if(IsWinner() == false) ChangePlayer();
		else
		{
			std::cout << std::endl << "PLAYER " << m_player_hexes[m_player] << " WINS!!! " << std::endl << std::endl;
			int i = 0;  std::cin >> i;
		}
    }
}

//INIT THE GRAPH DEPENDING ON THE GIVEN SIZE
void Board::Init(int board_size)
{
    m_board_size = board_size;
	std::vector<PLAYERS> hex_board_init(m_board_size*m_board_size, NOBODY);
    m_hex_board.swap(hex_board_init);

    Graph graph_init(m_board_size);

	std::vector<std::thread> threads;
	int num_threads = 0;
	const int MAX_THREADS_SUPPORTED = std::thread::hardware_concurrency();
   
	for (int i = 0; i < m_board_size; i++)
	{
		for (int j = 0; j < m_board_size; j++)
		{
			num_threads++;
			threads.push_back(std::thread(&Graph::ParallelInit, &graph_init, i, j, board_size));

			if (num_threads == MAX_THREADS_SUPPORTED)
			{
				for (auto itr = threads.begin(); itr != threads.end(); itr++) itr->join();
				threads.clear();
			}
		}	
	}

	for (auto itr = threads.begin(); itr != threads.end(); itr++) itr->join();

	m_graph_board = graph_init;
	m_num_nodes = m_graph_board.GetNumNodes();
	

    for(int i = 0; i < m_num_nodes; i++) empty_hex.push_back(i);
}



void Board::InputPlayer(int position)
{
	m_hex_board[position] = PLAYER;
	m_hex_to_check = position;
	std::vector<int>::iterator itr = find(empty_hex.begin(), empty_hex.end(), position);
	empty_hex.erase(itr);	
}

int Board::InputComputer()
{
	int node = AI.BestMovement(this); // We pass a pointer to the board to make the montecarlo simulation
	m_hex_board[node] = COMPUTER;
	m_hex_to_check = node;
	std::vector<int>::iterator itr = find(empty_hex.begin(), empty_hex.end(), node);
	empty_hex.erase(itr);

	return node;
}

void Board::Restart()
{
	for (auto itr = m_hex_board.begin(); itr != m_hex_board.end(); itr++)
	{
		*itr = NOBODY;
	}

	m_player = PLAYER;
	empty_hex.clear();

	for (int i = 0; i < m_num_nodes; i++) empty_hex.push_back(i);
}

void KnuthShuffle(std::vector<int>::iterator begin, std::vector<int>::iterator end) 
{
	for (unsigned int n = end - begin - 1; n >= 1; --n) 
	{
		unsigned int k = rand() % (n + 1);
		
		if (k != n) 
		{
			std::iter_swap(begin + k, begin + n);
		}
	}
}


void Board::MakeRandomMovement()
{
	int pos = 0;
	m_player = COMPUTER;

	//KnuthShuffle(empty_hex.begin(), empty_hex.end());
	std::random_shuffle(empty_hex.begin(), empty_hex.end());


    //Fill the board at random
	while (!empty_hex.empty())
    {
        ChangePlayer();
		pos = *(empty_hex.end() - 1);
		empty_hex.pop_back();
        m_hex_board[pos] = m_player; //PLAYER OR COMPUTER
    }
}


void Board::MakeComputerMovement(int movement)
{
    m_hex_board[movement] = COMPUTER;
    std::vector<int>::iterator itr = find(empty_hex.begin(), empty_hex.end(), movement );
    empty_hex.erase(itr);
}

//SEARCH THE TREE TO FIND IF THERE IS A WINNER AND RETURNS THE WINNER
PLAYERS Board::IsWinner()
{
	bool touch_up = false;
	bool touch_down = false;
	int movement = 0;

	m_tree.clear();

	m_hex_to_check = m_num_nodes + VIRTUAL_NODE::NORTH_NODE; //NORTH VIRTUAL NODE
	SearchNodeTree(COMPUTER);

	for (auto tree_b = m_tree.begin(), tree_e = m_tree.end(); tree_b != tree_e; ++tree_b)
	{
		movement = *tree_b;

		if (movement  < m_num_nodes) //If it's not a virtual node
		if (m_hex_board[movement] == COMPUTER)
		{
			if (movement/m_board_size == 0) touch_up = true;
			if (movement/m_board_size == m_board_size - 1) touch_down = true;
		}

		if (touch_up == true && touch_down == true)
		{
			m_shut_down = true;
			return COMPUTER;
		}
	}

	bool touch_right = false;
	bool touch_left = false;

	m_tree.clear();

	m_hex_to_check = m_num_nodes + VIRTUAL_NODE::WEST_NODE; //WEST VIRTUAL NODE
	SearchNodeTree(PLAYER);

	//Check if the tree touches the edges of the board
	for (auto tree_b = m_tree.begin(), tree_e = m_tree.end(); tree_b != tree_e; ++tree_b)
	{
		movement = *tree_b;

		if (movement < m_num_nodes) //If it's not a virtual node
		if (m_hex_board[movement] == PLAYER)
		{
			if ((movement + 1) % m_board_size == 0) touch_right = true;
			if ((1 + movement) % m_board_size == 1) touch_left = true;
		}

		if (touch_left == true && touch_right == true)
		{
			m_shut_down = true;
			return PLAYER;
		}
	}
	return NOBODY;
}

PLAYERS Board::IsWinnerComputer()
{
	bool touch_up = false;
	bool touch_down = false;
	int movement = 0;

	m_hex_to_check = m_num_nodes + VIRTUAL_NODE::NORTH_NODE; //NORTH VIRTUAL NODE
	SearchNodeTree(COMPUTER);

	for (auto tree_b = m_tree.begin(), tree_e = m_tree.end(); tree_b != tree_e; ++tree_b)
	{
		movement = *tree_b;

		if (movement < m_num_nodes) //If it's not a virtual node
		{
			if (m_hex_board[movement] == COMPUTER)
			{
				if (movement / m_board_size == 0) touch_up = true;
				if (movement / m_board_size == m_board_size - 1) touch_down = true;
			}
		}

		if (touch_up == true && touch_down == true)
		{
			m_tree.clear();
			return COMPUTER;
		}	
	}
	m_tree.clear();
	return NOBODY; //Or player
}


// A MODIFICATION ON THE PRIM ALGORITHM THAT DOESN'T CONSIDER THE COSTS
void Board::SearchNodeTree(PLAYERS player)
{

    int direction = 0, row = 0, column = 0;
    std::vector<bool> visited_nodes(m_num_nodes + 4);
    nodes_to_visit.push_back(m_hex_to_check);
    m_tree.push_back(m_hex_to_check);

    while(!nodes_to_visit.empty())
    {
		adjacent_edges = m_graph_board.GetEdges(*(nodes_to_visit.end() - 1));
        nodes_to_visit.pop_back();

        //For every node to visit, check all is edges if are not checked before and add it to the posible edges to check.
        for(auto adj_b = adjacent_edges.begin(),  adj_e = adjacent_edges.end(); adj_b != adj_e; ++adj_b )
        {
            direction = adj_b->m_direction;

            //If it's not a virtual node then check normally
            if(direction < m_num_nodes )
            {
                if( visited_nodes[direction] == false)
				if (m_hex_board[direction] == player) 
                {
					posible_edges.push_back(direction);
                }
            }

            //If it's a virtual node then check if it's from the current player
            else
            {
                if(visited_nodes[direction] == false && player == COMPUTER && (direction == m_num_nodes + VIRTUAL_NODE::NORTH_NODE || direction == m_num_nodes + VIRTUAL_NODE::SOUTH_NODE ))
                {
                    posible_edges.push_back(direction);
                }

                else if(visited_nodes[direction] == false  && player == PLAYER  &&
                       (direction == m_num_nodes + VIRTUAL_NODE::WEST_NODE || direction == m_num_nodes + VIRTUAL_NODE::EAST_NODE))
                {
					posible_edges.push_back(direction);
                }
            }
        }

        //Check the posible nodes to visit and add it to the tree if the id it's not visited
        while(!posible_edges.empty())
        {
            int hex = *(posible_edges.end()-1);

            if( visited_nodes[hex] == false)
            {
                visited_nodes[hex] = true;
                nodes_to_visit.push_back(hex);
                m_tree.push_back(hex);
            }
            posible_edges.pop_back();
        }
    }
}
