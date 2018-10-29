#shader vertex

#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 outTexCoord;

uniform mat4 trans;

void main()
{
	gl_Position =  trans * vec4(position, 0.0f, 1.0f);
	outTexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}


#shader fragment

#version 330 core
in vec2 outTexCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
	FragColor = texture(ourTexture, outTexCoord);
}


