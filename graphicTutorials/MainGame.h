#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <Bengine/Bengine.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/Sprite.h>
#include <Bengine/GLTexture.h>
#include <Bengine/Window.h>
#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/InputManager.h>
#include <Bengine/Timing.h>
#include <vector>
#include "Bullet.h"
enum class GameState {PALY,EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void processInput();
	void drawGame();

	Bengine::Window  _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;
	

	
	//Sprite _sprite;

	Bengine::GLSLProgram _colorProgram;
	Bengine::Camera2D _camera;

	Bengine::GLTexture _playerTexture;
	Bengine::SpriteBatch _spriteBatch;
	Bengine::InputManager _inputManager;
	Bengine::FpsLimiter _fpsLimiter;



	std::vector<Bullet> _bullets;
	float _maxFPS;
	float _fps;
	float _time;
};

