void main(void)
{
  vec4 color;
  if (gl_FrontFacing)
    color = vec4(0.0, 0.0, 1.0, 1.0);
  else
    color = vec4(0.0, 1.0, 0.0, 1.0);

  gl_FragColor = color;
}
