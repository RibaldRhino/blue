#include <string>
#include <cuda_runtime_api.h>
#include "window.hpp"

Window::Window(int width, int height, std::string title, bool fullscreen)
{
    glfwSetErrorCallback(OnError);
    glfwInit();

    if(!fullscreen)
        glfw_window_ = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    else
        glfw_window_ = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);

    glfwSetWindowUserPointer(glfw_window_, this);


    glfwSetWindowCloseCallback(glfw_window_, input_manager_->OnWindowClosed);
    glfwSetWindowFocusCallback(glfw_window_, input_manager_->OnWindowFocused);
    glfwSetWindowSizeCallback(glfw_window_, input_manager_->OnWindowResized);
    glfwSetWindowIconifyCallback(glfw_window_, input_manager_->OnWindowIconify);
    glfwSetWindowPosCallback(glfw_window_, input_manager_->OnWindowPositionChanged);
    glfwSetFramebufferSizeCallback(glfw_window_, input_manager_->OnWindowFramebufferResized);
    glfwSetWindowRefreshCallback(glfw_window_, input_manager_->OnWindowRefreshed);
    glfwSetKeyCallback(glfw_window_, input_manager_->OnKeyPressed);
    glfwSetMouseButtonCallback(glfw_window_, input_manager_->OnMouseButton);
    glfwSetScrollCallback(glfw_window_, input_manager_->OnScroll);
    glfwSetCursorEnterCallback(glfw_window_, input_manager_->OnCursorEnter);
    glfwSetCursorPosCallback(glfw_window_, input_manager_->OnCursorPositionChanged);
}


Window::~Window()
{
    glfwTerminate();
}

void Window::OnError(int errorcode, const char* description)
{

}
