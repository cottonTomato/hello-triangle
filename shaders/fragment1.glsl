#version 330 core

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct Light {
  vec3 position;
  vec3 direction;
  float innerCutoff;
  float outerCutoff;

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
  // ambient
  vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

  // diffuse
  vec3 norm = normalize(Normal);
  vec3 lightRayDir = normalize(light.position - Position);
  float diff = max(dot(norm, lightRayDir), 0.0f);
  vec3 diffuse = diff * light.diffuse * texture(material.diffuse, TexCoords).rgb;

  // specular
  vec3 viewDir = normalize(-Position);
  vec3 reflectDir = reflect(-lightRayDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
  vec3 specular = spec * light.specular * texture(material.specular, TexCoords).rgb;

  // flashlight
  float theta = dot(lightRayDir, -light.direction);
  float epsilon = light.innerCutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);
  diffuse *= intensity;
  specular *= intensity;

  // attenuation
  float distance = length(light.position - Position);
  float attenuation = 1.0 / (light.kc + light.kl * distance + light.kq * distance * distance);   
  diffuse *= attenuation;
  specular *= attenuation; 

  FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
