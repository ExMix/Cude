//
// Created by Yury Rakhuba on 13/06/15.
//

#include "LoadModelScene.hpp"

#include "base/Platform.hpp"
#include "base/assert.hpp"
#include "ogl/GLUtils.hpp"

#include <assimp/DefaultLogger.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/types.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Assimp;

glm::mat4 Convert(aiMatrix4x4 m)
{
  return glm::mat4(m.a1, m.b1, m.c1, m.d1,
                   m.a2, m.b2, m.c2, m.d2,
                   m.a3, m.b3, m.c3, m.d3,
                   m.a4, m.b4, m.c4, m.d4);
}

glm::vec4 Convert(aiVector3D const & v)
{
  return glm::vec4(v.x, v.y, v.z, 1.0);
}

LoadModelScene::LoadModelScene()
{
  Importer importer;

  unsigned int postProcessFlags = aiProcess_CalcTangentSpace |
                                  aiProcess_Debone |
                                  aiProcess_JoinIdenticalVertices |
                                  aiProcess_Triangulate |
                                  aiProcess_SortByPType;

  DefaultLogger::create("Default", Logger::VERBOSE);
  DefaultLogger::get()->attachStream(LogStream::createDefaultStream(aiDefaultLogStream_STDOUT, nullptr), Logger::Info);

  std::string sceneName = "Batman.obj1";
  aiScene const * scene = importer.ReadFile(Platform::GetResourcePath(sceneName), postProcessFlags);
  CreateMaterials(scene);
  CreateScene(scene);
  CalcSceneBounds(scene);

  GLCHECK(glClearColor(0.65, 0.65, 0.65, 1.0));
  GLCHECK(glEnable(GL_DEPTH_TEST));
  GLCHECK(glEnable(GL_BLEND));
  GLCHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  GLCHECK(glBlendEquation(GL_FUNC_ADD));

  m_prg.reset(new ShaderProgram(Platform::GetReader("model.vert"),
                                Platform::GetReader("model.frag")));
}

LoadModelScene::~LoadModelScene()
{
  DefaultLogger::kill();
}

void LoadModelScene::OnSize(int w, int h)
{
  w = std::max(w, 2);
  h = std::max(h, 2);

  if (w < h)
    std::swap(w, h);
  m_projection = glm::perspective(glm::radians(60.0), w / (double)h, 0.1, 10000.0);
}

void LoadModelScene::Update(double elapsedSeconds)
{
  static float angle = 0.0f;
  angle += elapsedSeconds;

  glm::vec3 positionOffset(1.0f, 0.0f, 0.0f);
  glm::mat4 m = glm::rotate(glm::mat4(1.0), angle, glm::vec3(0.0f, 1.0f, 0.0f));
  positionOffset = 1.3f * glm::length(m_min - m_max) * ((glm::vec4(positionOffset, 0.0) * m).xyz());

  glm::vec3 modelCenter = 0.5f * (m_min + m_max);
  m_modelView = glm::lookAt(modelCenter + positionOffset, modelCenter, glm::vec3(0.0f, 1.0f, 0.0f));
}

void LoadModelScene::Render()
{
  GLCHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

  m_prg->Bind();

  GLint projectionLoc = m_prg->GetUniform("u_projection");
  ASSERT(projectionLoc != -1, "");
  GLCHECK(glUniformMatrix4fv(projectionLoc, 1, GL_TRUE, glm::value_ptr(m_projection)));

  GLint ambientColorLoc = m_prg->GetUniform("u_ambientColor");
  if (ambientColorLoc != -1)
    GLCHECK(glUniform4f(ambientColorLoc, 0.1f, 0.1f, 0.1f, 0.1f));

  GLint lightLoc = m_prg->GetUniform("u_lightVector");
  if (lightLoc != -1)
  {
    glm::vec4 ligthDir(1.0, 0.0, 0.4, 0.0);
    ligthDir = glm::normalize(ligthDir * m_modelView);
    GLCHECK(glUniform3f(lightLoc, ligthDir.x, ligthDir.y, ligthDir.z));
  }

  RenderNode(m_sceneRoot, m_modelView);
}

void LoadModelScene::BindBuffer(TVertexBuffer & buffer, GLuint location, int componentCount)
{
  buffer->Bind();

  ASSERT(location != -1, "");
  GLCHECK(glEnableVertexAttribArray(location));
  GLCHECK(glVertexAttribPointer(location, componentCount, GL_FLOAT, GL_FALSE, componentCount * sizeof(float), nullptr));
}

void LoadModelScene::RenderNode(std::unique_ptr<Node> & node, glm::mat4 const & m)
{
  GLint diffuseLoc = m_prg->GetUniform("u_duffiseTexture");
  GLint normalLoc = m_prg->GetUniform("u_normalTexture");

  glm::mat4 nodeM = m * node->m_transform;
  GLint modelViewLoc = m_prg->GetUniform("u_modelView");
  ASSERT(modelViewLoc != -1, "");
  GLCHECK(glUniformMatrix4fv(modelViewLoc, 1, GL_TRUE, glm::value_ptr(nodeM)));

  for (size_t i = 0; i < node->m_meshes.size(); ++i)
  {
    MeshNode & mesh = node->m_meshes[i];

    if (!mesh.m_texCoords)
      continue;

    m_materials[mesh.m_materialIndex].Apply(0, 1);
    if (diffuseLoc != -1)
      GLCHECK(glUniform1i(diffuseLoc, 0));
    if (normalLoc != -1)
      GLCHECK(glUniform1i(normalLoc, 1));

    mesh.m_indices->Bind();
    BindBuffer(mesh.m_vertices, m_prg->GetAttribute("a_position"), 3);
    BindBuffer(mesh.m_normals, m_prg->GetAttribute("a_normal"), 3);
    BindBuffer(mesh.m_tangents, m_prg->GetAttribute("a_tangent"), 3);
    BindBuffer(mesh.m_bitangents, m_prg->GetAttribute("a_bitangent"), 3);
    BindBuffer(mesh.m_texCoords, m_prg->GetAttribute("a_diffuseTexCoord"), 2);

    GLCHECK(glDrawElements(GL_TRIANGLES, mesh.m_indicesCount, GL_UNSIGNED_INT, nullptr));
  }

  for (size_t i = 0; i < node->m_children.size(); ++i)
    RenderNode(node->m_children[i], nodeM);
}

