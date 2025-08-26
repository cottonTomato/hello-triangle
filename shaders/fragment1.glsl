#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

in vec3 Position;
in vec3 Normal;
// in vec3 LightPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform Light light;

void main() {
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(-light.direction);
  vec3 reflectDir = reflect(-lightDir, norm);
  vec3 viewDir = normalize(-Position);

  // ambient
  vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

  // diffuse
  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

  // specular
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

  FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
