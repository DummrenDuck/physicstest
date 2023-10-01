#ifndef __MESH_HPP__
#define __MESH_HPP__


#include <cstdint>
#include <vector>


class Mesh {

public:
  static void unbind();

  Mesh();
  ~Mesh();

  void bind() const;

  void setArrayBuffer(uint32_t, int32_t, std::vector<float>) const;

private:
  uint32_t m_vao;

};


#endif
