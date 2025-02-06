//
// Created by linux on 2/5/25.
//

#include "../include/MyApp.hpp"

#include <spdlog/spdlog.h>

namespace app {
    void MyApp::app_setup() {
        spdlog::info("App setup completed!");
    }

} // app