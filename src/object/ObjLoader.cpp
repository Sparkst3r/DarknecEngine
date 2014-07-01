#include <object/ObjLoader.h>
#include <fstream>
#include <sstream>
#include <State.h>

namespace ObjLoader {

	//struct Vertex {
	//	GLuint vertexIndex;
	//	GLuint normalIndex;
	//	GLuint textUVIndex;
	//};

	struct Vertex {
		glm::vec4 vertexIndex;
		glm::vec3 normalIndex;
		GLushort elementIndex;
	};

	struct Face {
		Vertex vertices[3];
	};

	void loadObj(const char* filename, std::vector<glm::vec4> &vertices, std::vector<glm::vec2> &uvs, std::vector<glm::vec3> &normals, std::vector<GLushort> &elements) {
		FILE* objFile;

		bool hasUVs = false;

		std::vector<unsigned short> normalIndices;

		std::vector<glm::vec4> tempVertices;
		std::vector<glm::vec3> tempNormals;
		std::vector<Face> faces;


		fopen_s(&objFile, filename, "r");
		if (objFile != NULL) {
			while (!feof(objFile)) {
				char lineHeader[128];
				//Read first word to determine tag
				fscanf_s(objFile, "%s", lineHeader, sizeof(lineHeader));

				//If vertex tag
				if (!strcmp(lineHeader, "v")) {
					glm::vec4 vertex = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
					fscanf_s(objFile, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
					vertices.push_back(vertex);
					tempVertices.push_back(vertex);
				}
				//If normal tag
				else if (!strcmp(lineHeader, "vn")) {
					glm::vec3 normal;
					fscanf_s(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
					tempNormals.push_back(normal);
				}
				//If UV tag
				else if (strcmp(lineHeader, "vt") == 0) {
					if (hasUVs == false) { //If model has vt tags the faces should be loaded with uvs in mind
						hasUVs = true;
					}
					glm::vec2 uv;
					fscanf_s(objFile, "%f %f\n", &uv.x, &uv.y);
					uvs.push_back(uv);
				}
				//If face tag
				else if (!strcmp(lineHeader, "f")) {
					unsigned int vertexIndex[3];
					unsigned int uvIndex[3];
					unsigned int normalIndex[3];

					if (hasUVs) {
						int matches = fscanf_s(objFile, "%u/%u/%u %u/%u/%u %u/%u/%u\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
					}
					else {
						int matches = fscanf_s(objFile, "%u//%u %u//%u %u//%u\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
					}

					Vertex vert1 = {tempVertices[vertexIndex[0] - 1], tempNormals[normalIndex[0] - 1], vertexIndex[0] - 1};
					Vertex vert2 = {tempVertices[vertexIndex[1] - 1], tempNormals[normalIndex[1] - 1], vertexIndex[1] - 1};
					Vertex vert3 = {tempVertices[vertexIndex[2] - 1], tempNormals[normalIndex[2] - 1], vertexIndex[2] - 1};

					faces.push_back({{vert1, vert2, vert3}});


					//Vertex faceVertex1{vertexIndex[0] - 1, normalIndex[0], uvIndex[0]};
					//Vertex faceVertex2{vertexIndex[1] - 1, normalIndex[1], uvIndex[1]};
					//Vertex faceVertex3{vertexIndex[2] - 1, normalIndex[2], uvIndex[2]};
					//faces.push_back({{faceVertex1, faceVertex2, faceVertex3}});
					//elements.push_back(vertexIndex[0] - 1);
					//elements.push_back(vertexIndex[1] - 1);
					//elements.push_back(vertexIndex[2] - 1);
					////normalIndices.push_back(normalIndex[0]);
					////normalIndices.push_back(normalIndex[1]);
					////normalIndices.push_back(normalIndex[2]);
					//normals.push_back(glm::normalize(tempNormals[normalIndex[0] - 1]));
					//normals.push_back(glm::normalize(tempNormals[normalIndex[1] - 1]));
					//normals.push_back(glm::normalize(tempNormals[normalIndex[2] - 1]));
				}
			}
		}
		for (Face face : faces) {
			vertices.push_back(face.vertices[0].vertexIndex);
			vertices.push_back(face.vertices[1].vertexIndex);
			vertices.push_back(face.vertices[2].vertexIndex);

			normals.push_back(face.vertices[0].normalIndex);
			normals.push_back(face.vertices[1].normalIndex);
			normals.push_back(face.vertices[2].normalIndex);

			elements.push_back(face.vertices[0].elementIndex);
			elements.push_back(face.vertices[1].elementIndex);
			elements.push_back(face.vertices[2].elementIndex);
		}


		//for (Face face : faces) {
		//	normals.push_back(tempNormals[face.vertices[0].normalIndex -1]);
		//	normals.push_back(tempNormals[face.vertices[1].normalIndex -1]);
		//	normals.push_back(tempNormals[face.vertices[2].normalIndex -1]);
		////	elements.push_back(face.vertices[0].vertexIndex);
		////	elements.push_back(face.vertices[1].vertexIndex);
		////	elements.push_back(face.vertices[2].vertexIndex);
		//}
	//	for (int normal = 0; normal < normalIndices.size() && normal < tempNormals.size(); normal++) {
	//		normals.push_back(tempNormals[normalIndices[normal]]);
	//	}

	}
}