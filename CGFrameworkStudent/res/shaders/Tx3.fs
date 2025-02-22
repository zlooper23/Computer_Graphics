varying vec2 v_uv; 

uniform sampler2D u_texture; 

void main()
{  
    vec4 color = texture2D( u_texture, v_uv );

    float gray = 0.299*color.x+0.587*color.y+0.114*color.z;

    gl_FragColor = vec4(gray, gray, 0.0, 1.0);
}