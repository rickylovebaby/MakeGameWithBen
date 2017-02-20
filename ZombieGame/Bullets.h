#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Zombie.h"
#include <Bengine/SpriteBatch.h>

class Human;
class Zombie;
class Agent;

const int BULLET_RADIUS = 5;

class Bullets
{
public:
	Bullets(glm::vec2 position,glm::vec2 direction,float damage,float speed);
	~Bullets();

	//when update return true,delete bullet
	bool update(const std::vector<std::string>& levelData);

	void draw(Bengine::SpriteBatch& spriteBatch); 

	bool collideWithAgent(Agent* agent);

	float getDamage() const { return _damage; }
private:
	bool collideWithWorld(const std::vector<std::string>& levelData);
	
	glm::vec2 _direction;
	glm::vec2 _position; 
	float _damage;
	float _speed;
};

