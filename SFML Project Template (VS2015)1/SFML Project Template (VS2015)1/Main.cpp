#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <fstream>
//#include <b2World.h>

using namespace sf;
using namespace std;

const float SCALE = 30.f;
const float DEG = 57.29577f;

b2Vec2 Gravity(0.f, 19.8f);


class Hero {
private:
	b2World* pWorld;
	float x, y;
public:
	Texture herotexture;
	Sprite herosprite;
	b2PolygonShape shape;
	b2BodyDef bdef;
	b2FixtureDef fdef;
	b2Body *heroBody;
	bool onGround;
	bool toRight;
	float CurrentFrame;

	Hero (String F, b2World* w) {
		pWorld = w;
		onGround = false;
		toRight = true;
		CurrentFrame = 0;
		herotexture.loadFromFile("hero/" + F);
		herotexture.setSmooth(true);
		herosprite.setTexture(herotexture);
		herosprite.setTextureRect(IntRect(0, 0, 105, 180));
		herosprite.setPosition(x, y);
		herosprite.setOrigin(52.5, 90);
		
		shape.SetAsBox(42.5 / SCALE, 90 / SCALE);		
		bdef.type = b2_dynamicBody;
		fdef.friction = 0.5;
		fdef.shape = &shape;
		fdef.density = 2.2;
		bdef.position.Set(0, 0);
		heroBody = pWorld->CreateBody(&bdef);
		heroBody->CreateFixture(&fdef);
		heroBody->SetUserData("hero");
		heroBody->SetFixedRotation(true); //запрет вращения героя
	}

	b2Vec2 getHeroPosition()
	{ 
		return heroBody->GetPosition();
	}

	b2Vec2 getHeroVel()
	{ 
		return heroBody->GetLinearVelocity();
	}

	void ApplyLinearImpulse(b2Vec2 vec)
	{
		heroBody->ApplyLinearImpulse(vec, heroBody->GetWorldCenter(), 1);
	}

};

class Barrier {
private:
	b2World* pWorld;
	float x, y;
public:
	Texture barrtexture;
	Sprite barrsprite;
	b2PolygonShape shape;
	b2BodyDef bdef;
	b2FixtureDef fdef;
	b2Body *barrBody;

	Barrier (String F, char* str, b2Vec2 pos, b2World* w) {
		pWorld = w;
		barrtexture.loadFromFile("images/" + F);
		barrtexture.setSmooth(true);
		barrsprite.setTexture(barrtexture);
		barrsprite.setTextureRect(IntRect(0, 0, 100, 100));
		barrsprite.setPosition(x, y);
		barrsprite.setOrigin(50, 50);

		shape.SetAsBox(50 / SCALE, 50 / SCALE);
		bdef.type = b2_dynamicBody;
		fdef.friction = 0.5;
		fdef.shape = &shape;
		fdef.density = 2.2;
		bdef.position.Set((pos.x + 50) / SCALE, (pos.y + 50)/ SCALE);
		barrBody = pWorld->CreateBody(&bdef);
		barrBody->CreateFixture(&fdef);
		barrBody->SetUserData(str);
	}

	void ApplyLinearImpulse(b2Vec2 vec)
	{
		barrBody->ApplyLinearImpulse(vec, barrBody->GetWorldCenter(), 1);
	}

};

class Brick {
public:
	Image brick_image;
	Texture brick;
	Sprite s_brick;

	Brick(String F)
	{
		brick_image.loadFromFile("images/" + F);//загружаем файл для карты
		brick.loadFromImage(brick_image);//заряжаем текстуру картинкой
		s_brick.setTexture(brick);//заливаем текстуру спрайтом
	}

	void Draw(float X, float Y, float w, float h) {
		s_brick.setPosition(X, Y);
		s_brick.setTextureRect(IntRect(0, 0, w, h));
	}

};

class Wall {
public:
	b2World* pWorld;
	b2PolygonShape gr;
	b2BodyDef bdef;
	b2FixtureDef fdef;
	b2Body *b_ground;
	Wall (int x, int y, int w, int h, b2World* pw) {
		pWorld = pw;
		gr.SetAsBox(w * 0.5 / SCALE, h * 0.5 / SCALE);
		bdef.position.Set((x + 0.5 * w) / SCALE, (y + 0.5 * h) / SCALE);
		fdef.friction = 1;
		fdef.shape = &gr;
		//fdef.density = 2;
		b_ground = pWorld->CreateBody(&bdef);
		b_ground->CreateFixture(&gr, 1);
	}
};


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

