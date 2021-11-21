#include "PGTile.h"

PGTile::PGTile()
{
}

PGTile::PGTile(int x_, int y_, char letter_, char backgroundTile_)
	: x(x_), y(y_), letter(letter_), backgroundTile(backgroundTile_)
{
}

PGTile::~PGTile()
{
}

int PGTile::getX()
{
	return x;
}

int PGTile::getY()
{
	return y;
}

int PGTile::getChoose(bool onXaxis)
{
	return y * onXaxis + x * !onXaxis; //if on x axis, return 1y + 0x, if not on x axis, return 0y + 1x
}

char PGTile::getLetter()
{
	return letter;
}

char PGTile::getBackTile()
{
	return backgroundTile;
}