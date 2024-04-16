#pragma once

#include <GL/glew.h>
#include "gl/buffer.hpp"
#include "gl/vertex_array.hpp"
#include "gl/float_layout.hpp"
#include "shader.hpp"

struct basic_pipeline: shader_program {
// Shader sources
static constexpr const GLchar* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 position;
    layout (location = 1) in vec3 color;
    out vec3 ourColor;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * view * model * vec4(position, 1.0f);
        ourColor = color;
    }
)glsl";

static constexpr const GLchar* fragmentShaderSource = R"glsl(
    #version 330 core
    in vec3 ourColor;
    out vec4 color;
    void main() {
        color = vec4(ourColor, 0.55f);
    }
)glsl";

    basic_pipeline(GLsizeiptr  size, const void * data, gl::float_layout position, gl::float_layout color) {
        // Compile and setup the shader
        shader vertex(GL_VERTEX_SHADER, vertexShaderSource);
        shader fragment(GL_FRAGMENT_SHADER, fragmentShaderSource);

        attach(vertex);
        attach(fragment);
        link();

        model.location = getUniformLocation("model");
        view.location = getUniformLocation("view");
        projection.location = getUniformLocation("projection");

        // Set up vertex data and buffers and configure vertex attributes
        vao.bind();
        vbo.bind();
        vbo.data(size * sizeof(float), data, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, position.size, GL_FLOAT, position.normalized, position.stride, position.pointer);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, color.size, GL_FLOAT, color.normalized, color.stride, color.pointer);
        glEnableVertexAttribArray(1);

        vbo.unbind();
        vao.unbind();

        count = size / (position.size + color.size);
        std::cerr << "Loaded mesh with " << count << " vertices\n";
    }

    void draw() {
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, count);
        vao.unbind();
    }

    uniform_mat4_ref model;
    uniform_mat4_ref view;
    uniform_mat4_ref projection;
    GLsizei count;

    gl::buffer vbo;
    gl::vertex_array vao;
};
