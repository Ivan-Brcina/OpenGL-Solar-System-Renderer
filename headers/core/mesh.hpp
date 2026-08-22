#pragma once
#include "application.hpp"
#include <vector>

class Mesh {
  private:
    unsigned int generateUVSphere(float radius,
                                  unsigned int sectors,
                                  unsigned int stacks,
                                  std::vector<Vertex>& vertices,
                                  std::vector<unsigned int>& indices);

    unsigned int generateCubemap(std::vector<Vertex>& vertices,
                                 std::vector<unsigned int>& indices);

  public:
    Mesh();

    unsigned int createSphere(GLuint& vao,
                              GLuint& vbo,
                              GLuint& ebo,
                              float radius,
                              unsigned int sectors,
                              unsigned int stacks);

    void createCube(GLuint& vao, GLuint& vbo);

};
