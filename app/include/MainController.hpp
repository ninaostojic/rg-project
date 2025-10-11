//
// Created by linux on 2/6/25.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP
#include <engine/core/Controller.hpp>

#include <Lights.hpp>

namespace app {

    class MainController : public engine::core::Controller {
        void initialize() override;

        bool loop() override;

        void set_uniforms(std::string name);

        void draw_micheal();

        void draw_point_light();

        void draw_pumpkins();

        void draw_ground();

        void draw_trees();

        void begin_draw() override;

        void draw() override;

        void end_draw() override;

        void update_camera();

        void update() override;

        void poll_events() override;

    public:
        std::string_view name() const override {
            return "app::MainController";
        }

        glm::vec3 light_color = {1.0f, 1.0f, 1.0f};
        float light_intensity = 2.0f;

    private:
        DirectionalLight m_directional_light;
        PointLight m_point_light;
        bool m_cursor_visible;
        int m_num_tree_instances = 300;

    };

} // app

#endif //MAINCONTROLLER_HPP
