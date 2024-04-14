#pragma once
#include <GL/glew.h>
#include "../stb_image.h"
#include <string_view>
#include <stdexcept>

namespace gl {
    struct texture {
        texture() {
            glGenTextures(1, &tex);
        }
        ~texture() {
            glDeleteTextures(1, &tex);
        }
        void bind() {
            glBindTexture(GL_TEXTURE_2D, tex);
        }
        void gradient(GLsizei width, GLsizei  height) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
        }
        void fromFile(std::string const & path) {
            // Load image
            int img_width, img_height, img_channels;
            unsigned char* img_data = stbi_load(path.c_str(), &img_width, &img_height, &img_channels, 0);
            if (!img_data) {
                throw std::runtime_error("Failed to load texture");
            }

            // Bind texture and load image data into it
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img_data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

            // Free image data after uploading to GPU
            stbi_image_free(img_data);

        }
        GLuint tex;
    };
}
