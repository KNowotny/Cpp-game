
/* JAKUB NOWOTNY S172452

Temat projektu: Gra Bubble Attack

Co zrealizowalem w biezacym tygodniu?

poprawienie wydajności i sterowania



Co planuje na kolejny tydzien?

Dodanie zapisywania wyniku do pliku


------------------------------------------------------------*/





/*------------------------------------------------------------

Program glowny

------------------------------------------------------------*/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
#include <stdio.h>
#define MAX_LICZBA_POZIOMOW 4
#define DIFFICULT_LEVELS 4
using namespace std;
using namespace sf;

class Enemy
{
private:
	Sprite body;
	Texture tex;
public:
	Enemy();
	void SetPos(int x, int y);
	void Move(int dx, int dy);
	Sprite GetSprite() { return body; };
	Vector2f GetPosition() { return body.getPosition(); };
	FloatRect GetBounds() { return body.getGlobalBounds(); };
};

Enemy::Enemy()
{
	tex.loadFromFile("crocodile.png");
	body = Sprite(tex, IntRect(0, 0, 129, 128));
	body.setPosition(0, 0);
}

void Enemy::SetPos(int x, int y)
{
	body.setPosition(x, y);
}

void Enemy::Move(int dx, int dy)
{
	body.move(dx, dy);
}

class Player
{
private:
	RectangleShape shot;
	Sprite body;
	Texture tex;
public:
	string name;
	Vector2f enemyPos;
	int dx;
	int dy;
	int difficult;
	bool isAlive;
	int score;
	void SetSprite(RenderWindow &window, Sprite ground);
	void Move(int dx, int windowWidth);
	Sprite GetSprite() { return body; };
	Vector2f GetPosition() { return body.getPosition(); };
	FloatRect GetBounds() { return body.getGlobalBounds(); };
	void SetShot(RenderWindow &window);
	void SetPosition(Vector2f position) { body.setPosition(position); };
	RectangleShape GetShot() { return shot; };
	bool Shot(Enemy *enemy);
};

void Player::SetShot(RenderWindow &window)
{
	shot.setSize(sf::Vector2f(2, window.getSize().y));
	shot.setOutlineColor(sf::Color::Red);
	shot.setOutlineThickness(5);
	shot.setPosition(body.getPosition().x + body.getGlobalBounds().width / 2, body.getPosition().y);
}

bool Player::Shot(Enemy *enemy)
{
	if (enemy->GetSprite().getGlobalBounds().intersects(shot.getGlobalBounds()))
	{
		enemy->SetPos(10000, 10000);
		return false;
	}
	else if (shot.getGlobalBounds().top <= 0)
	{
		return false;
	}
	else
	{
		shot.move(0, -0.3);
		return true;
	}
}

void Player::SetSprite(RenderWindow &window, Sprite ground)
{
	tex.loadFromFile("walrus.png");
	body = Sprite(tex, IntRect(0, 0, 129, 128));

	body.setPosition(window.getSize().x / 2 - body.getLocalBounds().width / 2, window.getSize().y - ground.getLocalBounds().height - body.getLocalBounds().height);
}

void Player::Move(int dx, int windowWidth)
{
	if (body.getPosition().x <= 0 && dx < 0)
	{
		cout << "kolizja" << endl;
	}
	else if (body.getPosition().x >= windowWidth - body.getGlobalBounds().width && dx > 0)
	{
		cout << "kolizja" << endl;
	}
	else
	{
		body.move(dx, 0);
	}
}


class Menu
{
private:
	Font font;
	Text menuNewGame[MAX_LICZBA_POZIOMOW + 1];
	Text menuDifficult[DIFFICULT_LEVELS + 1];
	Text menuEnd[1];
	Text menuScore[1];
	Text continueGame[1];
	Text status[2];
	Text pause[9];
	Text quit[2];
	int selectedItem = 0;

public:
	Menu(float width, float height);
	~Menu() {};
	void moveUp(int item);
	void moveDown(int item);
	int getSelectedItem() { return selectedItem; }
	void setSelectedItem() { selectedItem = 0; }
	void drawMenu(sf::RenderWindow &window);
	void drawEnd(sf::RenderWindow &window, int score, float width, float height, string name);
	void drawDifficultMenu(sf::RenderWindow &window);
	void drawScore(sf::RenderWindow &window, int score);
	void drawContinue(sf::RenderWindow &window, Player player);
	void drawNameStatus(sf::RenderWindow &window, string name);
	void drawPause(sf::RenderWindow &window);
	void drawQuit(RenderWindow &window);
};

