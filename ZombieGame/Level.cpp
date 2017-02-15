#include "Level.h"
#include <fstream>
#include <iostream>
#include <Bengine/Errors.h>
#include <Bengine/ResourceManager.h>

Level::Level(const std::string& fileName)
{

	std::ifstream file;
	file.open(fileName);
	if (file.fail()) {
		Bengine::fatalError("Failed to open" + fileName);
	}

	std::string tmp;
	file >> tmp >> _numHumans;
	while (std::getline(file, tmp)) {
		_levelData.push_back(tmp);
	}
	
	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	Bengine::Color whiteColor;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;
	for (int y = 0; y < _levelData.size(); y++) {
		for (int x = 0; x < _levelData[y].size(); x++) {

			char tile = _levelData[y][x];

			//get the dest rect
			glm::vec4 destRect(x * TILE_WIDTH, y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			switch (tile) {
			case 'B':
			case 'R':
				_spriteBatch.draw(destRect, uvRect, 
					Bengine::ResourceManager::getTexture("Textures/red_bricks.png").id,0.0f, whiteColor);
				break;
			case 'G':
				_spriteBatch.draw(destRect, uvRect,
					Bengine::ResourceManager::getTexture("Textures/glass.png").id, 0.0f, whiteColor);
				break;
			case 'L':
				_spriteBatch.draw(destRect, uvRect,
					Bengine::ResourceManager::getTexture("Textures/light_bricks.png").id, 0.0f, whiteColor);
				break;
			case '@':
				_levelData[y][x] = '.';// so we don't collide with a @
				_startPlayerPos.x = x * TILE_WIDTH;
				_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				_levelData[y][x] = '.';// so we don't collide with a Z
				_zombieStartPosition.emplace_back(x * TILE_WIDTH, y * TILE_WIDTH);
				break;
			case '.':
				break;
			default:
				std::printf("Unexpected symbol %c at (%d,%d)",tile,x,y);
				break;
			}
		}
	}

	_spriteBatch.end();

}

void Level::draw(){
	_spriteBatch.renderBatch();
}

Level::~Level()
{
}
