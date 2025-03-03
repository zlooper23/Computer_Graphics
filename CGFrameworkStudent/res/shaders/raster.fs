// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;

uniform sampler2D u_texture; 

uniform mat4 u_model;
uniform mat4 u_viewprojection;
uniform vec3 u_eye;
uniform vec3 u_lightPos;
uniform vec3 u_Ia;
uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform vec3 u_s;


void main()
{
	// Set the ouput color per pixel
	//vec3 color = normalize(v_world_normal);
	vec4 color = texture2D(u_texture, v_uv);

	gl_FragColor = vec4( color);
}