Menu::Menu(float width, float height)
{
	if (!font.loadFromFile("arial.ttf"))
	{
		cout << "blad czcionki";
		return;
	}
	for (int i = 0; i < 3; i++)
	{
		menuNewGame[i].setFont(font);
		menuNewGame[i].setCharacterSize(50);
		menuNewGame[i].setOutlineThickness(5);
		menuNewGame[i].setOutlineColor(Color::Black);
	}
	menuNewGame[0].setStyle(sf::Text::Bold);
	menuNewGame[0].setFillColor(sf::Color::Red);
	menuNewGame[0].setString("Nowa gra");
	menuNewGame[0].setPosition(sf::Vector2f(width / 2 - menuNewGame[0].getGlobalBounds().width / 2, height / (MAX_LICZBA_POZIOMOW + 1) * 2));
	menuNewGame[1].setFillColor(sf::Color::White);
	menuNewGame[1].setString("Pomoc");
	menuNewGame[1].setPosition(sf::Vector2f(width / 2 - menuNewGame[1].getGlobalBounds().width / 2, height / (MAX_LICZBA_POZIOMOW + 1) * 3));
	menuNewGame[2].setFillColor(sf::Color::White);
	menuNewGame[2].setString(L"Wyjście");
	menuNewGame[2].setPosition(sf::Vector2f(width / 2 - menuNewGame[2].getGlobalBounds().width / 2, height / (MAX_LICZBA_POZIOMOW + 1) * 4));
	menuNewGame[3].setFont(font);
	menuNewGame[3].setFillColor(sf::Color::White);
	menuNewGame[3].setString("MORSJANIN");
	menuNewGame[3].setOutlineThickness(10);
	menuNewGame[3].setOutlineColor(Color::Black);
	menuNewGame[3].setCharacterSize(120);
	menuNewGame[3].setPosition(sf::Vector2f(width / 2 - menuNewGame[3].getGlobalBounds().width / 2, height / (MAX_LICZBA_POZIOMOW + 1) * 1 - 65));

	for (int i = 0; i < 3; i++)
	{
		menuDifficult[i].setFont(font);
		menuDifficult[i].setFillColor(sf::Color::White);
		menuDifficult[i].setCharacterSize(50);
		menuDifficult[i].setOutlineThickness(5);
		menuDifficult[i].setOutlineColor(Color::Black);
	}
	menuDifficult[0].setFillColor(sf::Color::Red);
	menuDifficult[0].setStyle(sf::Text::Bold);
	menuDifficult[0].setString(L"Łatwy");
	menuDifficult[0].setPosition(sf::Vector2f(width / 2 - menuDifficult[0].getGlobalBounds().width / 2, height / (DIFFICULT_LEVELS + 1) * 2));
	menuDifficult[1].setFillColor(sf::Color::White);
	menuDifficult[1].setString(L"Średni");
	menuDifficult[1].setPosition(sf::Vector2f(width / 2 - menuDifficult[1].getGlobalBounds().width / 2, height / (DIFFICULT_LEVELS + 1) * 3));
	menuDifficult[2].setFillColor(sf::Color::White);
	menuDifficult[2].setString("Trudny");
	menuDifficult[2].setPosition(sf::Vector2f(width / 2 - menuDifficult[2].getGlobalBounds().width / 2, height / (DIFFICULT_LEVELS + 1) * 4));
	menuDifficult[3].setFont(font);
	menuDifficult[3].setFillColor(sf::Color::White);
	menuDifficult[3].setString("Wybierz poziom: ");
	menuDifficult[3].setCharacterSize(70);
	menuDifficult[3].setOutlineThickness(5);
	menuDifficult[3].setOutlineColor(Color::Black);
	menuDifficult[3].setPosition(sf::Vector2f(width / 2 - menuDifficult[3].getGlobalBounds().width / 2, height / (DIFFICULT_LEVELS + 1) * 1 - 45));
}

