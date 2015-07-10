//
// Created by Yury Rakhuba on 13/06/15.
//

#pragma once

#include "IScene.hpp"
#include "DummyController.hpp"
#include "SceneRegistry.hpp"
#include "ogl/Material.hpp"
#include "ogl/Buffer.hpp"
#include "ogl/ShaderProgram.hpp"

#include "glm/glm.hpp"

#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>

#include <vector>
#include <array>

class aiScene;
class aiNode;

class LoadModelScene : public IScene
{
public:
  LoadModelScene();
  ~LoadModelScene();

  void OnSize(int w, int h) override;
  void Update(double elapsedSeconds) override;
  void Render() override;
  IController & GetController() override;

private:
  using TVertexBuffer = std::unique_ptr<Buffer>;
  using TIndexBuffer = std::unique_ptr<Buffer>;

  TVertexBuffer AllocateBuffer(int size, void * data);

  struct MeshNode
  {
    MeshNode( int count, size_t materialIndex)
      : m_indicesCount(count)
      , m_materialIndex(materialIndex)
    {
    }

    TVertexBuffer m_vertices;
    TVertexBuffer m_normals;
    TVertexBuffer m_tangents;
    TVertexBuffer m_bitangents;
    TVertexBuffer m_texCoords;
    TIndexBuffer  m_indices;
    int m_indicesCount;
    size_t m_materialIndex;
  };

  struct Node
  {
    Node()
      : m_transform(1.0)
    {
    }

    std::vector<MeshNode> m_meshes;
    glm::mat4 m_transform;

    std::vector<std::unique_ptr<Node>> m_children;
  };

  void CreateMaterials(aiScene const * scene);

  void CreateScene(aiScene const * scene);
  void CreateSceneImpl(std::unique_ptr<Node> & parent, aiScene const * scene, aiNode const * node);
  void CalcSceneBounds(aiScene const * scene);
  void CalcSceneBoundsImpl(aiScene const * scene, aiNode const * node, glm::mat4 const & m);

  void BindBuffer(TVertexBuffer & buffer, GLint location, int componentCount);
  void RenderNode(std::unique_ptr<Node> & node, glm::mat4 const & m);

  std::unique_ptr<Node> m_sceneRoot;
  std::vector<Material> m_materials;
  glm::vec3 m_min, m_max;

private:
  DummyController m_controller;
  std::unique_ptr<ShaderProgram> m_prg;

  glm::mat4 m_projection;
  glm::mat4 m_modelView;
};

REGISTER_SCENE("LoadModelScene", LoadModelScene)
