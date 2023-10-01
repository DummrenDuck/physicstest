#ifndef __CUBE_HPP__
#define __CUBE_HPP__


#include "mesh.hpp"
#include "shader.hpp"
#include "glm/ext.hpp"
#include "btBulletDynamicsCommon.h"


class Cube {

public:
  static const char *VERTEX_SHADER;
  static const char *FRAGMENT_SHADER;

  Cube(glm::vec4, glm::vec3);
  Cube();
  ~Cube();

  void draw() const;

private:
  glm::vec4 m_color;

  Shader m_shader;
  Mesh m_mesh;

  btTransform m_btTransform;
  btCollisionShape *m_collisionShape;
  btRigidBody *m_rigidBody;

};


#endif
