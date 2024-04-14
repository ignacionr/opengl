#pragma once
#include <GL/glew.h>

namespace gl {
    struct buffer{
        buffer() {
            glGenBuffers(1, &buffer_);
        }
        ~buffer(){
            glDeleteBuffers(1, &buffer_);
        }
        void bind() {
            glBindBuffer(GL_ARRAY_BUFFER, buffer_);
        }
        void data_array(GLsizeiptr  size, const void * data, GLenum usage) {
            glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        }
    private:
        GLuint buffer_;
    };
}