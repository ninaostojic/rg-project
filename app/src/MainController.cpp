//
// Created by linux on 2/6/25.
//

#include "../include/MainController.hpp"
#include <engine/core/Engine.hpp>

namespace app {
    void MainController::initialize() {
    }

    bool MainController::loop() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).is_down()) {
            return false;
        }
        return true;
    }
} // app