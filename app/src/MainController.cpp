//
// Created by linux on 2/6/25.
//

#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include "../include/MainController.hpp"

#include <GUIController.hpp>
#include <WindowSizeObserver.hpp>
#include <spdlog/spdlog.h>

namespace app {
    void MainController::initialize() {
        auto observer = std::make_unique<WindowSizeObserver>();
        engine::core::Controller::get<engine::platform::PlatformController>()->register_platform_event_observer(
                std::move(observer));

        engine::graphics::OpenGL::enable_depth_testing();

        m_directional_light.direction = glm::vec3(0.0f, -2.0f, -2.0f);
        m_directional_light.ambient   = glm::vec3(0.005f, 0.005f, 0.005f);
        m_directional_light.diffuse   = glm::vec3(0.4f, 0.3f, 1.0f);
        m_directional_light.specular  = glm::vec3(0.1f, 0.1f, 0.1f);

        m_point_light.position = m_light_positions[0];
        m_point_light.ambient  = glm::vec3(0.005f, 0.005f, 0.005f);
        m_point_light.diffuse  = light_color * light_intensity;
        m_point_light.specular = glm::vec3(0.1f, 0.1f, 0.1f);

        m_point_light.constant  = 1.0f;
        m_point_light.linear    = 0.09f;
        m_point_light.quadratic = 0.032f;

        auto platform  = engine::core::Controller::get<engine::platform::PlatformController>();
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto window    = platform->window();
        engine::graphics::OpenGL::create_bloom_fbo(window->width(), window->height());
        engine::graphics::OpenGL::crate_blur_fbo(window->width(), window->height());

        glm::vec3 instances_data[m_num_tree_instances];
        for (int i = 0; i < m_num_tree_instances; i++) {
            float instance_x = 0.0f;
            float instance_y = 0.0f;
            while (instance_x >= -4.0f && instance_x <= 4.0f && instance_y >= -4.0f && instance_y <= 4.0f) {
                instance_x = (2.0f * (rand() % 100) / 100.0f - 1.0f) * 40.0f;
                instance_y = (2.0f * (rand() % 100) / 100.0f - 1.0f) * 40.0f;
            }
            instances_data[i] = glm::vec3(instance_x, 0, instance_y);
        }
        resources->model("dead_tree")->set_instancing_data(&instances_data, sizeof(glm::vec3), m_num_tree_instances);
        spdlog::info("MainController initialized");
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }

    void MainController::set_uniforms(std::string name) {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto graphics  = engine::core::Controller::get<engine::graphics::GraphicsController>();

        engine::resources::Shader *shader = resources->shader(name);

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());
        shader->set_vec3("viewingPosition", graphics->camera()->Position);

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
    }

    void MainController::draw_micheal() {
        auto resources                    = engine::core::Controller::get<engine::resources::ResourcesController>();
        engine::resources::Model *micheal = resources->model("micheal_myers");
        engine::resources::Shader *shader = resources->shader("textured_lit");

        shader->use();
        auto model = glm::mat4(1.0f);
        model      = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model      = glm::scale(model, glm::vec3(2.0f));
        shader->set_mat4("model", model);

        micheal->draw(shader);
    }

    void MainController::draw_point_light() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto cube      = resources->model("cube");
        auto shader    = resources->shader("solid_color");

        shader->use();
        auto model = glm::mat4(1.0f);
        model      = glm::translate(model, m_point_light.position);
        model      = glm::scale(model, glm::vec3(0.1f));
        shader->set_mat4("model", model);
        shader->set_vec4("color", glm::vec4(m_point_light.diffuse, 1.0f));
        cube->draw(shader);
    }

    void MainController::draw_pumpkins() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();
        auto pumpkin   = resources->model("pumpkin");
        auto shader    = resources->shader("textured_lit");

        shader->use();
        auto modelLeft = glm::mat4(1.0f);
        modelLeft      = glm::translate(modelLeft, glm::vec3(-1.75f, 0.0f, 0.0f));
        modelLeft      = glm::scale(modelLeft, glm::vec3(2.0f));
        modelLeft      = glm::rotate(modelLeft, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        shader->set_mat4("model", modelLeft);
        pumpkin->draw(shader);

        shader->use();
        auto modelRight = glm::mat4(1.0f);
        modelRight      = glm::translate(
                modelRight, glm::vec3(1.75f, 0.0f, 0.0f));
        modelRight = glm::scale(modelRight, glm::vec3(2.0f));
        modelRight = glm::rotate(modelRight, glm::radians(-30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader->set_mat4("model", modelRight);
        pumpkin->draw(shader);
    }

    void MainController::draw_ground() {
        auto resources                    = engine::core::Controller::get<engine::resources::ResourcesController>();
        engine::resources::Model *micheal = resources->model("mud_forest");
        engine::resources::Shader *shader = resources->shader("textured_lit");

        shader->use();
        auto model = glm::mat4(1.0f);
        model      = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model      = glm::scale(model, glm::vec3(2.0f));
        shader->set_mat4("model", model);

        micheal->draw(shader);
    }

    void MainController::draw_trees() {
        auto resources                    = engine::core::Controller::get<engine::resources::ResourcesController>();
        engine::resources::Model *micheal = resources->model("dead_tree");
        engine::resources::Shader *shader = resources->shader("textured_lit_instanced");

        shader->use();
        auto model = glm::mat4(1.0f);
        model      = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model      = glm::scale(model, glm::vec3(2.0f));
        shader->set_mat4("model", model);

        micheal->draw_instanced(shader, m_num_tree_instances);
    }

    void MainController::begin_draw() {
        engine::graphics::OpenGL::clear_buffers();
    }

    void MainController::draw() {
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

        engine::graphics::OpenGL::bind_and_clear_fbo_framebuffer();

        set_uniforms("textured_lit");
        set_uniforms("textured_lit_instanced");

        set_uniforms("solid_color");

        draw_ground();
        draw_micheal();
        draw_point_light();
        draw_pumpkins();
        draw_trees();

        auto blur_shader = resources->shader("blur");
        blur_shader->use();
        int amount      = 10;
        bool horizontal = true;
        for (int i = 0; i < amount; i++) {
            blur_shader->set_int("horizontal", horizontal);
            blur_shader->set_int("image", 0);
            engine::graphics::OpenGL::blur_framebuffer_texture(i, horizontal);
            horizontal = !horizontal;
        }

        auto fbo_shader = resources->shader("fbo_shader");
        fbo_shader->use();
        fbo_shader->set_int("sceneTexture", 0);
        fbo_shader->set_int("bloomTexture", 1);

        engine::graphics::OpenGL::apply_blur_framebuffer();
    }

    void MainController::end_draw() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        platform->swap_buffers();
    }

    void MainController::update_camera() {
        auto gui = engine::core::Controller::get<GUIController>();
        if (gui->gui_shown) {
            return;
        }
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

    void MainController::update_light() {
        const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

        m_point_light.diffuse = light_color * light_intensity;
        if (m_action_active) {
            m_timer_value += platform->dt();
        }
        if (m_timer_value >= 1.0f) {
            m_timer_value          = 0.0f;
            m_light_position_index = 1 - m_light_position_index;
            m_point_light.position = m_light_positions[m_light_position_index];
        }
    }

    void MainController::update() {
        update_light();
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

    void MainController::start_stop_light_action() {
        m_action_active = !m_action_active;
    }

} // app
