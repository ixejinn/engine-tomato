#include "tomato/services/WindowService.h"
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace tomato
{
    WindowService::WindowService(int width, int height, const char* title)
            : width_(width), height_(height)
        {
        // GLFW: initialize and configure
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        handle_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (handle_ == nullptr)
        {
            glfwDestroyWindow(handle_);
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(handle_);

        glfwSetFramebufferSizeCallback(handle_, OnFramebufferSizeChanged);

        glfwSetWindowUserPointer(handle_, this);

        // GLAD: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwDestroyWindow(handle_);
            glfwTerminate();
            handle_ = nullptr;
            throw std::runtime_error("Failed to initialize GLAD");
        }
    }

    WindowService::~WindowService()
    {
        glfwDestroyWindow(handle_);
        glfwTerminate();
    }

    bool WindowService::ShouldClose() const { return glfwWindowShouldClose(handle_); }
    void WindowService::RequestClose() { glfwSetWindowShouldClose(handle_, GLFW_TRUE); }

    void WindowService::SwapBuffers() { glfwSwapBuffers(handle_); }
    void WindowService::PollEvents() { glfwPollEvents(); }

    // !!! STATIC !!!
    void WindowService::OnFramebufferSizeChanged(GLFWwindow* window, int width, int height)
    {
        auto* self = static_cast<WindowService*>(glfwGetWindowUserPointer(window));
        self->width_ = width;
        self->height_ = height;

        glViewport(0, 0, width, height);
    }

    // !!! TEMPORAL FUNCTION !!!
    void WindowService::TMP_CheckEscapeKey()
    {
        if (glfwGetKey(handle_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(handle_, true);
    }
}