#pragma once

#include <GL/glew.h>

namespace gl {
    struct float_layout {
        float_layout(GLint size, GLsizei stride, std::size_t offset)
            : size(size), stride(stride * sizeof(float)), pointer((void*)(offset * sizeof(float))) {}

        GLint size;
        GLsizei stride;
        const void* pointer;
        GLboolean normalized = GL_FALSE;
    };
}