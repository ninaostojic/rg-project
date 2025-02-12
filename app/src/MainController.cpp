//
// Created by linux on 2/6/25.
//

#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include "../include/MainController.hpp"

#include <spdlog/spdlog.h>

namespace app {
    void MainController::initialize() {
        engine::graphics::OpenGL::enable_depth_testing();


        spdlog::info("MainController initialized");
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }
    void MainController::draw_corgi() {

        // Model
        auto resources                  = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model* corgi = resources->model("corgi");
        // Shader
        engine::resources::Shader* shader = resources->shader("basic");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::scale(model, glm::vec3(0.3f));
        shader->set_mat4("model", model);


        corgi->draw(shader);
    }
    void MainController::begin_draw() {

        engine::graphics::OpenGL::clear_buffers();
    }
    void MainController::draw() {
        // clear buffers (color buffer, depth buffer)
        draw_corgi();
        // swapbuffers
    }
    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

} // app