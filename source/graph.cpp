#include "graph.h"

Graph::Graph() : m_num_nodes(0)
{

}

//CONSTRUCTS THE GRAPH THAT REPRESENTS THE BOARD GIVING A SIZE
Graph::Graph(int board_size) : m_num_nodes(board_size*board_size)
{
    const int VIRTUAL_NODES = 4;
    m_graph.resize(m_num_nodes + VIRTUAL_NODES);
 
}


Graph::~Graph()
{

}

void Graph::ParallelInit( int i, int j, int board_size)
{
	int thisCell = board_size*i + j;
	
	if (j%board_size == 0) AddEdge(i*board_size, m_num_nodes + WEST_NODE); // Connect west with virtual node
	
	if (i == 0) AddEdge(thisCell, m_num_nodes + NORTH_NODE); // Connect north with virtual node
	
	if (j < (board_size - 1)) AddEdge(thisCell, thisCell + 1);
	else AddEdge(thisCell, m_num_nodes + EAST_NODE);    // Connect east with virtual node
	
	if (i < (board_size - 1))
	{
		AddEdge(thisCell, thisCell + board_size);
		if (j > 0) AddEdge(thisCell, thisCell + board_size - 1);	
	}
	else AddEdge(thisCell, m_num_nodes + SOUTH_NODE); // Connect south with virtual node
}


    //_____________________________________________________________________________________________________________________________
    //We add edge if it's not created yet checking in the matrix.
    //The matrix first paramater gives you the vector of edges, the key is the same that the number of the node so if you put key 3
    //it will give you the edges from node 3.
    //
    //Example: m_graph[3] == 3  then to acces to the edge you must iterate like this  graph[3][iterator]
    //______________________________________________________________________________________________________________________________

void Graph::AddEdge(int origin, int direction)
{
    bool edge_exist = false;

    for(auto edge_b = m_graph[origin].begin(), edge_e = m_graph[origin].end(); edge_b != edge_e ; ++edge_b )
    {
        if(edge_b->m_direction == direction)
        {
            edge_exist = true;
        }
    }

    if(edge_exist == false)
    {
        Edge edge1(origin, direction);
        m_graph[origin].push_back(edge1);
        edge1.Reverse();
        m_graph[direction].push_back(edge1);
    }
}
