#pragma once 
#include <cstdint>
#include "SDL.h"

namespace Renderer{
    class Buffer {
        public:

            int WIDTH = 2560;
            int HEIGHT = 1440;

            uint32_t* color_buffer = NULL;
            SDL_Renderer* renderer = NULL;
            SDL_Texture* color_buffer_texture = NULL;

            void ClearColorBuffer(uint32_t color);
            void RenderColorBuffer();
    };
    
}

