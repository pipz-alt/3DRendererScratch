#pragma once 

#include "Buffer.h"

namespace Renderer{
    class Draw : public Buffer{
        public:
            void Draw_Grid();
            void Draw_Rect(int x, int y, int width, int height, uint32_t color);
            void Draw_Pixel(int x, int y, uint32_t color);
            void Draw_Line(int x0, int y0, int x1, int y1, uint32_t color);
            void Draw_Triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
    };
}