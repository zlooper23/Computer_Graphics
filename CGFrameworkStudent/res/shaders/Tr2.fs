varying vec2 v_uv; 

uniform sampler2D u_texture; 

uniform float u_time;
uniform vec2 u_size;


void main()
{  
    vec2 size = u_size/40.0;
    float Pixels = 2048.0*abs(sin(u_time*0.1));
    float dx = size.x * (1.0/Pixels);
    float dy = size.y * (1.0/Pixels);
    vec2 Coord = vec2(dx * floor(v_uv.x / dx), dy * floor(v_uv.y / dy));
    vec4 color = texture2D(u_texture, Coord);

    gl_FragColor = vec4(color.xyz, 1.0);
}