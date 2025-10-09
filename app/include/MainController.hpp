//
// Created by linux on 2/6/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

#include <Lights.hpp>

namespace app {

class MainController : public engine::core::Controller{
    void initialize() override;

    bool loop() override;

    void draw_backpack();

    void begin_draw() override;

    void draw() override;

    void end_draw() override;

    void update_camera();

    void update() override;


public:
    std::string_view name() const override {
        return "app::MainController";
    }

private:
    DirectionalLight m_directional_light;
    PointLight m_point_light;
};

} // app

#endif //MAINCONTROLLER_HPP