void Menu::drawQuit(RenderWindow &window)
{
	quit[0].setString(L"Czy na pewno chcesz opuścić grę?");
	quit[0].setCharacterSize(70);

	quit[1].setString("Y/N");
	quit[1].setCharacterSize(70);

	for (int i = 0; i < 2; i++)
	{
		quit[i].setFont(font);
		quit[i].setFillColor(sf::Color::White);
		quit[i].setOutlineThickness(7);
		quit[i].setOutlineColor(Color::Black);
		quit[i].setPosition(sf::Vector2f(window.getSize().x / 2 - quit[i].getGlobalBounds().width / 2, window.getSize().y / 3 * (i + 1)));
		window.draw(quit[i]);
	}
}

void Menu::drawPause(RenderWindow &window)
{


	pause[0].setString(L"Morsjanin - gra zręcznościowa w której poruszamy sie Morsem w lewo lub prawo. ");

	pause[1].setString(L"Przeszkadza nam w tym krokodyl! Jednak możemy go zastrzelić. Celem jest jak najdłuższe przeżycie.");

	pause[2].setString("Sterowanie:");

	pause[3].setString(L"A lub ← - ruch w lewo");

	pause[4].setString(L"D lub → - ruch w prawo");

	pause[5].setString(L"Space - strzał");

	pause[6].setString("F1 - pauza");

	pause[7].setString(L"Esc - wyjście z gry, gra może zostać wznowiona");

	pause[8].setString(L"Q - powrót");

	for (int i = 0; i < 10; i++)
	{
		pause[i].setFont(font);
		pause[i].setFillColor(sf::Color::White);
		pause[i].setCharacterSize(40);
		pause[i].setOutlineThickness(4);
		pause[i].setOutlineColor(Color::Black);
		pause[i].setPosition(sf::Vector2f(window.getSize().x / 2 - pause[i].getGlobalBounds().width / 2, window.getSize().y / 10 * (i + 1)));
		window.draw(pause[i]);
	}
}

void Menu::drawNameStatus(RenderWindow &window, string name)
{
	for (int i = 0; i < 2; i++)
	{
		status[i].setFont(font);
		status[i].setFillColor(sf::Color::White);
		status[i].setCharacterSize(70);
		status[i].setOutlineThickness(5);
		status[i].setOutlineColor(Color::Black);
	}
	status[0].setStyle(sf::Text::Bold);
	status[0].setString("Wpisz nick: ");
	status[0].setPosition(sf::Vector2f(window.getSize().x / 2 - status[0].getGlobalBounds().width / 2, status[0].getGlobalBounds().height + 10));

	status[1].setStyle(sf::Text::Bold);
	status[1].setString(name);
	status[1].setPosition(sf::Vector2f(window.getSize().x / 2 - status[1].getGlobalBounds().width / 2, window.getSize().y / 2));

	window.draw(status[0]);
	window.draw(status[1]);
}

void Menu::drawScore(sf::RenderWindow &window, int score)
{
	menuScore[0].setFont(font);
	menuScore[0].setFillColor(sf::Color::White);
	menuScore[0].setStyle(sf::Text::Bold);
	string text = to_string(score);
	menuScore[0].setString(text);
	menuScore[0].setCharacterSize(70);
	menuScore[0].setOutlineThickness(5);
	menuScore[0].setOutlineColor(Color::Black);
	menuScore[0].setPosition(sf::Vector2f(window.getSize().x / 2 - menuScore[0].getGlobalBounds().width / 2, menuEnd[0].getGlobalBounds().height + 10));

	window.draw(menuScore[0]);
}

void Menu::drawContinue(sf::RenderWindow &window, Player player)
{
	string difficult;
	switch (player.difficult)
	{
	case 1:
		difficult = "Easy";
		break;
	case 2:
		difficult = "Medium";
		break;
	case 3:
		difficult = "Hard";
		break;
	default:
		break;
	}
	string status = player.name + " " + difficult + " " + to_string(player.score);
	continueGame[0].setString(status);
	continueGame[0].setPosition(sf::Vector2f(window.getSize().x / 2 - continueGame[0].getGlobalBounds().width / 2, continueGame[0].getGlobalBounds().height + 10));
	continueGame[1].setString("Grasz dalej? Y/N");
	continueGame[1].setPosition(sf::Vector2f(window.getSize().x / 2 - continueGame[1].getGlobalBounds().width / 2, continueGame[1].getGlobalBounds().height + 100));

	for (int i = 0; i < 2; i++)
	{
		continueGame[i].setFont(font);
		continueGame[i].setFillColor(sf::Color::White);
		continueGame[i].setStyle(sf::Text::Bold);
		continueGame[i].setCharacterSize(70);
		continueGame[i].setOutlineThickness(5);
		continueGame[i].setOutlineColor(Color::Black);

		window.draw(continueGame[i]);
	}
}

