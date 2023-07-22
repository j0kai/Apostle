// Basic Texture Shader

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;

void main()
{
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 color;

uniform float u_Time;
uniform vec2 u_Resolution;

vec3 palette(float t)
{
    vec3 a = vec3(0.5);
    vec3 b = vec3(0.5);
    vec3 c = vec3(1.0);
    vec3 d = vec3(0.263, 0.416, 0.557);
    
    return a + b*cos(6.28318 * (c*t+d));
}

void main()
{
	vec2 uv = (gl_FragCoord.xy * 2.0 - u_Resolution) / u_Resolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    
    for(float i = 0.0; i < 4.0; i++)
    {
        uv = fract(uv * 1.5) - 0.5;
        
        float d = length(uv) * exp(-length(uv0));
        
        vec3 col = palette(length(uv0) + i * 0.4 + u_Time * 0.4);
        
        d = sin(d * 8.0 + u_Time) / 8.0;
        
        d = abs(d);
        
        d = pow(0.01 / d, 1.2);
        
        finalColor += col * d;
    }

    // Output to screen
    color = vec4(finalColor, 1.0);
}




