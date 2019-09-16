#version 330 core
//Assignment2 

// uniform data
uniform mat4 uProjectionMatrix;
uniform mat4 uModelViewMatrix;

//objectColor 
uniform vec3 uColor;  

//Lighting, Specular, Ambience
uniform vec3 testLightColor; //lightColor
vec3 lightDir = vec3(0.25,0.25,-1);
//vec3 lightDir = vec3( 1, 0.821, 0.241);
uniform vec3 shininess;
uniform vec3 specColor;
uniform vec3 ambience;
uniform float diffuseLevel;


//Interpolated values from the vertex shaders
//in vec2 texCoord;

//Values that stay constant for the whole mesh
//Note: GLSL has a built-in data-type for texture objects called a 
//sampler that takes as a postfix the texture type we want 
uniform sampler2D myTextureSampler;


// viewspace data (this must match the output of the fragment shader)
in VertexData {
	vec3 position;
	vec3 normal;
	vec2 textureCoord;
} f_in;

// framebuffer output
out vec4 fb_color;
out vec3 texColor; 

void main() {
	//note: viewDir would be the eye at this case
	// calculate lighting (hack)
	//vec3 eye = normalize(-f_in.position); //original code

	//vec3 eye = vec3(testLightColor.x, testLightColor.y, testLightColor.z); //plugging in the uniform light values 
	//float light = abs(dot(normalize(f_in.normal), eye));

	//ambience-----------
	//float ambientStrength = 0.4; 
	vec3 ambient = ambience * testLightColor; 
	
	//diffuse------------ 
	vec3 norm = normalize(f_in.normal); 
	lightDir = normalize(-lightDir);
	//vec3 lightDirec = normalize(lightDir - f_in.position);
	float diff = max(dot(norm, lightDir), diffuseLevel); 
	vec3 diffuse = diff * testLightColor; 

	//specular----------- 
	//float specularStrength = 0.5; this would be equivalent to shininess 
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 viewDir = normalize(-f_in.position); 
	//float spec = pow(max(dot(reflectDir, viewDir), 0.0), 64); 
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64); //can change how shiny the object can be
	vec3 specular = shininess * spec * specColor; // here it would be spec * the specular color instead
	//vec3 specular = shininess * spec * lightDir * testLightColor;
	//vec3 color = mix(uColor / 4, uColor, light); //original code
	
	//Output color = color of the texture at the specified UV
	//vec3 texColor = texture(myTextureSampler, f_in.textureCoord).rgb;

	//vec3 result =  (ambient + diffuse ) * uColor * texColor + specular; //result is multiplied by the object colour here with texture Colour 
	vec3 result =  (ambient + diffuse ) * uColor  + specular; //result is multiplied by the object colour here

	
	//output to the frambuffer
	fb_color = vec4(result, 1.0);

	
}