void Menu::drawMenu(sf::RenderWindow &window)
{
	for (int i = 0; i < MAX_LICZBA_POZIOMOW; i++)
	{
		window.draw(menuNewGame[i]);
	}
}

void Menu::drawDifficultMenu(sf::RenderWindow &window)
{


	for (int i = 0; i < DIFFICULT_LEVELS; i++)
	{
		window.draw(menuDifficult[i]);
	}
}

void Menu::drawEnd(sf::RenderWindow &window, int score, float width, float height, string name)
{
	menuEnd[0].setFont(font);
	menuEnd[0].setFillColor(sf::Color::White);
	menuEnd[0].setCharacterSize(60);
	string text = "Koniec gry. Wynik " + name + " to: " + to_string(score);
	menuEnd[0].setString(text);
	menuEnd[0].setOutlineThickness(5);
	menuEnd[0].setOutlineColor(Color::Black);
	menuEnd[0].setPosition(sf::Vector2f(width / 2 - menuEnd[0].getGlobalBounds().width / 2 - 5, height / 2 - menuEnd[0].getGlobalBounds().height));

	window.draw(menuEnd[0]);
}

void Menu::moveUp(int item)
{
	switch (item)
	{
	case 0:
		if (selectedItem >= 1 && selectedItem < MAX_LICZBA_POZIOMOW - 1)
		{
			menuNewGame[selectedItem].setFillColor(sf::Color::White);
			menuNewGame[selectedItem].setStyle(sf::Text::Regular);
			selectedItem--;
			menuNewGame[selectedItem].setFillColor(sf::Color::Red);
			menuNewGame[selectedItem].setStyle(sf::Text::Bold);
		}
		break;
	case 2:
		if (selectedItem >= 1 && selectedItem < DIFFICULT_LEVELS - 1)
		{
			menuDifficult[selectedItem].setFillColor(sf::Color::White);
			menuDifficult[selectedItem].setStyle(sf::Text::Regular);
			selectedItem--;
			menuDifficult[selectedItem].setFillColor(sf::Color::Red);
			menuDifficult[selectedItem].setStyle(sf::Text::Bold);
		}
		break;
	default:
		break;
	}

}

void Menu::moveDown(int item)
{
	switch (item)
	{
	case 0:
		if (selectedItem >= 0 && selectedItem < MAX_LICZBA_POZIOMOW - 2)
		{
			menuNewGame[selectedItem].setFillColor(sf::Color::White);
			menuNewGame[selectedItem].setStyle(sf::Text::Regular);
			selectedItem++;
			menuNewGame[selectedItem].setFillColor(sf::Color::Red);
			menuNewGame[selectedItem].setStyle(sf::Text::Bold);
		}
		break;
	case 2:
		if (selectedItem >= 0 && selectedItem < DIFFICULT_LEVELS - 2)
		{
			menuDifficult[selectedItem].setFillColor(sf::Color::White);
			menuDifficult[selectedItem].setStyle(sf::Text::Regular);
			selectedItem++;
			menuDifficult[selectedItem].setFillColor(sf::Color::Red);
			menuDifficult[selectedItem].setStyle(sf::Text::Bold);
		}
		break;
	default:
		break;
	}

}

class Cloud
{
public:
	Texture tex1;
	Texture tex2;
	Texture tex3;
	Sprite cloud;
	Cloud();
	void setCloud(int choice);
	void setPos(int x, int y) {
		cloud.setPosition(x, y);
	};
};

Cloud::Cloud()
{
	tex1.loadFromFile("cloud1.png");
	tex2.loadFromFile("cloud2.png");
	tex3.loadFromFile("cloud3.png");
}

void Cloud::setCloud(int choice)
{

	switch (choice)
	{
	case 1:
		cloud.setTexture(tex1);
		break;
	case 2:
		cloud.setTexture(tex2);
		break;
	case 3:
		cloud.setTexture(tex3);
		break;
	default:
		break;
	};
};

