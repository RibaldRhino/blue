#include "World.hpp"
#include "Actor/Process/CarProcess.hpp"
#include "../Game/Game.hpp"
#include "Actor/Renderer/CarRenderer.hpp"


World::World()
{
    _car = std::unique_ptr<Actor>(new Actor(this));
    _processManagerUPtr = std::unique_ptr<ProcessManager>(new ProcessManager());
    _processManagerUPtr->AttachProcess(std::unique_ptr<AbstractProcess>(new CarProcess(_car.get())));
    _car->_meshUPtr->LoadMesh("assets/cube.obj");



    const char* vertex_shader =
            "#version 400\n"
                    "uniform mat4 model;"
                    "in vec3 vp;"
                    "void main () {"
                    " gl_Position = model * vec4 (vp, 1.0);"
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
    _shader_program = glCreateProgram ();
    glAttachShader (_shader_program, fs);
    glAttachShader (_shader_program, vs);
    glLinkProgram (_shader_program);
}

void World::Render()
{

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram (_shader_program);

    CarRenderer renderer(_car.get());
    renderer.Render(_shader_program);

    Game::getInstance().getWindow()->SwapBuffers();
}

void World::Update(double deltaTime)
{
    _processManagerUPtr->Update(deltaTime);
}
