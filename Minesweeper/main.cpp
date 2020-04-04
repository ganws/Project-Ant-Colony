#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <list>

#include "Button.h"
#include "Menu.h"
#include "MenuBar.h"
#include "StartMenu.h"
#include "MainMenu.h"

class Tile;
enum class GameState;
enum class GameState
{
	START,
	BASE,
	MENU,
	GAMEOVER,
	VICTORY
};


//////////////////////////////GLOBAL/////////////////////////

constexpr int WINDOW_WIDTH = 500;
constexpr int WINDOW_HEIGHT = 500;
sf::Vector2i tileRes = sf::Vector2i(20, 20);
int tileNum = tileRes.x * tileRes.y;
typedef std::vector<Tile> Tileset;
GameState playerGameState = GameState::START;
int SCORE = 0;
sf::Clock CLOCK;
float TIMER = 0.0f;


///////////////////////////////////////////////////////////////

enum class Difficulty
{
	// probability of tile containing a bomb

	EASY = 20,
	MEDIUM = 40,
	HARD = 50

};

class Tile : public sf::RectangleShape
{
	enum class TileState
	{
		UNCHECKED,
		CHECKED,
		FLAGGED
	};

public:
	//VISUAL ART
	float lineWidth{ 2 };
	sf::Color outlineColor = sf::Color(0, 0, 0); //black
	sf::Color initColor = sf::Color(100, 100, 100); //grey
	sf::Color checkedColor = sf::Color(50, 50, 50); //dark grey
	sf::Color fontColor = sf::Color(255, 255, 255); //white

	//FLAG
	sf::RectangleShape Flag;

	//BOMB
	sf::CircleShape Bomb;


	//PROPERTIES
	sf::Vector2i tilePos;
	TileState currentState{ TileState::UNCHECKED }; //initial state
	int neighbor_bomb_num{ 0 };
	sf::Text bombNumDisp;
	bool containBomb{ false };
	std::vector<Tile*> Neighbor_ptr;
	bool inList{ false }; //to prevent recursive propagation
	bool directNeighbor{ false };
	std::vector<Tile>* tileset_ptr;
	bool clickedTile{ false };

	static int RemainingEmptyTile;

public:

	Tile() {};
	~Tile() {};

	void initTile(sf::Vector2f pos, sf::Vector2f size, sf::Font& font)
	{

		this->setSize(size);
		this->setPosition(pos.x + size.x /2 , pos.y + size.y/2);
		this->setFillColor(initColor);
		this->setOutlineColor(outlineColor);
		this->setOutlineThickness(-lineWidth);
		//this->setOrigin(size.x/2, size.y /2);

		//bombdisp
		unsigned int charSize = static_cast<unsigned int>(tileNum / tileRes.x);
		this->bombNumDisp.setCharacterSize(charSize);
		this->bombNumDisp.setOrigin(charSize/2, charSize/2);
		this->bombNumDisp.setPosition(this->getPosition());
		this->bombNumDisp.setFillColor(sf::Color::White);
		this->bombNumDisp.setFont(font);

		//flag
		this->Flag.setSize(sf::Vector2f(size.x / 4, size.y / 4));
		this->Flag.setOrigin(sf::Vector2f(size.x / 8, size.y / 8));
		this->Flag.setPosition(this->getPosition());
		this->Flag.setFillColor(sf::Color::Transparent);

	}

	void setBombstat(bool isBomb)
	{
		this->containBomb = isBomb;
	}
	bool getBombstat()
	{
		return containBomb;
	}
	bool isChecked()
	{
		if (this->currentState == TileState::CHECKED)
			return true;
		else
			return false;
	}

	static void updateEmptyTileNum(int num)
	{
		RemainingEmptyTile = num;
	}

	static int getRemainEmptyTiles()
	{
		return RemainingEmptyTile;
	}


