#include "mesh.hpp"


#include <cstdint>
#include <vector>

#include "gl.hpp"


void Mesh::unbind() {
  glBindVertexArray(0);
}


Mesh::Mesh() {
  glGenVertexArrays(1, &m_vao);
}

Mesh::~Mesh() {
  glDeleteVertexArrays(1, &m_vao);
}


void Mesh::bind() const {
  glBindVertexArray(m_vao);
}


void Mesh::setArrayBuffer(uint32_t t_pos, int32_t t_size,
                          std::vector<float> t_data) const {
  bind();

  uint32_t buf;
  glGenBuffers(1, &buf);

  glBindBuffer(GL_ARRAY_BUFFER, buf);
  glBufferData(GL_ARRAY_BUFFER, sizeof(t_data.data()) * t_data.size(),
               t_data.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(t_pos);
  glVertexAttribPointer(t_pos, t_size, GL_FLOAT, false, 0, nullptr);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  unbind();
}
