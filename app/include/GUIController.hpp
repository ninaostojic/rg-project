//
// Created by ognjen on 10/11/25.
//

#ifndef MATF_RG_PROJECT_GUICONTROLLER_HPP
#define MATF_RG_PROJECT_GUICONTROLLER_HPP
#include <engine/core/Controller.hpp>

namespace app {
    class GUIController : public engine::core::Controller {
    public:
        std::string_view name() const override {
            return "app::GUIController";
        }

    private:
        void initialize() override;

        void poll_events() override;

        void draw() override;

    public:
        bool gui_shown = false;
    };
}

#endif //MATF_RG_PROJECT_GUICONTROLLER_HPP

