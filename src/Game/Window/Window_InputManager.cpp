#include <log.hpp>
#include <Event/EventManager.hpp>
#include "Window.hpp"
#include "Event/Game/Window/WindowEvents.hpp"

namespace game {


    void Window::InputManager::OnWindowClosed(GLFWwindow *glfwWindow) {
    }

    void Window::InputManager::OnWindowFramebufferResized(GLFWwindow *glfwWindow, int width, int height) {
        glViewport(0, 0, width, height);
        event::EventManager::get().QueueEvent(std::make_shared<event::OnWindowResized>(width, height));
    }

    void Window::InputManager::OnWindowResized(GLFWwindow *glfwWindow, int width, int height) {
    }

    void Window::InputManager::OnKeyPressed(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods) {
        event::IEventDataSPtr eventDataSPtr = nullptr;
        if(key == GLFW_KEY_W || key == GLFW_KEY_UP) {
            if(action == GLFW_PRESS) {
                eventDataSPtr = std::make_shared<event::OnUpPressed>();
            } else if(action == GLFW_RELEASE) {
                eventDataSPtr = std::make_shared<event::OnUpReleased>();
            }
        } else if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT) {
            if(action == GLFW_PRESS) {
                eventDataSPtr = std::make_shared<event::OnLeftPressed>();
            } else if(action == GLFW_RELEASE) {
                eventDataSPtr = std::make_shared<event::OnLeftReleased>();
            }
        } else if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) {
            if(action == GLFW_PRESS) {
                eventDataSPtr = std::make_shared<event::OnRightPressed>();
            } else if(action == GLFW_RELEASE) {
                eventDataSPtr = std::make_shared<event::OnRightReleased>();
            }
        } else if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN) {
            if(action == GLFW_PRESS) {
                eventDataSPtr = std::make_shared<event::OnDownPressed>();
            } else if(action == GLFW_RELEASE) {
                eventDataSPtr = std::make_shared<event::OnDownReleased>();
            }
        } else if (key == GLFW_KEY_ESCAPE) {
            if(action == GLFW_PRESS) {
                glfwSetWindowShouldClose(glfwWindow, 1);
                eventDataSPtr = std::make_shared<event::OnWindowClose>();
            }
        } else if (key == GLFW_KEY_ENTER && (mods & GLFW_MOD_ALT)) {
            if(action == GLFW_PRESS) {
                const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
                auto window_width = mode->width;
                auto window_height = mode->height;
                glfwSetWindowSize(glfwWindow, window_width, window_height);
                return;
            }
        }
        if(eventDataSPtr!= nullptr)
            event::EventManager::get().QueueEvent(eventDataSPtr);
    }

    void Window::InputManager::OnMouseButton(GLFWwindow *glfwWindow, int button, int action, int mods) {
        event::IEventDataSPtr eventDataSPtr = nullptr;

        if(button == GLFW_MOUSE_BUTTON_LEFT) {
            if(action == GLFW_PRESS) {
                eventDataSPtr = std::make_shared<event::OnLMBPressed>();
            } else if(action == GLFW_RELEASE) {
                eventDataSPtr = std::make_shared<event::OnLMBReleased>();
            }
        } else if( button == GLFW_MOUSE_BUTTON_RIGHT) {
            if(action == GLFW_PRESS) {
                eventDataSPtr = std::make_shared<event::OnRMBPressed>();
            } else if(action == GLFW_RELEASE) {
                eventDataSPtr = std::make_shared<event::OnRMBReleased>();
            }
        }

        if(eventDataSPtr!= nullptr)
            event::EventManager::get().QueueEvent(eventDataSPtr);
    }

    void Window::InputManager::OnCursorPositionChanged(GLFWwindow *glfwWindow, double xPos, double yPos) {
        int width, height;
        glfwGetWindowSize(glfwWindow, &width, &height);
        int centerX = width >> 1, centerY = height >> 1;
        event::IEventDataSPtr eventDataSPtr = std::make_shared<event::OnCursorPositionChanged>(glm::vec2((xPos - centerX)/(width), (centerY - yPos)/(height)));
        glfwSetCursorPos(glfwWindow, centerX, centerY);

        event::EventManager::get().QueueEvent(eventDataSPtr);
    }
}