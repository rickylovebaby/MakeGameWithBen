#include "Agent.h"
#include <Bengine/ResourceManager.h>
#include "Level.h"

Agent::Agent()
{
}


Agent::~Agent()
{
}

void Agent::draw(Bengine::SpriteBatch& _spriteBatch) {

	static int textureID = Bengine::ResourceManager::getTexture("Textures/circle.png").id;

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect;
	destRect.x = _position.x;
	destRect.y = _position.y;
	destRect.z = AGENT_WIDTH;
	destRect.w = AGENT_WIDTH;

	_spriteBatch.draw(destRect,uvRect,textureID,0.0f, _color);
}

void Agent::collideWithLevel(const std::vector<std::string>& levelData) {
	std::vector<glm::vec2> collideTilePositions;

	//check the four corner

	//first corner
	checkTilePosition(levelData, collideTilePositions, _position.x, _position.y);
	//second corner
	checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y);
	//third corner
	checkTilePosition(levelData, collideTilePositions, _position.x, _position.y + AGENT_WIDTH);
	//fourth corner
	checkTilePosition(levelData, collideTilePositions, _position.x + AGENT_WIDTH, _position.y + AGENT_WIDTH);

	//Do the collision
	for (int i = 0; i < collideTilePositions.size(); i++) {
		collideWithTile(collideTilePositions[i]);
	}

}

void Agent::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePositions,float x, float y) {
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH),
		floor(y / (float)TILE_WIDTH)); 


	if (levelData[cornerPos.y][cornerPos.x] != '.') {
		collideTilePositions.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}
}
//AABB collision
void Agent::collideWithTile(glm::vec2 tilePos) {
	const float AGENT_RADIUS = (float)AGENT_WIDTH / 2.0f;
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS; 

	glm::vec2 distVec = _position - tilePos;

	float xDepth = MIN_DISTANCE - distVec.x;
	float yDepth = MIN_DISTANCE - distVec.y;

	float absXDepth = abs(xDepth);
	float absYDepth = abs(yDepth);

	///if this is true,we are colliding
	if (absXDepth > 0 || absYDepth > 0) {
		if (absXDepth < absYDepth) {
			_position += xDepth;

		}
		else {
			_position += yDepth;
		}
	}

}