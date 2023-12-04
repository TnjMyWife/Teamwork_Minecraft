
#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif
 
uniform mat4 mvpMat;
attribute vec4 attrPos;
attribute vec2 attrTexCoord;
varying vec2 texture_coord;
 
void main()
{
    gl_Position = mvpMat * attrPos;
    texture_coord = attrTexCoord;
}

