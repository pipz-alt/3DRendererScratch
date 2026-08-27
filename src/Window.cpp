#include "Window.h"

namespace Renderer {
    
    Window::Window(int width, int height, const char *title) :  m_WIDTH(width), m_HEIGHT(height), m_title(title)
    {
        initialize_window();
    }
    
    Window::~Window()
    {
        if (window)
        {
            SDL_DestroyWindow(window);
        }
        
    }

    bool Window::initialize_window()
    {
    
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
            return false;
        }   

        SDL_DisplayMode displayMode;
        SDL_GetCurrentDisplayMode(0, &displayMode);

        m_WIDTH = displayMode.w;
        m_HEIGHT = displayMode.h;

        window = SDL_CreateWindow(
            "SDL2",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            m_WIDTH, m_HEIGHT,
            SDL_WINDOW_SHOWN
        );

        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

        if (!window) {
            std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
            SDL_Quit();
            return false;
        }
    
        return true;
    }
}