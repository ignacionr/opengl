#pragma once

namespace glew {
    struct init {
        init() {
            if (glewInit() != GLEW_OK) {
                throw std::runtime_error("Failed to initialize GLEW");
            }
        }
    };
}