//
// Created by Yury Rakhuba on 26/04/15.
//
#pragma once

#include "IScene.hpp"
#include "DummyController.hpp"
#include "SceneRegistry.hpp"

class ClearScene : public IScene
{
public:
  ClearScene();

  void OnSize(int w, int h) override {}
  void Update(double elapsedSeconds) override;
  void Render() override;
  IController & GetController() override;

private:
  DummyController m_controller;
};

REGISTER_SCENE("Clear Scene", ClearScene);
