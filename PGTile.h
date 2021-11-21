#pragma once
#include "Constants.h"
class PGTile //Player Generated Tile
{
public:
	PGTile();
	PGTile(int x_, int y_, char letter_, char backgroundTile_);
	~PGTile();
	int getX();
	int getY();
	int getChoose(bool onXaxis);
	char getLetter();
	char getBackTile();
private:
	int x;
	int y;
	char letter;
	char backgroundTile;
};