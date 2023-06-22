#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform float myUniform;
uniform float myUniformY;
uniform float myUniformZ;

out vec4 out_pos_to_color;
out vec4 color;

void main()
{
	color = vec4(aColor, 1.0);
	out_pos_to_color = vec4(aPos.x + myUniform, aPos.y + myUniformY, aPos.z + myUniformZ, 1.0);  
	gl_Position = vec4(aPos.x + myUniform, aPos.y + myUniformY, aPos.z + myUniformZ, 1.0);
};

