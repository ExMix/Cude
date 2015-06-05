//
// Created by Yury Rakhuba on 11/05/15.
//

#pragma once

#include "IScene.hpp"
#include "SceneRegistry.hpp"

#include "ogl/ShaderProgram.hpp"
#include "ogl/Buffer.hpp"

#include "glm/glm.hpp"

#include <memory>

class TriangleScene : public IScene
{
public:
  TriangleScene();
  ~TriangleScene();

  void OnSize(int w, int h) override;
  void Update(double elapsedSeconds) override;
  void Render() override;
  IController & GetController() override;

private:
  std::unique_ptr<ShaderProgram> m_prg;
  Buffer m_vertexBuffer;

  glm::mat4 m_modelView;
  glm::mat4 m_projection;

  bool m_modelViewChanged;
  bool m_projectionChanged;

};

REGISTER_SCENE("Triangle Scene", TriangleScene);


