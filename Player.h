#pragma once
#include <iostream>
class Player
{
public:
	Player();
	~Player();
	std::string getInv();
	bool getFromInv(std::string wordWanted);
	void addLetterToInv(char letter);
	int getScore();
	void addToScore(int addedScore);
private:
	std::string inventory;
	int score;
};