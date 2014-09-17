#include "World.hpp"
#include "Actor/Process/CarProcess.hpp"
#include "../Game/Game.hpp"
#include "Actor/Renderer/CarRenderer.hpp"
#include "Actor/Process/CameraProcess.hpp"
#include "Actor/Renderer/CameraRenderer.hpp"

World::World()
{
    _processManagerUPtr = std::unique_ptr<ProcessManager>(new ProcessManager());

    _car = std::unique_ptr<Actor>(new Actor(this));
    _car->_meshUPtr->LoadMesh("assets/autocolor3.obj");
    _car->_transformUPtr->Rotate(-M_PI_2, glm::vec3(0, 0, 1));
    _processManagerUPtr->AttachProcess(std::unique_ptr<AbstractProcess>(new CarProcess(_car.get())));
    _camera = std::unique_ptr<Camera>(new Camera(this));
    _camera->_transformUPtr->Translate(glm::vec3(5.f, 0.f, -15.f));
    _processManagerUPtr->AttachProcess(std::unique_ptr<AbstractProcess>(new CameraProcess(_camera.get())));
    _cube = std::unique_ptr<Actor>(new Actor(this));
    _cube->_meshUPtr->LoadMesh("assets/cube.obj");
    _cube->_transformUPtr->Translate(glm::vec3(5.f, 0.f, 5.f));
    //_processManagerUPtr->AttachProcess(std::unique_ptr<AbstractProcess>(new CarProcess(_cube.get())));

    const char* vertex_shader =
            "#version 400\n"
            "uniform mat4 model;"
            "uniform mat4 view, proj;"
            "uniform vec3 La, Ls, Ld;"
            "uniform vec3 Ka, Ks, Kd;"
            "in vec3 vertex_position;"
            "in vec3 vertex_normal;"
            "out vec3 position_eye, normal_eye;"
            "void main () {"
            "position_eye = vec3 (view * model * vec4 (vertex_position, 1.0));"
            "normal_eye = vec3 (view * model * vec4 (vertex_normal, 0.0));"
            "gl_Position = proj * vec4 (position_eye, 1.0);"
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

    CameraRenderer camRenderer(_camera.get());
    camRenderer.Render(_shader_program);
    CarRenderer renderer(_car.get());
    renderer.Render(_shader_program);
    CarRenderer renderer1(_cube.get());
    renderer1.Render(_shader_program);

    Game::getInstance().getWindow()->SwapBuffers();
}

void World::Update(double deltaTime)
{
    _processManagerUPtr->Update(deltaTime);
}
