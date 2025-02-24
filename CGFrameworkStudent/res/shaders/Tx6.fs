varying vec2 v_uv; 

uniform sampler2D u_texture; 

uniform float u_time;

uniform mat4 u_projviewmatrix;

void main()
{  
    vec2 blur = vec2(0.003);
    vec4 col = vec4(0.0, 0.0, 0.0, 1.0);
    col += texture2D( u_texture, vec2(v_uv.x, v_uv.y));
    col += texture2D( u_texture, vec2(v_uv.x+blur.x, v_uv.y));
    col += texture2D( u_texture, vec2(v_uv.x, v_uv.y+blur.y));
    col += texture2D( u_texture, vec2(v_uv.x+blur.x, v_uv.y+blur.y));
    col += texture2D( u_texture, vec2(v_uv.x-blur.x, v_uv.y));
    col += texture2D( u_texture, vec2(v_uv.x, v_uv.y-blur.y));
    col += texture2D( u_texture, vec2(v_uv.x-blur.x, v_uv.y-blur.y));
    col += texture2D( u_texture, vec2(v_uv.x+blur.x, v_uv.y-blur.y));
    col += texture2D( u_texture, vec2(v_uv.x-blur.x, v_uv.y+blur.y));
    col /= 9.0;

    gl_FragColor = vec4(col.xyz, 1.0);
}