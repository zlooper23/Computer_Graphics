varying vec2 v_uv; 

void main()
{  
    float s = step(v_uv.y, 0.35*sin((v_uv.x)*6.5)+0.5);
    gl_FragColor = vec4(0.0, 1.0-abs(s-v_uv.y), 0.0, 1.0);
}