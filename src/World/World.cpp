#include "World.hpp"
#include "Actor/Process/CarProcess.hpp"
#include "../Game/Game.hpp"
#include "Actor/Renderer/CarRenderer.hpp"
#include "Actor/Process/CameraProcess.hpp"
#include "Actor/Renderer/CameraRenderer.hpp"

World::World()
{
    glClearColor(0.1f, 0.1f, 0.15f, 0.0f);
    _processManagerUPtr = std::unique_ptr<ProcessManager>(new ProcessManager());


    _road = std::unique_ptr<Actor>(new Actor(this));
    _road->_meshUPtr->LoadMesh("assets/road.obj");
    _road->_transformUPtr->Scale(glm::vec3(3, 1, 10));
    _road->_transformUPtr->Rotate(M_PI_2, glm::vec3(0, 1, 0));
    _road->_meshUPtr->Ka = glm::vec3(0.2f, 0.2f, 0.2f);
    _road->_meshUPtr->Ks = glm::vec3(0.1f, 0.1f, 0.1f);
    _road->_meshUPtr->Kd = glm::vec3(0.0f, 0.0f, 0.0f);
    //_cube->_transformUPtr->Translate(glm::vec3(8.f, -3.f, -8.f));
    //_cube->_transformUPtr->Rotate(M_PI, glm::vec3(0.f, 1.f, 0.f));
    _car = std::unique_ptr<Actor>(new Actor(this));
    _car->_meshUPtr->LoadMesh("assets/autocolor4.obj");

    _car->_transformUPtr->Rotate(M_PI, glm::vec3(0, 1, 0));

    _car->_meshUPtr->Ka = glm::vec3(0.3f, 0.3f, 0.3f);
    _car->_meshUPtr->Ks = glm::vec3(1.0f, 1.0f, 1.0f);
    _car->_meshUPtr->Kd = glm::vec3(1.0f, 0.0f, 0.0f);
    _processManagerUPtr->AttachProcess(std::unique_ptr<AbstractProcess>(new CarProcess(_car.get())));
    _camera = std::unique_ptr<Camera>(new Camera(this));
    _camera = std::unique_ptr<Camera>(new Camera(this));
    _processManagerUPtr->AttachProcess(std::unique_ptr<AbstractProcess>(new CameraProcess(_camera.get())));
    _cube = std::unique_ptr<Actor>(new Actor(this));
    _cube->_meshUPtr->LoadMesh("assets/lamp1.obj");
    _cube->_transformUPtr->Translate(glm::vec3(8.f, -5.f, -10.f));
    _cube->_transformUPtr->Rotate(M_PI, glm::vec3(0.f, 1.f, 0.f));
    const char* vertex_shader =
            "#version 400\n"
            "uniform vec3 Ks, Kd, Ka;"
            "in layout (location = 0) vec3 vertex_position;"
            "in layout (location = 1) vec3 vertex_normal;"
            "uniform mat4 model;"
            "uniform mat4 view, proj;"
            "out vec3 position_eye, normal_eye;"
            "void main () {"
            "position_eye = vec3 (view * model * vec4 (vertex_position, 1.0));"
            "normal_eye = vec3 (view * model * vec4 (vertex_normal, 0.0));"
            "gl_Position = proj * vec4 (position_eye, 1.0);"
            "}";

    const char* fragment_shader =
            "#version 400\n"
            "uniform mat4 view, proj;"
            "uniform vec3 Ks, Kd, Ka;"
            "vec3 light_position_world = vec3 (0.0, 5.0, -15.0);"
            "vec3 Ls = vec3 (1.0, 1.0, 1.0);" // white specular colour
            "vec3 Ld = vec3 (0.7, 0.7, 0.7);" // dull white diffuse light colour
            "vec3 La = vec3 (0.2, 0.2, 0.2);" // grey ambient colour
            "float specular_exponent = 100.0;"
            "in vec3 position_eye;"
            "in vec3 normal_eye;"
            "out vec4 fragment_colour;"
            "void main () {"

            "vec3 Ia = La * Ka;"

            "vec3 light_position_eye = vec3 (view * vec4 (light_position_world, 1.0));"
            "vec3 distance_to_light_eye = light_position_eye - position_eye;"
            "vec3 direction_to_light_eye = normalize (distance_to_light_eye);"
            "float dot_prod = dot (direction_to_light_eye, normal_eye);"
            "dot_prod = max (dot_prod, 0.0);"
            "vec3 Id = Ld * Kd * dot_prod;"

            "vec3 reflection_eye = reflect (-direction_to_light_eye, normal_eye);"
            "vec3 surface_to_viewer_eye = normalize (-position_eye);"
            "float dot_prod_specular = dot (reflection_eye, surface_to_viewer_eye);"
            "dot_prod_specular = max (dot_prod_specular, 0.0);"
            "float specular_factor = pow (dot_prod_specular, specular_exponent);"
            "vec3 Is = Ls * Ks * specular_factor;"
            "fragment_colour = vec4 (Is + Id + Ia, 1.0);"

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
    CarRenderer renderer2(_road.get());
    renderer2.Render(_shader_program);
    Game::getInstance().getWindow()->SwapBuffers();
}

void World::Update(double deltaTime)
{
    _processManagerUPtr->Update(deltaTime);
}
