// This shader simulates a retro CRT TV with or without scanlines. The shader original is
// coded by a youtuber named “LetsGameDev”. Check out his channel:
// https://www.youtube.com/user/Tomzalat
// Modified by: Timur Turatbekov

#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

vec2 uv_curve(vec2 uv)
{
    uv = (uv - 0.5) * 2.0;

    uv.x *= 1.0 + pow(abs(uv.y) / 5.0, 2.0);
    uv.y *= 1.0 + pow(abs(uv.x) / 5.0, 2.0);

    uv /= 1.05;

    uv = (uv/2.0) + 0.5;
    return uv;
}

void main() {
    float PI = 3.14159;

    // vec4 texelColor = texture(texture0, fragTexCoord);
    vec2 curved_uv = uv_curve(fragTexCoord);

    float r = texture(texture0, curved_uv + vec2(0.0), 0.0).r;
    float g = texture(texture0, curved_uv + vec2(0.001), 0.0).g;
    float b = texture(texture0, curved_uv + vec2(-0.001), 0.0).b;
    float a = texture(texture0, curved_uv, 0.0).a;

    float s = sin(curved_uv.y * 200 * PI * 2.0);
    s = (s * 0.5 + 0.5) * 0.9 + 0.1;
    vec4 scan_line = vec4(vec3(pow(s, 0.25)), 1.0);

    finalColor = vec4(r, g, b, a) * colDiffuse * fragColor * scan_line;
}