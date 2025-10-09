//#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 5) in vec3 aOffset;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos + aOffset, 1.0));
    Normal = aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}

//#shader fragment
#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

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

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform PointLight pointLight;

void main()
{
    vec3 textureColor = texture(texture_diffuse1, TexCoords).rgb;
    vec3 result;

    vec3 ambient = directionalLight.ambient; // uvek prisutna, mala kolicina svetlosti

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-directionalLight.direction);

    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * directionalLight.diffuse; // svetlo koje direktno pada na povrsinu modela, glavni deo osvetljenja

    vec3 viewDirection = normalize(viewPosition - FragPos);
    vec3 reflectDirection = reflect(-lightDirection, norm);

    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
    vec3 specular = spec * directionalLight.specular; // odsjaj koji se vidi na glatkim povrsinama

    result += (ambient + diffuse + specular);

    vec3 pointAmbient = pointLight.ambient; // uvek prisutna, mala kolicina svetlosti

    vec3 pointLightDirection = normalize(pointLight.position - FragPos);

    float pointDiff = max(dot(norm, pointLightDirection), 0.0);
    vec3 pointDiffuse = pointDiff * pointLight.diffuse; // svetlo koje direktno pada na povrsinu modela, glavni deo osvetljenja

    vec3 pointReflectDirection = reflect(-pointLightDirection, norm);

    float pointSpec = pow(max(dot(viewDirection, pointReflectDirection), 0.0), 32);
    vec3 pointSpecular = pointSpec * pointLight.specular; // odsjaj koji se vidi na glatkim povrsinama

    float distance    = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance +
        		    pointLight.quadratic * (distance * distance));
    result += (pointAmbient + pointDiffuse + pointSpecular) * attenuation;

    FragColor = vec4(result * textureColor, 1.0);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 1.0) {
       BrightColor = vec4(FragColor.rgb, 1.0);
    } else {
       BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
