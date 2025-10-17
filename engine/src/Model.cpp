
#include <engine/resources/Model.hpp>
#include <engine/resources/Shader.hpp>

namespace engine::resources {

    void Model::draw(const Shader *shader) {
        shader->use();
        for (auto &mesh: m_meshes) {
            mesh.draw(shader);
        }
    }

    void Model::draw_instanced(const Shader *shader, int num_instances) {
        shader->use();
        for (auto &mesh: m_meshes) {
            mesh.draw_instanced(shader, num_instances);
        }
    }
    void Model::set_instancing_data(void* data, unsigned int element_size, int num_elements) {
        for (auto &mesh: m_meshes) {
            mesh.set_instancing_data(data, element_size, num_elements);
        }
    }

    void Model::destroy() {
        for (auto &mesh: m_meshes) {
            mesh.destroy();
        }
    }


}
