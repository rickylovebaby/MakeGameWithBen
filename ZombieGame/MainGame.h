#pragma once

#include <Bengine/Window.h>
#include <BEngine/GLSLProgram.h>
#include <Bengine/Camera2D.h>
#include <Bengine/InputManager.h>
#include <Bengine/SpriteBatch.h>
#include "Level.h"
#include "Player.h"


class Zombie;


enum class GameState {
	PLAY,EXIT
};


class MainGame
{
public:
    MainGame();
    ~MainGame();

    /// Runs the game
    void run();

private:
    /// Initializes the core systems
	void initLevel(); 

    void initSystems();
    /// Initializes the shaders
    void initShaders();

    /// Main game loop for the program
    void gameLoop();


	//update all agent
	void updateAgents();
    /// Handles input processing
    void processInput();
    /// Renders the game
    void drawGame();

private:
    /// Member Variables
    Bengine::Window _window; ///< The game window
	Bengine::GLSLProgram _textureProgram; ///< The shader program
	Bengine::InputManager _inputManager; ///< Handles input
    Bengine::Camera2D _camera; ///< Main Camera
	Bengine::SpriteBatch _agentSpriteBatch; 

	std::vector<Level*> _levels;


	int _screenHeight, _screenWidth;

	float _fps;
	int _currentLevel;

	Player* _player;
	std::vector<Human*> _humans;
	std::vector<Zombie*> _zombies;

	GameState _gameState;

};

