#version 330 core
in vec4 out_pos_to_color;
in vec4 color;

out vec4 FragColor;

uniform float myUniform;
    
void main()
{
       //FragColor = vec4(out_pos_to_color.x , out_pos_to_color.y , out_pos_to_color.z , 1.0f);
	FragColor = color;
};

