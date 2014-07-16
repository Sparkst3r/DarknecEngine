#include <object/Model.h>
#include <State.h>

void Model::loadObj(FILE* file) {
	std::vector<glm::vec4> tempVertices;
	std::vector<glm::vec3> tempNormals;

	while (!feof(file)) { //Loop while the file has more data
		char lineTag[16];
		//Read line tag into buffer
		fscanf_s(file, "%s", lineTag, sizeof(lineTag));

		//Vertex tag
		if (!strcmp(lineTag, "v")) {
			glm::vec4 vertex;
			fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertex.w = 1.0f;
			tempVertices.push_back(vertex);
		}
		else if (!strcmp(lineTag, "vn")) {
			glm::vec4 normal;
			fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normal.w = 1.0f;
			tempVertices.push_back(normal);
		}
	}
}

Model::Model(const char* filename, FileType type) {
	FILE* file;
	fopen_s(&file, filename, "r");

	if (!file) {
		loadObj(file);
	}
	else {
		Darknec::logger("Mesh", LogLevel::LOG_ERROR, "Mesh file not found. File: %s", filename);
	}


}