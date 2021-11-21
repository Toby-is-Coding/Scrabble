#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <time.h>
#include "Player.h"
#include "PGTile.h"
#include "Constants.h"
#include <vector>

void ClearScreen() {
	// Function which cleans the screen without flickering
	COORD cursorPosition;   cursorPosition.X = 0;   cursorPosition.Y = 0;   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

using namespace std;

//Declaring funcs
char randLetter();
int calcScore(PGTile* tilesLandedOn, int amountOfTLO);
bool wordExists(string word);
void playersTurn(Player player, vector<PGTile>* plGenTiles, char Background[yBoard][xBoard]);

int main()
{
	srand(time(NULL)); //Gets time at start of program to configure rand() func

	char Background[xBoard][yBoard]; //Background of board, 15x15

	//Setting distances of special tiles from the walls
	const char TW[3][2] = { {0, 0}, {0, 7}, {7, 0} }; //Triple Word
	const char TL[3][2] = { {1, 5}, {5, 1}, {5, 5} }; //Triple Letter
	const char DW[4][2] = { {1, 1}, {2, 2}, {3, 3}, {4, 4} }; //Double Word
	const char DL[7][2] = { {0, 3}, {2, 6}, {3, 0}, {3, 7}, {6, 2}, {7, 3} }; //Double Letter

	//Creating Background
	for (int i = -1; i < yBoard; i++) {
		for (int j = -1; j < xBoard; j++) {
			Background[i][j] = ' '; //default char

			//Building coordinate system
			if (i == -1) {
				Background[i][j] = j + 1;
			}
			else if (j == -1) {
				Background[i][j] = char(i + 1 + 65); //convert ascii code for capital letters to char
			}
			else {

				//Building special tiles
				for (int k = 0; k < 17; k++) { //17 - num of all special tiles on each quarter

					//TW TILES
					if (k <= 3) {
						if ((i == TW[k][0] && j == TW[k][1]) //Checking for TW tile in each quarter
							|| (i == yBoard - TW[k][0] - 1 && j == TW[k][1])
							|| (i == yBoard - TW[k][0] - 1 && j == xBoard - TW[k][1] - 1)
							|| (i == TW[k][0] && j == xBoard - TW[k][1] - 1))
						{
							cout << MAG;
							Background[i][j] = 'W';
							cout << NRM;
						}
					}

					//TL TILES
					else if (k <= 6) {
						if ((i == TL[k][0] && j == TL[k][1]) //Checking for TW tile in each quarter
							|| (i == yBoard - TL[k][0] - 1 && j == TL[k][1])
							|| (i == yBoard - TL[k][0] - 1 && j == xBoard - TL[k][1] - 1)
							|| (i == TL[k][0] && j == xBoard - TL[k][1] - 1))
						{
							cout << BLU;
							Background[i][j] = 'L';
							cout << NRM;
						}
					}

					//DW TILES
					else if (k <= 10) {
						if ((i == DW[k][0] && j == DW[k][1]) //Checking for TW tile in each quarter
							|| (i == yBoard - DW[k][0] - 1 && j == DW[k][1])
							|| (i == yBoard - DW[k][0] - 1 && j == xBoard - DW[k][1] - 1)
							|| (i == DW[k][0] && j == xBoard - DW[k][1] - 1))
						{
							cout << RED;
							Background[i][j] = 'w';
							cout << NRM;
						}
					}

					//DL TILES
					else {
						if ((i == DL[k][0] && j == DL[k][1]) //Checking for TW tile in each quarter
							|| (i == yBoard - DL[k][0] - 1 && j == DL[k][1])
							|| (i == yBoard - DL[k][0] - 1 && j == xBoard - DL[k][1] - 1)
							|| (i == DL[k][0] && j == xBoard - DL[k][1] - 1))
						{
							cout << CYN;
							Background[i][j] = 'l';
							cout << NRM;
						}
					}
				}
			}
		}
	}

	Player player;
	for (int i = 0; i < 7; i++) { //Player starts with 7 letters
		player.addLetterToInv(randLetter());
	}
	
	vector<PGTile> plGenTiles;
	//GAME LOOP
	while (player.getInv().length() != 0) {

	}
	return 0;
}

char randLetter()
{
	//Percentage based on the amount of times the letter appears
	char letterFrequency[100] = { 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'B', 'B', 'C', 'C', 'D', 'D',
							'D', 'D', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'E', 'F', 'F',
							'G', 'G', 'G', 'H', 'H', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'I', 'J', 'K',
							'L', 'L', 'L', 'L', 'M', 'M', 'N', 'N', 'N', 'N', 'N', 'N', 'O', 'O', 'O', 'O',
							'O', 'O', 'O', 'O', 'P', 'P', 'Q', 'R', 'R', 'R', 'R', 'R', 'R', 'S', 'S', 'S',
							'S', 'T', 'T', 'T', 'T', 'T', 'T', 'U', 'U', 'U', 'U', 'V', 'V', 'W', 'W', 'X',
							'Y', 'Y', 'Z', 'J', 'K' }; //J, K - because pls i dont wanna code in blank tiles
	int letterPos = rand() % 100; //0-99
	return letterFrequency[letterPos];
}

int calcScore(PGTile* tilesLandedOn, int amountOfTLO)
{
	int score = 0;
	int TWs = 0; //num of TWs landed on
	int DWs = 0; //num of DWs landed on
	for (int i = 0; i < amountOfTLO; i++) {
		//calculating score for each letter in all newly generated tiles, accounting for special letter tiles vv
		switch (tilesLandedOn[i].getLetter()) { //WHY SO LONG
		case 'A':
		case 'E':
		case 'I':
		case 'L':
		case 'N':
		case 'O':
		case 'R':
		case 'S':
		case 'T':
		case 'U':
			switch (tilesLandedOn[i].getBackTile()) {
			case 'L':
				score += 3;
				break;
			case 'l':
				score += 2;
				break;
			case 'W':
				TWs++;
			case 'w':
				DWs++;
			default:
				score++;
				break;
			}
			break;
		case 'D':
		case 'G':
			switch (tilesLandedOn[i].getBackTile()) {
			case 'L':
				score += 2*3;
				break;
			case 'l':
				score += 2*2;
				break;
			case 'W':
				TWs++;
			case 'w':
				DWs++;
			default:
				score += 2;
				break;
			}
			break;
		case 'B':
		case 'C':
		case 'M':
		case 'P':
			switch (tilesLandedOn[i].getBackTile()) {
			case 'L':
				score += 3 * 3;
				break;
			case 'l':
				score += 3 * 2;
				break;
			case 'W':
				TWs++;
			case 'w':
				DWs++;
			default:
				score += 3;
				break;
			}
			break;
		case 'F':
		case 'H':
		case 'V':
		case 'W':
		case 'Y':
			switch (tilesLandedOn[i].getBackTile()) {
			case 'L':
				score += 4 * 3;
				break;
			case 'l':
				score += 4 * 2;
				break;
			case 'W':
				TWs++;
			case 'w':
				DWs++;
			default:
				score += 4;
				break;
			}
			break;
		case 'K':
			switch (tilesLandedOn[i].getBackTile()) {
			case 'L':
				score += 5 * 3;
				break;
			case 'l':
				score += 5 * 2;
				break;
			case 'W':
				TWs++;
			case 'w':
				DWs++;
			default:
				score += 5;
				break;
			}
			break;
		case 'J':
		case 'X':
			switch (tilesLandedOn[i].getBackTile()) {
			case 'L':
				score += 8 * 3;
				break;
			case 'l':
				score += 8 * 2;
				break;
			case 'W':
				TWs++;
			case 'w':
				DWs++;
			default:
				score += 8;
				break;
			}
			break;
		case 'Z':
		case 'Q':
			switch (tilesLandedOn[i].getBackTile()) {
			case 'L':
				score += 10 * 3;
				break;
			case 'l':
				score += 10 * 2;
				break;
			case 'W':
				TWs++;
			case 'w':
				DWs++;
			default:
				score += 10;
				break;
			}
			break;
		}
	}
	if (TWs > 0) {
		score *= 3; //score = score * 3; shortcut
	}
	if (DWs > 0) {
		score *= 2; //score = score * 2; shortcut
	}
	return score;
}

bool wordExists(string word)
{
	bool wordFound = false;
	//Checking if word is valid
	ifstream dict("dict.txt"); //opening dict.txt as read-only
	switch (word[0]) { //starting positions for each letter
	case 'A':
		dict.seekg(0);
		break;
	case 'B':
		dict.seekg(96690);
		break;
	case 'C':
		dict.seekg(173805);
		break;
	case 'D':
		dict.seekg(325791);
		break;
	case 'E':
		dict.seekg(420239);
		break;
	case 'F':
		dict.seekg(487627);
		break;
	case 'G':
		dict.seekg(545080);
		break;
	case 'H':
		dict.seekg(591477);
		break;
	case 'I':
		dict.seekg(648632);
		break;
	case 'J':
		dict.seekg(723531);
		break;
	case 'K':
		dict.seekg(733876);
		break;
	case 'L':
		dict.seekg(746576);
		break;
	case 'M':
		dict.seekg(788074);
		break;
	case 'N':
		dict.seekg(877136);
		break;
	case 'O':
		dict.seekg(920307);
		break;
	case 'P':
		dict.seekg(976079);
		break;
	case 'Q':
		dict.seekg(1115054);
		break;
	case 'R':
		dict.seekg(1122289);
		break;
	case 'S':
		dict.seekg(1214621);
		break;
	case 'T':
		dict.seekg(1382880);
		break;
	case 'U':
		dict.seekg(1459016);
		break;
	case 'V':
		dict.seekg(1508737);
		break;
	case 'W':
		dict.seekg(1532510);
		break;
	case 'X':
		dict.seekg(1561881);
		break;
	case 'Y':
		dict.seekg(1563052);
		break;
	case 'Z':
		dict.seekg(1566582);
		break;
	}
	string currentLine;
	while (!wordFound && getline(dict, currentLine) && currentLine[0] == word[0]) { //loop through dict until word was found, or until 100% sure word does not exist (currentLine[0] != word[0])
		if (currentLine == word) {
			wordFound = true;
		}
	}
	return wordFound;
}

void playersTurn(Player player, vector<PGTile>* plGenTiles, char Background[yBoard][xBoard])
{
	cout << "Enter word you'd like to play" << endl;
	cout << RED << "If you have nothing to play, simply press enter to skip." << NRM << endl;

	//Displaying inventory
	string inventory = player.getInv();
	cout << YLW << "\nInventory: ";
	for (int i = 0; i < inventory.length(); i++) {
		cout << inventory[i] << ", ";
	}
	cout <<NRM << endl;

	//Getting word as input
	string word;
	cout << GRN;
	cin >> word;
	cout << NRM;
	if (word == "") {
		player.addLetterToInv(randLetter());
		return;
	}
	while (!wordExists(word)) { //loop until player chooses a valid word (while chosen word does not exist)
		cout << RED << "Word does not exist." << NRM << endl;
		cout << "Enter word you'd like to play" << endl;
		cout << GRN;
		cin >> word;
		cout << NRM;
	}

	//Coordinates to place word at
	bool validCoord = false;
	while (!validCoord) {
		cout << "Where will the word start from? Example: A1" << endl;
		char y_encoded;
		int x;
		cout << GRN;
		cin >> y_encoded;
		cin >> x;
		cout << NRM;
		int y = toupper(y_encoded); //turned into ascii code
		y -= 65; //A=0, B=1, etc.
		x--; //proper machine coordinates

		if (word.length() > yBoard - y && word.length() > xBoard - x) {
			cout << RED << "Word too close to wall" << NRM << endl;
			continue; //word too close to wall, ask for starter coordinates again
		}
		
		//Get direction of word
		char direc;
		cout << "Place word horizontally or vertically? h/v";
		cin >> direc;
		while (direc != 'h' && direc != 'v') {
			cout << RED << "Invalid direction - " << NRM << endl;
			cout << "h - horizontal\nv - vertical" << endl;
			cin >> direc;
		}

		int axisLevel; //x axis: same y || y axis: same x
		int axisItself; //X axis or Y axis
		if (direc == 'h') {
			bool cont = false;
			for (int i = 0; i < (*plGenTiles).size(); i++) { //check if word is too close to another word on left or right
				if ((*plGenTiles)[i].getY() == y && ((*plGenTiles)[i].getX() == x - 1 || (*plGenTiles)[i].getX() == x + word.length() + 1)) {
					cout << RED << "Word too close to another word!" << NRM << endl;
					cont = true;
				}
				//check for words above or below word
				if ((*plGenTiles)[i].getX() == x && ((*plGenTiles)[i].getY() == y + 1 || (*plGenTiles)[i].getY() == y - 1)) {
					for (int startingPos = y, bool foundSPos = false; !foundSPos; startingPos--) { //startingPos of word we're looking for starts at the same tile, gradually makes it's way upwards. Loop until we found it.
						bool foundTile = false;
						for (int j = 0; !foundTile && j < (*plGenTiles).size(); j++) { //loop through all PGTiles or until found what we wanted
							if ((*plGenTiles)[j].getX() == x && (*plGenTiles)[j].getY() == startingPos) { //if specified tile is on same axis as letter we're on && it's at current startingPos value (tile exists on startingPos)
								foundTile = true;
							}
						}
						if (!foundTile) { //if after going through all plGenTiles we've concluded that none are on startingPos, revert startingPos value to previous loop's value (last value where there was a plGenTile on startingPos)
							startingPos++;
							foundSPos = true;
						}
					}
					/*
					* WE HAVE STARTING POSITION OF WORD ABOVE LETTER WE'RE ON,,
					* SO NOW CREATE STRING OF THAT WORD AND CHECK IF IT EXISTS
					* IF NOT THEN "Word messes up other word around it!"
					*/
				}
			}
			if (cont) { //can't continue; the outer loop from the inner loops
				continue;
			}
			if (word.length() > xBoard - x) {
				cout << RED << "Word too close to wall" << NRM << endl;
				continue; //word too close to wall, ask for starter coordinates again
			}
			else {
				axisLevel = y; //on x axis, same y
				axisItself = x;
			}
		}
		else if (direc == 'v') {
			for (int i = 0; i < (*plGenTiles).size(); i++) { //check if word is too close to another word on up or down
				if ((*plGenTiles)[i].getX() == x && ((*plGenTiles)[i].getY() == y - 1 || (*plGenTiles)[i].getY() == y + word.length() + 1)) {
					cout << RED << "Word too close to another word!" << NRM << endl;
					continue;
				}
			}
			if (word.length() > yBoard - y) {
				cout << RED << "Word too close to wall" << NRM << endl;
				continue; //word too close to wall, ask for starter coordinates again
			}
			else {
				axisLevel = x; //on y axis, same x
				axisItself = y;
			}
		}

		//Check for characters in word that are already on board
		bool bumped = false; //if word bumped into unrelated other word
		string neededLetters = word;
		//Stop the loops if word has "bumped" into another
		for (int i = 0; i < (*plGenTiles).size() && !bumped; i++) { //Loop through all player generated tiles
			//Check for the ones that are on the same level as word, and in the same tiles the word is aiming for
			if (axisLevel == (*plGenTiles)[i].getChoose(axisLevel == y) &&
				(*plGenTiles)[i].getChoose(axisLevel != y) >= axisItself &&
				(*plGenTiles)[i].getChoose(axisLevel != y) <= axisItself + word.length())
			{
				for (int j = 0; j < word.length() && !bumped; j++) { //Loop through tiles "word" is trying to be on
					if ((*plGenTiles)[i].getLetter() == word[j]) { //narrow down letters needed to have in inv if one of the letters are already on the board
						neededLetters.erase(j, 1); //erase the 'j'th char
					}
					else {
						cout << RED << "Word has bumped into another" << NRM << endl;
						bumped = true; //if a PGTile is there and is not a part of the word
					}
				}
			}
		}
	}
}

/*
* TODO LIST:
* CHECK IF WORD COMPLETES OTHER WORDS AROUND IT
* CREATE CHAR ARRAY OF TILES WORD LANDS ON AND SEND IT TO CALC SCORE FUNC
* THE ACTUAL DISPLAY FUNCTION
*/