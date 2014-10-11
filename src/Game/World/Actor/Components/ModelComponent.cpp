#include <log.hpp>
#include <GL/glew.h>
#include "ModelComponent.hpp"
#include <assimp/cimport.h>        // Plain-C interface
#include <assimp/scene.h>          // Output data structure
#include <assimp/postprocess.h>    // Post processing flags

void game::ModelComponent::Load(const std::string& path) {
    const aiScene* scene = aiImportFile( path.c_str(),
                    aiProcess_Triangulate);

    if(!scene)
    {
        LOG(ERROR) << "Couldn't load: "<< path;
        return;
    }
    LOG(INFO)<<"Loading: "<<path;
    LOG(INFO)<<scene->mNumAnimations<<" animations.";
    LOG(INFO)<<scene->mNumCameras<<" cameras.";
    LOG(INFO)<<scene->mNumLights<<" lights.";
    LOG(INFO)<<scene->mNumMaterials<<" materials.";
    LOG(INFO)<<scene->mNumMeshes<<" meshes.";
    LOG(INFO)<<scene->mNumTextures<<" textures.";

    LOG(INFO)<<"Loading mesh";
    const aiMesh* mesh = scene->mMeshes[0];
    LOG(INFO)<<mesh->mNumVertices<<" vertices.";

    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray (vertexArrayObject);


    auto verticesCount = mesh->mNumVertices;
    if (mesh->HasPositions ()) {
        vertices.resize(verticesCount * 3);
        for (int i = 0; i < verticesCount; i++) {
            const aiVector3D* vp = &(mesh->mVertices[i]);
            vertices[i * 3] = (GLfloat)vp->x;
            vertices[i * 3 + 1] = (GLfloat)vp->y;
            vertices[i * 3 + 2] = (GLfloat)vp->z;
        }
    }
    if (mesh->HasNormals ()) {
        normals.resize(verticesCount * 3);
        for (int i = 0; i < verticesCount; i++) {
            const aiVector3D* vn = &(mesh->mNormals[i]);
            normals[i * 3] = (GLfloat)vn->x;
            normals[i * 3 + 1] = (GLfloat)vn->y;
            normals[i * 3 + 2] = (GLfloat)vn->z;
        }
    }
    if (mesh->HasTextureCoords (0)) {
        textureCoords.resize(verticesCount * 2);
        for (int i = 0; i < verticesCount; i++) {
            const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
            textureCoords[i * 2] = (GLfloat)vt->x;
            textureCoords[i * 2 + 1] = (GLfloat)vt->y;
        }
    }

    if (mesh->HasPositions ()) {

        glGenBuffers (1, &vertexBufferObject);
        glBindBuffer (GL_ARRAY_BUFFER, vertexBufferObject);
        glBufferData (
                GL_ARRAY_BUFFER,
                vertices.size() * sizeof (GLfloat),
                &vertices[0],
                GL_STATIC_DRAW
        );
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (0);
    }
    if (mesh->HasNormals ()) {
        glGenBuffers (1, &normalBufferObject);
        glBindBuffer (GL_ARRAY_BUFFER, normalBufferObject);
        glBufferData (
                GL_ARRAY_BUFFER,
                normals.size() * sizeof (GLfloat),
                &normals[0],
                GL_STATIC_DRAW
        );
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (1);
    }
    if (mesh->HasTextureCoords (0)) {
        glGenBuffers (1, &textureCoordsBufferObject);
        glBindBuffer (GL_ARRAY_BUFFER, textureCoordsBufferObject);
        glBufferData (
                GL_ARRAY_BUFFER,
                textureCoords.size() * sizeof (GLfloat),
                &textureCoords[0],
                GL_STATIC_DRAW
        );
        glVertexAttribPointer (2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray (2);
    }
    LOG(INFO)<<"Mesh loaded without errors";
    aiReleaseImport(scene);
}

game::ModelComponent::ModelComponent(ActorWPtr actorWPtr) {
    _actorWPtr = actorWPtr;

}
