#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
using namespace sf;
using namespace std;

int score = 0;

int absVal(int value)
{
	// returning absolute value
	if (value < 0)
	{
		return -value;
	}
	else
	{
		return value;
	}
}

int calcBarWidth(int matchedCount, int maxSpecies)
{
	// calculating the width of the progress bar
	float floatMaxSpecies = maxSpecies;
	float matchFloat = matchedCount;
	int percentFind = ((matchFloat / floatMaxSpecies) * 119);

	return percentFind;
}

void swapTiles(int gridSpecies[9][8], int row1, int row2, int col1, int col2)
{
	int temp;

	// swapping the species of the species array in the grid
	temp = gridSpecies[row1][col1];
	gridSpecies[row1][col1] = gridSpecies[row2][col2];
	gridSpecies[row2][col2] = temp;
}

void populateGrid(int tileSize, int gridXPos[9][8], int gridYPos[9][8], int gridRow[9][8], int gridCol[9][8], int gridSpecies[9][8])
{
	// seeding rand function
	srand(time(0));

	// populating properties of every tile in the grid
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			gridXPos[row][col] = col * tileSize; // setting the x position of every tile
			gridYPos[row][col] = row * tileSize; // setting the y position of every tile
			gridRow[row][col] = row;			 // setting the row number of every tile
			gridCol[row][col] = col;			 // setting the column number of every tile
			gridSpecies[row][col] = rand() % 7;	 // setting the animal species of every tile
		}
	}
}

void removeTilesHorizontal(int gridSpecies[9][8], int row, int col, int count)
{
	for (int r = row; r >= 0; r--)
	{
		for (int c = col; c < count + col; c++)
		{
			// After shifting the tiles, the top row is filled with random species
			if (r == 0)
			{
				gridSpecies[r][c] = rand() % 7;
			}
			// Shifting the tiles
			else
			{
				gridSpecies[r][c] = gridSpecies[r - 1][c];
			}
		}
	}
}

void removeTilesVertical(int gridSpecies[9][8], int row, int col, int count)
{
	for (int shift = 0; shift < count; shift++)
	{
		for (int r = row + count - 1; r >= 0; r--)
		{
			// After shifting the tiles, the top row is filled with random species
			if (r == 0)
			{
				gridSpecies[r][col] = rand() % 7;
			}
			// Shifting the tiles
			else
			{
				gridSpecies[r][col] = gridSpecies[r - 1][col];
			}
		}
	}
}

bool matchSpecies(int gridSpecies[9][8], int matchedCount[7], bool del = true)
{
	// Declaring check variable
	bool validSwap = false;

	// Horizontal Checking
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			// Checking for 3 or more tiles of the same species in a row
			int count = 1;
			while (col + count < 8 && gridSpecies[row][col] == gridSpecies[row][col + count])
			{
				count++;
			}
			// If there are 3 or more tiles of the same species in a row, they are removed
			if (count >= 3)
			{
				if (del)
				{
					score += 10 * count;								 // Adding to the score
					matchedCount[gridSpecies[row][col]] += count * 100;	 // Adding to the matched count
					removeTilesHorizontal(gridSpecies, row, col, count); // Removing the tiles
				}
				validSwap = true;
			}
		}
	}

	// Vertical Checking
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			int count = 1;
			while (row + count < 9 && gridSpecies[row][col] == gridSpecies[row + count][col])
			{
				count++;
			}

			if (count >= 3)
			{
				if (del)
				{
					score += 10 * count;
					matchedCount[gridSpecies[row][col]] += count * 100;
					removeTilesVertical(gridSpecies, row, col, count);
				}
				validSwap = true;
			}
		}
	}

	return validSwap;
}

