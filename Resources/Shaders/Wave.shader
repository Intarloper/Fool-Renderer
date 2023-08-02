#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float time;

void main(){
        float e = 2.718281828459;

	vec4 modelPos = model * vec4(aPos.xyz, 1.0);
	
	//float displace = sin(length(modelPos.xz) * 10.0 - time) * 0.75;

	float displace = 0.0;

	for(int i = 1; i < 3; i++){
		float zWave = pow(e, sin((length(modelPos.z) / i) * 10 - time) - 1 * .75);
		float xWave = pow(e, sin((length(modelPos.x) / i) * 15 - time) - 1 * .75);

		
		displace = displace + zWave + xWave;
	};
	gl_Position = proj * view * model * vec4(aPos.xy, aPos.z + displace, 1.0);
};


#shader fragment

#version 330 core

out vec4 FragColor;

void main(){
	FragColor = vec4(1.0);
};
