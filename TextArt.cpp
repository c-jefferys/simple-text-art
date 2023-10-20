/*
* This program allows display and editing of text art (also called ASCII art).
*/

#include <iostream>
#include <fstream>
#include <cctype>
#include <windows.h>
#include <conio.h>
#include <ctype.h>
using namespace std;

const int BUFFERSIZE = 20;
const int FILENAMESIZE = 255;
const int MAXROWS = 22;
const int MAXCOLS = 80;

// ASCII codes for special keys; for editing
const char ESC = 27;
const char LEFTARROW = 75;
const char UPARROW = 72;
const char RIGHTARROW = 77;
const char DOWNARROW = 80;
const char SPECIAL = (char)224;

/*
* Gets a filename from the user. If file can be opened for reading,
* this function loads the file's contents into canvas.
* File is a TXT file located in the SavedFiles folder.
* If file cannot be opened, error message is displayed and
* canvas is left unchanged.
*/
void loadCanvas(char canvas[][MAXCOLS]);

/*
* Gets a filename from the user. If file can be opened for writing,
* this function writes the canvas contents into the file.
* File is a TXT file located in the SavedFiles folder.
* If file cannot be opened, error message is displayed.
*/
void saveCanvas(char canvas[][MAXCOLS]);

/*
* Initializes canvas to contain all spaces.
*/
void initCanvas(char canvas[][MAXCOLS]);

/*
* Displays canvas contents on the screen, with a border
* around the right and bottom edges.
*/
void displayCanvas(char canvas[][MAXCOLS]);

/*
* Allows user to edit the canvas by moving the cursor around and
* entering characters. Editing continues until the ESC key is pressed.
*/
void editCanvas(char canvas[][MAXCOLS]);

/*
* Copies contents of the "from" canvas into the "to" canvas.
*/
void copyCanvas(char to[][MAXCOLS], char from[][MAXCOLS]);

/*
* Replaces all instances of a character in the canvas.
* oldCh is the character to be replaced.
* newCh character is the character to replace with.
*/
void replace(char canvas[][MAXCOLS], char oldCh, char newCh);

/*
* Shifts contents of the canvas by a specified number of rows and columns.
* rowValue is the number of rows by which to shift
*    positive numbers shift downward; negative numbers shift upward
* colValue is the number of rows by which to shift
*    positive numbers shift right; negative numbers shift left
*/
void moveCanvas(char canvas[][MAXCOLS], int rowValue, int colValue);

/*
* Clears a line on the output screen, then resets the cursor back to the
* beginning of this line.
* lineNum is the line number on the output screen to clear
* numOfChars is the number of characters to clear on this line
*/
void clearLine(int lineNum, int numOfChars);

/*
* Moves the cursor in the output window to a specified row and column.
* The next output produced by the program will begin at this position.
*/
void gotoxy(short row, short col);


int main()
{
	/*
	* canvas holds the array of characters for the canvas
	* canvasCopy holds a copy of the canvas used to undo
	* menuChoice hold's the user's choice from the menu
	* old and newCharChoice hold the characters that the user chooses for the replace function
	* rowMoved and colMoved hold the values for the moveCanvas function
	*/
	char canvas[MAXROWS][MAXCOLS];
	char canvasCopy[MAXROWS][MAXCOLS];
	char menuChoice = ' ', oldCharChoice = ' ', newCharChoice = ' ';
	int rowMoved, colMoved;

	//initializes the canvas with spaces
	initCanvas(canvas);

	//tolower function makes the user's character choice always lowercase
	//loop continues until user enters q to quit the program
	while (tolower(menuChoice) != 'q')
	{
		displayCanvas(canvas);

		//clears any lines used by previous function output
		clearLine(MAXROWS + 1, MAXCOLS + 1);
		clearLine(MAXROWS + 2, MAXCOLS + 1);
		clearLine(MAXROWS + 3, MAXCOLS + 1);

		//moves cursor to the line below the bottom canvas border
		gotoxy(MAXROWS + 1, 0);

		cout << "<E>dit / <M>ove / <R>eplace / <U>ndo / <L>oad / <S>ave / <Q>uit: ";
		cin >> menuChoice;

		//decides which functions to execute based on user's menu choice
		switch (tolower(menuChoice))
		{
		case 'e':
			//creates a copy before changes
			copyCanvas(canvasCopy, canvas);

			editCanvas(canvas);
			break;

		case 'm':
			copyCanvas(canvasCopy, canvas);

			//clears the menu line so new text can be displayed
			clearLine(MAXROWS + 1, MAXCOLS);

			//stores column units to move
			cout << "Enter column units to move: ";
			cin >> colMoved;

			clearLine(MAXROWS + 1, MAXCOLS);

			//stores row units to move
			cout << "Enter row units to move: ";
			cin >> rowMoved;

			moveCanvas(canvas, rowMoved, colMoved);
			break;

		case 'r':
			copyCanvas(canvasCopy, canvas);

			clearLine(MAXROWS + 1, MAXCOLS);

			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			//stores the character to be replaced
			cout << "Choose the character to be replaced: ";
			oldCharChoice = cin.get();

			if (oldCharChoice == '\n')
			{
				oldCharChoice = ' ';
			}

			cin.clear();
			cin.ignore((numeric_limits<streamsize>::max)(), '\n');

			clearLine(MAXROWS + 1, MAXCOLS);

			//stores character to be inserted
			cout << "Choose a new character to insert: ";
			newCharChoice = cin.get();

			if (newCharChoice == '\n')
			{
				newCharChoice = ' ';
			}

			replace(canvas, oldCharChoice, newCharChoice);
			break;

		case 'u':
			//uses the copy that was made before the last change and reverts the canvas back to that copy
			copyCanvas(canvas, canvasCopy);
			break;

		case 'l':
			copyCanvas(canvasCopy, canvas);
			loadCanvas(canvas);
			break;

		case 's':
			saveCanvas(canvas);
			break;

		case 'q':
			//exits the switch statement so the while loop and the program can end
			break;

		default:
			clearLine(MAXROWS + 1, MAXCOLS);

			//gives error message before redisplaying the menu for another choice
			cout << "'" << menuChoice << "' is not a valid selection. \n";
			system("pause");
		}
	}
	return 0;
}


