#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdlib.h> // memory management

class Mesh
{
public:
    glm::highp_vec3 Ka, Ks, Kd;
    GLuint vao;
    int pointCount;

    Mesh();
    bool LoadMesh(const char* file_name);
};
