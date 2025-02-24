varying vec2 v_uv; 

uniform sampler2D u_texture; 

uniform float u_time;
uniform vec2 u_size;

uniform mat4 u_projviewmatrix;

void main()
{  
    float Pixels = 512.0*2.0;
    float dx = u_size.x * (1.0 / Pixels) / 40.0;
    float dy = u_size.y * (1.0 / Pixels) / 40.0;
    vec2 Coord = vec2(dx * floor(v_uv.x / dx), dy * floor(v_uv.y / dy));
    vec4 color = texture2D(u_texture, Coord);

    gl_FragColor = vec4(color.xyz, 1.0);
}