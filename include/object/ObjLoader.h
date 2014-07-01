#ifndef DARKNEC_OBJLOADER_H
#define DARKNEC_OBJLOADER_H

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

namespace ObjLoader{

	void loadObj(const char* filename, std::vector<glm::vec4> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals, std::vector<GLushort> &elements);

}

#endif