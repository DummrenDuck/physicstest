#include <cstdint>
#include <random>

#include "gl.hpp"
#include "glm/ext.hpp"
#include "cube.hpp"
#include "btBulletDynamicsCommon.h"


#define WINDOW_WIDTH 800.0f
#define WINDOW_HEIGHT 600.0f
#define WINDOW_TITLE "physics test"

#define CAMERA_FOV 3.14f / 180.0f * 60.0f
#define CAMERA_NEAR 0.1f
#define CAMERA_FAR 100.0f

#define CAMERA_POS glm::vec3(25.0f, 25.0f, 25.0f)
#define CAMERA_LOOK_AT glm::vec3(0.0f, 0.0f, 0.0f)
#define CAMERA_UP glm::vec3(0.0f, 1.0f, 0.0f)


GLFWwindow *wnd;

glm::mat4 projection = glm::perspective(CAMERA_FOV,
                                        WINDOW_WIDTH / WINDOW_HEIGHT,
                                        CAMERA_NEAR, CAMERA_FAR);

glm::mat4 view = glm::lookAt(CAMERA_POS, CAMERA_LOOK_AT, CAMERA_UP);

glm::vec3 sunVector { 0.0f, 0.0f, 1.0f };


btDiscreteDynamicsWorld *dynamicsWorld;

btTransform *floorTransform;
btCollisionShape *floorCollisionShape;
btRigidBody *floorBody;


std::vector<Cube*> cubes;


void centerWindow() {
  const GLFWvidmode *vm = glfwGetVideoMode(glfwGetPrimaryMonitor());

  int32_t mw = vm->width;
  int32_t mh = vm->height;

  int32_t mx, my;
  glfwGetMonitorPos(glfwGetPrimaryMonitor(), &mx, &my);

  glfwSetWindowPos(wnd,
                   mx + mw / 2 - WINDOW_WIDTH / 2,
                   my + mh / 2 - WINDOW_HEIGHT / 2);
}


void start() {
  btDefaultCollisionConfiguration *collisionConfig =
    new btDefaultCollisionConfiguration();

  btCollisionDispatcher *collisionDispatcher =
    new btCollisionDispatcher(collisionConfig);

  btBroadphaseInterface *overlappingPairCache =
    new btDbvtBroadphase();

  btSequentialImpulseConstraintSolver *solver =
    new btSequentialImpulseConstraintSolver();

  dynamicsWorld = new btDiscreteDynamicsWorld(collisionDispatcher,
                                              overlappingPairCache,
                                              solver,
                                              collisionConfig);

  dynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));


  floorTransform = new btTransform();
  floorTransform->setIdentity();
  floorTransform->setOrigin(btVector3(0.0f, -10.0f, 0.0f));

  floorCollisionShape =
    new btBoxShape(btVector3(100.0f, 1.0f, 100.0f));

  btDefaultMotionState *motionState =
    new btDefaultMotionState(*floorTransform);

  float mass = 0.0f;
  btVector3 localInertia { 0.0f, 0.0f, 0.0f };

  auto rigidBodyInfo =
    btRigidBody::btRigidBodyConstructionInfo(mass,
                                             motionState,
                                             floorCollisionShape,
                                             localInertia);

  floorBody = new btRigidBody(rigidBodyInfo);
  floorBody->setFriction(1.0f);

  dynamicsWorld->addRigidBody(floorBody);


  std::random_device rnddev;
  std::mt19937 rndgen(rnddev());

  std::uniform_real_distribution<> rndclrdistr(0.0f, 1.0f);

  std::uniform_real_distribution<> rndposxdistr(-10.0f, 10.0f);
  std::uniform_real_distribution<> rndposydistr(0, 20.0f);
  std::uniform_real_distribution<> rndposzdistr(-10.0f, 10.0f);

  for (int i = 0; i < 500; i++) {
    glm::vec4 color {
      rndclrdistr(rndgen),
      rndclrdistr(rndgen),
      rndclrdistr(rndgen),
      1.0f,
    };

    glm::vec3 initPos {
      rndposxdistr(rndgen),
      rndposydistr(rndgen),
      rndposzdistr(rndgen),
    };

    cubes.push_back(new Cube(color, initPos));
  }
}


void update() {
  dynamicsWorld->stepSimulation(1.0f / 60.0f, 10);
}


void render() {
  for (auto cube : cubes)
    cube->draw();
}


int main() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, false);

  wnd = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE,
                         nullptr, nullptr);
  glfwMakeContextCurrent(wnd);
  glewInit();

  glfwSwapInterval(1);
  centerWindow();


  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);

  glClearColor(0.1f, 0.15f, 0.2f, 1.0f);


  start();

  while (!glfwWindowShouldClose(wnd)) {
    glfwPollEvents();
    update();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    render();

    glfwSwapBuffers(wnd);
  }


  glfwTerminate();

  return 0;
}
