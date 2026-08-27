#pragma once 

#include <SDL.h>
#include <cstdio>

namespace Renderer {
    class Window {
        public:
            Window(int width, int height, const char* title);
            ~Window();

            bool initialize_window();
            operator SDL_Window*() const { return window; }

            int GetWidth() const { return m_WIDTH; }
            int GetHeight() const { return m_HEIGHT; }

        private:
            SDL_Window* window = nullptr;
            int m_HEIGHT = 1920;
            int m_WIDTH = 1080;
            const char* m_title = "hello";
    };
}