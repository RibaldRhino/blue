#include <functional>
#include <random>
#include <time.h>
#include "WaterModelComponent.hpp"

game::WaterModelComponent::WaterModelComponent(GLuint resolution, float width, float height, float depth) :
        _width(width), _height(height), _depth(depth)
{
    _resolution = resolution;
    points.reserve(resolution*resolution*resolution);
    for(int i=0;i<resolution;i++) {
        for(int j=0;j<resolution;j++) {
            for(int k = 0; k< resolution;k++) {
                points.push_back((i*width)/resolution);
                points.push_back((j*width)/resolution);
                points.push_back((k*width)/resolution);
                points.push_back(0);
                radius.push_back(std::min(width, std::min(height, depth))/resolution/3);
            }
        }
    }
    glGenVertexArrays(1, &vao);
    glBindVertexArray (vao);

    glGenBuffers (1, &position_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, position_vbo);
    glBufferData (
            GL_ARRAY_BUFFER,
            points.size() * sizeof (GLfloat),
            &points[0],
            GL_DYNAMIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    glGenBuffers (1, &radius_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, radius_vbo);
    glBufferData (
            GL_ARRAY_BUFFER,
            radius.size() * sizeof (GLfloat),
            &radius[0],
            GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
}
