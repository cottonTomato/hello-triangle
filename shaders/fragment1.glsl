#version 330 core

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

struct DirLight
{
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight
{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float kc;
  float kl;
  float kq;
};

struct SpotLight
{
  vec3 position;
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float kc;
  float kl;
  float kq;
  float innerCutoff;
  float outerCutoff;
};

in vec3 Position;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;

uniform DirLight dirLight;

#define MAX_POINT_LIGHT 4
uniform PointLight pointLights[MAX_POINT_LIGHT];
uniform int numPointLights;

#define MAX_SPOT_LIGHT 2
uniform SpotLight spotLights[MAX_SPOT_LIGHT];
uniform int numSpotLights;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir);

void main()
{
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(-Position);

  vec3 result = calcDirLight(dirLight, norm, viewDir);

  for (int i = 0; i < numPointLights; i++)
  {
    result += calcPointLight(pointLights[i], norm, viewDir);
  }

  for (int i = 0; i < numSpotLights; i++)
  {
    result += calcSpotLight(spotLights[i], norm, viewDir);
  }

  FragColor = vec4(result, 1.0f);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightRayDir = normalize(-light.direction);
  vec3 reflectRayDir = reflect(-lightRayDir, normal);

  float diff = max(dot(normal, lightRayDir), 0.0f);
  float spec = pow(max(dot(viewDir, reflectRayDir), 0.0f), material.shininess);

  vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  vec3 diffuse =
      light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
  vec3 specular =
      light.specular * spec * texture(material.specular, TexCoords).rgb;

  return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightRayDir = normalize(light.position - Position);
  vec3 reflectRayDir = reflect(-lightRayDir, normal);

  float diff = max(dot(normal, lightRayDir), 0.0f);
  float spec = pow(max(dot(viewDir, reflectRayDir), 0.0f), material.shininess);

  vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  vec3 diffuse =
      light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
  vec3 specular =
      light.specular * spec * texture(material.specular, TexCoords).rgb;

  float distance = length(light.position - Position);
  float attenuation =
      1.0f / (light.kc + light.kl * distance + light.kq * distance * distance);
  diffuse *= attenuation;
  specular *= attenuation;

  return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir)
{
  vec3 lightRayDir = normalize(light.position - Position);
  vec3 reflectRayDir = reflect(-lightRayDir, normal);

  float diff = max(dot(normal, lightRayDir), 0.0f);
  float spec = pow(max(dot(viewDir, reflectRayDir), 0.0f), material.shininess);

  vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  vec3 diffuse =
      light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;
  vec3 specular =
      light.specular * spec * texture(material.specular, TexCoords).rgb;

  float distance = length(light.position - Position);
  float attenuation =
      1.0f / (light.kc + light.kl * distance + light.kq * distance * distance);
  diffuse *= attenuation;
  specular *= attenuation;

  float theta = dot(lightRayDir, -light.direction);
  float epsilon = light.innerCutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0f, 1.0f);
  diffuse *= intensity;
  specular *= intensity;

  return ambient + diffuse + specular;
}
