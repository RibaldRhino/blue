#include <string>
#include <Event/EventManager.hpp>
#include "log.hpp"
#include "Window.hpp"

namespace gamesystem {

    class Game;

    Window::Window(int width, int height, std::string title, bool fullScreen) :
            _inputManagerUPtr(std::unique_ptr<InputManager>(new InputManager())) {
        glfwSetErrorCallback(OnError);
        glfwInit();
        glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        if (!fullScreen)
            _glfwWindowPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        else
            _glfwWindowPtr = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);

        glfwMakeContextCurrent(_glfwWindowPtr);

        glewExperimental = GL_TRUE;
        glewInit();
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);


        glfwSetWindowUserPointer(_glfwWindowPtr, this);
        glfwSetWindowCloseCallback(_glfwWindowPtr, _inputManagerUPtr->OnWindowClosed);
        glfwSetWindowFocusCallback(_glfwWindowPtr, _inputManagerUPtr->OnWindowFocused);
        glfwSetWindowSizeCallback(_glfwWindowPtr, _inputManagerUPtr->OnWindowResized);
        glfwSetWindowIconifyCallback(_glfwWindowPtr, _inputManagerUPtr->OnWindowIconify);
        glfwSetWindowPosCallback(_glfwWindowPtr, _inputManagerUPtr->OnWindowPositionChanged);
        glfwSetFramebufferSizeCallback(_glfwWindowPtr, _inputManagerUPtr->OnWindowFramebufferResized);
        glfwSetWindowRefreshCallback(_glfwWindowPtr, _inputManagerUPtr->OnWindowRefreshed);
        glfwSetKeyCallback(_glfwWindowPtr, _inputManagerUPtr->OnKeyPressed);
        glfwSetMouseButtonCallback(_glfwWindowPtr, _inputManagerUPtr->OnMouseButton);
        glfwSetScrollCallback(_glfwWindowPtr, _inputManagerUPtr->OnScroll);
        glfwSetCursorEnterCallback(_glfwWindowPtr, _inputManagerUPtr->OnCursorEnter);
        glfwSetCursorPosCallback(_glfwWindowPtr, _inputManagerUPtr->OnCursorPositionChanged);

        _inputManagerUPtr->OnWindowResized(_glfwWindowPtr, width, height);

    }


    Window::~Window() {
        glfwTerminate();
    }

    void Window::OnError(int errorCode, const char *description) {
        LOG(ERROR)<<errorCode<<" "<<description;
    }

    void Window::SwapBuffers() {
        glfwSwapBuffers(_glfwWindowPtr);
    }

    void Window::PollEvents() {
        glfwPollEvents();
    }

    void Window::Clear() {
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}