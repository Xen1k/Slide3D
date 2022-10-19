#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;

uniform mat4 model;
uniform mat4 camMatrix;

out vec2 texCoord;

void main()
{
	gl_Position = camMatrix * model * vec4(aPos, 1.0f);
	texCoord = aTex;
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform float hasTexture;
uniform sampler2D tex0;
uniform vec3 color;

void main()
{
	FragColor = hasTexture != 0.f ? texture(tex0, texCoord) : vec4(color, 1.f);
}