	void onClick()
	{
		switch (currentState)
		{
		case TileState::UNCHECKED:
			if (!containBomb)
			{

				this->setFillColor(checkedColor);
				currentState = TileState::CHECKED;

				tileSound->play();

				updateEmptyTileNum(RemainingEmptyTile - 1);
				SCORE++;

				// reveal number
				if (this->neighbor_bomb_num == 0)
				{
					this->bombNumDisp.setFillColor(sf::Color::Transparent);
				}
				else
				{
					this->bombNumDisp.setFillColor(fontColor);
				}

				if (this->clickedTile || this->neighbor_bomb_num == 0)
					this->propagateTiles();

				if (RemainingEmptyTile <= 0)
					playerGameState = GameState::VICTORY;
			}
			else
			{
				//////////////GAME OVER//////////////

				playerGameState = GameState::GAMEOVER;
				std::cout << "BOMB!!!!\nGame over!\n";
				for (auto &t : *tileset_ptr)
				{
					if(t.containBomb)
						t.Bomb.setFillColor(sf::Color::Red);
				}

				bombSound->play();
			}

			break;
		case TileState::CHECKED:
			break;

		case TileState::FLAGGED:
			break;
		default:
			break;
		}
	};

	

	void onRightClick()
	{
		switch (this->currentState)
		{

			case TileState::UNCHECKED:
				this->Flag.setFillColor(sf::Color::Black);
				this->currentState = TileState::FLAGGED;
				break;

			case TileState::CHECKED:
				//do nothing
				break;

			case TileState::FLAGGED:
				this->Flag.setFillColor(sf::Color::Transparent);
				this->currentState = TileState::UNCHECKED;
				break;

			default:
				//do nothing
				break;
		}
	}

	void propagateTiles()
	{
		for (int i = 0; i < this->Neighbor_ptr.size(); i++)
		{
			Tile& currentNeighbor = *this->Neighbor_ptr[i];

			if (!currentNeighbor.containBomb&& currentNeighbor.currentState == TileState::UNCHECKED)
			{
				currentNeighbor.onClick();
			}
		}
	}

	void importResource(sf::Sound* tileSound, sf::Sound* bombSound, sf::Font *font)
	{
		this->tileSound = tileSound;
		this->bombSound = bombSound;
		this->font = font;
	}

	private:
		sf::Font* font;
		sf::Sound* tileSound;
		sf::Sound* bombSound;
};


