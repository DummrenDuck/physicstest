#ifndef __SHADER_HPP__
#define __SHADER_HPP__


#include <cstdint>
#include <string>

#include "gl.hpp"
#include "glm/ext.hpp"


class Shader {

public:
  static void unbind();

  Shader();
  ~Shader();

  void setVertSrc(std::string) const;
  void setFragSrc(std::string) const;

  void bind() const;

  int32_t uniformLocation(std::string) const;

  void uniform(std::string, glm::vec3) const;
  void uniform(std::string, glm::vec4) const;
  void uniform(std::string, glm::mat4) const;

private:
  uint32_t m_prog = glCreateProgram();
  uint32_t m_vs = glCreateShader(GL_VERTEX_SHADER);
  uint32_t m_fs = glCreateShader(GL_FRAGMENT_SHADER);

  void m_setSrc(uint32_t, std::string) const;

};


#endif
