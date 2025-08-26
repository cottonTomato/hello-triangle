#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  vec4 pos = view * model * vec4(aPosition, 1.0f);
  gl_Position = projection * pos;
}

