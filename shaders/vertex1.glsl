#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMat;
uniform mat4 projection;

void main() {
  vec4 pos = view * model * vec4(aPosition, 1.0f);
  Position = vec3(pos);
  Normal = normalMat * aNormal;
  TexCoords = aTexCoords;
  gl_Position = projection * pos;
}

