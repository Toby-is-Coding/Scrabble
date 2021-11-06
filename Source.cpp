#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <time.h>
#include "Player.h"
//CONSTANTS
#define xBoard 15
#define yBoard 15
//COLOURS
#define NRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YLW  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"

void ClearScreen() {
	// Function which cleans the screen without flickering
	COORD cursorPosition;   cursorPosition.X = 0;   cursorPosition.Y = 0;   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

using namespace std;

//Declaring funcs
char randLetter();
int calcScore(string word, char tilesLandedOn[]);
void playersTurn(Player player, char currentScreen[yBoard][xBoard]);

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
							Background[i][j] == 'W';
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
							Background[i][j] == 'L';
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
							Background[i][j] == 'w';
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
							Background[i][j] == 'l';
							cout << NRM;
						}
					}
				}
			}
		}
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

int calcScore(string word, char tilesLandedOn[])
{
	int score = 0;
	int TWs = 0; //num of TWs landed on
	int DWs = 0; //num of DWs landed on
	for (int i = 0; i < word.length(); i++) {
		//calculating score for each letter in word, accounting for special letter tiles vv
		switch (toupper(word[i])) { //WHY SO LONG
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
			switch (tilesLandedOn[i]) {
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
			switch (tilesLandedOn[i]) {
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
			switch (tilesLandedOn[i]) {
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
			switch (tilesLandedOn[i]) {
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
			switch (tilesLandedOn[i]) {
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
			switch (tilesLandedOn[i]) {
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
			switch (tilesLandedOn[i]) {
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

void playersTurn(Player player, char currentScreen[yBoard][xBoard])
{
	cout << "Enter word you'd like to play" << endl;

	//Displaying inventory
	string inventory = player.getInv();
	cout << YLW << "Inventory: ";
	for (int i = 0; i < inventory.length(); i++) {
		cout << inventory[i] << ", ";
	}
	cout <<NRM << endl;

	//Getting word as input
	string word;
	bool wordFound = false;
	while (!wordFound) { //loop until player chooses a valid word
		cout << GRN;
		cin >> word;
		while (word == "") {
			cout << RED << "Nothing is not an option." << GRN << endl;
			cin >> word;
		}
		cout << NRM;

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
		if (!wordFound) {
			cout << RED << "Word does not exist." << NRM << endl;
		}
	}

	//Coordinates to place word at
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

	char direc;

	/*
	MORE TO DO HERE:
	- check if word length is greater than distance to the wall
	- automatically set the direction of the word (h/v) if possible
	- loop getting coordinates until word is able to be placed
	*/
}