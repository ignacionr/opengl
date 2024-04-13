#pragma once
#include <GL/glew.h>

namespace gl {
    struct vertex_array {
        vertex_array() {
            glGenVertexArrays(1, &va);
        }
        ~vertex_array() {
            glDeleteVertexArrays(1, &va);
        }
        void bind() {
            glBindVertexArray(va);
        }
        void unbind() {
            glBindVertexArray(0);
        }
    private:
        GLuint va;
    };
}