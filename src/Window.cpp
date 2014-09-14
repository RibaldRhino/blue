#include <string>
#include "Window.hpp"

Window::Window(int width, int height, std::string title, bool fullScreen)
{
    glfwSetErrorCallback(OnError);
    glfwInit();

    if(!fullScreen)
        _glfwWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    else
        _glfwWindow = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);

    glfwSetWindowUserPointer(_glfwWindow, this);


    glfwSetWindowCloseCallback(_glfwWindow, _inputManager->OnWindowClosed);
    glfwSetWindowFocusCallback(_glfwWindow, _inputManager->OnWindowFocused);
    glfwSetWindowSizeCallback(_glfwWindow, _inputManager->OnWindowResized);
    glfwSetWindowIconifyCallback(_glfwWindow, _inputManager->OnWindowIconify);
    glfwSetWindowPosCallback(_glfwWindow, _inputManager->OnWindowPositionChanged);
    glfwSetFramebufferSizeCallback(_glfwWindow, _inputManager->OnWindowFramebufferResized);
    glfwSetWindowRefreshCallback(_glfwWindow, _inputManager->OnWindowRefreshed);
    glfwSetKeyCallback(_glfwWindow, _inputManager->OnKeyPressed);
    glfwSetMouseButtonCallback(_glfwWindow, _inputManager->OnMouseButton);
    glfwSetScrollCallback(_glfwWindow, _inputManager->OnScroll);
    glfwSetCursorEnterCallback(_glfwWindow, _inputManager->OnCursorEnter);
    glfwSetCursorPosCallback(_glfwWindow, _inputManager->OnCursorPositionChanged);
}


Window::~Window()
{
    glfwTerminate();
}

void Window::OnError(int errorCode, const char* description)
{

}
