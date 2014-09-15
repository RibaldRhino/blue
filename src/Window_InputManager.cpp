#include "Game.hpp"
#include "Window.hpp"
#include "Events/OnWindowClosedEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Events/KeyboardEvent.hpp"

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
    InputManager& inputManager = *windowPtr->getInputManager();
    windowPtr->getGame()->getEventManager()->QueueEvent(
            std::make_shared<MouseEvent>(inputManager.xMousePos, inputManager.yMousePos, button, action, mods)
    );
    inputManager.mouseButton = button;
    inputManager.mouseAction = action;
    inputManager.mouseMods = mods;
}

void Window::InputManager::OnCursorPositionChanged(GLFWwindow *glfwWindow, double xPos, double yPos)
{
    Window* windowPtr = (Window*) glfwGetWindowUserPointer(glfwWindow);
    InputManager& inputManager = *windowPtr->getInputManager();
    windowPtr->getGame()->getEventManager()->QueueEvent(
            std::make_shared<MouseEvent>(xPos, yPos, inputManager.mouseButton, inputManager.mouseAction, inputManager.mouseMods)
    );
    inputManager.xMousePos = xPos;
    inputManager.yMousePos = yPos;
}
