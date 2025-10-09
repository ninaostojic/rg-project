//#shader vertex
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoords = aTexCoord;
}

//#shader fragment
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D sceneTexture;
uniform sampler2D bloomTexture;

void main() {
    const float gamma = 2.2;
    const float exposure = 0.1;

    vec3 sceneColor = texture(sceneTexture, TexCoords).rgb;
    vec3 bloomColor = texture(bloomTexture, TexCoords).rgb;
    sceneColor += bloomColor;

    vec3 result = vec3(1.0) - exp(-sceneColor * exposure);
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);

}
