#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	// Only texture
	FragColor = texture(texture1, TexCoord);
	// Mixing Texture and colors
	//FragColor = texture(texture1, TexCoord) * vec4(ourColor, 1.0);  
}