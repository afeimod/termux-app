#version 300 es
precision mediump float;
precision mediump sampler2D;

in vec4 v_color;
in vec2 v_texcoord;

uniform sampler2D tex;

out vec4 FragColor;

void main() {
    FragColor = vec4(texture(tex, v_texcoord).rgb, 1.f);
}
