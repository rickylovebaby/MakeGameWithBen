#include "MainGame.h"
#include <iostream>
#include <random>
#include <ctime>
#include <SDL/SDL.h>
#include <Bengine/Bengine.h>
#include <Bengine/Timing.h>
#include "Zombie.h"
#include <Bengine/Errors.h>
#include "Gun.h"

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;
const float PLAYER_SPEED = 5.0f;

MainGame::MainGame() : _screenWidth(1024),
						_screenHeight(768),
						_gameState(GameState::PLAY),
						_fps(0),
						_player(nullptr),
						_numHumassKilled(0),
						_numZombiesKilled(0)
{
    // Empty
}

MainGame::~MainGame() 
{
	for (int i = 0; i < _levels.size(); i++) {
		delete _levels[i];
	}
	for (int i = 0; i < _humans.size(); i++) {
		delete _humans[i];

	}
	for (int i = 0; i < _zombies.size(); i++) {
		delete _zombies[i];
	}

}

void MainGame::run() {
	initSystems();
	initLevel();
	gameLoop();
	 
}

void MainGame::initSystems() {
	Bengine::init();
	_window.create("ZombieGame",_screenWidth,_screenHeight,0);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);

	initShaders();
	_agentSpriteBatch.init();
	_camera.init(_screenWidth, _screenHeight);


}

void MainGame::initLevel() {
	_levels.push_back(new Level("Levels/level1.txt"));
	_currentLevel = 0;
	 
	_player = new Player(); 
	_player->init(PLAYER_SPEED,_levels[_currentLevel]->getStartPlayerPos(),&_inputManager,&_camera,&_bullets); 

	_humans.push_back(_player);

	std::mt19937 randomEngine;
	randomEngine.seed(time(nullptr));

	std::uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth() - 2);
	std::uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight() - 2);


	//add all the random humans
	for (int i = 0; i < _levels[_currentLevel]->getNumHumans();i++) {
		_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
	}
	//Add the zombies
	const std::vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getZombieStartPlayerPos();
	for (int i = 0; i < zombiePositions.size(); i++) {
		_zombies.push_back(new Zombie);
		glm::vec2 pos(randX(randomEngine) * TILE_WIDTH, randY(randomEngine) * TILE_WIDTH);
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	//set up the player guns

	const float BULLET_SPEED = 20.0f;
	_player->addGun(new Gun("Magnum", 10, 1, 5.0f, 30, BULLET_SPEED));

	_player->addGun(new Gun("ShotGun", 30, 12, 20.0f, 4, BULLET_SPEED));

	_player->addGun(new Gun("MPS", 2, 1, 10.0f, 20, BULLET_SPEED));

}

void MainGame::initShaders() {
    // Compile our color shader
	_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}

void MainGame::gameLoop() {

	Bengine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);
	while (_gameState == GameState::PLAY) {
		fpsLimiter.begin();
		checkVictory();

		processInput();
		  
		updateAgents();
		updateBullets();

		_camera.setPosition(_player->getPosition());
		_camera.update();

		drawGame();

		_fps = fpsLimiter.end();
	}
}

void MainGame::updateAgents() {
	//update all player
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->update(_levels[_currentLevel]->getLevelData(),_humans,_zombies);
	}
	//update all zombies
	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies);
	}
	//update  human collision
	for (int i = 0; i < _humans.size(); i++) {
		for (int j = i + 1; j < _humans.size(); j++) {
			_humans[i]->collideWithAgent(_humans[j]);

		}
	}
	//update zombie collision
	for (int i = 0; i < _zombies.size(); i++) {
		for (int j = i + 1; j < _zombies.size(); j++) {
			_zombies[i]->collideWithAgent(_zombies[j]);

		}

		for (int j = 1; j < _humans.size(); j++) {
			if (_zombies[i]->collideWithAgent(_humans[j])) {
				_zombies.push_back(new Zombie);
				_zombies.back()->init(ZOMBIE_SPEED,_humans[j]->getPosition());

				//delete the human
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}

		}
		//collide with player
		if (_zombies[i]->collideWithAgent(_player)) {
			Bengine::fatalError("You Lose!");
		}

	}

	//don't forget to update zombie

}


void  MainGame::updateBullets() {

	//update and collide with world
	for (int i = 0; i < _bullets.size();) {
		//if update returns true, the bullets collided with a wall
		if (_bullets[i].update(_levels[_currentLevel]->getLevelData())) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	bool wasBulletRemove;
	//collide with humans and zombies
	for (int i = 0; i < _bullets.size();i++) {
		wasBulletRemove = false;
		//loop through zombies
		for (int j = 0; j < _zombies.size();) {
			if (_bullets[i].collideWithAgent(_zombies[j])) {
				//Damage zombie,and kill it if its out of health
				if (_zombies[j]->applyDamage(_bullets[i].getDamage())) {
					//if the zombie die , remove it
					delete _zombies[j];
					_zombies[j] = _zombies.back();  
					_zombies.pop_back();
					_numZombiesKilled++;
				}
				else {
					j++;
				}
				//remove the bullets
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				wasBulletRemove = true;
				i--;//make sure we don't skip a bullet
				//since the bullet die, no need to loop through any more zombies
				break;
			}
			else {
				j++;

			}
		}

		//loop through human
		if (wasBulletRemove == false) {
			for (int j = 1; j < _humans.size();) {
				if (_bullets[i].collideWithAgent(_humans[j])) {
					//Damage human,and kill it if its out of health
					if (_humans[j]->applyDamage(_bullets[i].getDamage())) {
						//if the human die , remove it
						delete _humans[j];
						_humans[j] = _humans.back();
						_humans.pop_back();
					}
					else {
						j++;
					}
					//remove the bullets
					_bullets[i] = _bullets.back();
					_bullets.pop_back();
					_numHumassKilled++;

					i--;//make sure we don't skip a bullet
						//since the bullet die, no need to loop through any more human
					break;
				}
				else {
					j++;

				}
			}
		}


	}

}
void MainGame::checkVictory() {
	if (_zombies.empty()) {

		std::printf("*** You win! ***\n  You killed %d humans and %d zombies. There are %d/%d civilians remaining",_numHumassKilled,_numZombiesKilled
		,_humans.size() - 1, _levels[_currentLevel]->getNumHumans());
		Bengine::fatalError("");
	}
}


void MainGame::processInput() {
    SDL_Event evnt;

    //Will keep looping until there are no more events to process
    while (SDL_PollEvent(&evnt)) {
        switch (evnt.type) {
            case SDL_QUIT:
                //Exit the game here!
				_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
                break;
            case SDL_KEYDOWN:
				_inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
				_inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(evnt.button.button);
                break;
        }
    
	}

}

void MainGame::drawGame() {
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_textureProgram.use();

	//Draw code goes here
	glActiveTexture(GL_TEXTURE0);
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("transformationMatrix");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//draw the level
	_levels[_currentLevel]->draw();

	_agentSpriteBatch.begin();
	//draw the humans
	for (int i = 0; i < _humans.size(); i++) {
		_humans[i]->draw(_agentSpriteBatch);
	}	
	for (int i = 0; i < _zombies.size(); i++) {
		_zombies[i]->draw(_agentSpriteBatch);
	}

	//draw the bullets
	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.end();

	_agentSpriteBatch.renderBatch();


	_textureProgram.unuse();
	//Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}