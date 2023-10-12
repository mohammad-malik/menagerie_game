#include "dependencies.h"

int main()
{
	// Rendering the window
	RenderWindow mainMenu(VideoMode(740, 490), "Main Menu");

	// Declaring textures
	Texture menuBackground_texture, normalButton_texture, timedButton_texture, helpButton_texture, quitButton_texture;
	initializeTextures(menuBackground_texture, normalButton_texture, timedButton_texture, helpButton_texture, quitButton_texture);

	// Creating sprites
	Sprite menu_background(menuBackground_texture), normalButton_sprite(normalButton_texture), timedButton_sprite(timedButton_texture), helpButton_sprite(helpButton_texture), quitButton_sprite(quitButton_texture);
	initializeSprites(normalButton_sprite, timedButton_sprite, helpButton_sprite, quitButton_sprite);

	// Load music file
	Music mainMenuMusic;

	if (!mainMenuMusic.openFromFile("assets/audio/8-bit-arcade-moodmode.ogg"))
		return -1;

	mainMenuMusic.setLoop(true);
	mainMenuMusic.setVolume(50);
	mainMenuMusic.play();

	SoundBuffer click1_buffer, click2_buffer, click_incorrect_buffer, click_match_buffer, click_nav_buffer;
	initializeSoundBuffers(click1_buffer, click2_buffer, click_incorrect_buffer, click_match_buffer, click_nav_buffer);
	Sound clickSound(click_nav_buffer);

	// Initiates event checker
	Event gameEvent;

	while (mainMenu.isOpen())
	{
		while (mainMenu.pollEvent(gameEvent)) // Checks for events and start loop
		{
			// closes window if close button is pressed
			if (gameEvent.type == Event::Closed)
			{
				mainMenu.close();
				return 0;
			}

			// checking if mouse is clicked anywhere else
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
					clickSound.setBuffer(click_nav_buffer);
					clickSound.play();

					mainMenu.close();
					mainMenuMusic.stop();

					RenderWindow window(VideoMode(740, 490), "Help ");

					// Rendering game content (text, background)
					Font gameFont;
					gameFont.loadFromFile("assets/font/Digitalt.ttf");

					Texture background;
					background.loadFromFile("assets/textures/background.png");

					Sprite backgroundSprite(background);

					Texture backButton_texture;
					backButton_texture.loadFromFile("assets/textures/Back.png");
					Sprite backButton_sprite(backButton_texture);
					backButton_sprite.setPosition(335, 410);

					Text titleText("Welcome to Menageire!", gameFont, 48);
					titleText.setFillColor(Color::Yellow);
					titleText.setPosition(140, 10);

					Text helpText("In this section, we will take you through the rules and basic \nintrouction of the game. So, here are the rules:\n", gameFont, 24);
					helpText.setFillColor(Color::White);
					helpText.setPosition(10, 80);

					Text instructions("1. This is a single player game.\n2. You NEED to match at least 3 tiles together to get points.\n3. If you match 3 tiles, you will get 10 points, \nif you match 4 tiles, you will get 20 points, and so on.\n4. Invalid moves will be reverted.", gameFont, 24);
					instructions.setFillColor(Color::Yellow);
					instructions.setPosition(15, 150);

					Text thankYou("We hope that this helps you in playing this game. Thank You!", gameFont, 24);
					thankYou.setFillColor(Color::White);
					thankYou.setPosition(10, 320);

					Music helpMusic;

					if (!helpMusic.openFromFile("assets/audio/8-bit-dream-land-moodmode.ogg"))
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
									clickSound.setBuffer(click_nav_buffer);
									clickSound.play();

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
					clickSound.setBuffer(click_nav_buffer);
					clickSound.play();

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
					RenderWindow game_normal(VideoMode(740, 490), "Menagerie Adventures");
					game_normal.setFramerateLimit(60);

					Texture bg_texture, animal_textures, cursor_texture, level_texture;
					bg_texture.loadFromFile("assets/textures/background.png");
					animal_textures.loadFromFile("assets/textures/animals.png");
					cursor_texture.loadFromFile("assets/textures/selection_box.png");
					level_texture.loadFromFile("assets/textures/level_base.png");

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
					int column1, column2, row1, row2, clickNumber = 0, soundPlayed = 0, scoreLimit = 150;
					float pitchIncrementer = 1.0f;
					Vector2i position;

					Music normalGameMusic;

					if (!normalGameMusic.openFromFile("assets/audio/8-bit-game-moodmode.ogg"))
						return -1;

					normalGameMusic.setLoop(true);
					normalGameMusic.play();

					// Game loop
					while (game_normal.isOpen())
					{
						// Event checker (waits for user input)
						while (game_normal.pollEvent(gameEvent))
						{
							if (gameEvent.type == Event::Closed) // Closes the window if the user clicks the close button
								game_normal.close();

							if (gameEvent.type == Event::MouseButtonPressed) // Checks if the user clicks the left mouse button
							{
								position = Mouse::getPosition(game_normal);
								clickNumber++;
							}
						}
						
						// Generating new level
						if (score >= scoreLimit)
						{	
							normalGameMusic.stop();

							SoundBuffer levelUp_buffer;
							levelUp_buffer.loadFromFile("assets/audio/levelUp.ogg");
							Sound levelUpSound(levelUp_buffer);
							levelUpSound.play();

							level++;

							// flashing the level text
							for (int i = 0; i < 5; i++)
							{
								game_normal.draw(background_sprite);
								game_normal.draw(level_sprite);

								levelText.setFillColor(Color::Yellow);
								game_normal.draw(levelText);

								game_normal.display();
								sleep(milliseconds(100));

								levelText.setFillColor(Color::White);
								game_normal.draw(levelText);
								game_normal.display();
								sleep(milliseconds(100));

							}

							pitchIncrementer += 0.2f;
							normalGameMusic.setPitch(pitchIncrementer);
							normalGameMusic.play();

							scoreLimit += 100 + (100 * level);
							populateGrid(tileSize, gridXPos, gridYPos, gridRow, gridCol, gridSpecies);
						}

						switch (clickNumber)
						{
						// Deducing row and col using coords for first click
						case 1:

							column1 = position.x / tileSize;
							row1 = position.y / tileSize;

							if (soundPlayed == 0)
							{
								clickSound.setBuffer(click1_buffer);
								clickSound.play();
								soundPlayed++;
							}
							break;

						// Deducing row and col using coords for second click
						case 2:
							column2 = position.x / tileSize;
							row2 = position.y / tileSize;

							// Addresses random clicks
							clickSound.setBuffer(click_incorrect_buffer);
							clickSound.play();
							soundPlayed++;

							// check if the tiles are adjacent
							if (absVal(column1 - column2) + absVal(row1 - row2) == 1)
							{
								swapTiles(gridSpecies, row1, row2, column1, column2);

								// if the swap doesn't result in a match, swap the tiles back
								if (!(matchSpecies(gridSpecies, 0, score, false)))
								{
									clickSound.setBuffer(click_incorrect_buffer);
									clickSound.play();

									swapTiles(gridSpecies, row1, row2, column1, column2);
								}
								else
								{
									clickSound.setBuffer(click_match_buffer);
									clickSound.play();
								}
							}

							soundPlayed = 0;
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
								animal_sprite.setPosition((gridXPos[row][col] + 5), (gridYPos[row][col] + 5));
								game_normal.draw(animal_sprite);
							}
						}

						// Checking for matching species
						matchSpecies(gridSpecies, matchedCount, score);

						// Checking if there are any possible moves left
						if (anyPossibleMoves(gridSpecies, score) == false)
						{
							// Showing game over screen
							game_normal.close();
							normalGameMusic.stop();

							// creating new window
							RenderWindow gameOverWindow(VideoMode(740, 490), "Game Over");
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
							finalScoreText.setPosition(264, 220);

							// gameover audio
							Music gameOverMusic;

							if (!(gameOverMusic.openFromFile("assets/audio/faint-courage-toby-fox.ogg")))
								return -1;

							gameOverMusic.play();
							gameOverMusic.setVolume(50);
							gameOverMusic.setLoop(true);

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
											clickSound.setBuffer(click_nav_buffer);
											clickSound.play();

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
						for (int i = 0; i < 7; i++)
							speciesSprites[i] = Sprite(barSpecies_textures[i]);

						Sprite *progressBarBaseSprites = new Sprite[7];
						for (int i = 0; i < 7; i++)
							progressBarBaseSprites[i] = Sprite(barBase_textures[i]);

						Sprite *progressBarOverlaySprites = new Sprite[7];
						for (int i = 0; i < 7; i++)
							progressBarOverlaySprites[i] = Sprite(barOverlay_textures[i]);

						// Setting progress bar base and bar size
						for (int i = 0; i < 7; i++)
							speciesSprites[i].setTextureRect(IntRect(49 * i, 0, 50, 50));

						for (int i = 0; i < 7; i++)
							progressBarBaseSprites[i].setTextureRect(IntRect(0, 0, 123, 24));

						for (int i = 0; i < 7; i++)
							progressBarOverlaySprites[i].setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[i], ((i * (-1000)) + 8000)), 20));

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
								RenderWindow winWindow(VideoMode(740, 490), "Final Score");
								winWindow.setFramerateLimit(60);

								Music youWinMusic;

								if (!(youWinMusic.openFromFile("assets/audio/tower-clear-new-super-mario-bros-wii-chiptune.ogg")))
									return -1;

								youWinMusic.setVolume(100);
								youWinMusic.play();

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
												Vector2i mousePos = Mouse::getPosition(winWindow);
												if (backButton_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
												{
													clickSound.setBuffer(click_nav_buffer);
													clickSound.play();

													winWindow.close();
													youWinMusic.stop();
													main();
												}
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
							}
						}

						// Setting progress bars positions
						for (int i = 0; i < 7; i++)
						{
							speciesSprites[i].setPosition(480, (55 + (60 * i)));
							progressBarBaseSprites[i].setPosition(545, (66 + (61 * (i - 0))));
							progressBarOverlaySprites[i].setPosition(547, (68 + (61 * (i - 0))));
						}

						// Drawing progress bars
						for (int i = 0; i < 7; i++)
							game_normal.draw(speciesSprites[i]);
						for (int i = 0; i < 7; i++)
							game_normal.draw(progressBarBaseSprites[i]);
						for (int i = 0; i < 7; i++)
							game_normal.draw(progressBarOverlaySprites[i]);

						// if the mouse cursor hovers over a tile, draw the cursor sprite on top of the tile
						if (game_normal.hasFocus())
						{
							Vector2i mousePos = Mouse::getPosition(game_normal);
							int column = mousePos.x / tileSize;
							int row = mousePos.y / tileSize;

							if ((column >= 0 && column < 8) && (row >= 0 && row <= 8))
								cursor_sprite.setPosition((column * tileSize) + 5, (row * tileSize) + 5);
							else
								cursor_sprite.setPosition(-100, -100); // move the sprite off-screen
						}
						else
							cursor_sprite.setPosition(-100, -100); // move the sprite off-screen if the window doesn't have focus

						game_normal.draw(cursor_sprite);
						game_normal.display();

						// deleting dynamic arrays
						delete[] speciesSprites;
						delete[] progressBarBaseSprites;
						delete[] progressBarOverlaySprites;

						// Clearing the window
						game_normal.clear();
					}
				}

				if (timedButton_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
				{
					clickSound.setBuffer(click_nav_buffer);
					clickSound.play();

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
					RenderWindow game_timed(VideoMode(740, 490), "Menagerie Adventures");
					game_timed.setFramerateLimit(60);

					Texture bg_texture, animal_textures, cursor_texture, level_texture;
					bg_texture.loadFromFile("assets/textures/background.png");
					animal_textures.loadFromFile("assets/textures/animals.png");
					cursor_texture.loadFromFile("assets/textures/selection_box.png");
					level_texture.loadFromFile("assets/textures/level_base.png");

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
					int column1, column2, row1, row2, clickNumber = 0, soundPlayed = 0;
					Vector2i position;

					Music timedGameMusic;

					if (!(timedGameMusic.openFromFile("assets/audio/party-event-moodmode.ogg")))
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

							if (gameEvent.type == Event::MouseButtonPressed) // Checks if the user clicks the left mouse button
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

							if (soundPlayed == 0)
							{
								clickSound.setBuffer(click1_buffer);
								clickSound.play();
								soundPlayed++;
							}

							break;

						// Deducing row and col using coords for second click
						case 2:
							column2 = position.x / tileSize;
							row2 = position.y / tileSize;

							// Addresses random clicks
							clickSound.setBuffer(click_incorrect_buffer);
							clickSound.play();
							soundPlayed++;

							// check if the tiles are adjacent
							if (absVal(column1 - column2) + absVal(row1 - row2) == 1)
							{
								swapTiles(gridSpecies, row1, row2, column1, column2);

								// if the swap doesn't result in a match, swap the tiles back
								if (!(matchSpecies(gridSpecies, 0, score, false)))
								{
									clickSound.setBuffer(click_incorrect_buffer);
									clickSound.play();

									swapTiles(gridSpecies, row1, row2, column1, column2);
								}
								else
								{
									clickSound.setBuffer(click_match_buffer);
									clickSound.play();
								}
							}

							soundPlayed = 0;
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
								animal_sprite.setPosition((gridXPos[row][col] + 5), (gridYPos[row][col] + 5));
								game_timed.draw(animal_sprite);
							}
						}

						// Checking for matching species
						matchSpecies(gridSpecies, matchedCount, score);

						// Drawing text
						game_timed.draw(timerBackground_sprite);

						elapsedTime = gameClock.getElapsedTime();
						int seconds = 31 - elapsedTime.asSeconds();
						int minutes = seconds / 60;
						seconds = seconds % 60;

						if (seconds <= 120 && minutes == 0)
						{
							Color customColor(255, 110, 110, 255);
							timerText.setFillColor(customColor);
							timedGameMusic.setPitch(1.5f);
						}
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

						// Checking if there are any possible moves left
						if ((anyPossibleMoves(gridSpecies, score) == false) || (seconds == 0 && minutes == 0))
						{
							// Showing game over screen
							game_timed.close();
							timedGameMusic.stop();

							// creating new window
							RenderWindow gameOverWindow(VideoMode(740, 490), "Game Over");
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
							finalScoreText.setPosition(264, 220);

							// gameover audio
							Music gameOverMusic;

							if (!(gameOverMusic.openFromFile("assets/audio/faint-courage-toby-fox.ogg")))
								return -1;

							gameOverMusic.play();
							gameOverMusic.setVolume(50);
							gameOverMusic.setLoop(true);

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
											clickSound.setBuffer(click_nav_buffer);
											clickSound.play();

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
						for (int i = 0; i < 7; i++)
							speciesSprites[i] = Sprite(barSpecies_textures[i]);

						Sprite *progressBarBaseSprites = new Sprite[7];
						for (int i = 0; i < 7; i++)
							progressBarBaseSprites[i] = Sprite(barBase_textures[i]);

						Sprite *progressBarOverlaySprites = new Sprite[7];
						for (int i = 0; i < 7; i++)
							progressBarOverlaySprites[i] = Sprite(barOverlay_textures[i]);

						// Setting progress bar base and bar size
						for (int i = 0; i < 7; i++)
							speciesSprites[i].setTextureRect(IntRect(49 * i, 0, 50, 50));

						for (int i = 0; i < 7; i++)
							progressBarBaseSprites[i].setTextureRect(IntRect(0, 0, 123, 24));

						for (int i = 0; i < 7; i++)
							progressBarOverlaySprites[i].setTextureRect(IntRect(0, 0, calcBarWidth(matchedCount[i], ((i * (-1000)) + 8000)), 20));

						// Close the window and game if the player has collected all the species
						int speciesRepo[7] = {8000, 7000, 6000, 5000, 4000, 3000, 2000};

						for (int i = 0; i < 7; i++)
						{
							// If the player has collected all the species
							if (matchedCount[i] >= speciesRepo[i])
							{
								game_timed.close();
								timedGameMusic.stop();

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
								RenderWindow winWindow(VideoMode(740, 490), "Final Score");
								winWindow.setFramerateLimit(60);

								Music youWinMusic;

								if (!youWinMusic.openFromFile("assets/audio/tower-clear-new-super-mario-bros-wii-chiptune.ogg"))
									return -1;

								youWinMusic.setVolume(100);
								youWinMusic.play();

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
												Vector2i mousePos = Mouse::getPosition(winWindow);
												if (backButton_sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
												{
													clickSound.setBuffer(click_nav_buffer);
													clickSound.play();

													youWinMusic.stop();
													winWindow.close();
													main();
												}
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
							}
						}

						// Setting progress bars positions
						for (int i = 0; i < 7; i++)
						{
							speciesSprites[i].setPosition(480, (55 + (60 * i)));
							progressBarBaseSprites[i].setPosition(545, (66 + (61 * (i - 0))));
							progressBarOverlaySprites[i].setPosition(547, (68 + (61 * (i - 0))));
						}

						// Drawing progress bars
						for (int i = 0; i < 7; i++)
							game_timed.draw(speciesSprites[i]);
						for (int i = 0; i < 7; i++)
							game_timed.draw(progressBarBaseSprites[i]);
						for (int i = 0; i < 7; i++)
							game_timed.draw(progressBarOverlaySprites[i]);

						// if the mouse cursor hovers over a tile, draw the cursor sprite on top of the tile
						if (game_timed.hasFocus())
						{
							Vector2i mousePos = Mouse::getPosition(game_timed);
							int column = mousePos.x / tileSize;
							int row = mousePos.y / tileSize;

							if ((column >= 0 && column < 8) && (row >= 0 && row <= 8))
								cursor_sprite.setPosition((column * tileSize) + 5, (row * tileSize) + 5);
							else
								cursor_sprite.setPosition(-100, -100); // move the sprite off-screen
						}
						else
							cursor_sprite.setPosition(-100, -100); // move the sprite off-screen if the window doesn't have focus

						game_timed.draw(cursor_sprite);
						game_timed.display();

						// deleting dynamic arrays
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