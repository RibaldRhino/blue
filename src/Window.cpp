#include <string>
#include "Window.hpp"
#include "Game.hpp"

Window::Window(int width, int height, std::string title, Game* game, bool fullScreen) :
    _gamePtr(game), _inputManagerUPtr(std::unique_ptr<InputManager>(new InputManager()))
{
    glfwSetErrorCallback(OnError);
    glfwInit();

    if(!fullScreen)
        _glfwWindowPtr = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    else
        _glfwWindowPtr = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);

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

void Window::OnError(int errorCode, const char* description)
{

}

Game *Window::getGame() {
    return _gamePtr;
}
