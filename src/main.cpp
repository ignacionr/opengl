#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#include "stb_image_write.h"
#include "tiny_obj_loader.h"

#include <iostream>
#include <vector>
#include <format>
#include <array>
#include <cmath>

const GLuint WIDTH = 800, HEIGHT = 600;

#include "glfw/init.hpp"
#include "glfw/window.hpp"
#include "glew/init.hpp"
#include "shader.hpp"
#include "gl/framebuffer.hpp"
#include "gl/texture.hpp"

#include "basic_pipeline.hpp"

std::vector<float> load_obj(std::string const & inputfile)
{
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    tinyobj::attrib_t attrib;
    std::string err, warn;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str(), nullptr, true, false);
    if (!ret || !err.empty())
    {
        throw std::runtime_error("Failed to load/parse .obj file");
    }

    // Assuming only one shape is in the OBJ
    auto mesh = shapes[0].mesh;
    std::cerr << std::format("Loaded mesh with {} vertices\n", mesh.indices.size());
    std::vector<float> vertices;
    vertices.reserve(mesh.indices.size() * 6);

    using point = std::array<float, 3>;
    point min = {std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
    point max = {std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min()};
    for (size_t f = 0; f < mesh.indices.size(); f++)
    {
        tinyobj::index_t idx = mesh.indices[f];
        // Push vertex
        tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
        tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
        tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
        min[0] = std::min(min[0], attrib.vertices[3 * idx.vertex_index + 0]);
        min[1] = std::min(min[1], attrib.vertices[3 * idx.vertex_index + 1]);
        min[2] = std::min(min[2], attrib.vertices[3 * idx.vertex_index + 2]);
        max[0] = std::max(max[0], attrib.vertices[3 * idx.vertex_index + 0]);
        max[1] = std::max(max[1], attrib.vertices[3 * idx.vertex_index + 1]);
        max[2] = std::max(max[2], attrib.vertices[3 * idx.vertex_index + 2]);
        vertices.push_back(vx);
        vertices.push_back(vy);
        vertices.push_back(vz);
        // Push color
        // vertices.push_back(1.0f * (0 == ((f + 0) % 3)));
        // vertices.push_back(1.0f * (0 == ((f + 1) % 3)));
        // vertices.push_back(1.0f * (0 == ((f + 2) % 3)));
        vertices.push_back(1.0f);
        vertices.push_back(1.0f);
        vertices.push_back(1.0f);
    }
    std::array<float, 3> center = {min[0] + (max[0] - min[0]) / 2, min[1] + (max[1] - min[1]) / 2, min[2] + (max[2] - min[2]) / 2};
    std::cerr << std::format("Center: ({}, {}, {})\n", center[0], center[1], center[2]);
    return vertices;
}

void write_to_stdout(void *context, void *data, int size) {
    fwrite(data, 1, size, stdout);
}

int main()
{
    glfw::init glfwInit;
    glfw::window window(WIDTH, HEIGHT, "OpenGL Offscreen");
    glfwMakeContextCurrent(window);

    glew::init glewInit;

    std::vector<float> vertices = load_obj("/workspaces/opengl/assets/teapot.obj");
    // std::vector<float> vertices = load_obj("/workspaces/opengl/assets/hand.obj");

    basic_pipeline shader(vertices.size(), vertices.data(), {3, 6, 0}, {3, 6, 3});

    // Set up framebuffer
    gl::framebuffer fbo;
    fbo.bind();

    gl::texture texColorBuffer;
    texColorBuffer.gradient(WIDTH, HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
        constexpr float maxDepth = 200.0f; // Maximum distance from the object
        constexpr float minDepth = 1.0f;  // Minimum distance (almost touching the object)
        constexpr int totalFrames = 300;  // Total number of animation frames
        shader.use();
        // Render loop
        for (int pass = 0; pass < 300; ++pass)
        {
            // Clear the colorbuffer
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Calculate normalized time from 0 to 1
            float t = pass / float(totalFrames);

            // Draw the triangle
            // shader.model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f + pass * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            // shader.model = glm::scale(glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f + pass * 2.0f), glm::vec3(1.0f, 0.0f, 0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
            // shader.model = glm::scale(glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f + pass * 2.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));
            shader.model = glm::scale(glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f  * 2.0f), glm::vec3(0.0f, 1.0f, 0.0f)), glm::vec3(0.5f, 0.5f, 0.5f));

            // Using cosine for non-linear movement
            // The cosine will oscillate between 1 and -1. Adjusting this range to go between maxDepth and minDepth
            float depth = minDepth + (maxDepth - minDepth) * (cos(2 * M_PI * t) * 0.5 + 0.5);

            // shader.view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -depth));

            // Center point of the object you want to focus on
            // glm::vec3 handcenter(-8.0f, -0.6631899f, 4.7030244f);
            constexpr glm::vec3 center(0.21f, 1.575f, 0.0f);

            // Camera position: Calculate based on the center and depth
            glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, depth);

            // Up vector for the camera (assuming Y is up)
            glm::vec3 up(0.0f, 2.0f, 0.0f);

            // Create the view matrix using glm::lookAt
            shader.view = glm::lookAt(cameraPos, center, up);

            shader.projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

            shader.draw();
            // Save the framebuffer to a file
            std::vector<unsigned char> buffer(WIDTH * HEIGHT * 3);
            glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
            stbi_flip_vertically_on_write(true); // Tell stb_image to flip the image in writing
            // stbi_write_png(std::format("output{}.png", pass).c_str(), WIDTH, HEIGHT, 3, buffer.data(), 0);
            stbi_write_png_to_func(write_to_stdout, NULL, WIDTH, HEIGHT, 3, buffer.data(), 0);
        }
    }

    return 0;
}
