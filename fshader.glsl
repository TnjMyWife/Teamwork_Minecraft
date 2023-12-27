varying vec2 texture_coord;
varying vec3 texture_color;
uniform sampler2D samp;
 
void main()
{
    if (gl_FrontFacing)
        {
            // ����
            vec4 texColor = texture2D(samp, texture_coord);
            gl_FragColor = texColor * vec4(texture_color, 1.0);
        }
        else
        {
            // ���棬������Ƭ��
            discard;
        }
        
}

