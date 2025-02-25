varying vec2 v_uv; 

uniform sampler2D u_texture; 

uniform float u_time;
uniform vec2 u_size;


void main()
{  
    float angle = mod(u_time, 3.1415*2.0);
    
    vec2 pos = v_uv-0.5;

    vec2 x = vec2(cos(angle), -sin(angle));
    vec2 y = vec2(sin(angle),cos(angle));
    mat2 rot = mat2(x.xy, y.xy);
    pos = pos*rot;

    pos = pos + 0.5;
    pos = mod(pos, 1.0);
    vec4 color = texture2D(u_texture, pos);

    gl_FragColor = vec4(color.xyz, 1.0);
}