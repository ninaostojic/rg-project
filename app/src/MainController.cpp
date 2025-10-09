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
        engine::resources::Model* corgi = resources->model("backpack");
        // Shader
        engine::resources::Shader* shader = resources->shader("textured_lit");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        auto model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        model = glm::scale(model, glm::vec3(0.3f));
        shader->set_mat4("model", model);

        shader->set_vec3("directionalLight.direction", glm::vec3(0.0f, -2.0f, -2.0f));

        shader->set_vec3("directionalLight.diffuse", glm::vec3(0.4f, 0.3f, 1.0f));
        shader->set_vec3("directionalLight.ambient", glm::vec3(0.4f, 0.3f, 1.0f));
        shader->set_vec3("directionalLight.specular", glm::vec3(0.4f, 0.3f, 1.0f));
        shader->set_vec3("viewingPosition", graphics->camera()->Position);
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

    void MainController::update_camera() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        float dt = platform->dt();
        if (platform->key(engine::platform::KEY_W)
                    .state() == engine::platform::Key::State::Pressed) {
            camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
                    }
        if (platform->key(engine::platform::KEY_S)
                    .state() == engine::platform::Key::State::Pressed) {
            camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
                    }
        if (platform->key(engine::platform::KEY_A)
                    .state() == engine::platform::Key::State::Pressed) {
            camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
                    }
        if (platform->key(engine::platform::KEY_D)
                    .state() == engine::platform::Key::State::Pressed) {
            camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
                    }
        auto mouse = platform->mouse();
        float sensitivity = 10.0f; // kamera se okrece previse sporo
        camera->rotate_camera(mouse.dx * sensitivity, mouse.dy * sensitivity);
        camera->zoom(mouse.scroll);
    }

    void MainController::update() {
        update_camera();
    }

} // app