// Basic Texture Shader

#type vertex
#version 450 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;
			
uniform mat4 u_ViewProjection;
uniform mat4 u_ModelTransform;

out vec2 v_TexCoords;			
	
void main()
{
	v_TexCoords = a_TexCoords;
	gl_Position = u_ViewProjection * u_ModelTransform * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core
			
layout(location = 0) out vec4 color;
			
in vec2 v_TexCoords;
			
uniform sampler2D u_Texture;
	
void main()
{
	color = texture(u_Texture, v_TexCoords);
}