#pragma once

namespace glfw {
    struct window {
        window(int width, int height, const char* title) {
            window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
            if (!window_) {
                throw std::runtime_error("Failed to create GLFW window");
            }
        }
        ~window() {
            glfwDestroyWindow(window_);
        }
        operator GLFWwindow*() const {
            return window_;
        }
    private:
        GLFWwindow* window_;
    };	
}