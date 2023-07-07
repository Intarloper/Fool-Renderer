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
	normal = aNorm;
};


#shader fragment

#version 330 core

in vec3 normal;
in vec3 FragPos;

out vec4 FragColor;


void main()
{
	vec3 objectColor = vec3(1.0f, 0.0f, 1.0f);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	vec3 lightPos = vec3(1.0f, 1.0f, 0.0f);
	//Lighiting Section
	//ambient
	float ambientIntensity = 0.1f;
	
	vec3 ambientValue = ambientIntensity * lightColor;
	vec3 ambientResult = ambientValue * objectColor;
	//diffuse
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambientResult + diffuse) * objectColor;
	//FragColor = vec4(result, 1.0f);
	FragColor = vec4(result, 1.0f);
};

