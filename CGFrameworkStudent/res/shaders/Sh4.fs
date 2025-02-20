varying vec2 v_uv; 

void main()
{  
    float a = floor(v_uv.x*16.0)/16.0;
    float b = floor(v_uv.y*16.0)/16.0;

    gl_FragColor = vec4(a, b, 0.0, 1.0);
}