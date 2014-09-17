#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include "CarRenderer.hpp"


void CarRenderer::Render(GLuint shader_program){

    glBindVertexArray (_actor->_meshUPtr->vao);

    int model_mat_location = glGetUniformLocation (shader_program, "model");
    glUseProgram (shader_program);
    glUniformMatrix4fv (model_mat_location, 1, GL_FALSE, glm::value_ptr(_actor->_transformUPtr->getMatrix()));

    glDrawArrays(GL_TRIANGLES, 0, _actor->_meshUPtr->pointCount);
};


CarRenderer::CarRenderer(Actor *actor) {
    _actor = actor;
}
