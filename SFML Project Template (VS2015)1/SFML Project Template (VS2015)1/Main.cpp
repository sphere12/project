#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <fstream>
//#include <b2World.h>
#include "Menu.h"

using namespace sf;
using namespace std;
//using namespace b2World;

const float SCALE = 30.f;
const float DEG = 57.29577f;

b2Vec2 Gravity(0.f, 19.8f);
//b2Vec2 Gravity(19.8f, 19.8f);
b2World World(Gravity);

class Hero {
private:
	float x, y;
public:
	//float dx, dy, speed;
	//bool dirRight;
	Texture herotexture;
	Sprite herosprite;

	Hero(String F) {
	//	dx = 0;
	//	dy = 0;
	//	speed = 0;
	//	dirRight = true; //
		herotexture.loadFromFile("hero/" + F);
		herotexture.setSmooth(true);
		herosprite.setTexture(herotexture);
		herosprite.setTextureRect(IntRect(0, 0, 105, 180));
		herosprite.setPosition(x, y);
		herosprite.setOrigin(52.5, 90);
	}

	float getHeroCoordinateX()
	{
		return x;
	}
	float getHeroCoordinateY()
	{
		return y;
	}

	//void update(float time)
	//{
	//	//x += dx * time;
	//	//y += dy * time;
	//	herosprite.setPosition(x, y);
	//	//dx = 0;
	//	//dy = 0;
	//	//ограничения передвижения героя за края карты
	//	//if (x < 0) x = 0;
	//	//if (y < -600) y = -600;
	//	//if (y > 420) y = 420;
	//}

};

class Map {
public:
	Image map_image;
	Texture map;
	Sprite s_map;

	Map(String F)
	{
		map_image.loadFromFile("images/" + F);//загружаем файл для карты
		map.loadFromImage(map_image);//заряжаем текстуру картинкой
		s_map.setTexture(map);//заливаем текстуру спрайтом
	}

	void Draw(float X, float Y, float w, float h) {
		s_map.setPosition(X, Y);
		s_map.setTextureRect(IntRect(0, 0, w, h));
	}

};

//class Gravity {
//
//
//};

void setWall(int x, int y, int w, int h)
{
	b2PolygonShape gr;
	gr.SetAsBox(w * 0.5 / SCALE, h * 0.5 / SCALE);

	b2BodyDef bdef;
	bdef.position.Set( (x + 0.5 * w)  / SCALE, (y + 0.5 * h) / SCALE);

	b2FixtureDef fdef;
	fdef.friction = 1;
	fdef.shape = &gr;
	//fdef.density = 2;

	b2Body *b_ground = World.CreateBody (&bdef);
	b_ground->CreateFixture(&gr, 1);
}

void setCircleWall(int x, int y, int r)
{
	b2CircleShape circle;
	circle.m_radius = r / SCALE;

	b2BodyDef bdef;
	bdef.position.Set(x / SCALE, y / SCALE);

	b2Body *b_ground = World.CreateBody(&bdef);
	b_ground->CreateFixture(&circle, 1);
}

//b2Body heroBody = setBody (char* s, double density)
//{
//	b2PolygonShape shape;
//	shape.SetAsBox(105 / SCALE, 180 / SCALE);
//
//	b2BodyDef bdef;
//	bdef.type = b2_dynamicBody;
//
//	bdef.position.Set(100 / SCALE, 0);
//	b2Body *heroBody = World.CreateBody(&bdef);
//	heroBody->CreateFixture(&shape, density);
//	heroBody->SetUserData(s);
//
//	return heroBody;
//}


void menu(RenderWindow & window) {
	Texture menuTexture1, menuTexture2, menuTexture3, aboutTexture, menuBackground;
	menuTexture1.loadFromFile("images/112.png");
	menuTexture2.loadFromFile("images/113.png");
	menuTexture3.loadFromFile("images/114.png");
	aboutTexture.loadFromFile("images/about3.png");
	menuBackground.loadFromFile("images/lake-baikal-ice.jpg");
	Sprite menu1(menuTexture1), menu2(menuTexture2), menu3(menuTexture3), about(aboutTexture), menuBg(menuBackground);
	bool isMenu = 1;
	int menuNum = 0;
	menu1.setPosition(70, 30);
	menu2.setPosition(70, 90);
	menu3.setPosition(70, 150);
	menuBg.setPosition(345, 0);
	//////////////////////////////МЕНЮ///////////////////
	while (isMenu)
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed) {
				window.close();
				isMenu = false;
			}
		}
		menu1.setColor(Color::White);
		menu2.setColor(Color::White);
		menu3.setColor(Color::White);
		menuNum = 0;
		window.clear(Color(129, 181, 221));
		if (IntRect(100, 30, 300, 50).contains(Mouse::getPosition(window))) { menu1.setColor(Color::Blue); menuNum = 1; }
		if (IntRect(100, 90, 300, 50).contains(Mouse::getPosition(window))) { menu2.setColor(Color::Blue); menuNum = 2; }
		if (IntRect(100, 150, 300, 50).contains(Mouse::getPosition(window))) { menu3.setColor(Color::Blue); menuNum = 3; }
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;//если нажали первую кнопку, то выходим из меню 
			if (menuNum == 2) {
				window.draw(about);
				window.display();
				while (!Keyboard::isKeyPressed(Keyboard::Escape)) {
								
				}
			}
			if (menuNum == 3) { window.close(); isMenu = false; }
		}
		window.draw(menuBg);
		window.draw(menu1);
		window.draw(menu2);
		window.draw(menu3);
		window.display();
	}
	////////////////////////////////////////////////////
}



