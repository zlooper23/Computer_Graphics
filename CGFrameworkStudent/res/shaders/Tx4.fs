varying vec2 v_uv; 

uniform sampler2D u_texture; 

void main()
{  
    vec4 color = texture2D( u_texture, v_uv );

    float gray = 0.299*color.x+0.587*color.y+0.114*color.z;

    float s = step(0.65, gray);

    gl_FragColor = vec4(s, s, s, 1.0);
}