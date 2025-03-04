// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;

uniform sampler2D u_texture; 
uniform sampler2D u_normal; 

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

uniform vec3 u_flag;


void main()
{
	// Set the ouput color per pixel
	//vec3 color = normalize(v_world_normal);
	vec4 color = texture2D(u_texture, v_uv);
	vec4 normal = texture2D(u_normal, v_uv);
	normal = (normal*2.0) - 1.0;
	
	vec3 ka;
	vec3 kd;
	vec3 ks;
	vec3 N;

	if(u_flag.x==1.0){
		ka = color.xyz*u_ka;
		kd = color.xyz*u_kd;
	}else{
		ka = u_ka*vec3(0.8, 0.6, 0.2);
		kd = u_kd*vec3(0.8, 0.6, 0.2);
	}
	if(u_flag.y==1.0){
		ks = vec3(color.w);
	}else{
		ks = u_ks;
	}
	if(u_flag.z==1.0){
		N = normalize((u_model * vec4( normal.xyz, 0.0)).xyz);
	}else{
		N = normalize(v_world_normal);
	}

	vec3 V = normalize(u_eye - v_world_position);
	vec3 L = normalize(u_lightPos - v_world_position);

	vec3 R = normalize(reflect(-L, N));
	float d = distance(u_lightPos, v_world_position);

	vec3 It = ka*u_Ia + (u_I/pow(d, 2.0))*(kd*clamp(dot(L, N), 0.0, 1.0) + ks * pow(clamp(dot(R, V), 0.0, 1.0), u_s));

	gl_FragColor = vec4(It, 1.0);

}
