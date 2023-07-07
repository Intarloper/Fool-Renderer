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
	gl_Position = proj * view * model * vec4(aPos.xyz, 1.0);
};


#shader fragment

#version 330 core
in vec4 out_pos_to_color;
in vec4 color;
in vec4 gl_FragCoord;
out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
	//Messing around with shader stuff, makes circles across the screen that are only rendered 
	//inside the cubes


	//vec2 screenSize = vec2(1280,720);
	//vec4 fc = gl_FragCoord;
	//fc = vec4(fc.xy / screenSize.xy * 2.0 - 1, 0 , 1);

	//float d = length(fc);
	//d = sin(d*10) / 10;
	
	
	//FragColor = vec4(out_pos_to_color.xyw, -1 * out_pos_to_color.z ) * d * 10;
	//Lighiting Section

	float ambientIntensity = 0.1f;
	
	vec3 ambientValue = ambientIntensity * lightColor;
	vec3 ambientResult = ambientValue * color.xyz;
	FragColor = vec4(ambientResult, 1.0f);
	
};

