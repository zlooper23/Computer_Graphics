// Interpolated variables from the vertex shader
varying vec2 v_uv;
varying vec3 v_color;

uniform sampler2D u_texture;

void main()
{
    vec3 textureColor = texture2D(u_texture, v_uv).rgb;

    gl_FragColor = vec4(v_color, 1.0);
}