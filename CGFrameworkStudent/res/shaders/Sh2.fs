varying vec2 v_uv; 

void main()
{  
    float d = distance(v_uv,vec2(0.5));
    vec3 color = vec3(d);
    gl_FragColor = vec4( color, 1.0 );
}


