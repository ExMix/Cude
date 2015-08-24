#version 330

in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_bitangent;
in vec2 v_diffuseTexCoord;

out vec4 color;

uniform vec4 u_ambientColor;

uniform sampler2D u_duffiseTexture;
uniform sampler2D u_normalTexture;
uniform vec3 u_lightVector;

uniform mat4 u_modelView;

void main(void)
{
  vec3 normal_tangentSpace = (u_modelView * vec4(v_normal, 0.0)).xyz;
  vec3 tangent_tangentSpace = (u_modelView * vec4(v_tangent, 0.0)).xyz;
  vec3 bitangent_tangentSpace = (u_modelView * vec4(v_bitangent, 0.0)).xyz;

  mat3 TBN = transpose(mat3(tangent_tangentSpace,
                            bitangent_tangentSpace,
                            normal_tangentSpace));

  vec3 light_tangetSpace = TBN * normalize(u_lightVector);
  vec3 normal_tex = normalize(2.0 * texture(u_normalTexture, v_diffuseTexCoord).rgb - 1.0);

  vec3 diffuseLight = texture(u_duffiseTexture, v_diffuseTexCoord).rgb * dot(normal_tex, normalize(light_tangetSpace));
  vec3 ambientLight = diffuseLight * vec3(u_ambientColor.r, u_ambientColor.g, u_ambientColor.b) * u_ambientColor.w;

  color = vec4(diffuseLight + ambientLight, 1.0);
}
