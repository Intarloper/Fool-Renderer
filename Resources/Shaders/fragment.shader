#version 330 core
in vec4 out_pos_to_color;
in vec4 color;

out vec4 FragColor;

    
void main()
{
	FragColor = vec4(out_pos_to_color.xyw, -1 * out_pos_to_color.z );
	//FragColor = color;
};

