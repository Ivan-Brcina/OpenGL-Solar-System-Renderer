#include "../../headers/core/mesh.hpp"

Mesh::Mesh() {}

unsigned int Mesh::createSphere(GLuint& vao,
                                GLuint& vbo,
                                GLuint& ebo,
                                float radius,
                                unsigned int sectors,
                                unsigned int stacks) {

  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  vertices.reserve((stacks + 1) * (sectors + 1));
  indices.reserve(stacks * sectors * 6);

  unsigned int idxCount = generateUVSphere(radius, sectors, stacks, vertices, indices);
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

  glBindVertexArray(0);
  
  return idxCount;
}

unsigned int Mesh::generateUVSphere(float radius, unsigned int sectors, unsigned int stacks,
                                    std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {

  const float PI = glm::pi<float>();

  for (unsigned int i = 0; i <= stacks; ++i) {
    float v = (float)i / (float)stacks;
    float phi = v * PI;

    for (unsigned int j = 0; j <= sectors; ++j) {
      float u = (float)j / (float)sectors;
      float theta = (1.0f - u) * 2.0f * PI;

      Vertex vertex;

      vertex.position.x = radius * std::cos(theta) * std::sin(phi);
      vertex.position.y = radius * std::cos(phi);
      vertex.position.z = radius * std::sin(theta) * std::sin(phi);

      vertex.normal = glm::normalize(vertex.position);

      vertex.texCoords = glm::vec2(u, v);
      vertices.push_back(vertex);
    }
  }

  for (unsigned int i = 0; i < stacks; ++i) {
    for (unsigned int j = 0; j < sectors; ++j) {

      unsigned int first = i * (sectors + 1) + j;
      unsigned int second = first + sectors + 1;

      indices.push_back(first);
      indices.push_back(second);
      indices.push_back(first + 1);

      indices.push_back(first + 1);
      indices.push_back(second);
      indices.push_back(second + 1);
    }
  }

  return indices.size();
}

const std::vector<float> skyboxVertices = {
  // positions
  -1.0f,  1.0f, -1.0f,
  -1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,

  -1.0f, -1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f, -1.0f,  1.0f,
  -1.0f, -1.0f,  1.0f,

  -1.0f,  1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
   1.0f,  1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f, -1.0f,

  -1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f, -1.0f,
   1.0f, -1.0f, -1.0f,
  -1.0f, -1.0f,  1.0f,
   1.0f, -1.0f,  1.0f
};


void Mesh::createCube(GLuint& vao, GLuint& vbo) {

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), skyboxVertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glBindVertexArray(0);
}
