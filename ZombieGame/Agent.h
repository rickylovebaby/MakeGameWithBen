#pragma once
#include <glm/glm.hpp>
#include <Bengine/SpriteBatch.h>
//#include <Bengine/Vertex.h>

const float AGENT_WIDTH = 60;
class Zombie;
class Human;

struct CollisionTile {
	char tile;

};

class Agent
{
public:
	Agent();
	virtual ~Agent(); 

	virtual void update(const std::vector<std::string>& levelData,
						std::vector<Human*>& humans,
						std::vector<Zombie*>& zombies) = 0;

	void collideWithLevel(const std::vector<std::string>& levelData);

	void draw(Bengine::SpriteBatch& _spriteBatch);

	glm::vec2 getPosition() const { return _position; }
protected:
	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions,
							float x, float y);

	void collideWithTile(glm::vec2 tilePos);

	glm::vec2 _position;
	Bengine::Color _color;
	float _speed;
};

