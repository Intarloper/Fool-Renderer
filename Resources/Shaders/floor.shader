#shader vertex

#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;

out vec4 glpos;
void main(){
	gl_Position = proj * view * model * vec4(pos.xy, pos.z + 0.1f, 1.0f);
	glpos = vec4(pos.xyz , 1.0f);
};





#shader fragment

#version 330 core
out vec4 FragColor;

in vec4 glpos;
void main(){

	FragColor = glpos;
};
