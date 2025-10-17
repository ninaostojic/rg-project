//
// Created by linux on 10/9/25.
//

#ifndef MATF_RG_PROJECT_LIGHTS_HPP
#define MATF_RG_PROJECT_LIGHTS_HPP
#include <glm/vec3.hpp>

struct DirectionalLight {
    glm::vec3 direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight {
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#endif //MATF_RG_PROJECT_LIGHTS_HPP