#include "dependencies.h"

int main()
{
	// Rendering the window
	RenderWindow mainMenu(VideoMode(740, 480), "Main Menu");

	// Declaring textures
	Texture menuBackground_texture, normalButton_texture, timedButton_texture, helpButton_texture, quitButton_texture;

	menuBackground_texture.loadFromFile("assets/textures/Menu_Background.png");
	normalButton_texture.loadFromFile("assets/textures/Normal.png");
	timedButton_texture.loadFromFile("assets/textures/Timed.png");
	helpButton_texture.loadFromFile("assets/textures/Help.png");
	quitButton_texture.loadFromFile("assets/textures/Quit.png");

	// Creating sprites
	Sprite menu_background(menuBackground_texture), normalButton_sprite(normalButton_texture), timedButton_sprite(timedButton_texture), helpButton_sprite(helpButton_texture), quitButton_sprite(quitButton_texture);

	normalButton_sprite.setPosition(309, 200);
	timedButton_sprite.setPosition(323, 250);
	helpButton_sprite.setPosition(333, 300);
	quitButton_sprite.setPosition(336, 350);

	// Load music file
	Music mainMenuMusic;

	if (!mainMenuMusic.openFromFile("assets/music/8-bit-arcade-moodmode.ogg"))
		return -1;

	mainMenuMusic.setLoop(true);
	mainMenuMusic.setVolume(50);
	mainMenuMusic.play();

	// Initiates event checker
	Event gameEvent;

	while (mainMenu.isOpen())
	{
		while (mainMenu.pollEvent(gameEvent)) // Checks for events and start loop
		{
			if (gameEvent.type == Event::Closed) // for close button
				mainMenu.close();

			if (gameEvent.type == Event::MouseButtonReleased && gameEvent.mouseButton.button == Mouse::Left)
			{
				Vector2i mousePos = Mouse::getPosition(mainMenu);
				if (quitButton_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					mainMenu.close();
					return 0;
				}

				if (helpButton_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					mainMenu.close();
					mainMenuMusic.stop();

					RenderWindow window(VideoMode(740, 480), "Help ");

					// Rendering game content (text, background)
					Font gameFont;
					gameFont.loadFromFile("assets/font/Digitalt.ttf");

					Texture background;
					background.loadFromFile("assets/textures/background.png");

					Sprite backgroundSprite(background);

					Texture backButton_texture;
					backButton_texture.loadFromFile("assets/textures/Back.png");
					Sprite backButton_sprite(backButton_texture);
					backButton_sprite.setPosition(335, 380);

					Text titleText("Hello and welcome to Menageire!", gameFont, 36);
					titleText.setFillColor(Color::Yellow);
					titleText.setPosition(105, 10);

					Text helpText("In this section, we will take you through the rules and basic \nintrouction of the game. So, here are the rules:\n", gameFont, 24);
					helpText.setFillColor(Color::White);
					helpText.setPosition(10, 80);

					Text instructions("1. This is a single player game.\n2. You NEED to match at least 3 tiles together to get points.\n3. If you match 3 tiles, you will get 10 points, if you match 4 tiles,\nyou will get 20 points, and so on.\n4. Invalid moves will be reverted.", gameFont, 24);
					instructions.setFillColor(Color::Yellow);
					instructions.setPosition(10, 150);

					Text thankYou("We hope that this helps you in playing this game. Thank You!", gameFont, 24);
					thankYou.setFillColor(Color::White);
					thankYou.setPosition(10, 300);

					Music helpMusic;

					if (!helpMusic.openFromFile("assets/music/8-bit-dream-land-moodmode.ogg"))
						return -1;

					helpMusic.setLoop(true);
					helpMusic.setVolume(50);
					helpMusic.play();

					// Game loop
					while (window.isOpen())
					{
						// Process events
						Event event;
						while (window.pollEvent(event))
						{
							// Close window: exit
							if (event.type == Event::Closed)
								window.close();

							if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
							{
								Vector2i mousePos = Mouse::getPosition(window);
								if (backButton_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
								{
									window.close();
									helpMusic.stop();
									main();
								}
							}
						}

						window.draw(backgroundSprite);
						window.draw(titleText);
						window.draw(helpText);
						window.draw(instructions);
						window.draw(thankYou);
						window.draw(backButton_sprite);
						window.display();
					}
				}

				if (normalButton_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					mainMenu.close();
					mainMenuMusic.stop();

					// Declaring required variables
					int score = 0;
					int level = 1;
					int matchedCount[7] = {};

					const int tileSize = 54;
					int gridXPos[9][8] = {};
					int gridYPos[9][8] = {};
					int gridRow[9][8] = {};
					int gridCol[9][8] = {};
					int gridSpecies[9][8] = {};

					// Populating information for the grid
					populateGrid(tileSize, gridXPos, gridYPos, gridRow, gridCol, gridSpecies);

					// Setting app window dimensions, title and framerate
					RenderWindow game_normal(VideoMode(740, 480), "Menagerie Adventures");
					game_normal.setFramerateLimit(60);

					Texture bg_texture, animal_textures, cursor_texture, level_texture;
					bg_texture.loadFromFile("assets/textures/background.png");
					animal_textures.loadFromFile("assets/textures/animals.png");
					cursor_texture.loadFromFile("assets/textures/cursor.png");
					level_texture.loadFromFile("assets/textures/Level_Base.png");

					// Creating sprite
					Sprite background_sprite(bg_texture), animal_sprite(animal_textures), cursor_sprite(cursor_texture), level_sprite(level_texture);
					level_sprite.setPosition(584, 12);

					Font gameFont;
					gameFont.loadFromFile("assets/font/Digitalt.ttf");

					Text scoreText("Score: " + to_string(score), gameFont, 24);
					scoreText.setFillColor(Color::White);
					scoreText.setPosition(460, 10);

					Text levelText("Level " + to_string(level), gameFont, 22);
					levelText.setFillColor(Color::White);
					levelText.setPosition(608, 12);

					// Variables for detecting mouse click and calculating col and row of click tile
					int column1, column2, row1, row2, clickNumber = 0, scoreLimit = 1000;
					Vector2i position;

					Music normalGameMusic;

					if (!normalGameMusic.openFromFile("assets/music/8-bit-game-moodmode.ogg"))
						return -1;

					normalGameMusic.setLoop(true);
					normalGameMusic.setVolume(50);
					normalGameMusic.play();

					// Game loop
					while (game_normal.isOpen())
					{
						// Event checker (waits for user input)
						while (game_normal.pollEvent(gameEvent))
						{
							if (gameEvent.type == Event::Closed) // Closes the window if the user clicks the close button
								game_normal.close();

							if ((gameEvent.type == Event::MouseButtonPressed)) // Checks if the user clicks the left mouse button
							{
								position = Mouse::getPosition(game_normal);
								clickNumber++;
							}
						}
						if (score >= scoreLimit)
						{
							level++;
							scoreLimit += 1000 + (100 * level);
							populateGrid(tileSize, gridXPos, gridYPos, gridRow, gridCol, gridSpecies);
						}

						switch (clickNumber)
						{
						// Deducing row and col using coords for first click
						case 1:
							column1 = position.x / tileSize;
							row1 = position.y / tileSize;

							break;

						// Deducing row and col using coords for second click
						case 2:
							column2 = position.x / tileSize;
							row2 = position.y / tileSize;

							// check if the tiles are adjacent
							if (absVal(column1 - column2) + absVal(row1 - row2) == 1)
							{
								swapTiles(gridSpecies, row1, row2, column1, column2);

								// if the swap doesn't result in a match, swap the tiles back
								if (!(matchSpecies(gridSpecies, 0, score, false)))
									swapTiles(gridSpecies, row1, row2, column1, column2);
							}

							clickNumber = 0;
							break;
						}

						game_normal.draw(background_sprite);

						for (int row = 0; row < 9; row++)
						{
							for (int col = 0; col < 8; col++)
							{
								int x = gridSpecies[row][col] * 49;
								int y = 0;
								int width = 49;
								int height = 49;

								animal_sprite.setTextureRect(IntRect(x, y, width, height));

								// Setting tiles at their position defined in the struct grid
								animal_sprite.setPosition(gridXPos[row][col], gridYPos[row][col]);
								game_normal.draw(animal_sprite);
							}
						}

						// Checking for matching species
						matchSpecies(gridSpecies, matchedCount, score);

						// Checking if there are any possible moves left
						if ((anyPossibleMoves(gridSpecies, score) == false))
						{
							// Showing game over screen
							game_normal.close();
							normalGameMusic.stop();

							// creating new window
							RenderWindow gameOverWindow(VideoMode(740, 480), "Game Over");
							gameOverWindow.setFramerateLimit(60);

							// creating sprite for back button
							Texture backButton_texture;
							backButton_texture.loadFromFile("assets/textures/Back.png");
							Sprite backButton_sprite(backButton_texture);
							backButton_sprite.setPosition(335, 300);

							// drawing the "game over" text
							Text gameOverText("Game Over", gameFont, 48);
							gameOverText.setFillColor(Color::Blue);
							gameOverText.setPosition(272, 160);

							// drawing the final score
							Text finalScoreText("Final Score: " + to_string(score), gameFont, 36);
							finalScoreText.setFillColor(Color::White);
							finalScoreText.setPosition(266, 220);

							// gameover audio
							Music gameOverMusic;

							if (!(gameOverMusic.openFromFile("assets/music/faint-courage-toby-fox.ogg")))
								return -1;

							gameOverMusic.play();
							gameOverMusic.setVolume(50);

							// Game loop
							while (gameOverWindow.isOpen())
							{
								// Event checker (waits for user input)
								while (gameOverWindow.pollEvent(gameEvent))
								{
									if (gameEvent.type == Event::Closed)
										gameOverWindow.close();

									if (gameEvent.type == Event::MouseButtonReleased)
									{
										Vector2i mousePos = Mouse::getPosition(gameOverWindow);
										if (backButton_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
										{
											gameOverWindow.close();
											gameOverMusic.stop();
											main();
										}
									}
								}

								// Drawing text
								gameOverWindow.draw(background_sprite);
								gameOverWindow.draw(gameOverText);
								gameOverWindow.draw(finalScoreText);
								gameOverWindow.draw(backButton_sprite);
								gameOverWindow.display();
							}
						}

						// Drawing text
						game_normal.draw(level_sprite);
						levelText.setString("Level " + to_string(level));
						game_normal.draw(levelText);

						scoreText.setString("Score: " + to_string(score));
						game_normal.draw(scoreText);

						// Custom cursor position on mouse position
						cursor_sprite.setPosition((Mouse::getPosition(game_normal)).x - 25, (Mouse::getPosition(game_normal)).y - 25);

						// Displaying progress bars
						Texture barBase_textures[7];
						Texture barOverlay_textures[7];
						Texture barSpecies_textures[7];

						// Loading progress bars
						for (int i = 0; i < 7; i++)
						{
							barBase_textures[i].loadFromFile("assets/textures/progress_base.png");
							barOverlay_textures[i].loadFromFile("assets/textures/pb_fill.png");
							barSpecies_textures[i].loadFromFile("assets/textures/animals.png");
						}

						// Creating sprites using loaded textures
						Sprite *speciesSprites = new Sprite[7];
						Sprite *progressBarBaseSprites = new Sprite[7];
						Sprite *progressBarOverlaySprites = new Sprite[7];

						for (int i = 0; i < 7; i++)
						{
							speciesSprites[i] = Sprite(barSpecies_textures[i]);
							progressBarBaseSprites[i] = Sprite(barBase_textures[i]);
							progressBarOverlaySprites[i] = Sprite(barOverlay_textures[i]);
						}

						// Setting progress bar base and bar size
						for (int i = 0; i < 7; i++)
						{
							speciesSprites[i].setTextureRect(IntRect(49 * i, 0, 50, 50));
							progressBarBaseSprites[i].setTextureRect(IntRect(0, 0, 123, 24));
							progressBarOverlaySprites[i].setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[i], ((i * (-1000)) + 8000)), 20));
						}

						// Close the window and Game if the player has collected all the species
						int speciesRepo[7] = {8000, 7000, 6000, 5000, 4000, 3000, 2000};

						for (int i = 0; i < 7; i++)
						{
							// If the player has collected all the species, Game
							if (matchedCount[i] >= speciesRepo[i])
							{
								game_normal.close();
								normalGameMusic.stop();

								// geting the winning animal and showing it on the screen
								Texture winningAnimal_texture;
								winningAnimal_texture.loadFromFile("assets/textures/animals.png");
								Sprite winningAnimal_sprite(winningAnimal_texture);
								winningAnimal_sprite.setTextureRect(IntRect(49 * i, 0, 50, 50));
								winningAnimal_sprite.setPosition(342, 150);

								// drawing the final score
								Text finalScoreText("Final Score: " + to_string(score), gameFont, 24);
								finalScoreText.setFillColor(Color::White);
								finalScoreText.setPosition(295, 240);

								// drawing the "you win" text
								Text youWinText("You Win!", gameFont, 36);
								youWinText.setFillColor(Color::Yellow);
								youWinText.setPosition(305, 200);

								// creating the back button
								Texture backButton_texture;
								backButton_texture.loadFromFile("assets/textures/Back.png");
								Sprite backButton_sprite(backButton_texture);
								backButton_sprite.setPosition(335, 300);

								// creating new window
								RenderWindow winWindow(VideoMode(740, 480), "Final Score");
								winWindow.setFramerateLimit(60);

								Music youWinMusic;
								
								if (!youWinMusic.openFromFile("assets/music/tower-clear-new-super-mario-bros-wii-chiptune.ogg"))
									return -1;
								
								youWinMusic.play();
								youWinMusic.setVolume(100);

								// Game loop
								while (winWindow.isOpen())
								{
									while (winWindow.pollEvent(gameEvent))
									{
										if (gameEvent.type == Event::Closed)
											winWindow.close();

										if (gameEvent.type == Event::MouseButtonReleased && gameEvent.mouseButton.button == Mouse::Left)
										{
											Vector2i mousePos = Mouse::getPosition(winWindow);
											if (backButton_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
											{
												winWindow.close();
												youWinMusic.stop();
												main();
											}
										}
									}

									// Drawing text
									winWindow.draw(background_sprite);
									winWindow.draw(finalScoreText);
									winWindow.draw(youWinText);
									winWindow.draw(backButton_sprite);
									winWindow.draw(winningAnimal_sprite);
									winWindow.display();
								}

								main();
							}
						}

						// Setting progress bars positions
						for (int i = 0; i < 7; i++)
						{
							speciesSprites[i].setPosition(460, 50 + (60 * i));
							progressBarBaseSprites[i].setPosition(535, (62 + (61 * (i - 0))));
							progressBarOverlaySprites[i].setPosition(537, (64 + (61 * (i - 0))));
						}

						// Drawing progress bars
						for (int i = 0; i < 7; i++)
						{
							game_normal.draw(speciesSprites[i]);
							game_normal.draw(progressBarBaseSprites[i]);
							game_normal.draw(progressBarOverlaySprites[i]);
						}
						// Display cursor and window
						game_normal.draw(cursor_sprite);

						// Drawing game window
						game_normal.display();

						// deleting the dynamic arrays
						delete[] speciesSprites;
						delete[] progressBarBaseSprites;
						delete[] progressBarOverlaySprites;

						// Clearing the window
						game_normal.clear();
					}
				}

				if (timedButton_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					mainMenu.close();
					mainMenuMusic.stop();

					// Declaring required variables
					int score = 0;
					int matchedCount[7] = {};

					const int tileSize = 54;
					int gridXPos[9][8] = {};
					int gridYPos[9][8] = {};
					int gridRow[9][8] = {};
					int gridCol[9][8] = {};
					int gridSpecies[9][8] = {};

					// Populating information for the grid
					populateGrid(tileSize, gridXPos, gridYPos, gridRow, gridCol, gridSpecies);

					// Setting app window dimensions, title and framerate
					RenderWindow game_timed(VideoMode(740, 480), "Menagerie Adventures");
					game_timed.setFramerateLimit(60);

					Texture bg_texture, animal_textures, cursor_texture, level_texture;
					bg_texture.loadFromFile("assets/textures/background.png");
					animal_textures.loadFromFile("assets/textures/animals.png");
					cursor_texture.loadFromFile("assets/textures/cursor.png");
					level_texture.loadFromFile("assets/textures/Level_Base.png");

					// Creating sprite
					Sprite background_sprite(bg_texture), animal_sprite(animal_textures), cursor_sprite(cursor_texture), timerBackground_sprite(level_texture);
					timerBackground_sprite.setPosition(584, 12);

					Font gameFont;
					gameFont.loadFromFile("assets/font/Digitalt.ttf");

					Text scoreText("Score: " + to_string(score), gameFont, 24);
					scoreText.setFillColor(Color::White);
					scoreText.setPosition(460, 10);

					Text timerText(("Timer: 0s"), gameFont, 18);
					timerText.setFillColor(Color::White);
					timerText.setPosition(594, 15);

					Clock gameClock;
					Time elapsedTime;

					// Variables for detecting mouse click and calculating col and row of click tile
					int column1, column2, row1, row2, clickNumber = 0;
					Vector2i position;

					Music timedGameMusic;

					if (!(timedGameMusic.openFromFile("assets/music/party-event-moodmode.ogg")))
						return -1;

					timedGameMusic.setVolume(50);
					timedGameMusic.play();

					// Game loop
					while (game_timed.isOpen())
					{
						// Event checker (waits for user input)
						while (game_timed.pollEvent(gameEvent))
						{
							if (gameEvent.type == Event::Closed) // Closes the window if the user clicks the close button
								game_timed.close();

							if ((gameEvent.type == Event::MouseButtonPressed)) // Checks if the user clicks the left mouse button
							{
								position = Mouse::getPosition(game_timed);
								clickNumber++;
							}
						}

						// Checking click number and swapping tiles
						switch (clickNumber)
						{
						// Deducing row and col using coords for first click
						case 1:
							column1 = position.x / tileSize;
							row1 = position.y / tileSize;

							break;

						// Deducing row and col using coords for second click
						case 2:
							column2 = position.x / tileSize;
							row2 = position.y / tileSize;

							// check if the tiles are adjacent
							if (absVal(column1 - column2) + absVal(row1 - row2) == 1)
							{
								swapTiles(gridSpecies, row1, row2, column1, column2);

								// if the swap doesn't result in a match, swap the tiles back
								if (!(matchSpecies(gridSpecies, 0, score, false)))
									swapTiles(gridSpecies, row1, row2, column1, column2);
							}

							clickNumber = 0;
							break;
						}

						game_timed.draw(background_sprite);

						for (int row = 0; row < 9; row++)
						{
							for (int col = 0; col < 8; col++)
							{
								int x = gridSpecies[row][col] * 49;
								int y = 0;
								int width = 49;
								int height = 49;

								animal_sprite.setTextureRect(IntRect(x, y, width, height));

								// Setting tiles at their position defined in the struct grid
								animal_sprite.setPosition(gridXPos[row][col], gridYPos[row][col]);
								game_timed.draw(animal_sprite);
							}
						}

						// Checking for matching species
						matchSpecies(gridSpecies, matchedCount, score);

						// Drawing text
						game_timed.draw(timerBackground_sprite);

						elapsedTime = gameClock.getElapsedTime();
						int seconds = 120 - elapsedTime.asSeconds();
						int minutes = seconds / 60;
						seconds = seconds % 60;

						bool timeLow = false;

						if (seconds == 30 && minutes == 0)
							timeLow = true;

						string secondsString = to_string(seconds);
						if (seconds < 10)
							secondsString = "0" + secondsString;

						string minutesString = to_string(minutes);
						if (minutes < 10)
							minutesString = "0" + minutesString;

						timerText.setString("Time: " + minutesString + ":" + secondsString);
						game_timed.draw(timerText);

						scoreText.setString("Score: " + to_string(score));
						game_timed.draw(scoreText);

						if (timeLow)
						{
							timerText.setFillColor(Color::Red);
							timedGameMusic.setPitch(1.5f);
						
							timeLow = false;
						}

						// Checking if there are any possible moves left
						if ((anyPossibleMoves(gridSpecies, score) == false) || (seconds == 0 && minutes == 0))
						{
							// Showing game over screen
							game_timed.close();
							timedGameMusic.stop();

							// creating new window
							RenderWindow gameOverWindow(VideoMode(740, 480), "Game Over");
							gameOverWindow.setFramerateLimit(60);

							// creating sprite for back button
							Texture backButton_texture;
							backButton_texture.loadFromFile("assets/textures/Back.png");
							Sprite backButton_sprite(backButton_texture);
							backButton_sprite.setPosition(335, 300);

							// drawing the "game over" text
							Text gameOverText("Game Over", gameFont, 48);
							gameOverText.setFillColor(Color::Blue);
							gameOverText.setPosition(272, 160);

							// drawing the final score
							Text finalScoreText("Final Score: " + to_string(score), gameFont, 36);
							finalScoreText.setFillColor(Color::White);
							finalScoreText.setPosition(266, 220);

							// gameover audio
							Music gameOverMusic;

							if (!(gameOverMusic.openFromFile("assets/music/faint-courage-toby-fox.ogg")))
								return -1;

							gameOverMusic.play();
							gameOverMusic.setVolume(50);

							// Game loop
							while (gameOverWindow.isOpen())
							{
								// Event checker (waits for user input)
								while (gameOverWindow.pollEvent(gameEvent))
								{
									if (gameEvent.type == Event::Closed)
										gameOverWindow.close();

									if (gameEvent.type == Event::MouseButtonReleased && gameEvent.mouseButton.button == Mouse::Left)
									{
										Vector2i mousePos = Mouse::getPosition(gameOverWindow);
										if (backButton_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
										{
											gameOverWindow.close();
											gameOverMusic.stop();
											main();
										}
									}
								}

								// Drawing text
								gameOverWindow.draw(background_sprite);
								gameOverWindow.draw(gameOverText);
								gameOverWindow.draw(finalScoreText);
								gameOverWindow.draw(backButton_sprite);
								gameOverWindow.display();
							}
						}

						// Custom cursor position on mouse position
						cursor_sprite.setPosition((Mouse::getPosition(game_timed)).x - 25, (Mouse::getPosition(game_timed)).y - 25);

						// Displaying progress bars
						Texture barBase_textures[7];
						Texture barOverlay_textures[7];
						Texture barSpecies_textures[7];

						// Loading progress bars
						for (int i = 0; i < 7; i++)
						{
							barBase_textures[i].loadFromFile("assets/textures/progress_base.png");
							barOverlay_textures[i].loadFromFile("assets/textures/pb_fill.png");
							barSpecies_textures[i].loadFromFile("assets/textures/animals.png");
						}

						// Creating sprites using loaded textures
						Sprite *speciesSprites = new Sprite[7];
						Sprite *progressBarBaseSprites = new Sprite[7];
						Sprite *progressBarOverlaySprites = new Sprite[7];

						for (int i = 0; i < 7; i++)
						{
							speciesSprites[i] = Sprite(barSpecies_textures[i]);
							progressBarBaseSprites[i] = Sprite(barBase_textures[i]);
							progressBarOverlaySprites[i] = Sprite(barOverlay_textures[i]);
						}

						// Setting progress bar base and bar size
						for (int i = 0; i < 7; i++)
						{
							speciesSprites[i].setTextureRect(IntRect(49 * i, 0, 50, 50));
							progressBarBaseSprites[i].setTextureRect(IntRect(0, 0, 123, 24));
							progressBarOverlaySprites[i].setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[i], ((i * (-1000)) + 8000)), 20));
						}
						
						// Close the window and Game if the player has collected all the species
						int speciesRepo[7] = {8000, 7000, 6000, 5000, 4000, 3000, 2000};

						for (int i = 0; i < 7; i++)
						{
							// If the player has collected all the species, Game
							if (matchedCount[i] >= speciesRepo[i])
							{
								game_timed.close();

								// geting the winning animal and showing it on the screen
								Texture winningAnimal_texture;
								winningAnimal_texture.loadFromFile("assets/textures/animals.png");
								Sprite winningAnimal_sprite(winningAnimal_texture);
								winningAnimal_sprite.setTextureRect(IntRect(49 * i, 0, 50, 50));
								winningAnimal_sprite.setPosition(342, 150);

								// drawing the final score
								Text finalScoreText("Final Score: " + to_string(score), gameFont, 24);
								finalScoreText.setFillColor(Color::White);
								finalScoreText.setPosition(295, 240);

								// drawing the "you win" text
								Text youWinText("You Win!", gameFont, 36);
								youWinText.setFillColor(Color::Yellow);
								youWinText.setPosition(305, 200);

								// creating the back button
								Texture backButton_texture;
								backButton_texture.loadFromFile("assets/textures/Back.png");
								Sprite backButton_sprite(backButton_texture);
								backButton_sprite.setPosition(335, 300);

								// creating new window
								RenderWindow winWindow(VideoMode(740, 480), "Final Score");
								winWindow.setFramerateLimit(60);

								// Game loop
								while (winWindow.isOpen())
								{
									while (winWindow.pollEvent(gameEvent))
									{
										if (gameEvent.type == Event::Closed)
											winWindow.close();

										if (gameEvent.type == Event::MouseButtonReleased && gameEvent.mouseButton.button == Mouse::Left)
										{
											Vector2i mousePos = Mouse::getPosition(winWindow);
											if (backButton_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
											{
												winWindow.close();
												main();
											}
										}
									}

									// Drawing text
									winWindow.draw(background_sprite);
									winWindow.draw(finalScoreText);
									winWindow.draw(youWinText);
									winWindow.draw(backButton_sprite);
									winWindow.draw(winningAnimal_sprite);
									winWindow.display();
								}

								main();
							}
						}

						// Setting progress bars positions
						for (int i = 0; i < 7; i++)
						{
							speciesSprites[i].setPosition(460, 50 + (60 * i));
							progressBarBaseSprites[i].setPosition(535, (62 + (61 * (i - 0))));
							progressBarOverlaySprites[i].setPosition(537, (64 + (61 * (i - 0))));
						}

						// Drawing progress bars
						for (int i = 0; i < 7; i++)
						{
							game_timed.draw(speciesSprites[i]);
							game_timed.draw(progressBarBaseSprites[i]);
							game_timed.draw(progressBarOverlaySprites[i]);
						}

						// Display cursor and window
						game_timed.draw(cursor_sprite);

						// Drawing game window
						game_timed.display();

						// deleting the dynamic arrays
						delete[] speciesSprites;
						delete[] progressBarBaseSprites;
						delete[] progressBarOverlaySprites;

						// Clearing the window
						game_timed.clear();
					}
				}
			}
		}

		// Draw all sprites for main menu
		mainMenu.draw(menu_background);
		mainMenu.draw(normalButton_sprite);
		mainMenu.draw(timedButton_sprite);
		mainMenu.draw(helpButton_sprite);
		mainMenu.draw(quitButton_sprite);

		// Display cursor and window
		mainMenu.display();
	}

	return 0;
}