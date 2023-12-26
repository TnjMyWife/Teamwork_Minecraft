#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
 
varying vec2 texture_coord;
uniform sampler2D samp;
 
void main()
{
    if (gl_FrontFacing)
        {
            // 正面
            gl_FragColor = texture2D(samp, texture_coord);
        }
        else
        {
            // 背面，丢弃该片段
            discard;
        }
        
}

