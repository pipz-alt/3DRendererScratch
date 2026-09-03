#include "Application.h"

#include "Window.h"
#include <math.h>

#include <cstdio>
#include <cstring>
#include <string>
#include "Matrix.h"


namespace Renderer
{
    Application::Application()
    {
    
    }

    bool Application::Run()
    {
        Window window(WIDTH, HEIGHT, "SDL");

        WIDTH = window.GetWidth();
        HEIGHT = window.GetHeight();


        renderer = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (!renderer) {
            std::fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
            return false;
        };

        Setup();

        while (running) {
             ProcessInput();
             Update();
             Render();
        }

        SDL_DestroyRenderer(renderer);   
        return true;
    }   

    Application::~Application()
    {
        free(color_buffer);
        SDL_Quit();                       
    }

    void Application::Setup()
    {

        render_method = RENDER_WIRE;
        cull_method = CULL_BACKFACE;

        color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * HEIGHT * WIDTH);

        color_buffer_texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            WIDTH,
            HEIGHT
        );

        load_cube_mesh_data();
        //load_obj_files_data(ASSETS_DIR "box.obj");

    }

    void Application::Render()
    {
        Draw_Grid();
        
        num_triangles = triangles_to_render_x.size();
        
        for(int i = 0; i < num_triangles; i++){
              triangle_t triangle = triangles_to_render_x[i];
              
            // this just hightlights the vertices in yelloq

            //   Draw_Rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00);
            //   Draw_Rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00);
            //   Draw_Rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00);

             
            if(render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIRE)
            { 
                draw_filled_triangle(
                    *this,
                    triangle.points[0].x,
                    triangle.points[0].y,
                    triangle.points[1].x,
                    triangle.points[1].y,
                    triangle.points[2].x,
                    triangle.points[2].y,
                    triangle.color
              );
            }

            if(render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX || render_method == RENDER_FILL_TRIANGLE_WIRE)
            {
                Draw_Triangle(
                    triangle.points[0].x,
                    triangle.points[0].y,
                    triangle.points[1].x,
                    triangle.points[1].y,
                    triangle.points[2].x,
                    triangle.points[2].y,
                    0xFF00FF00
                );
            }

            // Draw triangle vertex points
            if (render_method == RENDER_WIRE_VERTEX) 
            {
                Draw_Rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFFFF00);
                Draw_Rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFFFFFF00);
                Draw_Rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFFFFFF00);
            }
        }
        // fixed
        //draw_filled_triangle(*this, 300, 100, 50, 400, 500, 700, 0xFF00FF00);

        triangles_to_render_x.clear();
        RenderColorBuffer();
        ClearColorBuffer(0x00000000);
        SDL_RenderPresent(renderer);
    }
    

    void Application::Update()
    {
        int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

        // Only delay execution if we are running too fast
        if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
            SDL_Delay(time_to_wait);
        }

        previous_frame_time = SDL_GetTicks();

        mesh.rotation.x += 0.01;
        mesh.rotation.y += 0.01;
        mesh.rotation.z += 0.01;

        mesh.scale.x += 0.002;
        mesh.translation.x += 0.02;
        mesh.translation.y += 0;
        mesh.translation.z += 0;

        mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
        mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
        mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
        mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);
        mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
    
        int num_faces = mesh.faces.size();
        // Loop all triangle faces of our mesh
        for (int i = 0; i < num_faces; i++) {
            face_t mesh_face = mesh.faces[i];

            vec3_t face_vertices[3];
            face_vertices[0] = mesh.vertices[mesh_face.a - 1];
            face_vertices[1] = mesh.vertices[mesh_face.b - 1];
            face_vertices[2] = mesh.vertices[mesh_face.c - 1];

            vec4_t transformed_vertices[3];

            // Loop all three vertices of this current face and apply transformations
            for (int j = 0; j < 3; j++) {
                    vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

                    transformed_vertex = mat4_mul_vec4(scale_matrix, transformed_vertex);
                    transformed_vertex = mat4_mul_vec4(translation_matrix, transformed_vertex);
                    transformed_vertex = mat4_mul_vec4(rotation_matrix_x, transformed_vertex);
                    transformed_vertex = mat4_mul_vec4(rotation_matrix_y, transformed_vertex);  
                    transformed_vertex = mat4_mul_vec4(rotation_matrix_z, transformed_vertex);

                    // transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
                    // transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
                    // transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

                    transformed_vertex.z += 5;

                    // save transformed vertex in the array of transformed vertices
                    transformed_vertices[j] = transformed_vertex;
            }

            // Backface culling
            if (cull_method == CULL_BACKFACE){
                vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]);
                vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]);
                vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]);

                vec3_t vector_ab = vec3_sub(vector_b, vector_a);
                vec3_t vector_ac = vec3_sub(vector_c, vector_a);

                vec3_normalize(&vector_ab);
                vec3_normalize(&vector_ac);

                // compute face normal
                vec3_t normal = vec3_cross(vector_ab, vector_ac);

                // normalize face normal
                vec3_normalize(&normal);

                vec3_t camera_ray = vec3_sub(camera_position, vector_a);

                float dot_normal_camera = vec3_dot(camera_ray, normal);

                if (dot_normal_camera < 0){
                    continue;
                }
            }

            vec2_t projected_points[3];
            triangle_t projected_triangle;

                for (int j = 0; j < 3; j++) {

                        projected_points[j] = project(vec3_from_vec4(transformed_vertices[j]));

                        projected_points[j].x += (WIDTH / 2);
                        projected_points[j].y += (HEIGHT/ 2);
                }

                float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0f;

                projected_triangle = {
                    .points = {
                        {projected_points[0].x, projected_points[0].y},
                        {projected_points[1].x, projected_points[1].y},
                        {projected_points[2].x, projected_points[2].y},
                    },
                    .color = mesh_face.color,
                    .avg_depth = avg_depth
                };

                triangles_to_render_x.push_back(projected_triangle);
        }

        int num_triangles = triangles_to_render_x.size();
        for (int i = 0; i < num_triangles; i++) {
            for (int j = 0; j < num_triangles - 1; j++) {
                if (triangles_to_render_x[j].avg_depth < triangles_to_render_x[j + 1].avg_depth) {
                    triangle_t temp_triangle = triangles_to_render_x[j];
                    triangles_to_render_x[j] = triangles_to_render_x[j + 1];
                    triangles_to_render_x[j + 1] = temp_triangle;
                }
            }
        }

        // for (int i)

        /*
        for (int i = 0; i <N_POINTS; i++)
        {
            vec3_t point = cube_points[i];

            vec3_t transform_point = vec3_rotate_y(point, cube_rotation.y);
            transform_point = vec3_rotate_x(transform_point, cube_rotation.z);

            transform_point.z -= camera_position.z;

            vec2_t projected_point = project(transform_point);

            projected_points[i] = projected_point;
        }*/
    }
        


    void Application::ProcessInput()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        case SDL_QUIT:
                            running = false;
                            break;
                        case SDL_KEYDOWN:
                            if (event.key.keysym.sym == SDLK_ESCAPE)
                                running = false;
                            if (event.key.keysym.sym == SDLK_1)
                                render_method = RENDER_WIRE_VERTEX;
                            if (event.key.keysym.sym == SDLK_2)
                                render_method = RENDER_WIRE;
                            if (event.key.keysym.sym == SDLK_3)
                                render_method = RENDER_FILL_TRIANGLE;
                            if (event.key.keysym.sym == SDLK_4)
                                render_method = RENDER_FILL_TRIANGLE_WIRE;
                            if (event.key.keysym.sym == SDLK_c)
                                cull_method = CULL_BACKFACE;
                            if (event.key.keysym.sym == SDLK_d)
                                cull_method = CULL_NONE;
                            break;
                    }
                }
    }

    void Application::load_obj_files_data(char* filename)
    {
        FILE* file = nullptr;
        fopen_s(&file, filename, "r");

        char line[1024];

        while (fgets(line, 1024, file)){
            // vertex information
            if (strncmp(line, "v ", 2) == 0) {
                vec3_t vertex;
                sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                mesh.vertices.push_back(vertex);
            }

            if (strncmp(line, "f ", 2) == 0) {
                int vertex_indices[3];
                int texture_indices[3];
                int normal_indices[3];

                sscanf(
                    line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                    &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                    &vertex_indices[2], &texture_indices[2], &normal_indices[2]
                );

                face_t face = {
                    .a = vertex_indices[0],
                    .b = vertex_indices[1],
                    .c = vertex_indices[2]
                };

                mesh.faces.push_back(face);
            }

        }
    }

    vec2_t Application::project(vec3_t point)
    {
        vec2_t projected_point = {
            .x = (fov_factor * point.x) / point.z,
            .y = (fov_factor * point.y) / point.z
        };

        return projected_point;
    }

    void Application::load_cube_mesh_data()
    {
        for (int i = 0; i < N_CUBE_VERTICES; i++) {
            vec3_t cube_vertex = cube_vertices[i];
            mesh.vertices.push_back(cube_vertex);
        }

        for (int i = 0; i < N_CUBE_FACES; i++){
            face_t cube_face = cube_faces[i];
            mesh.faces.push_back(cube_face);
        }

    }
}