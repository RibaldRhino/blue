#include "World.hpp"
#include "Actor/Process/CarProcess.hpp"
#include "../Game/Game.hpp"
#include "Actor/Renderer/CarRenderer.hpp"


World::World()
{
    _car = std::unique_ptr<Actor>(new Actor(this));
    _processManagerUPtr = std::unique_ptr<ProcessManager>(new ProcessManager());
    _processManagerUPtr->AttachProcess(std::unique_ptr<AbstractProcess>(new CarProcess(_car.get())));
}

void World::Render()
{
    const char* vertex_shader =
    "#version 400\n"
    "in vec3 vp;"
    "void main () {"
    " gl_Position = vec4 (vp, 1.0);"
    "}";

    const char* fragment_shader =
    "#version 400\n"
    "out vec4 frag_colour;"
    "void main () {"
    " frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
    "}";

    GLuint vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vertex_shader, NULL);
    glCompileShader (vs);
    GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &fragment_shader, NULL);
    glCompileShader (fs);
    GLuint shader_program = glCreateProgram ();
    glAttachShader (shader_program, fs);
    glAttachShader (shader_program, vs);
    glLinkProgram (shader_program);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram (shader_program);

    CarRenderer renderer;
    renderer.Render();

    Game::getInstance().getWindow()->SwapBuffers();
}

void World::Update(double deltaTime)
{
    _processManagerUPtr->Update(deltaTime);
}
