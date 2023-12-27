uniform mat4 mvpMat;
attribute vec4 attrPos;
attribute vec2 attrTexCoord;
attribute vec3 attrTexColor;
varying vec2 texture_coord;
varying vec3 texture_color;
 
void main()
{
    gl_Position = mvpMat * attrPos;
    texture_coord = attrTexCoord;
    texture_color = attrTexColor;
}

