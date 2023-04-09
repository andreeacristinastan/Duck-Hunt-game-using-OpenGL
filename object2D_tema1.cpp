#include "object2D_tema1.h"
#include "lab_extra/basic_text/basic_text.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"
#include "math.h"

Mesh* object2D_tema1::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float w,
    float h,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(h, 0 , 0), color),
        VertexFormat(corner + glm::vec3(h , w, 0), color),
        VertexFormat(corner + glm::vec3(0, w , 0), color)

    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {

        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}

Mesh* object2D_tema1::CreateCircle(
    const std::string& name,
    GLfloat x,
    GLfloat y,
    GLfloat radius,
    glm::vec3 color,
    bool fill)
{
    int triangleAmount = 20;

    GLfloat twicePi = 2.0f * M_PI;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    for (int i = 0; i <= triangleAmount; i++) {
        vertices.push_back(VertexFormat(glm::vec3(x + (radius * cos(i * twicePi / triangleAmount)), y + (radius * sin(i * twicePi / triangleAmount)), 0), color));
        indices.push_back(i);
    }

    Mesh* circle = new Mesh(name);

    if (fill) {
        // Draw 2 triangles. Add the remaining 2 indices
        circle->SetDrawMode(GL_TRIANGLE_FAN);
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* object2D_tema1::CreateTriangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill) {
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(2*length, 0,0), color),
        VertexFormat(corner + glm::vec3(-length, length, 0), color),
        VertexFormat(corner + glm::vec3(-length, -length, 0), color),
        
    };

    Mesh* triangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2 };

    if (!fill) {
        triangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    triangle->InitFromData(vertices, indices);
    return triangle;
}