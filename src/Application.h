#pragma once 
#include <cstdint>
#include <SDL.h>

#include "Structures.h"
#include <vector>
#include "Triangle.h"
#include "Mesh.h"
#include "Draw.h" 

#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

namespace Renderer {

    class Application : public Draw{

        public:

            Application();
            ~Application();

            bool Run();
            void Setup();
            void Render();
            void Update();
            void ProcessInput();

            void load_obj_files_data(char* filename);

            bool running = true;
            int previous_frame_time = 0;

            vec2_t project(vec3_t point);

            static const int N_POINTS = 9 * 9 * 9;
            vec3_t cube_points[N_POINTS];
            vec2_t projected_points[N_POINTS];
            float fov_factor = 640;
            // vec3_t cube_rotation = { .x = 0, .y = 0, .z = 0 };

            vec3_t camera_position = { 0, 0, 0 };

            int WIDTH = 2560;
            int HEIGHT = 1440;

            #define N_CUBE_VERTICES 8 
            
            vec3_t cube_vertices[N_CUBE_VERTICES] = {
                { .x = -1,  .y = -1, .z = -1 },  // 1
                { .x = -1,  .y = 1,  .z = -1 },  // 2
                { .x = 1,   .y = 1,  .z = -1 },  // 3
                { .x = 1,   .y = -1, .z = -1 },  // 4
                { .x = 1,   .y = 1,  .z = 1 },   // 5
                { .x = 1,   .y = -1, .z = 1 },   // 6
                { .x = -1,  .y = 1,  .z = 1 },   // 7
                { .x = -1,  .y = -1, .z = 1 }    // 8   
            };

            #define N_CUBE_FACES (6 * 2)
            face_t cube_faces[N_CUBE_FACES] = {
                // front
                { .a = 1, .b = 2, .c = 3 },
                { .a = 1, .b = 3, .c = 4 },

                // right
                { .a = 4, .b = 3, .c = 5 },
                { .a = 4, .b = 5, .c = 6 },

                // back
                { .a = 6, .b = 5, .c = 7 },
                { .a = 6, .b = 7, .c = 8 },

                // left
                { .a = 8, .b = 7, .c = 2 },
                { .a = 8, .b = 2, .c = 1 },

                
                // top
                { .a = 2, .b = 7, .c = 5 },
                { .a = 2, .b = 5, .c = 3 },

                // bottom
                { .a = 6, .b = 8, .c = 1 },
                { .a = 6, .b = 1, .c = 4 }
            };

            // mesh_t mesh = {
            //     .vertices = NULL,
            //     .faces = NULL,
            //     .rotation = { 0, 0, 0 }

            // }

            triangle_t triangles_to_render[N_CUBE_FACES];
            std::vector<triangle_t> triangles_to_render_x;

            int num_triangles;

            void load_cube_mesh_data();

        };
}