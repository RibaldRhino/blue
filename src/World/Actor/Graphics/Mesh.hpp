#pragma once

#include <glm/glm.hpp>

class Mesh {

private:
    std::vector<glm::vec3> _vertices;
    std::vector<glm::vec3> _normals;
    std::vector<glm::vec3> _textureCoords;
    std::vector<int> _indices;
public:
    Mesh() {};
    ~Mesh() {};

};