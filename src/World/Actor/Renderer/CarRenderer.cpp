#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include "CarRenderer.hpp"


void CarRenderer::Render(GLuint shader_program){

    glBindVertexArray (_actor->_meshUPtr->vao);
    int model_mat_location = glGetUniformLocation (shader_program, "model");
    glUniformMatrix4fv (model_mat_location, 1, GL_FALSE, glm::value_ptr(_actor->_transformUPtr->getMatrix()));
    int ks = glGetUniformLocation (shader_program, "Ks");
    glUniform3fv(ks, 1, glm::value_ptr(_actor->_meshUPtr->Ks));
    int kd = glGetUniformLocation (shader_program, "Kd");
    glUniform3fv(kd, 1, glm::value_ptr(_actor->_meshUPtr->Kd));
    int ka = glGetUniformLocation (shader_program, "Ka");
    glUniform3fv(ka, 1, glm::value_ptr(_actor->_meshUPtr->Ka));
    glDrawArrays(GL_TRIANGLES, 0, _actor->_meshUPtr->pointCount);
};


CarRenderer::CarRenderer(Actor *actor) {
    _actor = actor;
}