bool startGame() {
	b2World World(Gravity);
	b2World* pointerWorld = &World;
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


	RenderWindow window(sf::VideoMode(1000, 400), "Project");
	window.setFramerateLimit(70); //макс кол-во кадров в секунду
	View view;
	Hero hero("hero_ice_two_floor.png", pointerWorld);
	Brick brick("background_3.png");
	Brick brick2("100.png");
	Brick brick3("ici100.png");
	Clock clock;
	//Barrier barr1("ici100.png", b2Vec2 (100,100));
	Barrier icicle_down("ici100.png", "barr1", b2Vec2(300, 0), pointerWorld);
	Barrier icicle_up("ici100up.png", "barr2", b2Vec2(1200, 1700), pointerWorld);
	////box2d////
	//walls
	Wall(-100, -100, 6200, 100, pointerWorld);
	Wall(-100, 0, 100, 1800, pointerWorld);
	Wall(-100, 1800, 6200, 100, pointerWorld);
	Wall(6000, 0, 100, 1800, pointerWorld);

	for (int i = 0; i < 18; i++)
	{
		for (int j = 0; j < 60; j++)
			if (map_mas[i][j] == 1)
				Wall(j * 100, i * 100, 100, 100, pointerWorld);
			else if (map_mas[i][j] == 2)
				Barrier icicle_down("ici100.png", "barr1", b2Vec2(100 * j, 100 * i), pointerWorld);
			else if (map_mas[i][j] == 3)
				Barrier icicle_up("ici100up.png", "barr2", b2Vec2(100 * j, 100 * i), pointerWorld);
	}

	menu(window);//вызов меню


				 //////main cycle////////
	while (window.isOpen())
	{
		window.setSize(sf::Vector2u(1000, 600));
		float time = clock.getElapsedTime().asMicroseconds(); //дать прошедшее время в микросекундах
		clock.restart(); //перезагружает время
		time = time / 800; //скорость игры

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		b2Vec2 vel = hero.getHeroVel();
		//float angVel = heroBody->GetAngularVelocity();
		//if (Keyboard::isKeyPressed(Keyboard::Tab)) { return 1; }//если таб, то перезагружаем игру
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { window.clear(); return true; }//если эскейп, то перезагружаем игру

		if (Keyboard::isKeyPressed(Keyboard::Left) && hero.onGround)
		{
			if (vel.x > -5)
				hero.ApplyLinearImpulse(b2Vec2(-100, 0));
			hero.toRight = false;
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right) && hero.onGround)
		{
			if (vel.x < 5)
				hero.ApplyLinearImpulse(b2Vec2(100, 0));
			hero.toRight = true;
		}

		else if ((Keyboard::isKeyPressed(Keyboard::Up)) && hero.onGround)
		{
			if (hero.toRight)
				hero.herosprite.setTextureRect(IntRect(105 * 10, 0, 105, 180));
			else
				hero.herosprite.setTextureRect(IntRect(105, 180, 105, 180));
			hero.ApplyLinearImpulse(b2Vec2(0, -1000));
			hero.onGround = false;
		}

		//else if (Keyboard::isKeyPressed(Keyboard::Down)) { hero.dy += 0.1; }
		else if (hero.onGround)
			if (hero.toRight)
				hero.herosprite.setTextureRect(IntRect(105 * 9, 0, 105, 180));
			else
				hero.herosprite.setTextureRect(IntRect(105 * 2, 180, 105, 180));
		else if (vel.y > 0.5)
			if (hero.toRight)
				hero.herosprite.setTextureRect(IntRect(105 * 11, 0, 105, 180));
			else
				hero.herosprite.setTextureRect(IntRect(0, 180, 105, 180));

		if ((vel.x != 0) && hero.onGround)
		{
			if (hero.toRight)
			{
				hero.CurrentFrame += 0.01 * time;
				if (hero.CurrentFrame > 8) hero.CurrentFrame = 1;
				hero.herosprite.setTextureRect(IntRect(105 * int(hero.CurrentFrame), 0, 105, 180));
			}
			else
			{
				hero.CurrentFrame += 0.01 * time;
				if (hero.CurrentFrame > 8) hero.CurrentFrame = 1;
				hero.herosprite.setTextureRect(IntRect(105 * int(hero.CurrentFrame + 2), 180, 105, 180));
			}
		}

		view.setCenter(hero.getHeroPosition().x * SCALE, hero.getHeroPosition().y * SCALE);

		window.setView(view);
		window.clear(Color(12, 106, 89));

		World.Step(1 / 60.f, 8, 3);

		////////// draw map /////////
		//background
		brick.Draw(0, 0, 6000, 1800);
		window.draw(brick.s_brick);
		//walls
		for (int i = -1; i < 61; i++)
		{
			brick2.Draw(100 * i, 1800, 100, 100);
			window.draw(brick2.s_brick);
		}
		for (int i = -1; i < 61; i++)
		{
			brick2.Draw(100 * i, -100, 100, 100);
			window.draw(brick2.s_brick);
		}
		for (int i = 0; i < 18; i++)
		{
			brick2.Draw(-100, 100 * i, 100, 100);
			window.draw(brick2.s_brick);
		}
		for (int i = 0; i < 18; i++)
		{
			brick2.Draw(6000, 100 * i, 100, 100);
			window.draw(brick2.s_brick);
		}
		//map
		for (int i = 0; i < 18; i++)
		{
			for (int j = 0; j < 60; j++)
				if (map_mas[i][j] == 1)
				{
					brick2.Draw(100 * j, 100 * i, 100, 100);
					window.draw(brick2.s_brick);
				}
		}

		brick3.Draw(1000, 0, 100, 100);
		window.draw(brick3.s_brick);

		////draw bodies//// 
		for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext())
		{
			if (it->GetUserData() == "barr1")
			{
				b2Vec2 pos = it->GetPosition();
				float angle = it->GetAngle();
				icicle_down.barrsprite.setPosition(pos.x * SCALE, pos.y * SCALE);
				icicle_down.barrsprite.setRotation(angle * DEG);
				window.draw(icicle_down.barrsprite);
			}
			if (it->GetUserData() == "barr2")
			{
				b2Vec2 pos = it->GetPosition();
				float angle = it->GetAngle();
				icicle_up.barrsprite.setPosition(pos.x * SCALE, pos.y * SCALE);
				icicle_up.barrsprite.setRotation(angle * DEG);
				window.draw(icicle_up.barrsprite);
			}
			if (it->GetUserData() == "hero")
			{
				b2Vec2 pos = it->GetPosition();
				float angle = it->GetAngle();
				hero.herosprite.setPosition(pos.x * SCALE, pos.y * SCALE);
				hero.herosprite.setRotation(angle * DEG);
				window.draw(hero.herosprite);
			}
		}

		///////check if onGround////////
		b2Vec2 heroPos = hero.getHeroPosition();
		b2Vec2 heroPos1 = hero.getHeroPosition();
		b2Vec2 heroPos2 = hero.getHeroPosition();
		hero.onGround = false;
		heroPos.y += 91 / SCALE;
		heroPos1.y += 91 / SCALE;
		heroPos2.y += 91 / SCALE;
		heroPos1.x += 42.5 / SCALE;
		heroPos2.x -= 42.5 / SCALE;
		for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext())
			for (b2Fixture *f = it->GetFixtureList(); f != 0; f = f->GetNext())
				if (f->TestPoint(heroPos) || f->TestPoint(heroPos1) || f->TestPoint(heroPos2))
					hero.onGround = true;

		///////Events////////
		b2Vec2 heroPos4 = hero.getHeroPosition();
		b2Vec2 heroPos5 = hero.getHeroPosition();
		b2Vec2 heroPos6 = hero.getHeroPosition();
		b2Vec2 heroPos7 = hero.getHeroPosition();
		b2Vec2 heroPos8 = hero.getHeroPosition();
		b2Vec2 heroPos9 = hero.getHeroPosition();
		heroPos4.y -= 91 / SCALE;
		heroPos5.y -= 91 / SCALE;
		heroPos4.x += 43.5 / SCALE;
		heroPos5.x -= 43.5 / SCALE;

		heroPos6.y += 70 / SCALE;
		heroPos7.y -= 70 / SCALE;
		heroPos6.x += 43.5 / SCALE;
		heroPos7.x += 43.5 / SCALE;

		heroPos8.y += 70 / SCALE;
		heroPos9.y -= 70 / SCALE;
		heroPos8.x -= 43.5 / SCALE;
		heroPos9.x -= 43.5 / SCALE;

		for (b2Body* it = World.GetBodyList(); it != 0; it = it->GetNext())
			if (it->GetUserData() == "barr2" || it->GetUserData() == "barr1")
				for (b2Fixture *f = it->GetFixtureList(); f != 0; f = f->GetNext())
					if (f->TestPoint(heroPos) || f->TestPoint(heroPos1) || f->TestPoint(heroPos2)
						|| f->TestPoint(heroPos4) || f->TestPoint(heroPos5) || f->TestPoint(heroPos6)
						|| f->TestPoint(heroPos7) || f->TestPoint(heroPos8) || f->TestPoint(heroPos9))
					{
						window.clear();
						return true;
					}
		///////////////////
		window.display();
	}

	return 0;
}

void gameRunning() {//ф-ция перезагружает игру , если это необходимо
	if (startGame()) { gameRunning(); }////если startGame() == true, то вызываем занова ф-цию isGameRunning, которая в свою очередь опять вызывает startGame() 
}

int main()
{
	gameRunning();//запускаем процесс игры
	return 0;
}