void gotoxy(short row, short col)
{
	COORD pos = { col, row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}


void clearLine(int lineNum, int numOfChars)
{
	// Move cursor to the beginning of the specified line on the console
	gotoxy(lineNum, 0);

	// Write a specified number of spaces to overwrite characters
	for (int x = 0; x < numOfChars; x++)
		cout << " ";

	// Move cursor back to the beginning of the line
	gotoxy(lineNum, 0);
}


void replace(char canvas[][MAXCOLS], char oldCh, char newCh)
{
	//for loops loop through canvas
	for (int row = 0; row < MAXROWS; row++)
	{
		for (int col = 0; col < MAXCOLS; col++)
		{
			/*if statement checks the old character and replaces
			it with the new character if valid*/
			if (canvas[row][col] == oldCh)
			{
				canvas[row][col] = newCh;
			}
		}
	}
}


void editCanvas(char canvas[][MAXCOLS])
{
	char moveInput;
	int row = 0, col = 0;

	clearLine(MAXROWS + 1, MAXCOLS);
	cout << "Press <ESC> to stop editing";

	// Move cursor to row,col and then get
	// a single character from the keyboard
	gotoxy(row, col);
	moveInput = _getch();


	while (moveInput != ESC)
	{
		//if statement to move the cursor using arrowkeys 
		if (moveInput == SPECIAL)
		{
			moveInput = _getch();
			switch (moveInput)
			{
			case LEFTARROW:
				if (col > 0)
				{
					col--;
				}
				break;
			case RIGHTARROW:
				if (col < MAXCOLS - 1)
				{
					col++;
				}
				break;
			case UPARROW:
				if (row > 0)
				{
					row--;
				}
				break;
			case DOWNARROW:
				if (row < MAXROWS - 1)
				{
					row++;
				}
				break;
			}
		}
		//if key is a special condition 
		else if (moveInput == '\0')
		{
			moveInput = _getch();
		}
		//puts character where cursor is 
		else if (32 <= moveInput && moveInput <= 126)
		{
			canvas[row][col] = moveInput;
			printf("%c", moveInput);
		}

		gotoxy(row, col);
		moveInput = _getch();
	}
}


void moveCanvas(char canvas[][MAXCOLS], int rowValue, int colValue)
{
	//movedCanvas holds a copy of the canvas that has been moved based on row and col value
	char movedCanvas[MAXROWS][MAXCOLS];

	//initializes movedCanvas with spaces
	initCanvas(movedCanvas);

	//loops through each row of canvas
	for (int row = 0; row < MAXROWS; row++)
	{
		//loops through each column of canvas
		for (int col = 0; col < MAXCOLS; col++)
		{
			//checks if new row position is in the array
			//goes to next iteration if it goes out of canvas
			if (row + rowValue < 0 || row + rowValue >= MAXROWS)
			{
				continue;
			}
			//checks if new column position is in the array
			//goes to next iteration if it goes out of canvas
			else if (col + colValue < 0 || col + colValue >= MAXCOLS)
			{
				continue;
			}

			movedCanvas[row + rowValue][col + colValue] = canvas[row][col];
		}
	}

	initCanvas(canvas);

	copyCanvas(canvas, movedCanvas);
}


void initCanvas(char canvas[][MAXCOLS])
{
	//loops through each row
	for (int row = 0; row < MAXROWS; row++)
	{
		//loops through each column
		for (int col = 0; col < MAXCOLS; col++)
		{
			//initializes each array position with a space
			canvas[row][col] = ' ';
		}
	}
}


void displayCanvas(char canvas[][MAXCOLS])
{
	// Clear the screen
	system("cls");

	//displays the canvas, one character at a time 
	for (int row = 0; row < MAXROWS; row++)
	{
		for (int col = 0; col < MAXCOLS; col++)
		{
			cout << canvas[row][col];
		}

		//displays the '|' character for the right canvas border at the end of each row
		cout << "|" << endl;
	}

	for (int i = 0; i < MAXCOLS; i++)
	{
		//displays the bottom canvas border
		cout << "-";
	}
}


void copyCanvas(char to[][MAXCOLS], char from[][MAXCOLS])
{
	//Loops through canvas and copys to a new array
	for (int row = 0; row < MAXROWS; row++)
	{
		for (int col = 0; col < MAXCOLS; col++)
		{
			to[row][col] = from[row][col];
		}
	}
}


void saveCanvas(char canvas[][MAXCOLS])
{
	/*
	* outFile holds the file that the canvas is saved to
	* input array holds the user's input for the name of the file
	* fileName array holds the properly formatted name of the file
	*/
	ofstream outFile;
	char name[FILENAMESIZE - BUFFERSIZE];
	char fileName[FILENAMESIZE];

	cin.clear();
	cin.ignore((numeric_limits<streamsize>::max)(), '\n');

	//clears the menu line so new text can be displayed
	clearLine(MAXROWS + 1, MAXCOLS);
	cout << "Enter the name of the file. (Don't include .txt): ";

	//stores the user's input in an array, subtract BUFFERSIZE to leave space for SavedFiles and .txt
	cin.getline(name, FILENAMESIZE - BUFFERSIZE);

	//combines SavedFiles, name, and .txt to create a properly formated file name (SavedFiles\\name.txt)
	snprintf(fileName, FILENAMESIZE, "%s%s%s", "SavedFiles\\", name, ".txt");

	outFile.open(fileName);

	//checks if the file was opened
	if (!outFile)
	{
		//displays error message before returning to the menu
		clearLine(MAXROWS + 1, MAXCOLS);
		cerr << "ERROR: File could not be created.\n";
		system("pause");
		return;
	}

	//loops through each row
	for (int row = 0; row < MAXROWS; row++)
	{
		//loops through each column
		//copies characters from the canvas to the output file one at a time
		for (int col = 0; col < MAXCOLS; col++)
		{
			outFile << canvas[row][col];
		}

		//new line to move to the next row on the document
		outFile << endl;
	}

	outFile.close();

	//displays message before returning to menu
	clearLine(MAXROWS + 1, MAXCOLS);
	cout << "File saved!\n";
	system("pause");
}

/*
* Gets a filename from the user. If file can be opened for reading,
* this function loads the file's contents into canvas.
* File is a TXT file located in the SavedFiles folder.
* If file cannot be opened, error message is displayed and
* canvas is left unchanged.
*/
void loadCanvas(char canvas[][MAXCOLS])
{
	//file to load
	ifstream inFile;

	//char arrays to hold the file name
	char fileName[FILENAMESIZE];
	char name[FILENAMESIZE - BUFFERSIZE];

	//holds the current character to be loaded from the file
	char fileChar;

	//tracks the current row and column of the canvas
	int row = 0;
	int col = 0;

	cin.clear();
	cin.ignore((numeric_limits<streamsize>::max)(), '\n');

	//clears the menu output line so new text can be displayed
	clearLine(MAXROWS + 1, MAXCOLS);

	cout << "Enter the filename (don't enter 'txt'): ";

	//stores the user's input in an array, subtract BUFFERSIZE to leave space for SavedFiles and .txt
	cin.getline(name, FILENAMESIZE - BUFFERSIZE);

	//combines SavedFiles, name, and .txt to create a properly formated file name (SavedFiles\\name.txt)
	snprintf(fileName, FILENAMESIZE, "%s%s%s", "SavedFiles\\", name, ".txt");

	inFile.open(fileName);

	//checks to see if file was opened
	if (!inFile)
	{
		//displays error message before returning to menu
		clearLine(MAXROWS + 1, MAXCOLS);
		cerr << "ERROR: input file could not be opened\n";
		system("pause");
		return;
	}

	//clears the canvas so the .txt document can be loaded
	initCanvas(canvas);

	//loops through each row of the canvas, checks for eof
	for (row = 0; row < MAXROWS && !inFile.eof(); row++)
	{
		//stores a character from the document
		//after first iteration, this gets a character to override the \n that is stored
		fileChar = inFile.get();

		//loops through each column of the canvas 
		//puts the character from the file in the canvas if it isn't a \n or end of file
		for (col = 0; col < MAXCOLS && !inFile.eof() && fileChar != '\n'; col++)
		{
			canvas[row][col] = fileChar;
			fileChar = inFile.get();
		}

		//reads the remaining characters on the line without storing them if the canvas row is already full
		if (col == MAXCOLS)
		{
			while (fileChar != '\n' && !inFile.eof())
			{
				fileChar = inFile.get();
			}
		}
	}
}