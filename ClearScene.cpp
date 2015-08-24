//
// Created by Yury Rakhuba on 26/04/15.
//

#include "ClearScene.hpp"

#include <OpenGL/gl3.h>

ClearScene::ClearScene()
{
  glClearColor(0.4, 0.65, 0.65, 1.0);
}

void ClearScene::Update(double elapsedSeconds)
{
}

void ClearScene::Render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

IController & ClearScene::GetController()
{
  return m_controller;
}