bool anyPossibleMoves(int gridSpecies[9][8])
{
	// Declaring a copy of the grid
	int copyGridSpecies[9][8] = {};

	// Copying the grid
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			copyGridSpecies[row][col] = gridSpecies[row][col];
		}
	}

	// Checking if there are any possible moves
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 8; col++)
		{
			// Checking if the tile is not on the bottom row
			if (col + 1 <= 7)
			{
				// Swapping the tiles
				swapTiles(copyGridSpecies, row, row, col, col + 1);
				if (matchSpecies(copyGridSpecies, 0, false))
				{
					return true;
				}
				else
				{
					swapTiles(copyGridSpecies, row, row, col, col + 1);
				}
			}
			// Checking if the tile is not on the rightmost column
			if (col - 1 >= 0)
			{
				// Swapping the tiles
				swapTiles(copyGridSpecies, row, row, col, col - 1);
				if (matchSpecies(copyGridSpecies, 0, false))
				{
					return true;
				}
				else
				{
					swapTiles(copyGridSpecies, row, row, col, col - 1);
				}
			}
			// Checking if the tile is not on the topmost row
			if (row - 1 >= 0)
			{
				// Swapping the tiles
				swapTiles(copyGridSpecies, row, row - 1, col, col);
				if (matchSpecies(copyGridSpecies, 0, false))
				{
					return true;
				}
				else
				{
					swapTiles(copyGridSpecies, row, row - 1, col, col);
				}
			}
			// Checking if the tile is not on the bottom row
			if (row + 1 <= 8)
			{
				// Swapping the tiles
				swapTiles(copyGridSpecies, row, row + 1, col, col);
				if (matchSpecies(copyGridSpecies, 0, false))
				{
					return true;
				}
				else
				{
					swapTiles(copyGridSpecies, row, row + 1, col, col);
				}
			}
		}
	}

	return false;
}

