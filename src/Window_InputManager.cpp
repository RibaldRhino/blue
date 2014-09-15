#include "Game.hpp"
#include "Window.hpp"
#include "OnWindowClosedEvent.hpp"
#include "MouseEvent.hpp"
#include "KeyboardEvent.hpp"

void Window::InputManager::OnWindowClosed(GLFWwindow *glfwWindow)
{
    Window* windowPtr = (Window*) glfwGetWindowUserPointer(glfwWindow);
    windowPtr->getGame()->getEventManager()->TriggerEvent(std::make_shared<OnWindowClosedEvent>());
}

void Window::InputManager::OnWindowFramebufferResized(GLFWwindow *glfwWindow, int width, int height)
{

}

void Window::InputManager::OnWindowResized(GLFWwindow *glfwWindow, int width, int height)
{

}

void Window::InputManager::OnKeyPressed(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods)
{
    Window* windowPtr = (Window*) glfwGetWindowUserPointer(glfwWindow);
    windowPtr->getGame()->getEventManager()->QueueEvent(
            std::make_shared<KeyboardEvent>(key, scancode, action, mods)
    );
}

void Window::InputManager::OnMouseButton(GLFWwindow *glfwWindow, int button, int action, int mods)
{
    Window* windowPtr = (Window*) glfwGetWindowUserPointer(glfwWindow);
    windowPtr->getGame()->getEventManager()->QueueEvent(
            std::make_shared<MouseEvent>(InputManager::xMousePos, InputManager::yMousePos, button, action, mods)
    );
    InputManager::mouseButton = button;
    InputManager::mouseAction = action;
    InputManager::mouseMods = mods;
}

void Window::InputManager::OnCursorPositionChanged(GLFWwindow *glfwWindow, double xPos, double yPos)
{
    Window* windowPtr = (Window*) glfwGetWindowUserPointer(glfwWindow);
    windowPtr->getGame()->getEventManager()->QueueEvent(
            std::make_shared<MouseEvent>(xPos, yPos, InputManager::mouseButton, InputManager::mouseAction, InputManager::mouseMods)
    );
    InputManager::xMousePos = xPos;
    InputManager::yMousePos = yPos;
}
