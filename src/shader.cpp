#include "shader.hpp"


#include <cstdint>
#include <string>
#include <iostream>

#include "gl.hpp"
#include "glm/ext.hpp"


void Shader::unbind() {
  glUseProgram(0);
}


Shader::Shader() {}

Shader::~Shader() {
  glDeleteProgram(m_prog);
  glDeleteShader(m_vs);
  glDeleteShader(m_fs);
}


void Shader::setVertSrc(std::string t_src) const {
  m_setSrc(m_vs, t_src);
}

void Shader::setFragSrc(std::string t_src) const {
  m_setSrc(m_fs, t_src);
}


void Shader::bind() const {
  glUseProgram(m_prog);
}


int32_t Shader::uniformLocation(std::string t_name) const {
  return glGetUniformLocation(m_prog, t_name.c_str());
}


void Shader::uniform(std::string t_name, glm::vec3 t_value) const {
  glProgramUniform3fv(m_prog, uniformLocation(t_name), 1,
                      glm::value_ptr(t_value));
}

void Shader::uniform(std::string t_name, glm::vec4 t_value) const {
  glProgramUniform4fv(m_prog, uniformLocation(t_name), 1,
                      glm::value_ptr(t_value));
}

void Shader::uniform(std::string t_name, glm::mat4 t_value) const {
  glProgramUniformMatrix4fv(m_prog, uniformLocation(t_name), 1, false,
                            glm::value_ptr(t_value));
}


void Shader::m_setSrc(uint32_t t_shdr, std::string t_src) const {
  const char *csrc = t_src.c_str();

  glShaderSource(t_shdr, 1, &csrc, nullptr);
  glCompileShader(t_shdr);

  int32_t compiled;
  glGetShaderiv(t_shdr, GL_COMPILE_STATUS, &compiled);

  if (!compiled) {
    int32_t logLen;
    glGetShaderiv(t_shdr, GL_INFO_LOG_LENGTH, &logLen);

    char log[logLen];
    glGetShaderInfoLog(t_shdr, logLen, nullptr, log);

    std::cout << log << std::endl;
    exit(1);
  }

  glAttachShader(m_prog, t_shdr);
  glLinkProgram(m_prog);

  int32_t linked;
  glGetProgramiv(m_prog, GL_LINK_STATUS, &linked);

  if (!linked) {
    int32_t logLen;
    glGetProgramiv(m_prog, GL_INFO_LOG_LENGTH, &logLen);

    char log[logLen];
    glGetProgramInfoLog(m_prog, logLen, nullptr, log);

    std::cout << log << std::endl;
    exit(1);
  }
}