int main()
{
	float Vx = 0, Vy = 0;
	bool onGround = false;
	bool toRight = true;

	int map_mas[18][60];
	//ofstream fout;
	ifstream fin;
	//fout.open("output.txt");
	fin.open("map.txt");
	
	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 60; j++)
			fin >> map_mas[i][j];
	}

	//for (int i = 0; i < 18; i++)
	//{
	//	for (int j = 0; j < 60; j++)
	//		map_mas[i][j] = 0;
	//}
	
	//for (int i = 0; i < 18; i++)
	//{
	//	for (int j = 0; j < 60; j++)
	//		fout << map_mas[i][j] <<" ";
	//	fout << "\n";
	//}


	RenderWindow window(sf::VideoMode(1000, 400), "Project");
	window.setFramerateLimit(70); //макс кол-во кадров в секунду

	View view;

	Hero hero("hero_ice_two_floor.png");
	Map map("background_3.png");
	Map map2("100.png");

	float CurrentFrame = 0;
	Clock clock;

	RectangleShape rectangle (Vector2f(120, 50));
	//sf::RectangleShape rectangle;
	//rectangle.setSize(sf::Vector2f(100, 50));
	//rectangle.setOutlineColor(sf::Color::Red);
	//rectangle.setOutlineThickness(5);
	//rectangle.setPosition(10, 20);

	////box2d////
	//walls
	setWall (-100, -100, 6200, 100);
	setWall (-100, 0, 100, 1800);
	setWall (-100, 1800, 6200, 100);
	setWall (6000, 0, 100, 1800);

	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 60; j++)
			if (map_mas[i][j])
				setWall (j * 100, i * 100, 100, 100);
	}

	//setWall (900, 50, 100, 50);

	//setWall (500, 650, 100, 200);
	//setCircleWall (600, 1200, 700);
	
	//HERO
	//b2Body* heroBody = setBody ("hero", 2);

	b2PolygonShape shape;
	shape.SetAsBox (42.5 / SCALE, 90 / SCALE);
	
	b2BodyDef bdef;
	bdef.type = b2_dynamicBody;

	b2FixtureDef fdef;
	fdef.friction = 0.5;
	fdef.shape = &shape;
	fdef.density = 2.2;

	bdef.position.Set (0, 0);
	b2Body *heroBody = World.CreateBody(&bdef);
	//heroBody->CreateFixture(&shape, 2);
	heroBody->CreateFixture(&fdef);
	heroBody->SetUserData("hero");
	heroBody->SetFixedRotation(true); //запрет вращения героя

	//RenderWindow window(sf::VideoMode(1376, 768), "Kychka-pc.ru 31");
	menu(window);//вызов меню
	window.setSize(sf::Vector2u(1000, 600));

	while (window.isOpen())
	{

		if (hero.getHeroCoordinateX() > 1000)
		{
			b2Vec2 Gravity(19.8f, 19.8f);
			b2World World(Gravity);
		}
		float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 800; //скорость игры

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		b2Vec2 vel = heroBody->GetLinearVelocity();
		//float angVel = heroBody->GetAngularVelocity();

		if (Keyboard::isKeyPressed(Keyboard::Left) && onGround )
		{
			//CurrentFrame += 0.01 * time;
			//if (CurrentFrame > 8) CurrentFrame = 1;
			//hero.herosprite.setTextureRect(IntRect(105 * int(CurrentFrame + 2) , 180, 105, 180));
			if (vel.x > -5)
				heroBody->ApplyLinearImpulse(b2Vec2(-100, 0), heroBody->GetWorldCenter(), 1);
			toRight = false;	
		}
		//if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D))))
		else if (Keyboard::isKeyPressed(Keyboard::Right) && onGround)
		{
			//CurrentFrame += 0.01 * time;
			//if (CurrentFrame > 8) CurrentFrame = 1;
			//hero.herosprite.setTextureRect (IntRect (105 * int (CurrentFrame), 0, 105, 180));
			if (vel.x < 5)
				heroBody->ApplyLinearImpulse (b2Vec2 (100, 0), heroBody->GetWorldCenter (), 1);
			toRight = true;
		}

		else if ((Keyboard::isKeyPressed(Keyboard::Up)) && onGround) 
		{ 			 
			if (toRight) 
				hero.herosprite.setTextureRect (IntRect (105 * 10, 0, 105, 180));			
			else 
				hero.herosprite.setTextureRect (IntRect (105, 180, 105, 180));
			heroBody->ApplyLinearImpulse (b2Vec2 (0, -1000), heroBody->GetWorldCenter (), 1);
			onGround = false;
		}

		//else if (Keyboard::isKeyPressed(Keyboard::Down)) { hero.dy += 0.1; }
		else if (onGround) 
			if (toRight)
				hero.herosprite.setTextureRect (IntRect (105 * 9, 0, 105, 180));
			else 
				hero.herosprite.setTextureRect(IntRect(105 * 2, 180, 105, 180));
		else if (vel.y > 0.5)
			if (toRight) 
				hero.herosprite.setTextureRect (IntRect (105 * 11, 0, 105, 180));
			else 
				hero.herosprite.setTextureRect (IntRect (0, 180, 105, 180)); 
		
		if ( (vel.x != 0) && onGround)
		{
			if (toRight)
			{
				CurrentFrame += 0.01 * time;
				if (CurrentFrame > 8) CurrentFrame = 1;
				hero.herosprite.setTextureRect(IntRect(105 * int(CurrentFrame), 0, 105, 180));
			}
			else
			{
				CurrentFrame += 0.01 * time;
				if (CurrentFrame > 8) CurrentFrame = 1;
				hero.herosprite.setTextureRect(IntRect(105 * int(CurrentFrame + 2), 180, 105, 180));
			}
		}
		
		//hero.update (time);

		//Vx = hero.getHeroCoordinateX();
		//Vy = hero.getHeroCoordinateY();
		b2Vec2 pos = heroBody->GetPosition ();
		Vx = pos.x * SCALE;
		Vy = pos.y * SCALE;

		//ограничения камеры
		//if (Vx < 500) Vx = 500;//убираем из вида левую сторону
		//if (Vy < -200) Vy = -200;//верхнюю сторону
		//if (Vy > 180) Vy = 180;//нижнюю сторону

		view.setCenter (Vx, Vy);

		window.setView (view);
		window.clear (Color (12, 106, 89));

		World.Step (1 / 60.f, 8, 3);

		///////check if onGround////////
		b2Vec2 heroPos = heroBody->GetPosition ();
		b2Vec2 heroPos1 = heroBody->GetPosition ();
		b2Vec2 heroPos2 = heroBody->GetPosition();
		onGround = false;
		heroPos.y += 91 / SCALE;
		heroPos1.y += 91 / SCALE;
		heroPos2.y += 91 / SCALE;
		heroPos1.x += 42.5 / SCALE;
		heroPos2.x -= 42.5 / SCALE;
		for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext())
			for (b2Fixture *f = it->GetFixtureList(); f != 0; f = f->GetNext())
				if (f->TestPoint(heroPos)|| f->TestPoint(heroPos1)|| f->TestPoint(heroPos2)) onGround = true;

		////////// draw map /////////
		//map.s_map.setPosition(0, 0);
		//map.s_map.setTextureRect(IntRect(0, 0, 1024, 600));
		//window.draw(map.s_map);
		map.Draw(0, 0, 6000, 1800);
		window.draw(map.s_map);
		//map.Draw(6000, -1000, 6000, 1800);
		//window.draw(map.s_map);
		for (int i = -1; i < 61; i++)
		{
			map2.Draw (100 * i, 1800, 100, 100);
			window.draw (map2.s_map);
		}
		for (int i = -1; i < 61; i++)
		{
			map2.Draw (100 * i, -100, 100, 100);
			window.draw (map2.s_map);
		}
		for (int i = 0; i < 18; i++)
		{
			map2.Draw (-100, 100 * i, 100, 100);
			window.draw(map2.s_map);
		}
		for (int i = 0; i < 18; i++)
		{
			map2.Draw(6000, 100 * i, 100, 100);
			window.draw(map2.s_map);
		}

		for (int i = 0; i < 18; i++)
		{
			for (int j = 0; j < 60; j++)
				if (map_mas[i][j])
				{
					map2.Draw(100 * j, 100 * i, 100, 100);
					window.draw(map2.s_map);
				}
		}

		//window.draw(hero.herosprite);
		//window.draw(rectangle);
		
		////draw bodies//// 
		for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext())
		{
			if (it->GetUserData() == "hero")
			{
				b2Vec2 pos = it->GetPosition();
				float angle = it->GetAngle();
				hero.herosprite.setPosition (pos.x * SCALE, pos.y * SCALE);
				hero.herosprite.setRotation (angle * DEG);
				window.draw (hero.herosprite);
			}
		}
		///////////////////

		window.display();
	}

	return 0;
}
