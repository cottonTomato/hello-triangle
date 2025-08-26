#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 texCoord;

out vec3 Color;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMat;

void main() {
  vec4 pos = view * model * vec4(aPosition, 1.0f);
  vec3 Position = vec3(pos);
  vec3 Normal = normalMat * aNormal;

  float ambientStrength = 0.1f;
  float specularStrength = 0.5f;
  int shininess = 32;

  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - Position);
  vec3 viewDir = normalize(-Position);
  vec3 reflectDir = reflect(-lightDir, norm);

  float diffuseCoeff = max(dot(norm, lightDir), 0.0f);
  float specularCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);

  vec3 ambient = ambientStrength * lightColor;
  vec3 diffuse = diffuseCoeff * lightColor;
  vec3 specular = specularStrength * specularCoeff * lightColor;

  Color = (ambient + diffuse + specular) * objectColor;
  gl_Position = projection * pos;
}

