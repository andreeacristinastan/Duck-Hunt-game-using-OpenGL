#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D_tema1
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float w, float h, glm::vec3 color, bool fill = false);
    Mesh* CreateCircle(const std::string& name, GLfloat x, GLfloat y, GLfloat radius, glm::vec3 color, bool fill);
    Mesh* CreateTriangle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
}
