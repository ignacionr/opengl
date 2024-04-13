#pragma once

#include "shader_src.hpp"

struct shader_program {
    shader_program() {
        // Compile and setup the shader
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
    ~shader_program() {
        glDeleteProgram(shaderProgram);
    }
    void use() {
        glUseProgram(shaderProgram);
    }
    GLint getUniformLocation(const GLchar* name) {
        return glGetUniformLocation(shaderProgram, name);
    }
    GLuint shaderProgram;
};
