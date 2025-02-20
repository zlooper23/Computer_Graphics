varying vec2 v_uv; 

void main()
{  
    gl_FragColor = vec4((sin(v_uv.x*30.0)), 0.0, (sin(v_uv.y*30.0)), 1.0);
    //gl_FragColor = vec4(floor(mod(floor(v_uv.x*22.0), 4.0)/3.0) ,0.0, floor(mod(floor(v_uv.y*22.0), 4.0)/3.0), 1.0 );
}
