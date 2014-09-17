#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdlib.h> // memory management

class Mesh
{
public:
    GLuint vao;
    int pointCount;
    bool LoadMesh(const char* file_name);
};
