#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 normal;
out vec3 FragPos;
void main()
{
	gl_Position = proj * view * model * vec4(aPos.xyz, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	normal = mat3(transpose(inverse(model))) * aNorm;
	//normal = aNorm;
};	


#shader fragment

#version 330 core

in vec3 normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform float specValue;
uniform float ambientIntensity;

struct PointLight {
	vec3 position;
	vec3 color;

	float constant;
	float linear;
	float quadratic;
};


uniform PointLight light;

uniform bool lightType;
vec3 result;

void main()
{
	//Lighiting Section
	//ambient
	vec3 ambientValue = ambientIntensity * lightColor;
	vec3 ambientResult = ambientValue * objectColor;

	//diffuse
	vec3 lightDirection = normalize( lightPos - FragPos);
	vec3 norm = normalize(normal);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor;
	//specular
	
	float specStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDirection, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), specValue);
	vec3 specular = specStrength * spec * lightColor;
	
	//point light calc + implementation
	if(lightType){
		float distance = length(light.position - FragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + 
				   light.quadratic * (distance * distance));
	
		ambientResult *= attenuation;
		diffuse *= attenuation;
		specular *= attenuation;
		
	};
	
	vec3 result = (ambientResult + diffuse + specular) * objectColor;
	
	//FragColor = vec4(result, 1.0f);
	FragColor = vec4(result, 1.0f);
};

