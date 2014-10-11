#include "Actor/Actor.hpp"
#include "World.hpp"
#include <memory>
#include "Game/World/Actor/Components/Basic/BasicRendererComponent.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <cerrno>
#include <Game/World/Actor/Components/ModelComponent.hpp>
#include <Game/World/Actor/Components/TransformComponent.hpp>
#include <Game/World/Actor/Components/Camera/CameraComponent.hpp>
#include <string.h>
#include "log.hpp"
#include <Game/World/Actor/Components/Camera/CameraLogicComponent.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Game/World/Actor/Components/Background/BackgroundRendererComponent.hpp>

namespace game {

    std::string get_file_contents(const char *filename)
    {
        std::ifstream in(filename, std::ios::in | std::ios::binary);
        if (in)
        {
            std::ostringstream contents;
            contents << in.rdbuf();
            in.close();
            return(contents.str());
        }
        throw(errno);
    }

    void GetShaderCompileLog(GLuint shader)
    {
        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<char> errorLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
            LOG(ERROR)<<&errorLog[0];
        }
    }

    void GetProgramLinkLog(GLuint program)
    {
        int isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if(isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
            LOG(ERROR)<<&infoLog[0];
        }
    }

    World::World() {
        auto vert = get_file_contents("Shader/shader_vert.glsl");
        auto frag = get_file_contents("Shader/shader_frag.glsl");
        const char* vertex_shader = vert.c_str();
        const char* fragment_shader = frag.c_str();

        GLuint vs = glCreateShader (GL_VERTEX_SHADER);
        glShaderSource (vs, 1, &vertex_shader, NULL);
        glCompileShader (vs);
        GetShaderCompileLog(vs);
        GLuint fs = glCreateShader (GL_FRAGMENT_SHADER);
        glShaderSource (fs, 1, &fragment_shader, NULL);
        glCompileShader (fs);
        GetShaderCompileLog(fs);
        GLuint shader_program = glCreateProgram ();
        glAttachShader (shader_program, fs);
        glAttachShader (shader_program, vs);
        glLinkProgram (shader_program);
        GetProgramLinkLog(shader_program);


        _cameraSPtr = std::make_shared<Actor>();
        auto cameraTransformSPtr = std::make_shared<TransformComponent>(_cameraSPtr);
        cameraTransformSPtr->MoveBy(glm::vec3(0, 0.5f, -8));
        _cameraSPtr->AddComponent(cameraTransformSPtr);
        auto cameraSPtr = std::make_shared<CameraComponent>(_cameraSPtr);
        cameraSPtr->Load(shader_program);
        _cameraSPtr->AddComponent(cameraSPtr);
        auto cameraLogicSPtr = std::make_shared<CameraLogicComponent>(_cameraSPtr);
        _cameraSPtr->AddComponent(cameraLogicSPtr);


        _backgroundSPtr = std::make_shared<Actor>();
        auto backgroundRendererSPtr = std::make_shared<BackgroundRendererComponent>(_backgroundSPtr, shader_program);
        _backgroundSPtr->AddComponent(backgroundRendererSPtr);

        _floorSPtr = std::make_shared<Actor>();
        auto floorTransformSPtr = std::make_shared<TransformComponent>(_floorSPtr);
        auto floorModelSPtr = std::make_shared<ModelComponent>(_floorSPtr);
        floorModelSPtr->Load("assets/floor.obj");
        auto floorRendererSPtr = std::make_shared<BasicRendererComponent>(_floorSPtr, shader_program);
        _floorSPtr->AddComponent(floorTransformSPtr);
        _floorSPtr->AddComponent(floorModelSPtr);
        _floorSPtr->AddComponent(floorRendererSPtr);


        _boxSPtr = std::make_shared<Actor>();
        auto boxTransformSPtr = std::make_shared<TransformComponent>(_boxSPtr);
        boxTransformSPtr->MoveBy(glm::vec3(0, 0.5f, 0));
        auto boxModelSPtr = std::make_shared<ModelComponent>(_boxSPtr);
        boxModelSPtr->Load("assets/box.obj");
        auto boxRendererSPtr = std::make_shared<BasicRendererComponent>(_boxSPtr, shader_program);
        _boxSPtr->AddComponent(boxTransformSPtr);
        _boxSPtr->AddComponent(boxModelSPtr);
        _boxSPtr->AddComponent(boxRendererSPtr);


    }

    void World::Render() {
        auto backgroundRenderComponent = std::dynamic_pointer_cast<BackgroundRendererComponent>(_backgroundSPtr->getComponent(ComponentType::RENDER_COMPONENT));
        backgroundRenderComponent->Render();
        auto  renderComponent = std::dynamic_pointer_cast<BasicRendererComponent>(_boxSPtr->getComponent(ComponentType::RENDER_COMPONENT));
        renderComponent->Render();
        renderComponent = std::dynamic_pointer_cast<BasicRendererComponent>(_floorSPtr->getComponent(ComponentType::RENDER_COMPONENT));
        renderComponent->Render();
    }

    void World::Update(double deltaTime) {
        auto cameraLogicComponent = std::dynamic_pointer_cast<CameraLogicComponent>(_cameraSPtr->getComponent(ComponentType::LOGIC_COMPONENT));
        cameraLogicComponent->Update(deltaTime);
    }
}