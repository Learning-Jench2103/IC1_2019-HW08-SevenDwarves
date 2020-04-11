#include<iostream>
#include<regex>
#include<string>
#include<cstring>
#include<math.h>
#include<iomanip>
#include<Windows.h>
using namespace std;

inline void inputCoordinate(int&, int&);
void editCheckerBoard(int checkerboard[][7][7], int x, int y, int n);
void printCheckerBoard(int checkerboard[][7][7], int);
int getNumber(string, int, int);
bool findPosition(int column, int n);
int chooseOption_0to9(int, int, string, string);

bool debug = true;
string input;
int x, y;
int y_temp[7];	//7 steps
int checkerboard[7][7][7];	//7 steps, 7 rows, 7 columns
int temp[7];

int main() {
	do {
		bool valid = true;
		system("cls");
		int counter;
		// initialize checkerboard[7][7][7] //
		for (int i = 0; i < 7; i++) {
			for (int row = 0; row < 7; row++) {
				for (int column = 0; column < 7; column++) {
					checkerboard[i][row][column] = 0;
				}
			}
		}

		// Let user input the first coordinate and then edit as well as store the new checkerboard //
		inputCoordinate(x, y);
		if (debug) {
			cout << "x = " << x << " , y = " << y << endl << endl;
		}
		editCheckerBoard(checkerboard, x, y, 0);
		
		// find where the remain dwarfs are //
		y_temp[0] = y;
		if (findPosition(0, 1)) {
			printCheckerBoard(checkerboard, 6);
		}
		else {
			cout << "System can not print the checkerboard!" << endl << endl;
		}
		
		cout << endl;
		Sleep(1000);

	} while (chooseOption_0to9(1, 2, "Do you want to execute again?", "(1:Yes   2:No):") == 1);
	//system("pause");
	return 0;
}

inline void inputCoordinate(int& x, int& y) {
	// set up regular expression //
	regex pattern("^0*[0-6],0*[0-6]$");

	// let user input and then check if it's legal //
	while (1) {
		cout << "Please input where you want to put the first dwarf at (Ex: 3,4) :";
		getline(cin, input);
		if (regex_match(input, pattern)) {
			x = getNumber(input, 0, input.find(",") - 1);
			y = getNumber(input, input.find(",") + 1, input.length() - 1);
			break;
		}
		else {
			cout << "Input Error!" << endl << endl;
			Sleep(1000);
			system("cls");
			continue;
		}
	}
}

void editCheckerBoard(int checkerboard[][7][7], int x, int y, int n) {
	// the position //
	checkerboard[n][y][x] = 1;

	// the column //
	for (int row = 0; row < 7; row++) {
		if (row == y)continue;
		checkerboard[n][row][x] = -1;
	}

	// the row //
	for (int column = 0; column < 7; column++) {
		if (column == x)continue;
		checkerboard[n][y][column] = -1;
	}

	// the diagonals //
	for (int column = 0; column < 7; column++) {
		if (column == x)continue;
		if (y - (x - column) >= 0 && (y - (x - column)) < 7) {
			checkerboard[n][y - (x - column)][column] = -1;
		}
		if (y + (x - column) >= 0 && y + (x - column) < 7) {
			checkerboard[n][y + (x - column)][column] = -1;
		}
	}
}

void printCheckerBoard(int checkerboard[][7][7], int n) {
	// show by text //
	cout << "Position from row 0 to row 6 : ";
	for (int row = 0; row < 7; row++) {
		int column = 0;
		while (1) {
			if (checkerboard[n][row][column] == 1) {
				cout << column;

				break;
			}
			if (column > 6) {
				cout << "Non";
				break;
			}
			else {
				column++;
			}
		}
		if (row < 6) {
			cout << " , ";
		}
	}
	cout << endl << endl;

	// show by graph //
	cout << "  ";
	for (int column = 0; column < 7; column++) {
		cout << setw(4) << column;
	}
	cout << endl;
	cout << "   ";
	for (int i = 0; i < 29; i++) {
		cout << "-";
	}
	cout << endl;
	for (int row = 0; row < 7; row++) {
		cout << " " << row << " | ";
		for (int column = 0; column < 7; column++) {
			if (checkerboard[n][row][column] == 1) {
				cout << "D";
			}
			else {
				cout << " ";
			}
			cout << " | ";
		}
		cout << endl;
		cout << "   ";
		for (int i = 0; i < 29; i++) {
			cout << "-";
		}
		cout << endl;
	}
	cout << endl;
}

int getNumber(string str, int start, int end) {
	int value = 0;
	for (int i = start; i <= end; i++) {
		value += ((__int64)str[i] - '0') * pow(10, end - i);
	}
	return value;
}

bool findPosition(int column, int n) {
	if (column == x) {	//column x has set by user
		column++;
	}
	if (column == 7 || n == 7) {
		return true;
	}

	// start trying rows from previous column //
	y_temp[column] = y_temp[column - 1];
	if (y_temp[column] > 6) {
		y_temp[column] = 0;
	}

	int counter = 0;	//count how many times the program has tried finding a position

	do {
		// initialize checkerboard[7][7][7] by the previous step //
		for (int row = 0; row < 7; row++) {
			for (int col = 0; col < 7; col++) {
				checkerboard[n][row][col] = checkerboard[n - 1][row][col];
			}
		}

		// then try next row //
		y_temp[column]++;
		if (y_temp[column] > 6) {
			y_temp[column] = 0;
		}

		// find position on the column //
		while (1) {
			if (checkerboard[n][y_temp[column]][column] == 0) {
				editCheckerBoard(checkerboard, column, y_temp[column], n);
				break;
			}
			else {
				counter++;
				// do not need to try more than 7 times (rows) //
				if (counter == 7) {
					return false;
				}
				y_temp[column]++;
				if (y_temp[column] > 6) {
					y_temp[column] = 0;
				}
			}
		}
	} while (!findPosition(column + 1, n + 1));	//if the next column could not find a position, then come back to find another position on this column

	return true;

}

int chooseOption_0to9(int min, int max, string question, string option) {
	while (1) {
		cout << question << option;
		getline(cin, input);
		if (input.length() != 1) {
			cout << "One digit only please!" << endl << endl;
			Sleep(1000);
			continue;
		}
		// check if the characters been inputted are not out of range //
		if ((input[0] - '0') < min || (input[0] - '0') > max) {
			cout << min << " to " << max << " please!" << endl << endl;
			Sleep(1000);
			continue;
		}
		break;
	}
	return (input[0] - '0');
}