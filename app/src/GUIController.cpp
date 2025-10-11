//
// Created by ognjen on 10/11/25.
//

#include "GUIController.hpp"
#include <engine/core/Controller.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/platform/PlatformController.hpp>

#include <MainController.hpp>
#include <imgui.h>

namespace app {
    void GUIController::initialize() {
        gui_shown = false;
    }

    void GUIController::poll_events() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

        if (platform->key(engine::platform::KeyId::KEY_F2)
                    .state() == engine::platform::Key::State::JustPressed) {
            gui_shown = !gui_shown;
        }
    }

    void GUIController::draw() {
        if (!gui_shown)
            return;
        auto graphics        = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto camera          = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
        auto main_controller = engine::core::Controller::get<MainController>();
        graphics->begin_gui();
        // Draw camera info
        ImGui::Begin("Light");
        ImGui::ColorEdit3("Directional light color", &main_controller->directional_light_color[0]);
        ImGui::ColorEdit3("Point light color", &main_controller->light_color[0]);
        ImGui::DragFloat("Point light intensity", &main_controller->light_intensity, 0.01f, 0.0f, 10.0f);
        if (ImGui::Button("Start/Stop")) {
            main_controller->start_stop_light_action();
        }
        ImGui::End();
        graphics->end_gui();
    }
}