void createTiles(Tileset& tileset, sf::Font& font)
{
	sf::Vector2f size = sf::Vector2f(WINDOW_WIDTH / tileRes.x, WINDOW_HEIGHT / tileRes.y);
	tileset.resize(tileRes.x * tileRes.y);

	for (int j = 0; j < tileRes.x; j++)
		for (int i = 0; i < tileRes.y; i++)
		{

			sf::Vector2f initialPosition = sf::Vector2f(i * size.x, j * size.y);
			int indx = i + j * tileRes.x;
			Tile* currentTile = &tileset[indx];

			tileset[indx].initTile(initialPosition, size, font);
			tileset[indx].setOrigin(sf::Vector2f(size.x / 2, size.y / 2));
			tileset[indx].tileset_ptr = &tileset;
			tileset[indx].tilePos.x = i;
			tileset[indx].tilePos.y = j;

			if (j > 0)
				currentTile->Neighbor_ptr.push_back(&tileset[i + (j - 1) * tileRes.x]); //north

			if (i < tileRes.x - 1)
				currentTile->Neighbor_ptr.push_back(&tileset[(i + 1) + j * tileRes.x]); //east

			if (j < tileRes.y - 1)
				currentTile->Neighbor_ptr.push_back(&tileset[i + (j + 1) * tileRes.x]); //south

			if (i > 0)
				currentTile->Neighbor_ptr.push_back(&tileset[(i - 1) + j * tileRes.x]); //wst

			if (j > 0 && i < tileRes.x - 1)
				currentTile->Neighbor_ptr.push_back(&tileset[(i + 1) + (j - 1) * tileRes.x]); //north east

			if (j > 0 && i > 0)
				currentTile->Neighbor_ptr.push_back(&tileset[(i - 1) + (j - 1) * tileRes.x]); //north west

			if (j < tileRes.y - 1 && i < tileRes.x - 1)
				currentTile->Neighbor_ptr.push_back(&tileset[(i + 1) + (j + 1) * tileRes.x]); //south east

			if (j < tileRes.y - 1 && i >0)
				currentTile->Neighbor_ptr.push_back(&tileset[(i - 1) + (j + 1) * tileRes.x]); //south west

		}

};
void generateBomb(Tileset& tileset, Difficulty difficulty)
{
	float chance = 0;
	int emptyTiles = tileNum;

	switch (difficulty)
	{
	case Difficulty::EASY:
		chance = 10;
		break;
	case Difficulty::MEDIUM:
		chance = 25;
		break;
	case Difficulty::HARD:
		chance = 30;
		break;
	default:
		break;
	}

	// Update bomb number for neighboring tiles
	for (int m = 0; m < tileset.size(); m++)
	{

		//Find neighbor tiles
		int& X = tileset[m].tilePos.x;
		int& Y = tileset[m].tilePos.y;
		int& Len = tileRes.x;

		std::vector<sf::Vector2i> neighborPos;
		neighborPos.resize(8);

		neighborPos[0].x = X - 1; //top left
		neighborPos[0].y = Y - 1;

		neighborPos[1].x = X; //top 
		neighborPos[1].y = Y - 1;

		neighborPos[2].x = X + 1; //top right
		neighborPos[2].y = Y - 1;

		neighborPos[3].x = X - 1; //left
		neighborPos[3].y = Y;

		neighborPos[4].x = X + 1; //right
		neighborPos[4].y = Y;

		neighborPos[5].x = X - 1; //bot left
		neighborPos[5].y = Y + 1;

		neighborPos[6].x = X; //bot 
		neighborPos[6].y = Y + 1;

		neighborPos[7].x = X + 1; //bot right
		neighborPos[7].y = Y + 1;


		float randval = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 100.0f;
		if (randval < chance)
		{
			tileset[m].setBombstat(true);
			emptyTiles--;

			sf::CircleShape& bomb = tileset[m].Bomb;
			bomb.setFillColor(sf::Color::Transparent);
			sf::Vector2f size= tileset[m].getSize();
			bomb.setRadius(size.x /4 );
			bomb.setOrigin(bomb.getRadius(), bomb.getRadius());
			bomb.setPosition(tileset[m].getPosition());

			for (int i = 0; i < 8; i++)
			{
				//check if indx is out of bound
				if (!(neighborPos[i].x < 0 || neighborPos[i].x > tileRes.x - 1 ||
					neighborPos[i].y < 0 || neighborPos[i].y > tileRes.y - 1))
				{

					tileset[neighborPos[i].x + neighborPos[i].y * tileRes.x].neighbor_bomb_num++;

				}
			}
		}

		for (auto& t : tileset)
		{
			t.bombNumDisp.setString(std::to_string(t.neighbor_bomb_num));
			t.bombNumDisp.setFillColor(sf::Color::Transparent);
		}
		//for (int i = 0; i < 8; i++)
		//{
		//	//check if indx is out of bound
		//	if (!(neighborPos[i].x < 0 || neighborPos[i].x > tileRes.x - 1 ||
		//		neighborPos[i].y < 0 || neighborPos[i].y > tileRes.y - 1))
		//	{
		//		//if (i == 1 || i == 3 || i == 4 || i || 6)
		//			tileset[m].Neighbor_ptr.push_back(&tileset[neighborPos[i].x + neighborPos[i].y * tileRes.x]);
		//	}
		//}


		Tile::updateEmptyTileNum(emptyTiles);
	}
}

void restartGame(Tileset &tileset, sf::Font &font, Difficulty difficulty)
{
	tileset.clear();
	TIMER = 0.0f;
	createTiles(tileset, font);
	generateBomb(tileset, difficulty);
	playerGameState = GameState::BASE;
}

/////////////////////////////////////////////////////////////
////////////////// MAIN GAME LOOP ///////////////////////////
int Tile::RemainingEmptyTile;

int main()