IController & LoadModelScene::GetController()
{
  return m_controller;
}

void LoadModelScene::CreateScene(aiScene const * scene)
{
  m_sceneRoot.reset(new Node());
  CreateSceneImpl(m_sceneRoot, scene, scene->mRootNode);
}

LoadModelScene::TVertexBuffer LoadModelScene::AllocateBuffer(int size, void * data)
{
  TVertexBuffer buffer(new Buffer(Buffer::Type::VertexBuffer));
  buffer->Create();
  buffer->Allocate(size, data);

  return std::move(buffer);
}

void LoadModelScene::CreateSceneImpl(std::unique_ptr<Node> & root, aiScene const * scene, aiNode const * node)
{
  root->m_transform = Convert(node->mTransformation);

  for (uint32_t meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex)
  {
    aiMesh const * mesh = scene->mMeshes[node->mMeshes[meshIndex]];
    if (!mesh->HasTextureCoords(0))
      return;

    //////////////////////////////////////////////////////
    //      Prepare texture coordinates                 //
    unsigned int numVertices = mesh->mNumVertices;
    std::vector<aiVector2D> coordBuffer;
    coordBuffer.reserve(numVertices);
    for (int i = 0; i < numVertices; ++i)
    {
      aiVector3D & coord = mesh->mTextureCoords[0][i];
      coordBuffer.push_back(aiVector2D(coord.x, coord.y));
    }

    //////////////////////////////////////////////////////
    //            Prepare indeces                       //
    std::vector<uint32_t> indices;
    for (int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
    {
      aiFace face = mesh->mFaces[faceIndex];
      if (face.mNumIndices != 3)
        continue;

      indices.insert(indices.end(), face.mIndices, face.mIndices + 3);
    }

    root->m_meshes.emplace_back(indices.size(), mesh->mMaterialIndex);
    MeshNode & meshNode = root->m_meshes.back();
    ASSERT(indices.size() < std::numeric_limits<uint16_t>::max(), "");
    TIndexBuffer indexBuffer(new Buffer(Buffer::Type::IndexBuffer));
    indexBuffer->Create();
    indexBuffer->Allocate(sizeof(uint32_t) * indices.size(), indices.data());
    meshNode.m_indices = std::move(indexBuffer);
    meshNode.m_vertices = AllocateBuffer(sizeof(aiVector3D) * numVertices, mesh->mVertices);
    meshNode.m_texCoords = AllocateBuffer(sizeof(aiVector2D) * numVertices, coordBuffer.data());
    meshNode.m_normals = AllocateBuffer(sizeof(aiVector3D) * numVertices, mesh->mNormals);
    meshNode.m_tangents = AllocateBuffer(sizeof(aiVector3D) * numVertices, mesh->mTangents);
    meshNode.m_bitangents = AllocateBuffer(sizeof(aiVector3D) * numVertices, mesh->mBitangents);
  }

  for (uint32_t nodeIndex = 0; nodeIndex < node->mNumChildren; ++nodeIndex)
  {
    root->m_children.emplace_back(new Node());
    CreateSceneImpl(root->m_children.back(), scene, node->mChildren[nodeIndex]);
  }
}

void LoadModelScene::CalcSceneBounds(aiScene const * scene)
{
  m_min = glm::vec3( 1e10f,  1e10f,  1e10f);
  m_max = glm::vec3(-1e10f, -1e10f, -1e10f);
  CalcSceneBoundsImpl(scene, scene->mRootNode, glm::mat4(1.0));
}

void LoadModelScene::CalcSceneBoundsImpl(aiScene const * scene, aiNode const * node, glm::mat4 const & m)
{
  glm::mat4 nodeM = m * Convert(node->mTransformation);

  for (uint32_t meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex)
  {
    aiMesh const * mesh = scene->mMeshes[node->mMeshes[meshIndex]];
    for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex)
    {
      glm::vec4 v = nodeM * Convert(mesh->mVertices[vertexIndex]);

      m_min.x = std::min(m_min.x, v.x);
      m_min.y = std::min(m_min.y, v.y);
      m_min.z = std::min(m_min.z, v.z);

      m_max.x = std::max(m_max.x, v.x);
      m_max.y = std::max(m_max.y, v.y);
      m_max.z = std::max(m_max.z, v.z);
    }
  }

  for (uint32_t childIndex = 0; childIndex < node->mNumChildren; ++childIndex)
    CalcSceneBoundsImpl(scene, node->mChildren[childIndex], nodeM);
}

void LoadModelScene::CreateMaterials(aiScene const * scene)
{
  for (int i = 0; i < scene->mNumMaterials; ++i)
  {
    m_materials.emplace_back();
    m_materials.back().Init(scene->mMaterials[i]);
  }
}
