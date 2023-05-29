// Flat Color Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;
uniform mat4 u_ModelTransform;

void main()
{
	gl_Position = u_ViewProjection * u_ModelTransform * vec4(a_Position, 1.0f);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
}