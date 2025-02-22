varying vec2 v_uv; 

uniform sampler2D u_texture; 

void main()
{  
    float blur = 0.01;
    vec4 c1 = texture2D( u_texture, vec2(v_uv.x, v_uv.y));
    vec4 c2 = texture2D( u_texture, vec2(v_uv.x+blur, v_uv.y));
    vec4 c3 = texture2D( u_texture, vec2(v_uv.x, v_uv.y+blur));
    vec4 c4 = texture2D( u_texture, vec2(v_uv.x+blur, v_uv.y+blur));
    vec4 c5 = texture2D( u_texture, vec2(v_uv.x-blur, v_uv.y));
    vec4 c6 = texture2D( u_texture, vec2(v_uv.x, v_uv.y-blur));
    vec4 c7 = texture2D( u_texture, vec2(v_uv.x-blur, v_uv.y-blur));
    vec4 c8 = texture2D( u_texture, vec2(v_uv.x+blur, v_uv.y-blur));
    vec4 c9 = texture2D( u_texture, vec2(v_uv.x-blur, v_uv.y+blur));

    //ec4 col = (c1+c2+c3+c4+c5+c6+c7+c8+c9)/9.0;
    vec4 col = (c1+c4+c7+c8+c9)/5.0;




    gl_FragColor = vec4(col.xyz, 1.0);
}