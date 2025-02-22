varying vec2 v_uv; 

uniform sampler2D u_texture; 

void main()
{  
    vec4 color = texture2D( u_texture, v_uv );

    float d = distance(v_uv,vec2(0.5));

    gl_FragColor = vec4(color.xyz-d*0.55, 1.0);
}