void myDelay(int opoznienie)
{
	sf::Clock zegar;
	sf::Time czas;
	while (1)
	{
		czas = zegar.getElapsedTime();
		if (czas.asMilliseconds() > opoznienie)
		{
			zegar.restart();
			break;
		}
	}
}

int main()
{
	Player *dane;
	Player *plik;

	dane = new Player;
	plik = new Player;

	FILE *fp;
	FILE *fg;

	RenderWindow window(VideoMode::getDesktopMode(), "Morsjanin", Style::Fullscreen);

	Menu *menu = new Menu(window.getSize().x, window.getSize().y);

	sf::Texture textureB, textureG, texCloud1, texCloud2, texCloud3;
	textureB.loadFromFile("back.png");
	sf::Sprite background(textureB);
	textureG.loadFromFile("ground.png");
	background.setScale(
		window.getSize().x / background.getLocalBounds().width,
		window.getSize().y / background.getLocalBounds().height);
	sf::Sprite ground(textureG);
	ground.setPosition(0, window.getSize().y - textureG.getSize().y);
	ground.setScale(
		window.getSize().x / ground.getLocalBounds().width,
		window.getSize().y / ground.getLocalBounds().height);

	Player player;
	player.SetSprite(window, ground);

	Enemy *enemy = new Enemy;

	int dx1 = 0, dy1 = 0, playerSpeed = 6;

	Text text;
	string str;
	sf::Font font;
	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setCharacterSize(55);
	text.setFillColor(sf::Color::Yellow);

	int menu_selected_flag = 0, score = 0, loadScore = 0, pauseScore = 0, enemyWaiter = 0, lastMenu = 0;;
	bool shot = false, writing = true;

	Clock clock, scoreClock, shotclock;
	srand(time(NULL));

	Cloud cloud[10];
	for (int i = 0; i < 10; i++)
	{
		cloud[i].setCloud(i % 3 + 1);
		cloud[i].setPos(rand() % int(window.getSize().x - cloud[i].cloud.getLocalBounds().width), rand() % int(window.getSize().y - cloud[i].cloud.getLocalBounds().height));
	};

	while (window.isOpen())
	{

		Event event;
		while (window.pollEvent(event))
		{

			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up || Keyboard::isKeyPressed(Keyboard::Key::W))
				{
					myDelay(50);
					menu->moveUp(menu_selected_flag);
				}

				if (event.key.code == sf::Keyboard::Down || Keyboard::isKeyPressed(Keyboard::Key::S))
				{
					myDelay(50);
					menu->moveDown(menu_selected_flag);
				}
				if (menu_selected_flag == 0)
				{
					if (event.key.code == sf::Keyboard::Enter && menu->getSelectedItem() == 0)
					{
						enemy = new Enemy();

						menu_selected_flag = 1;
						menu->setSelectedItem();
						menu = new Menu(window.getSize().x, window.getSize().y);
						score = 0;
						writing = true;
						str.clear();
						text.setString(str);
					}

					if (event.key.code == sf::Keyboard::Enter && menu->getSelectedItem() == 1)
					{
						menu_selected_flag = 7;
					}

					if (event.key.code == sf::Keyboard::Enter && menu->getSelectedItem() == 2)
					{
						menu_selected_flag = 8;
						lastMenu = 0;
					}

					if (event.key.code == sf::Keyboard::Escape)
					{
						lastMenu = 0;
						menu_selected_flag = 8;
					}
				}

				if (menu_selected_flag == 2)
				{
					if (event.key.code == sf::Keyboard::Up || Keyboard::isKeyPressed(Keyboard::Key::W))
					{
						myDelay(50);
						menu->moveUp(1);
					}

					if (event.key.code == sf::Keyboard::Down || Keyboard::isKeyPressed(Keyboard::Key::S))
					{
						myDelay(50);
						menu->moveDown(1);
					}
					if (event.key.code == sf::Keyboard::Escape)
					{
						menu_selected_flag = 8;
						lastMenu = 0;
					}
				}
			}

			if (menu_selected_flag == 2)
			{
				loadScore = 0;

				if (event.key.code == sf::Keyboard::Enter && menu->getSelectedItem() == 0 && clock.getElapsedTime().asSeconds() > 0.5)
				{
					dx1 = 11;
					dy1 = 13;
					dane->dx = 11;
					dane->dy = 13;
					menu_selected_flag = 3;
					dane->difficult = 1;
					scoreClock.restart();
				}

				if (event.key.code == sf::Keyboard::Enter && menu->getSelectedItem() == 1 && clock.getElapsedTime().asSeconds() > 0.5)
				{
					dx1 = 13;
					dy1 = 15;
					dane->dx = 11;
					dane->dy = 13;
					menu_selected_flag = 3;
					dane->difficult = 2;
					scoreClock.restart();
				}

				if (event.key.code == sf::Keyboard::Enter && menu->getSelectedItem() == 2 && clock.getElapsedTime().asSeconds() > 0.5)
				{
					dx1 = 15;
					dy1 = 17;
					dane->dx = 11;
					dane->dy = 13;
					menu_selected_flag = 3;
					dane->difficult = 3;
					scoreClock.restart();
				}
				if (event.key.code == sf::Keyboard::Escape)
				{
					menu_selected_flag = 8;
					lastMenu = 0;
				}
				pauseScore = 0;
				fg = fopen("gracze.dat", "w+b");
				fwrite(dane, sizeof(Player), 1, fg);
				fclose(fg);
			}


			if (menu_selected_flag == 1)
			{
				if ((event.type == sf::Event::TextEntered) && writing)
				{
					if (event.text.unicode == 8)
					{
						str = str.substr(0, str.size() - 1);

						text.setString(str);
					}
					if (event.text.unicode > 45 && event.text.unicode < 123 && event.text.unicode != 13 && event.text.unicode != 8)
					{
						str += static_cast<char>(event.text.unicode);

						text.setString(str);

					}
					if (event.text.unicode == 13 && !text.getString().isEmpty())
					{
						writing = false;
						dane->isAlive = true;
						dane->name = text.getString();
						menu_selected_flag = 2;
						menu->setSelectedItem();
						clock.restart();
					}
				}

				if (event.key.code == sf::Keyboard::Escape)
				{
					lastMenu = 0;
					menu_selected_flag = 8;
				}
			}
		}

		if (menu_selected_flag == 0)
		{
			fp = fopen("gracze.dat", "r+b");
			fread(plik, sizeof(Player), 1, fp);
			fclose(fp);
			if (plik->isAlive)
			{
				menu_selected_flag = 6;
			}
			window.clear();
			window.draw(background);
			menu->drawMenu(window);
		}

		if (menu_selected_flag == 1)
		{
			window.clear();
			window.draw(background);
			menu->drawNameStatus(window, text.getString());
		}

		if (menu_selected_flag == 2)
		{
			window.clear();
			window.draw(background);
			menu->drawDifficultMenu(window);
		}

		if (menu_selected_flag == 3)
		{
			score = pauseScore + scoreClock.getElapsedTime().asSeconds() + loadScore;

			if (clock.getElapsedTime().asMilliseconds() > 15)
			{

				if (enemy->GetPosition().x > window.getSize().x)
				{
					enemyWaiter++;
					if (enemyWaiter == 30)
					{
						if (dy1 < 0)
						{
							dy1 = -dy1;
						}
						enemy->SetPos(rand() % (window.getSize().x - (int)enemy->GetBounds().height), 0);
						enemyWaiter = 0;
					}
				}
				else
				{
					if (enemy->GetPosition().x + enemy->GetSprite().getLocalBounds().width > window.getSize().x || enemy->GetPosition().x < 0)
						dx1 = -dx1;
					if (enemy->GetPosition().y + enemy->GetBounds().height > window.getSize().y - ground.getLocalBounds().height || enemy->GetPosition().y < 0)
						dy1 = -dy1;
					dane->dx = dx1;
					dane->dy = dy1;
					enemy->Move(dx1, dy1);
				}
				if ((Keyboard::isKeyPressed(Keyboard::Key::Left) || Keyboard::isKeyPressed(Keyboard::Key::A)) && !shot)
				{
					player.Move(-playerSpeed, window.getSize().x);
				}
				else if ((Keyboard::isKeyPressed(Keyboard::Key::Right) || Keyboard::isKeyPressed(Keyboard::Key::D)) && !shot)
				{
					player.Move(playerSpeed, window.getSize().x);
				}
				else if ((Keyboard::isKeyPressed(Keyboard::Key::Space)) && (!shot && shotclock.getElapsedTime().asMilliseconds() > 500))
				{
					shotclock.restart();
					player.SetShot(window);
					shot = true;
				}
				if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
				{
					menu_selected_flag = 8;
					lastMenu = 3;
				}

				if (Keyboard::isKeyPressed(Keyboard::Key::F1))
				{
					pauseScore = score;
					menu_selected_flag = 5;
				}
				dane->score = score;
				dane->SetPosition(player.GetPosition());
				dane->enemyPos = enemy->GetPosition();

				fg = fopen("gracze.dat", "w+b");
				fwrite(dane, sizeof(Player), 1, fg);
				fclose(fg);

				clock.restart();
			}

			window.clear(Color(84, 146, 247));
			for (int i = 0; i < 10; i++)
			{
				window.draw(cloud[i].cloud);
			}
			window.draw(enemy->GetSprite());
			if (shot)
			{
				shot = player.Shot(enemy);
				window.draw(player.GetShot());
			}
			window.draw(ground);
			window.draw(player.GetSprite());
			if (player.GetBounds().intersects(enemy->GetSprite().getGlobalBounds()))
			{
				menu_selected_flag = 4;
			}

			menu->drawScore(window, score);
		}

		if (menu_selected_flag == 4)
		{
			window.clear();
			window.draw(background);
			dane->isAlive = false;
			menu->drawEnd(window, score, window.getSize().x, window.getSize().y, text.getString());

			fg = fopen("gracze.dat", "w+b");
			fwrite(dane, sizeof(Player), 1, fg);
			fclose(fg);

			if (Keyboard::isKeyPressed(Keyboard::Key::Space) || Keyboard::isKeyPressed(Keyboard::Key::Escape))
			{
				menu->setSelectedItem();
				menu_selected_flag = 0;
			}
		}

		if (menu_selected_flag == 5)
		{
			window.clear();
			window.draw(background);
			menu->drawPause(window);
			if (Keyboard::isKeyPressed(Keyboard::Key::Q))
			{
				score = 0;
				scoreClock.restart();
				menu_selected_flag = 3;
			}
		}

		if (menu_selected_flag == 6)
		{
			window.clear();
			dane->score = plik[0].score;
			window.draw(background);
			dane->name = plik[0].name;
			dane->isAlive = plik->isAlive;

			menu->drawContinue(window, dane[0]);
			if (Keyboard::isKeyPressed(Keyboard::Key::Y))
			{
				player.SetPosition(plik[0].GetPosition());
				dane->difficult = plik[0].difficult;
				loadScore = plik[0].score;
				enemy = new Enemy();
				enemy->SetPos(plik[0].enemyPos.x, plik[0].enemyPos.y);
				score = 0;
				menu->setSelectedItem();
				menu = new Menu(window.getSize().x, window.getSize().y);
				switch (plik[0].difficult)
				{
				case 1:
					dx1 = plik->dx;
					dy1 = plik->dy;
					break;
				case 2:
					dx1 = plik->dx;
					dy1 = plik->dy;
					break;
				case 3:
					dx1 = plik->dx;
					dy1 = plik->dy;
					break;
				default:
					break;
				}
				scoreClock.restart();
				menu_selected_flag = 3;
			}
			if (Keyboard::isKeyPressed(Keyboard::Key::N))
			{
				dane->isAlive = false;
				menu_selected_flag = 0;
			}

			fg = fopen("gracze.dat", "w+b");
			fwrite(dane, sizeof(Player), 1, fg);
			fclose(fg);
		}
		if (menu_selected_flag == 7)
		{
			window.clear();
			window.draw(background);
			menu->drawPause(window);
			if (Keyboard::isKeyPressed(Keyboard::Key::Q))
			{
				menu_selected_flag = 0;
			}
		}
		if (menu_selected_flag == 8)
		{
			window.clear();
			window.draw(background);
			menu->drawQuit(window);
			if (Keyboard::isKeyPressed(Keyboard::Key::Y))
			{
				return 0;
			}
			else if (Keyboard::isKeyPressed(Keyboard::Key::N))
			{
				menu_selected_flag = lastMenu;
			}
		}
		window.display();
	}

	return 0;
}