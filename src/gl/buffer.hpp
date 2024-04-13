#pragma once

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
        void data_array(auto size, auto data, GLenum usage) {
            glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        }
    private:
        GLuint buffer_;
    };
}