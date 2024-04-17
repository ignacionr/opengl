#pragma once

#include <GL/glew.h>

struct shader_program {
    struct shader {
        shader(GLenum type, const GLchar* source) {
            shader_ = glCreateShader(type);
            glShaderSource(shader_, 1, &source, NULL);
            glCompileShader(shader_);
        }
        ~shader() {
            glDeleteShader(shader_);
        }
        GLuint shader_;
    };

    struct uniform_mat4_ref {
        uniform_mat4_ref(GLint location = -1): location(location) {}
        void operator=(const glm::mat4& value) {
            glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
            if (glGetError() != GL_NO_ERROR) {
                throw std::runtime_error("Failed to set uniform");
            }
        }
        GLint location;
    };

    shader_program() {
        shaderProgram = glCreateProgram();
    }

    void attach(const shader& shader) {
        glAttachShader(shaderProgram, shader.shader_);
    }

    void link() {
        glLinkProgram(shaderProgram);
    }

    virtual ~shader_program() {
        glDeleteProgram(shaderProgram);
    }
    void use() {
        glUseProgram(shaderProgram);
    }
    GLint getUniformLocation(const GLchar* name) {
        return glGetUniformLocation(shaderProgram, name);
    }
private:
    GLuint shaderProgram;
};
