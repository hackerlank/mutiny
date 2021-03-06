#include "ParticleRenderer.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "Shader.h"
#include "Transform.h"
#include "GameObject.h"
#include "Material.h"
#include "Vector2.h"
#include "Debug.h"

#include <memory>
#include <functional>

namespace mutiny
{

namespace engine
{

ParticleRenderer::~ParticleRenderer()
{

}

void ParticleRenderer::awake()
{
  material = NULL;

  glGenBuffers(1, &positionBufferId);
  // TODO:
  //_positionBufferId.reset(&positionBufferId, std::bind(glDeleteBuffers, 1, &positionBufferId));
  glGenBuffers(1, &uvBufferId);
  // TODO:
  //_uvBufferId.reset(&uvBufferId, std::bind(glDeleteBuffers, 1, &uvBufferId));

  std::vector<float> values;
  std::vector<Vector3> vertices;

  vertices.push_back(Vector3(-1, 1, 0));
  vertices.push_back(Vector3(-1, -1, 0));
  vertices.push_back(Vector3(1, -1, 0));
  vertices.push_back(Vector3(1, -1, 0));
  vertices.push_back(Vector3(1, 1, 0));
  vertices.push_back(Vector3(-1, 1, 0));

  for(size_t i = 0; i < vertices.size(); i++)
  {
    values.push_back(vertices.at(i).x);
    values.push_back(vertices.at(i).y);
    values.push_back(vertices.at(i).z);
  }

  glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
  glBufferData(GL_ARRAY_BUFFER, values.size() * sizeof(values[0]), &values[0], GL_STATIC_DRAW);

  values.clear();
  std::vector<Vector2> uv;

  uv.push_back(Vector2(0, 1));
  uv.push_back(Vector2(0, 0));
  uv.push_back(Vector2(1, 0));
  uv.push_back(Vector2(1, 0));
  uv.push_back(Vector2(1, 1));
  uv.push_back(Vector2(0, 1));

  for(size_t i = 0; i < uv.size(); i++)
  {
    values.push_back(uv.at(i).x);
    values.push_back(uv.at(i).y);
  }

  glBindBuffer(GL_ARRAY_BUFFER, uvBufferId);
  glBufferData(GL_ARRAY_BUFFER, values.size() * sizeof(values[0]), &values[0], GL_STATIC_DRAW);
}

void ParticleRenderer::render()
{
  ref<Shader> shader;
  ref<Transform> transform = getGameObject()->getTransform();
  ref<ParticleEmitter> emitter = getGameObject()->getComponent<ParticleEmitter>();

  if(emitter.expired())
  {
    Debug::logWarning("No particle emitter attached");
    return;
  }

  if(transform.expired())
  {
    Debug::log("Failed to find Transform");
    return;
  }

  if(material.expired())
  {
    material = Application::context->particleMaterial;
    //Debug::log("ParticleRenderer set to default material");
  }

  shader = material->getShader();

  Vector3 axisMod;
  Matrix4x4 viewMat = Matrix4x4::getIdentity();
  axisMod = Camera::getCurrent()->getGameObject()->getTransform()->getRotation();
  axisMod.x *= -1;
  axisMod.y *= -1;
  viewMat = viewMat.rotate(axisMod);
  axisMod = Camera::getCurrent()->getGameObject()->getTransform()->getPosition();
  axisMod.x *= -1;
  axisMod.y *= -1;
  viewMat = viewMat.translate(axisMod);

  material->setMatrix("in_Projection", Camera::getCurrent()->getProjectionMatrix());
  material->setMatrix("in_View", viewMat);

  GLint positionAttribId = glGetAttribLocation(shader->programId->getGLuint(), "in_Position");

  if(positionAttribId != -1)
  {
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferId);
    glVertexAttribPointer(positionAttribId, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(positionAttribId);
  }

  GLint uvAttribId = glGetAttribLocation(shader->programId->getGLuint(), "in_Uv");

  if(uvAttribId != -1)
  {
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferId);
    glVertexAttribPointer(uvAttribId, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(uvAttribId);
  }

  glDisable(GL_DEPTH_TEST);
  for(size_t i = 0; i < emitter->particles.size(); i++)
  {
    Matrix4x4 modelMat = Matrix4x4::getIdentity();
    axisMod = emitter->particles.at(i).position;
    axisMod.z *= -1;
    modelMat = modelMat.translate(axisMod);
    modelMat = modelMat.rotate(Vector3(0, 0, 180.0f));

    material->setMatrix("in_Model", modelMat);

    material->setPass(0, material);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }
  glEnable(GL_DEPTH_TEST);

  if(positionAttribId != -1)
  {
    glDisableVertexAttribArray(positionAttribId);
  }

  if(uvAttribId != -1)
  {
    glDisableVertexAttribArray(uvAttribId);
  }
}

void ParticleRenderer::setMaterial(ref<Material> material)
{
  this->material = material;
}

ref<Material> ParticleRenderer::getMaterial()
{
  return material;
}

}

}

