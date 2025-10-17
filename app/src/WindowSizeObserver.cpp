//
// Created by ognjen on 10/11/25.
//

#include "../include/WindowSizeObserver.hpp"
#include <engine/graphics/OpenGL.hpp>

namespace app {
    void WindowSizeObserver::on_window_resize(int width, int height) {
        engine::graphics::OpenGL::create_bloom_fbo(width, height);
        engine::graphics::OpenGL::crate_blur_fbo(width, height);
    }
} // app
