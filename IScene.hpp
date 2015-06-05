//
// Created by Yury Rakhuba on 24/04/15.
//

#pragma once

#include "IController.hpp"

class IScene
{
public:
  virtual ~IScene(){}
  virtual void OnSize(int w, int h) = 0;
  virtual void Update(double elapsedSeconds) = 0;
  virtual void Render() = 0;
  virtual IController & GetController() = 0;
};
