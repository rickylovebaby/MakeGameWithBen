#include "Window.h"
#include "Errors.h"

namespace Bengine {
	Window::Window()
	{
	}


	Window::~Window()
	{
	}

	int Window::create(std::string windowName, int screenWidth, int screenHeight, unsigned int currentFlags) {
		Uint32 flags = SDL_WINDOW_OPENGL;
		if (currentFlags & INVISIBLE) {
			flags |= SDL_WINDOW_HIDDEN;
		}
		if (currentFlags & FULLSCREEN) {
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}
		if (currentFlags & BORDERLESS) {
			flags |= SDL_WINDOW_BORDERLESS;
		}

		_sdlwindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, flags);

		if (_sdlwindow == nullptr) {
			fatalError("SDL window could not be created!");
		}

		//Set up our OpenGL context
		SDL_GLContext glContext = SDL_GL_CreateContext(_sdlwindow);
		if (glContext == nullptr) {
			fatalError("SDL_GL context could not be created!");
		}

		//set up glew(optional but recommended)
		glewExperimental = true;
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			fatalError("Could not initialize glew!");
		}


		//check the opengl version
		std::printf("*** Opengl Version : %s ***", glGetString(GL_VERSION));

		//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

		//set vsync,turn off
		SDL_GL_SetSwapInterval(0);

		//Enable alpha blend
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		return 0;
	}

	void Window::swapBuffer() {
		SDL_GL_SwapWindow(_sdlwindow);
	}
}