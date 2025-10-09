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

        m_directional_light.direction = glm::vec3(0.0f, -2.0f, -2.0f);
        m_directional_light.ambient   = glm::vec3(0.4f, 0.3f, 1.0f);
        m_directional_light.diffuse   = glm::vec3(0.4f, 0.3f, 1.0f);
        m_directional_light.specular  = glm::vec3(0.4f, 0.3f, 1.0f);

        m_point_light.position = glm::vec3(0.0, -5.0f, 1.0f);
        m_point_light.ambient  = glm::vec3(0.1f, 1.0f, 0.2f);
        m_point_light.diffuse  = glm::vec3(0.1f, 1.0f, 0.2f);
        m_point_light.specular = glm::vec3(0.1f, 1.0f, 0.2f);

        m_point_light.constant  = 1.0f;
        m_point_light.linear    = 0.09f;
        m_point_light.quadratic = 0.032f;

        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto window = platform->window();
        engine::graphics::OpenGL::create_bloom_fbo(window->width(), window->height());

        spdlog::info("MainController initialized");
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }

    void MainController::draw_backpack() {
        // Model
        auto resources                  = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics                   = engine::core::Controller::get<engine::graphics::GraphicsController>();
        engine::resources::Model *backpack = resources->model("backpack");
        // Shader
        engine::resources::Shader *shader = resources->shader("textured_lit");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        auto model = glm::mat4(1.0f);
        model      = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
        model      = glm::scale(model, glm::vec3(0.3f));
        shader->set_mat4("model", model);

        shader->set_vec3("directionalLight.direction", m_directional_light.direction);

        shader->set_vec3("directionalLight.diffuse", m_directional_light.diffuse);
        shader->set_vec3("directionalLight.ambient", m_directional_light.ambient);
        shader->set_vec3("directionalLight.specular", m_directional_light.specular);

        shader->set_vec3("pointLight.position", m_point_light.position);

        shader->set_vec3("pointLight.diffuse", m_point_light.diffuse);
        shader->set_vec3("pointLight.ambient", m_point_light.ambient);
        shader->set_vec3("pointLight.specular", m_point_light.specular);

        shader->set_float("pointLight.constant", m_point_light.constant);
        shader->set_float("pointLight.linear", m_point_light.linear);
        shader->set_float("pointLight.quadratic", m_point_light.quadratic);

        shader->set_vec3("viewingPosition", graphics->camera()->Position);
        backpack->draw(shader);
    }

    void MainController::draw_point_light() {
        auto resources                  = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics                   = engine::core::Controller::get<engine::graphics::GraphicsController>();

        auto cube   = resources->model("cube");
        auto shader = resources->shader("solid_color");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        auto model = glm::mat4(1.0f);
        model      = glm::translate(model, m_point_light.position);
        model      = glm::scale(model, glm::vec3(0.3f));
        shader->set_mat4("model", model);
        shader->set_vec4("color", glm::vec4(m_point_light.diffuse, 1.0f));
        cube->draw(shader);
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::draw() {
        engine::graphics::OpenGL::bind_and_clear_fbo_framebuffer();

        draw_backpack();
        draw_point_light();

        auto fbo_shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("fbo_shader");
        fbo_shader->use();
        fbo_shader->set_int("fboTexture", 0);
        engine::graphics::OpenGL::draw_framebuffer();
    }

    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

    void MainController::update_camera() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto camera   = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        float dt      = platform->dt();
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
        auto mouse        = platform->mouse();
        float sensitivity = 10.0f; // kamera se okrece previse sporo
        camera->rotate_camera(mouse.dx * sensitivity, mouse.dy * sensitivity);
        camera->zoom(mouse.scroll);
    }

    void MainController::update() {
        update_camera();
    }

    void MainController::poll_events() {
        const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KEY_F1)
                    .state() == engine::platform::Key::State::JustPressed) {
            m_cursor_visible = !m_cursor_visible;
            platform->set_enable_cursor(m_cursor_visible);
        }
    }

} // app
