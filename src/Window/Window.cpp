#include <string>

#include "Window.hpp"

class Game;

Window::Window(int width, int height, std::string title, Game* game, bool fullScreen) :
    _gamePtr(game), _inputManagerUPtr(std::unique_ptr<InputManager>(new InputManager()))
{
    glfwSetErrorCallback(OnError);
    glfwInit();

    if(!fullScreen)
        _glfwWindowPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    else
        _glfwWindowPtr = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);

    glfwMakeContextCurrent(_glfwWindowPtr);

    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glClearDepth(1);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);


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
}


Window::~Window()
{
    glfwTerminate();
}

void Window::OnError(int errorCode, const char* description) {

}

void Window::SwapBuffers() {
    glfwSwapBuffers(_glfwWindowPtr);
}

void Window::PollEvents()
{
    glfwPollEvents();
}
