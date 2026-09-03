#pragma once
#include "Structures.h"
#include "Triangle.h"
#include <vector>

namespace Renderer {

    typedef struct {
        std::vector<vec3_t> vertices;
        std::vector<face_t> faces;
        vec3_t rotation;
        vec3_t scale;
        vec3_t translation;
    } mesh_t;

    extern mesh_t mesh;
}