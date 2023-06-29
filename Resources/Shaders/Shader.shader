#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec4 out_pos_to_color;
out vec4 color;

void main()
{
	
	color = vec4(aColor, 1.0);
	out_pos_to_color =  proj * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);  
	gl_Position = proj * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
};


#shader fragment

#version 330 core
in vec4 out_pos_to_color;
in vec4 color;

out vec4 FragColor;

    
void main()
{
	FragColor = vec4(out_pos_to_color.xyw, -1 * out_pos_to_color.z );
	//FragColor = color;
};

