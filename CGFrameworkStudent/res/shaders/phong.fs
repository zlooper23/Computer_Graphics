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
uniform float u_s;
uniform vec3 u_I;


void main()
{
	// Set the ouput color per pixel
	//vec3 color = normalize(v_world_normal);
	vec4 color = texture2D(u_texture, v_uv);

	vec3 V = normalize(u_eye - v_world_position);
	vec3 L = normalize(u_lightPos - v_world_position);
	vec3 N = normalize(v_world_normal);
	vec3 R = normalize(reflect(-L, N));
	float d = distance(u_lightPos, v_world_position);

	//vec3 It = u_ka*u_Ia + (u_I/pow(d, 2.0))*(u_kd*clamp(dot(L, N), 0.0, 1.0) + u_ks * pow(clamp(dot(R, V), 0.0, 1.0), u_s));
	vec3 It = color.xyz*u_Ia + (u_I/pow(d, 2.0))*(color.xyz*clamp(dot(L, N), 0.0, 1.0) + vec3(color.w) * pow(clamp(dot(R, V), 0.0, 1.0), u_s));


	//gl_FragColor = vec4(It*color.xyz, 1.0);
    //gl_FragColor = vec4(It*vec3(0.8, 0.651, 0.118), 1.0);
	gl_FragColor = vec4(It, 1.0);

}