{
	/////////////////// SETTING /////////////////////////

	Difficulty difficulty = Difficulty::EASY;
	CLOCK.restart();

	

	/////////////////// WINDOW ////////////////////////////////

	sf::View view;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");
	window.setFramerateLimit(60);

	/////////////////// GAME RESOURCE /////////////////////////

	sf::Font arial;
	if(!arial.loadFromFile("../Resource/arial.ttf"))
		return -1;

	sf::SoundBuffer balloonBuff;
	if(!balloonBuff.loadFromFile("../Resource/balloon_pop.wav"))
		return -1;

	sf::SoundBuffer blopBuff;
	//blopBuff.loadFromFile("../Resource/blopsound.wav");
	if (!blopBuff.loadFromFile("../Resource/blopsound.wav"))
		return -1;

	sf::SoundBuffer tinyButtonBuff;
	if (!tinyButtonBuff.loadFromFile("../Resource/click.wav"))
		return -1;

	sf::Sound balloonSound;
	balloonSound.setBuffer(balloonBuff);

	sf::Sound blopSound;
	blopSound.setBuffer(blopBuff);

	sf::Sound tinyButtonSound;
	tinyButtonSound.setBuffer(tinyButtonBuff);

	/////////////////// GAME OBJECT /////////////////////////
	Tileset tileset;

	srand(time(NULL));
	createTiles(tileset, arial);
	generateBomb(tileset, difficulty);

	for (auto& t : tileset)
		t.importResource(&blopSound, &balloonSound, &arial);

	//////////////////// STARTING SCREEN /////////////////////////////

	sf::Vector2f menuSize = { 400,300};
	sf::Vector2f menuPos = { WINDOW_WIDTH / 2 - menuSize.x / 2, WINDOW_HEIGHT / 2 - menuSize.y / 2 };

	StartMenu startmenu = StartMenu(menuSize, menuPos, sf::Color(40, 40, 40), { 350, 50 }, sf::Color(150, 150, 150), arial);
	startmenu.setTextOnMenu("Select Difficulty", 25, sf::Color::White);
	startmenu.button1.setText("EASY", 25);
	startmenu.button2.setText("MEDIUM", 25);
	startmenu.button3.setText("HARD", 25);

	//////////////////// MENU BAR /////////////////////////////

	MenuBar menubar = MenuBar({ 0,0 }, { WINDOW_WIDTH , 40 }, sf::Color(0, 0, 200), arial, { 50, 25 });



	/////////////  MAIN MENU  ////////////////
	menuSize = { 300,100 };
	menuPos = { WINDOW_WIDTH / 2 - menuSize.x / 2, WINDOW_HEIGHT / 2 - menuSize.y / 2 };

	MainMenu mainmenu(menuSize, menuPos, sf::Color(40, 40, 40), { 80, 50 }, sf::Color(150, 150, 150), arial);
	mainmenu.setTextOnMenu("Menu", 15, sf::Color::White);
	mainmenu.button1.setText("Quit", 20);
	mainmenu.button2.setText("Resume", 20);
	mainmenu.button3.setText("Restart", 20);


	/////////////  GAME OVER MENU  ////////////////
	menuSize = { 200,100 };
	menuPos = { WINDOW_WIDTH / 2 - menuSize.x / 2, WINDOW_HEIGHT / 2 - menuSize.y / 2 };

	Menu menu1(menuSize, menuPos, sf::Color(40,40,40), { 80, 50 }, sf::Color(150,150, 150), arial);
	menu1.setTextOnMenu("Game over!!!", 15, sf::Color::White);
	menu1.button1.setText("Quit", 20);
	menu1.button2.setText("Restart", 20);

	
	while (window.isOpen())
	{
		sf::Event event;


		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();


			if (event.type == sf::Event::MouseButtonReleased)
			{
				sf::Vector2i mouseAbsPos = sf::Mouse::getPosition(window);
				sf::Vector2f mousePos = window.mapPixelToCoords(mouseAbsPos);
		
				switch (playerGameState)
				{

				case GameState::START:
					
					if (startmenu.button1.isMouseOver(window))
					{
						tinyButtonSound.play();
						difficulty = Difficulty::EASY;
						restartGame(tileset, arial, difficulty);
					}

					if (startmenu.button2.isMouseOver(window))
					{
						tinyButtonSound.play();
						difficulty = Difficulty::MEDIUM;
						restartGame(tileset, arial, difficulty);
					}

					if (startmenu.button3.isMouseOver(window))
					{
						tinyButtonSound.play();
						difficulty = Difficulty::HARD;
						restartGame(tileset, arial, difficulty);
					}

					break;


				case GameState::BASE:
					if (menubar.button1.isMouseOver_ABS(window))
					{
						playerGameState = GameState::MENU;
					}

					for (auto& tile : tileset)
					{
						if (tile.getGlobalBounds().contains(mousePos))
						{
							if (event.mouseButton.button == sf::Mouse::Button::Left)
							{
								tile.clickedTile = true;
								tile.onClick();

							}
							if (event.mouseButton.button == sf::Mouse::Button::Right)
							{
								tile.onRightClick();
							}
						}
					}
					break;

				case GameState::MENU:

					if (mainmenu.button1.isMouseOver(window))
					{
						tinyButtonSound.play();
						window.close();
					}

					if (mainmenu.button2.isMouseOver(window))
					{
						tinyButtonSound.play();
						playerGameState = GameState::BASE;
					}

					if (mainmenu.button3.isMouseOver(window))
					{
						tinyButtonSound.play();
						playerGameState = GameState::START;
					}



					break;

				case GameState::GAMEOVER:

					if (menu1.button1.isMouseOver(window))
					{
						tinyButtonSound.play();

						window.close();
					}

					if (menu1.button2.isMouseOver(window))
					{
						tinyButtonSound.play();

						playerGameState = GameState::START;
					}

					break;

				case GameState::VICTORY:

					if (menu1.button1.isMouseOver(window))
					{
						tinyButtonSound.play();

						window.close();
					}

					if (menu1.button2.isMouseOver(window))
					{
						tinyButtonSound.play();

						playerGameState = GameState::START;
					}

					break;

				default:
					break;
				}

			}

			if (event.type == sf::Event::MouseMoved)
			{
				menu1.button1.isMouseOver(window);
				menu1.button2.isMouseOver(window);
				mainmenu.button1.isMouseOver(window);
				mainmenu.button2.isMouseOver(window);
				mainmenu.button3.isMouseOver(window);
				startmenu.button1.isMouseOver(window);
				startmenu.button2.isMouseOver(window);
				startmenu.button3.isMouseOver(window);
				menubar.button1.isMouseOver_ABS(window);

			}

		}

		// TIME
		if (playerGameState == GameState::BASE)
			TIMER  +=  CLOCK.getElapsedTime().asSeconds();
		CLOCK.restart();

		menubar.updateScore(Tile::RemainingEmptyTile, static_cast<int>(TIMER));		
		window.clear();

		view = window.getDefaultView();
		float heightRatio = (WINDOW_HEIGHT - menubar.getSize().y) / WINDOW_HEIGHT; 
		view.setViewport(sf::FloatRect(0.0f, 1-heightRatio, 1.0f, heightRatio));

		window.setView(window.getDefaultView());
		menubar.drawTo(window);

		window.setView(view);
		//draw
		for (auto& t : tileset)
		{
			window.draw(t);
			window.draw(t.bombNumDisp);
			window.draw(t.Flag);
			window.draw(t.Bomb);
		}

		if (playerGameState == GameState::GAMEOVER)
		{
			menu1.setMessage("Game over!");
			menu1.drawTo(window);
		}

		if (playerGameState == GameState::VICTORY)
		{
			menu1.setMessage("Victory!");
			menu1.drawTo(window);
		}

		if (playerGameState == GameState::START)
		{
			startmenu.drawTo(window);
		}

		if (playerGameState == GameState::MENU)
		{
			mainmenu.drawTo(window);
		}



		window.display();
	}

	return 0;
}