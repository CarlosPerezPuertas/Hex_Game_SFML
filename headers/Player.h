#pragma once

//Abstract class Player
class Player
{
	public:
		Player();
		virtual ~Player();
		
		virtual void MakeMove() const = 0;
};


//Human and computer
class Human : public Player
{
	public:
		Human();
		virtual ~Human();

		virtual void MakeMove() const;
};


void Human::MakeMove() const
{

}

class Computer : public Player
{
public:
	Computer();
	virtual ~Computer();

	virtual void MakeMove() const;
};

void Computer::MakeMove() const
{

}


