attribute vec3 a_position;

uniform mat4 u_modelView;
uniform mat4 u_projection;

void main(void)
{
  gl_Position = vec4(a_position, 1.0) * u_modelView * u_projection;
}
