#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <vector>
#include <format>

const GLuint WIDTH = 800, HEIGHT = 600;

#include "colored_cube.hpp"

#include "shader_src.hpp"
#include "glfw/init.hpp"
#include "glfw/window.hpp"
#include "glew/init.hpp"
#include "shader.hpp"
#include "gl/vertex_array.hpp"
#include "gl/buffer.hpp"

int main() {
    glfw::init glfwInit;
    glfw::window window(WIDTH, HEIGHT, "OpenGL Offscreen");
    glfwMakeContextCurrent(window);

    glew::init glewInit;

    // Set up vertex data and buffers and configure vertex attributes
    gl::buffer vbo;
    gl::vertex_array vao;

    vao.bind();
    vbo.bind();
    vbo.data_array(sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    vao.unbind();

    // Set up framebuffer
    GLuint fbo, texColorBuffer;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer is not complete!" << std::endl;
        return -1;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glEnable(GL_DEPTH_TEST);

    {
        shader_program shader;
        shader.use();
        // Render loop
        for (int pass = 0; pass < 10; ++pass) {
            // Clear the colorbuffer
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Draw the triangle
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f + pass * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
            glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, &model[0][0]);
            glUniformMatrix4fv(shader.getUniformLocation("view"), 1, GL_FALSE, &view[0][0]);
            glUniformMatrix4fv(shader.getUniformLocation("projection"), 1, GL_FALSE, &projection[0][0]);

            vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, 36);
            vao.unbind();

            // Save the framebuffer to a file
            std::vector<unsigned char> buffer(WIDTH * HEIGHT * 3);
            glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
            stbi_flip_vertically_on_write(true);  // Tell stb_image to flip the image in writing
            stbi_write_png(std::format("output{}.png", pass).c_str(), WIDTH, HEIGHT, 3, buffer.data(), 0);
        }
    }

    // Cleanup
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &texColorBuffer);
    return 0;
}
