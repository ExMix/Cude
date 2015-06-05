//
// Created by Yury Rakhuba on 24/04/15.
//

#pragma once

#include "IScene.hpp"

#include <functional>
#include <memory>
#include <string>

class SceneRegistry
{
public:
  SceneRegistry(SceneRegistry const & other) = delete;
  SceneRegistry(SceneRegistry && other) = delete;
  SceneRegistry & operator=(SceneRegistry const & other) = delete;
  SceneRegistry & operator=(SceneRegistry && other) = delete;

  using TSceneCreator = std::function<std::unique_ptr<IScene> ()>;
  SceneRegistry(std::string && title, TSceneCreator && creator);

  using TSceneEnumeratorCallback = std::function<void (std::string const & title, TSceneCreator const & creatorFn)>;
  static void EnumerateScenes(TSceneEnumeratorCallback const & callback);

private:
  static void RegisterScene(SceneRegistry * scene);
  static SceneRegistry * m_firstScene;

private:
  SceneRegistry * m_nextScene = nullptr;
  std::string m_title;
  TSceneCreator m_creatorFn;
};

#define REGISTER_SCENE(title, className) \
  std::unique_ptr<IScene> Create##className();\
  static SceneRegistry SceneRegistry##className(title, std::bind(&Create##className));\
  std::unique_ptr<IScene> Create##className() { return std::unique_ptr<IScene>(new className()); }


