//
// Created by ognjen on 10/11/25.
//

#ifndef MATF_RG_PROJECT_WINDOWSIZEOBSERVER_HPP
#define MATF_RG_PROJECT_WINDOWSIZEOBSERVER_HPP

#include <engine/platform/PlatformEventObserver.hpp>

namespace app {
    class WindowSizeObserver : public engine::platform::PlatformEventObserver {
        void on_window_resize(int width, int height) override;
    };
}; // app

#endif //MATF_RG_PROJECT_WINDOWSIZEOBSERVER_HPP
