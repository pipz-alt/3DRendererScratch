#include "Draw.h"
#include "math.h"
#include "Buffer.h"

namespace Renderer{
    void Draw::Draw_Grid()
    {
        for (int y = 0; y < HEIGHT; y += 10) {
            for (int x = 0; x < WIDTH; x += 10) {
                color_buffer[(WIDTH * y) + x] = 0xFF333333;
            }
        }
    }

    void Draw::Draw_Rect(int x, int y, int width, int height, uint32_t color)
    {
        for (int i = 0; i < width; i++){
            for (int j = 0; j < height; j++) {
                int current_x = x + i;
                int current_y = y + j;
                Draw_Pixel(current_x, current_y, color);
            }
        }
    }

    void Draw::Draw_Pixel(int x, int y, uint32_t color)
    {
        if (x >= 0 && x < WIDTH && y >=0 && y < HEIGHT){
            color_buffer[(WIDTH * y) + x] = color;
        }
       
    }

    void Draw::Draw_Line(int x0, int y0, int x1, int y1, uint32_t color)
    {
        int delta_x = (x1 - x0);
        int delta_y = (y1 - y0);

        int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

        float x_inc = delta_x / (float)longest_side_length;
        float y_inc = delta_y / (float)longest_side_length;

        float current_x = x0;
        float current_y = y0;

        for (int i = 0; i <= longest_side_length; i++){
            Draw_Pixel(round(current_x), round(current_y), color);
            current_x += x_inc;
            current_y += y_inc;
        }
    }

    void Draw::Draw_Triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
    {
        Draw_Line(x0, y0, x1, y1,  color);
        Draw_Line(x1, y1, x2, y2, color);
        Draw_Line(x2, y2, x0, y0, color);
    }
}
