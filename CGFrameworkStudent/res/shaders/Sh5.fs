varying vec2 v_uv; 

void main()
{  
    float a = mod(floor(v_uv.x*16.0), 2.0);
    float b = mod(floor(v_uv.y*16.0), 2.0);
    float s = abs(a-b);

    gl_FragColor = vec4(s, s, s, 1.0);
}