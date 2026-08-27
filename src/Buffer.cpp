 #include "Buffer.h"
 
namespace Renderer{

    void Buffer::ClearColorBuffer(uint32_t color)
    {
        for (int y= 0; y < HEIGHT; y++){
            for(int x = 0; x < WIDTH; x++){
                color_buffer[(WIDTH * y) + x] = color;
            }
        }    
    }

    void Buffer::RenderColorBuffer()
    {
        SDL_UpdateTexture(
            color_buffer_texture, 
            NULL,
            color_buffer,
            (int)(WIDTH*sizeof(uint32_t))
        );

        SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
    }

}
