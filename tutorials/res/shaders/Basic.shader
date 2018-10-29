#shader vertex

#version 330 core
layout(location = 0) in vec4 position;
out vec4 toFragColor;

uniform vec4 ourColor;

void main()
{
	gl_Position = position;
	toFragColor = ourColor;
}


#shader fragment

#version 330 core

in vec4 toFragColor;
out vec4 FragColor;

void main()
{
	FragColor = toFragColor;
}


