//
// Created by linux on 2/5/25.
//

#include "../include/MyApp.hpp"

#include <GUIController.hpp>
#include <MainController.hpp>
#include <spdlog/spdlog.h>

namespace app {
    void MyApp::app_setup() {
        spdlog::info("App setup completed!");
        auto main_controller = register_controller<app::MainController>();
        auto gui_controller  = register_controller<GUIController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
        gui_controller->after(main_controller);
    }
} // app
