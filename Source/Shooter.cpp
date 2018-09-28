// Shooter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <SFML\Graphics.hpp>

#include "Player.h"
#include "Renderer.h"
#include "Bullet.h"
#include "Enemy.h"
#include "CollisionManager.h"

#include <iostream>


int main()
{
	sf::RenderWindow window({ 1260,800 },"Shooter");

	sf::Event event;

	CollisionManager collisionManager;

	sf::Texture playerTex;
	playerTex.loadFromFile("player.png");
	sf::Sprite playerSprite;
	playerSprite.setTexture(playerTex);

	sf::Texture pBulletTex;
	pBulletTex.loadFromFile("playerBullet.png");
	sf::Sprite pBulletSprite;
	pBulletSprite.setTexture(pBulletTex);

	BulletDescription playerBulletDesc(pBulletSprite, { 0,0 }, { 0,-800 }, { 0,0 }, CircleHitbox(25, 30, 50), 50);

	Player player(playerSprite, { 550,600 }, 700, 0.15f, playerBulletDesc);

	Renderer renderer(window);

	std::vector<Bullet*> bullets;

	std::vector<Enemy*> enemies;

	std::vector<sf::Vector2f> path = { {0.f,0.f}, {50.f,50.f}, {600.f,100.f} }; //temp
	enemies.push_back(new Enemy(1000, playerSprite, 600, path, 10)); //temp

	while (true) { //Main game loop
		
		while (window.pollEvent(event)){ 
			switch (event.type)
			{
			case sf::Event::Closed: //window is closed
				window.close();
				return 0;

			default:
				break;
			}
		}

		player.Shoot(bullets);

		player.Move(); 
		for (unsigned int i = 0; i < bullets.size(); ++i) { //move bullets
			bullets[i]->Move();
		}
		for (unsigned int i = 0; i < enemies.size(); ++i) { //move enemies
			enemies[i]->Move();
		}

		window.clear(); //clear the window

		//Check collisions
		for (int i = 0; i < bullets.size(); ++i) { //check for collisions between bullets and the player
			collisionManager.CheckCollision(*bullets[i], player);
		}
		for (int i = 0; i < enemies.size(); ++i) { //check for collisions between enemies and the player
			collisionManager.CheckCollision(*enemies[i], player);
		}
		for (int i = 0; i < bullets.size(); ++i) { //check for collisions between bullets and enemies
			for (int ii = 0; ii < enemies.size(); ++ii) { //check for collisions between enemies and the player
				collisionManager.CheckCollision(*bullets[i], *enemies[ii]);
			}
		}

		//delete destroyed objects
		for (std::vector<Bullet*>::iterator it = bullets.begin(); it != bullets.end();) { //delete destroyed bullets
			if ((*it)->IsDestroyed()) {
				delete *it; *it = nullptr;
				it = bullets.erase(it);
			}
			else {
				it++;
			}
		}
		for (std::vector<Enemy*>::iterator it = enemies.begin(); it != enemies.end();) { //delete destroyed enemies
			if ((*it)->IsDestroyed()) {
				delete *it; 
				*it = nullptr;
				it = enemies.erase(it);
				std::cout << "enemy destroyed" << std::endl;
			}
			else {
				it++;
			}
		}

		//render objects
		renderer.Render(player.GetRenderDescription()); //render player
		for (unsigned int i = 0; i < bullets.size(); ++i) { //render bullets
			renderer.Render(bullets[i]->GetRenderDescription());
		}
		for (unsigned int i = 0; i < enemies.size(); ++i) { //render enemies
			renderer.Render(enemies[i]->GetRenderDescription());

		}


		window.display(); //update the display
	}

	
}

