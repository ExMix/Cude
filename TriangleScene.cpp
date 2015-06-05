//
// Created by Yury Rakhuba on 11/05/15.
//

#include "TriangleScene.hpp"
#include "DummyController.hpp"

#include "ogl/GLUtils.hpp"

#include "base/Platform.hpp"
#include "base/assert.hpp"
#include "base/math.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

TriangleScene::TriangleScene()
  : m_vertexBuffer(Buffer::Type::VertexBuffer)
{
  float vertices[] =
  {
    1.0, 1.0, 1.0,
    2.0, 3.0, 1.0,
    3.0, 1.0, 1.0
  };

  m_vertexBuffer.Create();
  m_vertexBuffer.Bind();
  m_vertexBuffer.Allocate(sizeof(float) * std::distance(std::begin(vertices), std::end(vertices)), vertices);

  m_prg.reset(new ShaderProgram(Platform::GetReader("simple.vsh"),
                                Platform::GetReader("solid.fsh")));

  GLCHECK(glClearColor(0.65f, 0.65f, 0.65f, 1.0f));
}

TriangleScene::~TriangleScene()
{
  m_vertexBuffer.Destroy();
}

void TriangleScene::Update(double elapsedSeconds)
{
  static float s_rotationAngle = 0.0f;
  s_rotationAngle += math::pi2 * elapsedSeconds;

  glm::mat4 shiftM = glm::translate(glm::mat4(1.0), glm::vec3(2.0f, 2.0f, 0.0f));
  glm::mat4 backShiftM = glm::translate(glm::mat4(1.0), glm::vec3(-2.0f, -2.0f, 0.0f));
  glm::mat4 rotateM = glm::rotate(glm::mat4(1.0), s_rotationAngle, glm::vec3(0.0, 1.0, 0.0));
  glm::vec4 eye(2.0f, 2.0f, -5.0f, 1.0f);

  eye = backShiftM * rotateM * shiftM * eye;

  m_modelView = glm::lookAt(eye.xyz(),
                            glm::vec3(2.0f, 2.0f, 0.0f),
                            glm::vec3(0.0f, 1.0f, 0.0f));
  m_modelViewChanged = true;
}

void TriangleScene::Render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_prg->Bind();

  if (m_projectionChanged)
  {
    GLint projectionLoc = m_prg->GetUniform("u_projection");
    ASSERT(projectionLoc != -1, "");
    GLCHECK(glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, glm::value_ptr(m_projection)));
    m_projectionChanged = false;
  }

  if (m_modelViewChanged)
  {
    GLint modelViewLoc = m_prg->GetUniform("u_modelView");
    ASSERT(modelViewLoc != -1, "");
    GLCHECK(glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, glm::value_ptr(m_modelView)));
    m_modelViewChanged = false;
  }

  GLint attribLocation = m_prg->GetAttribute("a_position");
  ASSERT(attribLocation != -1, "");
  GLCHECK(glEnableVertexAttribArray(attribLocation));
  GLCHECK(glVertexAttribPointer(attribLocation, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), nullptr));

  GLCHECK(glDrawArrays(GL_TRIANGLES, 0, 3));
}

IController & TriangleScene::GetController()
{
  static DummyController controller;
  return controller;
}

void TriangleScene::OnSize(int w, int h)
{
  float fW = w;
  float fH = h;
  if (fW < fH)
    std::swap(fW, fH);

  m_projection = glm::perspective(static_cast<float>(math::pi2), fW / fH, 1.0f, 100.0f);
  m_projectionChanged = true;
}
