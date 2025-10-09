//#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

uniform vec3 viewPosition;

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirectionalLight directionalLight;

void main()
{
    vec3 textureColor = texture(texture_diffuse1, TexCoords).rgb;

    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * directionalLight.ambient; // uvek prisutna, mala kolicina svetlosti

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-directionalLight.direction);

    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * directionalLight.diffuse; // svetlo koje direktno pada na povrsinu modela, glavni deo osvetljenja

    float specularStrength = 0.5;
    vec3 viewDirection = normalize(viewPosition - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, norm);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = specularStrength * spec * directionalLight.specular; // odsjaj koji se vidi na glatkim povrsinama

    vec3 result = (ambient + diffuse + specular) * textureColor;
    FragColor = vec4(result, 1.0);
}
