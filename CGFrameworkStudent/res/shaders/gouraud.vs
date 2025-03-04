// Global variables from the CPU
uniform mat4 u_model;
uniform mat4 u_viewprojection;
uniform vec3 u_eye;
uniform vec3 u_lightPos;
uniform vec3 u_Ia;  
uniform vec3 u_ka;  
uniform vec3 u_kd;  
uniform vec3 u_ks;  
uniform float u_s;  
uniform vec3 u_I;   

// Variables to pass to the fragment shader
varying vec2 v_uv;
varying vec3 v_color;

uniform sampler2D u_texture;

void main()
{
    v_uv = gl_MultiTexCoord0.xy;

    // Convert local position to world space
    vec3 world_position = (u_model * vec4(gl_Vertex.xyz, 1.0)).xyz;

    // Convert local normal to world space
	vec3 world_normal = (u_model * vec4( gl_Normal.xyz, 0.0)).xyz;

    // Compute view and light directions
    vec3 V = normalize(u_eye - world_position);
    vec3 L = normalize(u_lightPos - world_position);
    vec3 R = reflect(-L, world_normal);
    vec3 N = normalize(world_normal);

    // Compute distance attenuation
    float d = distance(u_lightPos, world_position);
    float attenuation = 1.0 / pow(d, 2.0);

    // Compute ambient, diffuse, and specular lighting
    vec3 ambient = u_ka * u_Ia;
    vec3 diffuse = u_kd * u_I * attenuation * max(dot(L, N), 0.0);
    vec3 specular = u_ks * u_I * attenuation * pow(max(dot(R, V), 0.0), u_s);

    // Compute final vertex color
    v_color = ambient + diffuse + specular;

	// Project the vertex using the model view projection matrix
    gl_Position = u_viewprojection * vec4(world_position, 1.0);
}