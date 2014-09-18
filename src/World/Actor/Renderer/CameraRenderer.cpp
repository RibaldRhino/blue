#include <glm/gtc/type_ptr.hpp>
#include "CameraRenderer.hpp"

CameraRenderer::CameraRenderer(Camera *camera) : _camera(camera) {}

void CameraRenderer::Render(GLuint shader_program) {
    int view_mat_location = glGetUniformLocation (shader_program, "view");
    glUseProgram (shader_program);
    glUniformMatrix4fv (view_mat_location, 1, GL_FALSE, glm::value_ptr(_camera->getViewMatrix()));
    int proj_mat_location = glGetUniformLocation (shader_program, "proj");
    glUseProgram (shader_program);
    glUniformMatrix4fv (proj_mat_location, 1, GL_FALSE, glm::value_ptr(_camera->getProjectionMatrix()));
}
