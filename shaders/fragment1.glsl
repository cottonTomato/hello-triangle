#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float kc;
  float kl;
  float kq;
};

in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform Light light;

void main() {
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(light.position);
  vec3 reflectDir = reflect(-lightDir, norm);
  vec3 viewDir = normalize(-Position);

  float distance = length(light.position - Position);
  float attenuation = 1.0f / (light.kc + light.kl * distance + light.kq * distance * distance);

  // ambient
  vec3 ambient = attenuation * light.ambient * texture(material.diffuse, TexCoords).rgb;

  // diffuse
  float diff = max(dot(norm, lightDir), 0.0f);
  vec3 diffuse = attenuation * diff * light.diffuse * texture(material.diffuse, TexCoords).rgb;

  // specular
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specular = attenuation * spec * light.specular * texture(material.specular, TexCoords).rgb;

  FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
