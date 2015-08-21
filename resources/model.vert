attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec3 a_tangent;
attribute vec3 a_bitangent;
attribute vec2 a_diffuseTexCoord;

varying vec3 v_normal;
varying vec3 v_tangent;
varying vec3 v_bitangent;
varying vec2 v_diffuseTexCoord;

uniform mat4 u_modelView;
uniform mat4 u_projection;
uniform vec3 u_lightPosition;
uniform vec3 u_viewVector;

void main(void)
{
  v_normal = a_normal;
  v_tangent = a_tangent;
  v_bitangent = a_bitangent;
  v_diffuseTexCoord = a_diffuseTexCoord;
  gl_Position = vec4(a_position, 1.0) * u_modelView * u_projection;
}
