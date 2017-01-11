#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
enum class GameState {PLAY,EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();
	
private:
	void initSystems();
	void gameLoop();
	void processInput();


	SDL_Window* _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;


};

