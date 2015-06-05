//
// Created by Yury Rakhuba on 24/04/15.
//

#include "SceneRegistry.hpp"

SceneRegistry::SceneRegistry(std::string && title, SceneRegistry::TSceneCreator && creator)
  : m_title(std::move(title))
  , m_creatorFn(std::move(creator))
{
  RegisterScene(this);
}

void SceneRegistry::EnumerateScenes(SceneRegistry::TSceneEnumeratorCallback const & callback)
{
  SceneRegistry * head = m_firstScene;
  while (head != nullptr)
  {
    callback(head->m_title, head->m_creatorFn);
    head = head->m_nextScene;
  }
}

void SceneRegistry::RegisterScene(SceneRegistry * scene)
{
  if (m_firstScene == nullptr)
    m_firstScene = scene;
  else
  {
    SceneRegistry * head = m_firstScene;
    while (head->m_nextScene != nullptr)
      head = head->m_nextScene;

    head->m_nextScene = scene;
  }
}

SceneRegistry * SceneRegistry::m_firstScene;
