#version 330

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec3 a_tangent;
layout(location = 3) in vec3 a_bitangent;
layout(location = 4) in vec2 a_diffuseTexCoord;

out vec3 v_normal;
out vec3 v_tangent;
out vec3 v_bitangent;
out vec2 v_diffuseTexCoord;

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
