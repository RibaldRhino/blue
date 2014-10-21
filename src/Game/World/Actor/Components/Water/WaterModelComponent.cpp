#include <functional>
#include <random>
#include <time.h>
#include "WaterModelComponent.hpp"

game::WaterModelComponent::WaterModelComponent(int resolution, float width, float height, float depth)
{
    std::mt19937::result_type seed = time(0);
    auto rand = std::bind(std::uniform_real_distribution<float>(-1, 1), std::mt19937(seed));
    //auto rand_y= std::bind(std::uniform_real_distribution<float>(-height/2, height/2), std::mt19937(seed));
    //auto rand_z = std::bind(std::uniform_real_distribution<float>(-depth/2, depth/2), std::mt19937(seed));
    _resolution = resolution;
    points.reserve(resolution*3);
    while(resolution--) {
        points.push_back(rand()*width/2);
        points.push_back(rand()*height/2);
        points.push_back(rand()*depth/2);
        points.push_back(0.05f);
    }
    glGenVertexArrays(1, &vao);
    glBindVertexArray (vao);

    glGenBuffers (1, &vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vbo);
    glBufferData (
            GL_ARRAY_BUFFER,
            points.size() * sizeof (GLfloat),
            &points[0],
            GL_DYNAMIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(12));
}