int main()
{
	// Rendering the window
	RenderWindow app(VideoMode(740, 480), "Main Menu");

	// Declaring textures
	Texture menu_bg, normal_bt, timed_bt, help_bt, quit_bt;

	// Loading images
	menu_bg.loadFromFile("sprites/Menu_Background.png");
	normal_bt.loadFromFile("sprites/Normal.png");
	timed_bt.loadFromFile("sprites/Timed.png");
	help_bt.loadFromFile("sprites/Help.png");
	quit_bt.loadFromFile("sprites/Quit.png");

	// Creating sprites
	Sprite menu_background(menu_bg), normal_but(normal_bt), timed_but(timed_bt), help_but(help_bt), quit_but(quit_bt);

	// Setting sprite positions
	normal_but.setPosition(307, 200);
	timed_but.setPosition(322, 250);
	help_but.setPosition(332, 300);
	quit_but.setPosition(335, 350);

	// Initiates event checker
	Event e;

	// Game loop
	while (app.isOpen())
	{
		while (app.pollEvent(e)) // Checks for events and starts loop
		{
			// Closes the window if the user clicks the close button
			if (e.type == Event::Closed)
			{
				app.close();
			}

			// Checks if the user clicks the left mouse button
			if (e.type == Event::MouseButtonReleased && e.mouseButton.button == Mouse::Left)
			{
				// Gets the mouse position
				Vector2i mousePos = Mouse::getPosition(app);

				// Normal mode
				if (normal_but.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					// Closes the menu window
					app.close();

					// Declaring required variables
					const int tileSize = 54;
					int matchedCount[7] = {};

					// Arrays for all tile information like xPos, yPos, gridRow, gridCol, gridSpecies
					int gridXPos[9][8] = {};
					int gridYPos[9][8] = {};
					int gridRow[9][8] = {};
					int gridCol[9][8] = {};
					int gridSpecies[9][8] = {};

					// Setting app window dimensions, title and framerate
					RenderWindow app(VideoMode(740, 480), "Menagerie Adventures");
					app.setFramerateLimit(60);

					// Loading font
					Font font;
					font.loadFromFile("sprites/Font/Digitalt.ttf");

					// Declaring textures
					Texture bg, spr, cursor, Level;

					// Loading background, animals and cursor texture from files
					bg.loadFromFile("sprites/background.png");
					spr.loadFromFile("sprites/animals.png");
					cursor.loadFromFile("sprites/cursor.png");
					Level.loadFromFile("sprites/Level_Base.png");

					// Creating sprite
					Sprite background(bg), animals(spr), cursorSprite(cursor), level(Level);

					// Creating text for score
					Text ScoreText;
					ScoreText.setFont(font);
					ScoreText.setCharacterSize(24);
					ScoreText.setFillColor(Color::White);
					ScoreText.setPosition(460, 10);

					// Creating sprite for level text
					Text levelText;
					levelText.setFont(font);
					levelText.setCharacterSize(24);
					levelText.setFillColor(Color::White);
					levelText.setPosition(610, 10);
					level.setPosition(584, 12);

					// Populating information for the grid
					populateGrid(tileSize, gridXPos, gridYPos, gridRow, gridCol, gridSpecies);

					// Variables for detecting mouse click and calculating col and row of click tile
					int clickNum = 0, col1, col2, row1, row2;
					Vector2i position;

					// Initiates event checker
					Event event;

					// Game loop
					while (app.isOpen())
					{
						while (app.pollEvent(e))						// Checks for events and starts loop
						{
							if (e.type == Event::Closed)				// Closes the window if the user clicks the close button
								app.close();
							if (e.type == Event::MouseButtonPressed) 	// Checks if the user clicks the left mouse button
							{
								if (e.key.code == Mouse::Left)
								{
									// Getting mouse position
									position = Mouse::getPosition(app);
									// Incrementing click number
									clickNum++;
								}
							}
						}
						// Checking click count and swapping tiles
						if (clickNum == 1)
						{
							// Deducing row and col using coords for first click
							col1 = position.x / tileSize;
							row1 = position.y / tileSize;
						}
						if (clickNum == 2)
						{
							// Deducing row and col using coords for second click
							col2 = position.x / tileSize;
							row2 = position.y / tileSize;

							// check if the tiles are adjacent
							if (absVal(col1 - col2) + absVal(row1 - row2) == 1)
							{
								// swap the tiles
								swapTiles(gridSpecies, row1, row2, col1, col2);
								if (!matchSpecies(gridSpecies, 0, false))
								{
									swapTiles(gridSpecies, row1, row2, col1, col2);
								}
							}

							// reset the click
							clickNum = 0;
						}

						// Drawing background
						app.draw(background);

						// Drawing tiles on the grid with information from the tile struct
						for (int row = 0; row < 9; row++)
						{
							for (int col = 0; col < 8; col++)
							{
								// Int Rect takes x, y, width and height
								animals.setTextureRect(IntRect(gridSpecies[row][col] * 49, 0, 49, 49));

								// Setting tiles at their position defined in the struct grid
								animals.setPosition(gridXPos[row][col], gridYPos[row][col]);

								// Drawing the tile
								app.draw(animals);
							}
						}

						// Checking for matching species
						matchSpecies(gridSpecies, matchedCount);

						// Checking if there are any possible moves left
						if (!anyPossibleMoves(gridSpecies))
						{
							app.close();
							main();
						}

						// Updates the level text depending on the score
						int levelNum = 1;
						if (score >= 500)
						{
							levelNum = 2;
						}

						if (score >= 1000)
						{
							levelNum = 3;
						}

						// Draw the level text
						app.draw(level);
						levelText.setString("Level " + to_string(levelNum));
						app.draw(levelText);

						// Update the score text
						ScoreText.setString("Score: " + to_string(score));
						app.draw(ScoreText);

						// custom cursor position
						cursorSprite.setPosition((Mouse::getPosition(app)).x - 25, (Mouse::getPosition(app)).y - 25);

						// Displaying progress bars
						{
							// Declaring textures
							Texture bar1, bar2, bar3, bar4, bar5, bar6, bar7, progbase1, progbase2, progbase3, progbase4, progbase5, progbase6, progbase7, barSpecies1, barSpecies2, barSpecies3, barSpecies4, barSpecies5, barSpecies6, barSpecies7;

							// Loading progress bars
							barSpecies1.loadFromFile("sprites/animals.png");
							barSpecies2.loadFromFile("sprites/animals.png");
							barSpecies3.loadFromFile("sprites/animals.png");
							barSpecies4.loadFromFile("sprites/animals.png");
							barSpecies5.loadFromFile("sprites/animals.png");
							barSpecies6.loadFromFile("sprites/animals.png");
							barSpecies7.loadFromFile("sprites/animals.png");

							progbase1.loadFromFile("sprites/progress_base.png");
							progbase2.loadFromFile("sprites/progress_base.png");
							progbase3.loadFromFile("sprites/progress_base.png");
							progbase4.loadFromFile("sprites/progress_base.png");
							progbase5.loadFromFile("sprites/progress_base.png");
							progbase6.loadFromFile("sprites/progress_base.png");
							progbase7.loadFromFile("sprites/progress_base.png");

							bar1.loadFromFile("sprites/pb_fill.png");
							bar2.loadFromFile("sprites/pb_fill.png");
							bar3.loadFromFile("sprites/pb_fill.png");
							bar4.loadFromFile("sprites/pb_fill.png");
							bar5.loadFromFile("sprites/pb_fill.png");
							bar6.loadFromFile("sprites/pb_fill.png");
							bar7.loadFromFile("sprites/pb_fill.png");

							// Creating sprites using loaded textures
							Sprite Species1(barSpecies1);
							Sprite Species2(barSpecies2);
							Sprite Species3(barSpecies3);
							Sprite Species4(barSpecies4);
							Sprite Species5(barSpecies5);
							Sprite Species6(barSpecies6);
							Sprite Species7(barSpecies7);

							Sprite progBase1(progbase1);
							Sprite progBase2(progbase2);
							Sprite progBase3(progbase3);
							Sprite progBase4(progbase4);
							Sprite progBase5(progbase5);
							Sprite progBase6(progbase6);
							Sprite progBase7(progbase7);

							Sprite progBar1(bar1);
							Sprite progBar2(bar2);
							Sprite progBar3(bar3);
							Sprite progBar4(bar4);
							Sprite progBar5(bar5);
							Sprite progBar6(bar6);
							Sprite progBar7(bar7);

							// Setting progress base and bar size
							Species1.setTextureRect(IntRect(0, 0, 49, 49));
							Species2.setTextureRect(IntRect(49, 0, 49, 49));
							Species3.setTextureRect(IntRect(98, 0, 49, 49));
							Species4.setTextureRect(IntRect(147, 0, 49, 49));
							Species5.setTextureRect(IntRect(196, 0, 49, 49));
							Species6.setTextureRect(IntRect(245, 0, 49, 49));
							Species7.setTextureRect(IntRect(294, 0, 49, 49));

							progBase1.setTextureRect(IntRect(0, 0, 123, 24));
							progBase2.setTextureRect(IntRect(0, 0, 123, 24));
							progBase3.setTextureRect(IntRect(0, 0, 123, 24));
							progBase4.setTextureRect(IntRect(0, 0, 123, 24));
							progBase5.setTextureRect(IntRect(0, 0, 123, 24));
							progBase6.setTextureRect(IntRect(0, 0, 123, 24));
							progBase7.setTextureRect(IntRect(0, 0, 123, 24));

							progBar1.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[0], 8000), 20));
							progBar2.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[1], 7000), 20));
							progBar3.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[2], 6000), 20));
							progBar4.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[3], 5000), 20));
							progBar5.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[4], 4000), 20));
							progBar6.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[5], 3000), 20));
							progBar7.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[6], 2000), 20));

							// Close the window and restart the game if the player has collected all the species
							int speciesRepo[7] = {8000, 7000, 6000, 5000, 4000, 3000, 2000};
							
							for (int i = 0; i < 7; i++)
							{
								// If the player has collected all the species, restart the game
								if ((matchedCount[i]) >= speciesRepo[i])
								{
									score = 0;
									app.close();
									main();
								}
							}

							// Setting progress bars positions
							Species1.setPosition(460, 50);
							Species2.setPosition(460, 110);
							Species3.setPosition(460, 170);
							Species4.setPosition(460, 230);
							Species5.setPosition(460, 290);
							Species6.setPosition(460, 350);
							Species7.setPosition(460, 410);

							progBase1.setPosition(535, 62);
							progBase2.setPosition(535, 123);
							progBase3.setPosition(535, 183);
							progBase4.setPosition(535, 244);
							progBase5.setPosition(535, 305);
							progBase6.setPosition(535, 366);
							progBase7.setPosition(535, 427);

							progBar1.setPosition(537, 64);
							progBar2.setPosition(537, 125);
							progBar3.setPosition(537, 185);
							progBar4.setPosition(537, 246);
							progBar5.setPosition(537, 307);
							progBar6.setPosition(537, 368);
							progBar7.setPosition(537, 429);

							// Drawing progress bars
							app.draw(Species1);
							app.draw(Species2);
							app.draw(Species3);
							app.draw(Species4);
							app.draw(Species5);
							app.draw(Species6);
							app.draw(Species7);

							app.draw(progBase1);
							app.draw(progBase2);
							app.draw(progBase3);
							app.draw(progBase4);
							app.draw(progBase5);
							app.draw(progBase6);
							app.draw(progBase7);

							app.draw(progBar1);
							app.draw(progBar2);
							app.draw(progBar3);
							app.draw(progBar4);
							app.draw(progBar5);
							app.draw(progBar6);
							app.draw(progBar7);
						}

						// Display cursor and window
						app.draw(cursorSprite);

						// Drawing game window
						app.display();
					}
				}

				// Time trial mode
				if (timed_but.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					// Closes the main menu window
					app.close();

					// Declaring required variables
					const int tileSize = 54;
					int matchedCount[7] = {};

					// Arrays for all tile information like xPos, yPos, gridRow, gridCol, gridSpecies
					int gridXPos[9][8] = {};
					int gridYPos[9][8] = {};
					int gridRow[9][8] = {};
					int gridCol[9][8] = {};
					int gridSpecies[9][8] = {};

					// Setting app window dimensions, title and framerate
					RenderWindow app(VideoMode(740, 480), "Menagerie Adventures");
					app.setFramerateLimit(60);

					// Declaring font
					Font font;
					font.loadFromFile("sprites/Font/Digitalt.ttf");

					// Declaring textures
					Texture bg, spr, cursor;

					// Loading background, animals and cursor texture from files
					bg.loadFromFile("sprites/background.png");
					spr.loadFromFile("sprites/animals.png");
					cursor.loadFromFile("sprites/cursor.png");

					// Creating sprite using loaded textures
					Sprite background(bg), animals(spr), cursorSprite(cursor);

					// Creating text for score
					Text ScoreText;
					ScoreText.setFont(font);
					ScoreText.setCharacterSize(24);
					ScoreText.setFillColor(Color::White);
					ScoreText.setPosition(460, 10);

					// Creating text for timer
					Text timerText;
					timerText.setFont(font);
					timerText.setCharacterSize(24);
					timerText.setFillColor(Color::White);
					timerText.setPosition(600, 10);

					// Declaring clock and elapsed time
					Clock clock;
					Time elapsed;

					// Populating information for the grid tiles
					populateGrid(tileSize, gridXPos, gridYPos, gridRow, gridCol, gridSpecies);

					// Variables for detecting mouse click and calculating col and row of click tile
					int clickNum = 0, col1, col2, row1, row2;
					Vector2i position;

					// Declaring event
					Event event;

					// Game loop
					while (app.isOpen())
					{
						while (app.pollEvent(e)) // Checking for events
						{
							if (e.type == Event::Closed) // If window is closed, exits game
								app.close();
							if (e.type == Event::MouseButtonPressed) // If mouse is clicked
							{
								if (e.key.code == Mouse::Left) // If left mouse button is clicked
								{
									// Getting mouse position
									position = Mouse::getPosition(app);
									// Incrementing click number
									clickNum++;
								}
							}
						}
						// Checking click count and swapping tiles
						if (clickNum == 1)
						{
							// Deducing row and col using coords for first click
							col1 = position.x / tileSize;
							row1 = position.y / tileSize;
						}
						if (clickNum == 2)
						{
							// Deducing row and col using coords for second click
							col2 = position.x / tileSize;
							row2 = position.y / tileSize;

							// Check if the tiles are adjacent
							if (absVal(col1 - col2) + absVal(row1 - row2) == 1)
							{
								swapTiles(gridSpecies, row1, row2, col1, col2);
								if (!matchSpecies(gridSpecies, 0, false))
								{
									swapTiles(gridSpecies, row1, row2, col1, col2);
								}
							}

							// Reset the click
							clickNum = 0;
						}

						// Drawing background
						app.draw(background);

						// Drawing tiles on the grid with information from the tile struct
						for (int row = 0; row < 9; row++)
						{
							for (int col = 0; col < 8; col++)
							{
								// Int Rect takes x, y, width and height
								animals.setTextureRect(IntRect(gridSpecies[row][col] * 49, 0, 49, 49));
								// Setting tiles at their position defined in the struct grid
								animals.setPosition(gridXPos[row][col], gridYPos[row][col]);

								// Drawing the tile
								app.draw(animals);
							}
						}

						// Checks for matches and removes them
						matchSpecies(gridSpecies, matchedCount);

						// Checking if there are any possible moves left
						if (!anyPossibleMoves(gridSpecies))
						{
							// If there are no possible moves, the game ends
							app.close();
							main();
						}

						// Update the score text
						ScoreText.setString("Score: " + to_string(score));
						app.draw(ScoreText);

						// Update the timer text
						elapsed = clock.getElapsedTime();
						int seconds = 120 - elapsed.asSeconds();

						if (seconds == 0)
						{
							app.close();
							main();
						}

						// Draws the timer
						timerText.setString("Timer: " + to_string(seconds) + " s");
						app.draw(timerText);

						// Custom cursor position
						cursorSprite.setPosition((Mouse::getPosition(app)).x - 25, (Mouse::getPosition(app)).y - 25);

						// Displaying progress bars
						{
							// Declaring textures
							Texture bar1, bar2, bar3, bar4, bar5, bar6, bar7, progbase1, progbase2, progbase3, progbase4, progbase5, progbase6, progbase7, barSpecies1, barSpecies2, barSpecies3, barSpecies4, barSpecies5, barSpecies6, barSpecies7;

							// Loading progress bars
							barSpecies1.loadFromFile("sprites/animals.png");
							barSpecies2.loadFromFile("sprites/animals.png");
							barSpecies3.loadFromFile("sprites/animals.png");
							barSpecies4.loadFromFile("sprites/animals.png");
							barSpecies5.loadFromFile("sprites/animals.png");
							barSpecies6.loadFromFile("sprites/animals.png");
							barSpecies7.loadFromFile("sprites/animals.png");

							progbase1.loadFromFile("sprites/progress_base.png");
							progbase2.loadFromFile("sprites/progress_base.png");
							progbase3.loadFromFile("sprites/progress_base.png");
							progbase4.loadFromFile("sprites/progress_base.png");
							progbase5.loadFromFile("sprites/progress_base.png");
							progbase6.loadFromFile("sprites/progress_base.png");
							progbase7.loadFromFile("sprites/progress_base.png");

							bar1.loadFromFile("sprites/pb_fill.png");
							bar2.loadFromFile("sprites/pb_fill.png");
							bar3.loadFromFile("sprites/pb_fill.png");
							bar4.loadFromFile("sprites/pb_fill.png");
							bar5.loadFromFile("sprites/pb_fill.png");
							bar6.loadFromFile("sprites/pb_fill.png");
							bar7.loadFromFile("sprites/pb_fill.png");

							// Creating sprites using loaded textures
							Sprite Species1(barSpecies1);
							Sprite Species2(barSpecies2);
							Sprite Species3(barSpecies3);
							Sprite Species4(barSpecies4);
							Sprite Species5(barSpecies5);
							Sprite Species6(barSpecies6);
							Sprite Species7(barSpecies7);

							Sprite progBase1(progbase1);
							Sprite progBase2(progbase2);
							Sprite progBase3(progbase3);
							Sprite progBase4(progbase4);
							Sprite progBase5(progbase5);
							Sprite progBase6(progbase6);
							Sprite progBase7(progbase7);

							Sprite progBar1(bar1);
							Sprite progBar2(bar2);
							Sprite progBar3(bar3);
							Sprite progBar4(bar4);
							Sprite progBar5(bar5);
							Sprite progBar6(bar6);
							Sprite progBar7(bar7);

							// Setting progress base and bar size
							Species1.setTextureRect(IntRect(0, 0, 49, 49));
							Species2.setTextureRect(IntRect(49, 0, 49, 49));
							Species3.setTextureRect(IntRect(98, 0, 49, 49));
							Species4.setTextureRect(IntRect(147, 0, 49, 49));
							Species5.setTextureRect(IntRect(196, 0, 49, 49));
							Species6.setTextureRect(IntRect(245, 0, 49, 49));
							Species7.setTextureRect(IntRect(294, 0, 49, 49));

							progBase1.setTextureRect(IntRect(0, 0, 123, 24));
							progBase2.setTextureRect(IntRect(0, 0, 123, 24));
							progBase3.setTextureRect(IntRect(0, 0, 123, 24));
							progBase4.setTextureRect(IntRect(0, 0, 123, 24));
							progBase5.setTextureRect(IntRect(0, 0, 123, 24));
							progBase6.setTextureRect(IntRect(0, 0, 123, 24));
							progBase7.setTextureRect(IntRect(0, 0, 123, 24));

							progBar1.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[0], 8000), 20));
							progBar2.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[1], 7000), 20));
							progBar3.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[2], 6000), 20));
							progBar4.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[3], 5000), 20));
							progBar5.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[4], 4000), 20));
							progBar6.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[5], 3000), 20));
							progBar7.setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[6], 2000), 20));

							// Closes the window and restart the game if the player has collected all the species
							int speciesRepo[7] = {8000, 7000, 6000, 5000, 4000, 3000, 2000};
							for (int i = 0; i < 7; i++)
							{
								// If the player has collected the required amount of species, the game restarts
								if ((matchedCount[i]) >= speciesRepo[i])
								{
									score = 0;
									app.close();
									main();
								}
							}

							// Setting progress bars positions
							Species1.setPosition(460, 50);
							Species2.setPosition(460, 110);
							Species3.setPosition(460, 170);
							Species4.setPosition(460, 230);
							Species5.setPosition(460, 290);
							Species6.setPosition(460, 350);
							Species7.setPosition(460, 410);

							progBase1.setPosition(535, 62);
							progBase2.setPosition(535, 123);
							progBase3.setPosition(535, 183);
							progBase4.setPosition(535, 244);
							progBase5.setPosition(535, 305);
							progBase6.setPosition(535, 366);
							progBase7.setPosition(535, 427);

							progBar1.setPosition(537, 64);
							progBar2.setPosition(537, 125);
							progBar3.setPosition(537, 185);
							progBar4.setPosition(537, 246);
							progBar5.setPosition(537, 307);
							progBar6.setPosition(537, 368);
							progBar7.setPosition(537, 429);

							// Drawing progress bars
							app.draw(Species1);
							app.draw(Species2);
							app.draw(Species3);
							app.draw(Species4);
							app.draw(Species5);
							app.draw(Species6);
							app.draw(Species7);

							app.draw(progBase1);
							app.draw(progBase2);
							app.draw(progBase3);
							app.draw(progBase4);
							app.draw(progBase5);
							app.draw(progBase6);
							app.draw(progBase7);

							app.draw(progBar1);
							app.draw(progBar2);
							app.draw(progBar3);
							app.draw(progBar4);
							app.draw(progBar5);
							app.draw(progBar6);
							app.draw(progBar7);
						}

						// Display cursor and window
						app.draw(cursorSprite);

						// Drawing app
						app.display();
					}
				}

				// Help page
				if (help_but.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					// Close the main menu window
					app.close();

					// Creates the help window
					RenderWindow window(VideoMode(740, 480), "Help ");

					// Creates a font
					Font font;
					font.loadFromFile("sprites/Font/Digitalt.ttf");

					// Creates a background
					Texture background;
					background.loadFromFile("sprites/background.png");

					// Creates a sprite
					Sprite bg(background);

					// Help text properties
					Text HelpText;
					HelpText.setFont(font);
					HelpText.setCharacterSize(24);
					HelpText.setFillColor(Color::White);
					HelpText.setPosition(10, 10);

					// Start the game loop
					while (window.isOpen())
					{
						// Process events
						Event event;
						while (window.pollEvent(event))
						{
							// Closes window
							if (event.type == Event::Closed)
								window.close();
						}

						// Help text
						HelpText.setString("Hello and welcome to Menageire!\nIn this section, we will take you through the rules and basic \nintrouction of the game. So, lets start with the rows:\n1. It is a single player game.\n2. You NEED to match at least 3 tiles together to get points.\n3. If you match 3 tiles, you will get 10 points, if you match 5 tiles,\nyou will get 50 points.\n4. Invalid moves will NOT be allowed.\n\n\nWe hope that this helps you in playing this game.\n\n\nThank You!");

						// Draws the background
						window.draw(bg);

						// Draws the text
						window.draw(HelpText);

						// Updates the window
						window.display();
					}
				}

				// Quit game
				if (quit_but.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					// Closes the window
					app.close();
				}
			}
		}

		// Draw all sprites for main menu
		app.draw(menu_background);
		app.draw(normal_but);
		app.draw(timed_but);
		app.draw(help_but);
		app.draw(quit_but);

		// Display cursor and window
		app.display();
	}
	return 0;
}