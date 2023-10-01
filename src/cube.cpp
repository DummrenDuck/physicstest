#include "cube.hpp"


#include <vector>
#include <cstdint>

#include "main.hpp"
#include "gl.hpp"
#include "glm/ext.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "btBulletDynamicsCommon.h"


const char *Cube::VERTEX_SHADER = "#version 330 core\n"
  "layout (location = 0) in vec3 v_pos;\n"
  "layout (location = 1) in vec3 v_norm;\n"
  "out vec3 f_pos, f_norm;\n"
  "uniform mat4 u_projMat, u_viewMat, u_transMat;\n"
  "void main() {\n"
  "  f_pos = v_pos;\n"
  "  f_norm = v_norm;\n"
  "  gl_Position = u_projMat * u_viewMat * u_transMat\n"
  "    * vec4(v_pos, 1.0f);"
  "}\n\0";

const char *Cube::FRAGMENT_SHADER = "#version 330 core\n"
  "in vec3 f_pos, f_norm;"
  "uniform vec4 u_color;\n"
  "uniform mat4 u_projMat, u_viewMat, u_transMat;\n"
  "uniform vec3 u_sunVector;\n"
  "out vec4 o_fragColor;\n"
  "vec3 sunColorMult() {\n"
  "  float factor = dot(vec3(mat3(u_transMat) * normalize(f_norm)),\n"
  "                     normalize(u_sunVector));\n"
  "  return vec3(max(0.0f, factor));"
  "}\n"
  "void main() {\n"
  "  "
  "  o_fragColor = u_color + vec4(sunColorMult() * 0.5f, 1.0f);\n"
  "}\n\0";


Cube::Cube(glm::vec4 t_color, glm::vec3 t_initPos) {
  m_color = t_color;

  std::vector<float> positions {
    1, 1, 1,
    -1, 1, 1,
    -1, -1, 1,
    1, -1, 1,

    1, 1, -1,
    1, -1, -1,
    -1, -1, -1,
    -1, 1, -1,

    -1, 1, 1,
    -1, 1, -1,
    -1, -1, -1,
    -1, -1, 1,

    1, 1, 1,
    1, -1, 1,
    1, -1, -1,
    1, 1, -1,

    1, 1, 1,
    1, 1, -1,
    -1, 1, -1,
    -1, 1, 1,

    1, -1, 1,
    -1, -1, 1,
    -1, -1, -1,
    1, -1, -1,
  };

  std::vector<float> normals {
    1, 1, 1,
    -1, 1, 1,
    -1, -1, 1,
    1, -1, 1,

    1, 1, -1,
    1, -1, -1,
    -1, -1, -1,
    -1, 1, -1,

    -1, 1, 1,
    -1, 1, -1,
    -1, -1, -1,
    -1, -1, 1,

    1, 1, 1,
    1, -1, 1,
    1, -1, -1,
    1, 1, -1,

    1, 1, 1,
    1, 1, -1,
    -1, 1, -1,
    -1, 1, 1,

    1, -1, 1,
    -1, -1, 1,
    -1, -1, -1,
    1, -1, -1,
  };

  m_mesh.setArrayBuffer(0, 3, positions);
  m_mesh.setArrayBuffer(1, 3, normals);

  m_shader.setVertSrc(VERTEX_SHADER);
  m_shader.setFragSrc(FRAGMENT_SHADER);


  m_btTransform.setIdentity();
  m_btTransform.setOrigin(btVector3(t_initPos.x, t_initPos.y,
                                    t_initPos.z));

  m_collisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));

  float mass = 1.0f;
  btVector3 localInertia { 0.0f, 0.0f, 0.0f };

  m_collisionShape->calculateLocalInertia(mass, localInertia);

  btDefaultMotionState *motionState =
    new btDefaultMotionState(m_btTransform);

  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,
                                                  motionState,
                                                  m_collisionShape,
                                                  localInertia);

  m_rigidBody = new btRigidBody(rbInfo);
  dynamicsWorld->addRigidBody(m_rigidBody);
}

Cube::Cube()
  : Cube({ 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }) {}

Cube::~Cube() {}


void Cube::draw() const {
  btTransform rbTransform;
  m_rigidBody->getMotionState()->getWorldTransform(rbTransform);

  glm::vec3 pos {
    rbTransform.getOrigin().x(),
    rbTransform.getOrigin().y(),
    rbTransform.getOrigin().z(),
  };

  glm::quat rot {
    (float) rbTransform.getRotation().x(),
    (float) rbTransform.getRotation().y(),
    (float) rbTransform.getRotation().z(),
    (float) rbTransform.getRotation().w(),
  };


  glm::mat4 trans { 1.0f };

  trans = glm::translate(trans, pos);
  trans *= glm::mat4_cast(rot);


  m_mesh.bind();
  m_shader.bind();

  m_shader.uniform("u_projMat", projection);
  m_shader.uniform("u_viewMat", view);
  m_shader.uniform("u_transMat", trans);

  m_shader.uniform("u_color", m_color);

  m_shader.uniform("u_sunVector", sunVector);


  uint32_t indices[] = {
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    8, 9, 10,
    10, 11, 8,

    12, 13, 14,
    14, 15, 12,

    16, 17, 18,
    18, 19, 16,

    20, 21, 22,
    22, 23, 20,

    24, 25, 26,
    26, 27, 24,
  };

  glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(uint32_t),
                 GL_UNSIGNED_INT, indices);

  Mesh::unbind();
  Shader::unbind();
}
