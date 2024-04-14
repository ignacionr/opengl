#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <vector>
#include <format>

const GLuint WIDTH = 800, HEIGHT = 600;

#include "colored_cube.hpp"

#include "glfw/init.hpp"
#include "glfw/window.hpp"
#include "glew/init.hpp"
#include "shader.hpp"
#include "gl/framebuffer.hpp"
#include "gl/texture.hpp"

#include "basic_pipeline.hpp"

int main() {
    glfw::init glfwInit;
    glfw::window window(WIDTH, HEIGHT, "OpenGL Offscreen");
    glfwMakeContextCurrent(window);

    glew::init glewInit;

    basic_pipeline shader(sizeof(vertices), vertices, {3, 6, 0}, {3, 6, 3});

    // Set up framebuffer
    gl::texture texColorBuffer;
    gl::framebuffer fbo;
    fbo.bind();

    texColorBuffer.bind();
    texColorBuffer.gradient(WIDTH, HEIGHT);

    glEnable(GL_DEPTH_TEST);

    {
        shader.use();
        // Render loop
        for (int pass = 0; pass < 10; ++pass) {
            // Clear the colorbuffer
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Draw the triangle
            shader.model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f + pass * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            shader.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
            shader.projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

            shader.draw();
            // Save the framebuffer to a file
            std::vector<unsigned char> buffer(WIDTH * HEIGHT * 3);
            glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
            stbi_flip_vertically_on_write(true);  // Tell stb_image to flip the image in writing
            stbi_write_png(std::format("output{}.png", pass).c_str(), WIDTH, HEIGHT, 3, buffer.data(), 0);
        }
    }

    return 0;
}
