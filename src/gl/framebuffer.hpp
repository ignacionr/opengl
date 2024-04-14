#pragma once
#include <GL/glew.h>

namespace gl {
    struct framebuffer {
        framebuffer() {
            glGenFramebuffers(1, &fb);
        }
        ~framebuffer() {
            glDeleteFramebuffers(1, &fb);
        }
        void bind() {
            glBindFramebuffer(GL_FRAMEBUFFER, fb);
        }
        void unbind() {
            glBindFramebuffer(GL_FRAMEBUFFER, fb);
        }
        GLuint fb;
    };
}