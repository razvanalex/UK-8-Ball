#version 330

// get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 color;

layout(location = 0) out vec4 out_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
	vec3 N = world_normal;
	vec3 V = normalize(eye_position - world_position);
	vec3 L = normalize(-light_direction);

	float ambient_light, diffuse_light, specular_light;
	
	// Define ambient light component
	ambient_light = 0.25;

	// Compute diffuse light component
	diffuse_light = material_kd * max(dot(L, N), 0);

	// Compute specular light component
	specular_light = 0;
	
	if (diffuse_light > 0)
	{
		vec3 R = -reflect(L, N);
		specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
	}

	// Compute light
	float light = ambient_light + diffuse_light + specular_light;
	vec3 color = light * color;

	// Write pixel out color
	out_color = vec4(color, 1);
}