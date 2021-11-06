#include "Player.h"
#include <iostream>

Player::Player()
	: inventory(""), score(0)
{
}

Player::~Player()
{
}

std::string Player::getInv()
{
	return inventory;
}

bool Player::getFromInv(std::string wordWanted)
{
	int lettersFound = 0;
	for (int i = 0; i < inventory.length(); i++) {
		for (int j = 0; j < wordWanted.length(); j++) {
			if (inventory[i] == wordWanted[j]) {
				lettersFound++;
			}
		}
	}
	if (lettersFound >= wordWanted.length()) { //lettersFound will be larger than length if there are duplicate letters
		for (int i = 0; i < wordWanted.length(); i++) { //looping only for length of wordWanted so it won't delete all duplicate letters, just one
			inventory.erase(inventory.find(wordWanted[i]), 1); //erase 1 character at location of letter wanted
		}
		return true;
	}
	else {
		return false;
	}
}

void Player::addLetterToInv(char letter)
{
	inventory += letter;
}

int Player::getScore()
{
	return score;
}

void Player::addToScore(int addedScore)
{
	score += addedScore;
}