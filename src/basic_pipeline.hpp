#pragma once

#include <GL/glew.h>
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
        color = vec4(ourColor, 1.0f);
    }
)glsl";

    basic_pipeline() {
        // Compile and setup the shader
        shader vertex(GL_VERTEX_SHADER, vertexShaderSource);
        shader fragment(GL_FRAGMENT_SHADER, fragmentShaderSource);

        attach(vertex);
        attach(fragment);
        link();

        model.location = getUniformLocation("model");
        view.location = getUniformLocation("view");
        projection.location = getUniformLocation("projection");
    }

    uniform_mat4_ref model;
    uniform_mat4_ref view;
    uniform_mat4_ref projection;
};
