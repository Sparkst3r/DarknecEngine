//
// Copyright 2012-2013, Syoyo Fujita.
// 
// Licensed under 2-clause BSD liecense.
//



#include <cstdlib>
#include <cstring>
#include <cassert>

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <time.h>
#include <iostream>

#include <State.h>

#include "object/fast_tiny_obj_loader.h"

#include <complex>

namespace ObjLoader {
	struct vertex_index {
		int vertIndex, textIndex, normIndex;
		vertex_index() {};
		vertex_index(int index) : vertIndex(index), textIndex(index), normIndex(index) {};
		vertex_index(int vertIndex, int textIndex, int normIndex) : vertIndex(vertIndex), textIndex(textIndex), normIndex(normIndex) {};
	};

	struct obj_shape {
		std::vector<float> v;
		std::vector<float> vn;
		std::vector<float> vt;
	};

	// for std::map
	static inline bool operator<(const vertex_index& a, const vertex_index& b) {
		if (a.vertIndex != b.vertIndex) return (a.vertIndex < b.vertIndex);
		if (a.normIndex != b.normIndex) return (a.normIndex < b.normIndex);
		if (a.textIndex != b.textIndex) return (a.textIndex < b.textIndex);

		return false;
	}

	int MaterialFileReader::operator() (const std::string& matId, std::map<std::string, Material>& matMap) {
		std::string filepath;

		if (!m_mtlBasePath.empty()) {
			filepath = std::string(m_mtlBasePath) + matId;
		}
		else {
			filepath = matId;
		}

		FILE* file;
		fopen_s(&file, filepath.c_str(), "r");

		if (file == NULL) { //File could not be opened
			Darknec::logger("OBJLoader", LogLevel::LOG_ERROR, "Cannot locate file : %s", filepath.c_str());
			return 1;
		}
		else {
			return LoadMtl(matMap, file);
		}
	}



	// Make index zero-base, and also support relative index. 
	static inline int fixIndex(int idx, int n) {
		int i;

		if (idx > 0) {
			i = idx - 1;
		}
		else if (idx == 0) {
			i = 0;
		}
		else { // negative value = relative
			i = n + idx;
		}
		return i;
	}

	static inline std::string parseString(const char*& token) {
		std::string s;
		int b = strspn(token, " \t");
		int e = strcspn(token, " \t\r");
		s = std::string(&token[b], &token[e]);

		token += (e - b);
		return s;
	}



	// Parse triples: i, i/j/k, i//k, i/j
	static vertex_index parseTriple(const char* &token, int vsize, int vnsize, int vtsize) {
		vertex_index vi(-1);

		vi.vertIndex = fixIndex(atoi(token), vsize);
		token += strcspn(token, "/ \t\r");
		if (token[0] != '/') {
			return vi;
		}
		token++;

		// i//k
		if (token[0] == '/') {
			token++;
			vi.normIndex = fixIndex(atoi(token), vnsize);
			token += strcspn(token, "/ \t\r");
			return vi;
		}

		// i/j/k or i/j
		vi.textIndex = fixIndex(atoi(token), vtsize);
		token += strcspn(token, "/ \t\r");
		if (token[0] != '/') {
			return vi;
		}

		// i/j/k
		token++;  // skip '/'
		vi.normIndex = fixIndex(atoi(token), vnsize);
		token += strcspn(token, "/ \t\r");
		return vi;
	}

	static unsigned int updateVertex(
		std::map<vertex_index, unsigned int>& vertexCache,
		std::vector<float>& positions,
		std::vector<float>& normals,
		std::vector<float>& texcoords,
		const std::vector<float>& in_positions,
		const std::vector<float>& in_normals,
		const std::vector<float>& in_texcoords,
		const vertex_index& i) {
		const std::map<vertex_index, unsigned int>::iterator it = vertexCache.find(i);

		if (it != vertexCache.end()) {
			// found cache
			return it->second;
		}

		assert(in_positions.size() > (unsigned int) (3 * i.vertIndex + 2));

		positions.push_back(in_positions[3 * i.vertIndex + 0]);
		positions.push_back(in_positions[3 * i.vertIndex + 1]);
		positions.push_back(in_positions[3 * i.vertIndex + 2]);

		if (i.normIndex >= 0) {
			normals.push_back(in_normals[3 * i.normIndex + 0]);
			normals.push_back(in_normals[3 * i.normIndex + 1]);
			normals.push_back(in_normals[3 * i.normIndex + 2]);
		}

		if (i.textIndex >= 0) {
			texcoords.push_back(in_texcoords[2 * i.textIndex + 0]);
			texcoords.push_back(in_texcoords[2 * i.textIndex + 1]);
		}

		unsigned int idx = positions.size() / 3 - 1;
		vertexCache[i] = idx;

		return idx;
	}

	void InitMaterial(Material& material) {
		material.name = "";
		material.ambient_texname = "";
		material.diffuse_texname = "";
		material.specular_texname = "";
		material.normal_texname = "";
		for (int i = 0; i < 3; i++) {
			material.ambient[i] = 0.f;
			material.diffuse[i] = 0.f;
			material.specular[i] = 0.f;
			material.transmittance[i] = 0.f;
			material.emission[i] = 0.f;
		}
		material.illum = 0;
		material.dissolve = 1.f;
		material.shininess = 1.f;
		material.ior = 1.f;
		material.unknown_parameter.clear();
	}

	static bool
		exportFaceGroupToShape(
		Shape& shape,
		const std::vector<float> &in_positions,
		const std::vector<float> &in_normals,
		const std::vector<float> &in_texcoords,
		const std::vector<std::vector<vertex_index> >& faceGroup,
		const Material &material,
		const std::string &name,
		const bool is_material_seted) {
			if (faceGroup.empty()) {
				return false;
			}

			// Flattened version of vertex data
			std::vector<float> positions;
			std::vector<float> normals;
			std::vector<float> texcoords;
			std::map<vertex_index, unsigned int> vertexCache;
			std::vector<unsigned int> indices;

			// Flatten vertices and indices
			for (size_t faceIndex = 0; faceIndex < faceGroup.size(); faceIndex++) {
				const std::vector<vertex_index>& face = faceGroup[faceIndex];

				vertex_index i0 = face[0];
				vertex_index i1(-1);
				vertex_index i2 = face[1];

				size_t npolys = face.size();

				// Polygon -> triangle fan conversion
				for (size_t k = 2; k < npolys; k++) {
					i1 = i2;
					i2 = face[k];

					unsigned int v0 = updateVertex(vertexCache, positions, normals, texcoords, in_positions, in_normals, in_texcoords, i0);
					unsigned int v1 = updateVertex(vertexCache, positions, normals, texcoords, in_positions, in_normals, in_texcoords, i1);
					unsigned int v2 = updateVertex(vertexCache, positions, normals, texcoords, in_positions, in_normals, in_texcoords, i2);

					indices.push_back(v0);
					indices.push_back(v1);
					indices.push_back(v2);
				}

			}

			//
			// Construct shape.
			//
			shape.name = name;
			shape.mesh.vertices.swap(positions);
			shape.mesh.normals.swap(normals);
			shape.mesh.texcoords.swap(texcoords);
			shape.mesh.indices.swap(indices);

			if (is_material_seted) {
				shape.material = material;
			}
			else {
				InitMaterial(shape.material);
				shape.material.diffuse[0] = 1.f;
				shape.material.diffuse[1] = 1.f;
				shape.material.diffuse[2] = 1.f;
			}

			return true;

		}

	int LoadMtl(std::map<std::string, Material>& material_map, FILE* file) {
		material_map.clear();

		Material material;

		while (!feof(file)) { //Loop while the file has more data
			char lineTag[16];
			//Read line tag into buffer
			fscanf_s(file, "%s", lineTag, sizeof(lineTag));

			//Define new material tag
			if (!strcmp(lineTag, "newmtl")) {
				// flush previous material.
				material_map.insert(std::pair<std::string, Material>(material.name, material));

				// initial temporary material
				InitMaterial(material);

				// set new mtl name
				char namebuf[4096];
				fscanf_s(file, "%s", namebuf, sizeof(namebuf)); //Here
				material.name = namebuf;
			}

#pragma region Ambient Diffuse Specular Transmittance Emmision
			//Ambient colour
			else if (!strcmp(lineTag, "Ka")) {
				float r, g, b;
				fscanf_s(file, "%f %f %f\n", &r, &g, &b);
				material.ambient[0] = r;
				material.ambient[1] = g;
				material.ambient[2] = b;
			}
			//Diffuse colour
			else if (!strcmp(lineTag, "Kd")) {
				float r, g, b;
				fscanf_s(file, "%f %f %f\n", &r, &g, &b);
				material.diffuse[0] = r;
				material.diffuse[1] = g;
				material.diffuse[2] = b;
			}
			//Specular colour
			else if (!strcmp(lineTag, "Ks")) {
				float r, g, b;
				fscanf_s(file, "%f %f %f\n", &r, &g, &b);
				material.specular[0] = r;
				material.specular[1] = g;
				material.specular[2] = b;
			}
			//Transmittance colour
			else if (!strcmp(lineTag, "Kt")) {
				float r, g, b;
				fscanf_s(file, "%f %f %f\n", &r, &g, &b);
				material.transmittance[0] = r;
				material.transmittance[1] = g;
				material.transmittance[2] = b;
			}
			//Emmision colour
			else if (!strcmp(lineTag, "Ke")) {
				float r, g, b;
				fscanf_s(file, "%f %f %f\n", &r, &g, &b);
				material.emission[0] = r;
				material.emission[1] = g;
				material.emission[2] = b;
			}
#pragma endregion

#pragma region Refraction Shininess Illumination Dissolve

			//Index of refraction
			else if (!strcmp(lineTag, "Ni")) {
				float ior;
				fscanf_s(file, "%f\n", &ior);
				material.ior = ior;
			}
			//Shininess
			else if (!strcmp(lineTag, "Ns")) {
				float shininess;
				fscanf_s(file, "%f\n", &shininess);
				material.ior = shininess;
			}

			//Illumination model
			else if (!strcmp(lineTag, "illum")) {
				int illumination;
				fscanf_s(file, "%i\n", &illumination);
				material.illum = illumination;
			}
			//Dissolve / Transparency
			else if (!strcmp(lineTag, "Tr") || !strcmp(lineTag, "d")) {
				float dissolve;
				fscanf_s(file, "%f\n", &dissolve);
				material.dissolve = dissolve;
			}
#pragma endregion

#pragma region Texture
			//Ambient Texture
			else if (!strcmp(lineTag, "map_Ka")) {
				char textureFileBuffer[512];
				fscanf_s(file, "%s", textureFileBuffer, sizeof(textureFileBuffer));
				material.ambient_texname = std::string(textureFileBuffer);
			}
			//Diffuse Texture
			else if (!strcmp(lineTag, "map_Kd")) {
				char textureFileBuffer[512];
				fscanf_s(file, "%s", textureFileBuffer, sizeof(textureFileBuffer));
				material.diffuse_texname = std::string(textureFileBuffer);
			}
			//Specular Texture
			else if (!strcmp(lineTag, "map_Ks")) {
				char textureFileBuffer[512];
				fscanf_s(file, "%s", textureFileBuffer, sizeof(textureFileBuffer));
				material.specular_texname = std::string(textureFileBuffer);
			}
			//Normal Texture
			else if (!strcmp(lineTag, "map_Ns")) {
				char textureFileBuffer[512];
				fscanf_s(file, "%s", textureFileBuffer, sizeof(textureFileBuffer));
				material.normal_texname = std::string(textureFileBuffer);
			}
#pragma endregion
		}
		// flush last material.
		material_map.insert(std::pair<std::string, Material>(material.name, material));

		return 0;
	}



	int LoadObj(std::vector<Shape>& shapes, const char* filename, const char* mtl_basepath) {

		shapes.clear();

		FILE* file;
		fopen_s(&file, filename, "r");

		if (file == NULL) { //File could not be opened
			Darknec::logger("OBJLoader", LogLevel::LOG_ERROR, "Cannot locate file : %s", filename);
			return 1;
		}

		std::string basePath;
		if (mtl_basepath) {
			basePath = mtl_basepath;
		}
		MaterialFileReader matFileReader(basePath);

		int startTime = SDL_GetTicks();
		int error = LoadObj(shapes, file, matFileReader);
		Darknec::logger("OBJLoader", LogLevel::LOG_INFO, "Loaded OBJ file: %s; Took : %i milliseconds.", filename, (SDL_GetTicks() - startTime));

		return error;
	}

	int LoadObj(std::vector<Shape>& shapes, FILE* file, MaterialReader& readMatFn) {
		std::vector<float> v;
		std::vector<float> vn;
		std::vector<float> vt;
		std::vector<std::vector<vertex_index>> faceGroup;
		std::string name;

		// material
		std::map<std::string, Material> material_map;
		Material material;
		bool is_material_seted = false;

		while (!feof(file)) { //Loop while the file has more data
			char lineTag[16];
			//Read line tag into buffer
			fscanf_s(file, "%s", lineTag, sizeof(lineTag));

			//Vertex tag
			if (!strcmp(lineTag, "v")) {
				float x, y, z;
				fscanf_s(file, "%f %f %f\n", &x, &y, &z);
				v.push_back(x);
				v.push_back(y);
				v.push_back(z);
			}
			//Normal tag
			else if (!strcmp(lineTag, "vn")) {
				float x, y, z;
				fscanf_s(file, "%f %f %f\n", &x, &y, &z);
				vn.push_back(x);
				vn.push_back(y);
				vn.push_back(z);
			}
			//Texture tag
			else if (!strcmp(lineTag, "vt")) {
				float x, y;
				fscanf_s(file, "%f %f\n", &x, &y);
				vt.push_back(x);
				vt.push_back(y);
			}
			//Face tag
			else if (!strcmp(lineTag, "f")) {


				std::vector<vertex_index> face;

				//TODO fix for triples

				int vertexIndex[3];
				int uvIndex[3];
				int normalIndex[3];
				int match;


				match = fscanf_s(file, "%i//%i %i//%i %i//%i\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);


				//	int matches = fscanf_s(file, "%i/%i/%i %i/%i/%i %i/%i/%i\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				//int matches = fscanf_s(file, "%i//%i %i//%i %i//%i\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1],  &normalIndex[1], &vertexIndex[2], &normalIndex[2]);


				face.push_back({vertexIndex[0] - 1, uvIndex[0] - 1, normalIndex[0] - 1});
				face.push_back({vertexIndex[1] - 1, uvIndex[1] - 1, normalIndex[1] - 1});
				face.push_back({vertexIndex[2] - 1, uvIndex[2] - 1, normalIndex[2] - 1});

				faceGroup.push_back(face);
			}

			// use mtl
			else if (!strcmp(lineTag, "usemtl")) {
				char namebuf[4096];

				fscanf_s(file, "%s", namebuf, sizeof(namebuf));
				if (material_map.find(namebuf) != material_map.end()) {
					material = material_map[namebuf];
					is_material_seted = true;
				}
				else {
					// { error!! material not found }
					InitMaterial(material);
				}
			}

			// load mtl
			else if (!strcmp(lineTag, "mtllib")) {
				char namebuf[4096];
				fscanf_s(file, "%s", namebuf, sizeof(namebuf)); //Here
				printf(namebuf);
				int err_mtl = readMatFn(namebuf, material_map);
				if (err_mtl != 0) {
					faceGroup.clear();  // for safety
					//	return err_mtl;
				}
			}

			//// group name
			//if (!strcmp(lineTag, "g")) {
			//	// flush previous face group.
			//	Shape shape;
			//	bool ret = exportFaceGroupToShape(shape, v, vn, vt, faceGroup, material, name, is_material_seted);
			//	if (ret) {
			//		shapes.push_back(shape);
			//	}
			//	is_material_seted = false;
			//	faceGroup.clear();
			//	std::vector<std::string> names;
			//	while (!isNewLine(token[0])) {
			//		std::string str = parseString(token);
			//		names.push_back(str);
			//		token += strspn(token, " \t\r"); // skip tag
			//	}
			//	assert(names.size() > 0);
			//	// names[0] must be 'g', so skipt 0th element.
			//	if (names.size() > 1) {
			//		name = names[1];
			//	}
			//	else {
			//		name = "";
			//	}
			//	continue;
			//}

			// object name
			else if (!strcmp(lineTag, "o")) {
				// flush previous face group.
				Shape shape;
				bool ret = exportFaceGroupToShape(shape, v, vn, vt, faceGroup, material, name, is_material_seted);
				if (ret) {
					shapes.push_back(shape);
				}

				is_material_seted = false;
				faceGroup.clear();

				// @todo { multiple object name? }
				char namebuf[4096];
				fscanf_s(file, "%s", namebuf, sizeof(namebuf)); //here
				name = std::string(namebuf);


			}

			// Ignore unknown command.
		}

		Shape shape;
		bool ret = exportFaceGroupToShape(shape, v, vn, vt, faceGroup, material, name, is_material_seted);
		if (ret) {
			shapes.push_back(shape);
		}
		is_material_seted = false; // for safety
		faceGroup.clear();  // for safety

		return 0;
	}


}

namespace CObjLoader {
	using namespace ObjLoader;

	void write(std::ofstream &stream, const std::vector<Shape> &shapes) {
		const auto dataWidth = sizeof(float); //Size of float
		const int shapeCount = shapes.size(); //Number of shapes

		stream.write((const char*) &shapeCount, sizeof(int)); //Write shapesCount

		for (Shape shape : shapes) {
			const int indexCount = shape.mesh.indices.size();
			const int vertexCount = shape.mesh.vertices.size();
			const int normalCount = shape.mesh.normals.size();

			stream.write((const char*) &indexCount, dataWidth);
			stream.write((const char*) &vertexCount, dataWidth);
			stream.write((const char*) &normalCount, dataWidth);

			stream.write((const char*) &shape.mesh.indices[0], dataWidth * indexCount);
			stream.write((const char*) &shape.mesh.vertices[0], dataWidth * vertexCount);
			stream.write((const char*) &shape.mesh.normals[0], dataWidth * normalCount);

			stream.write((const char*) &shape.material.ambient[0], dataWidth * 3);
			stream.write((const char*) &shape.material.diffuse[0], dataWidth * 3);
			stream.write((const char*) &shape.material.specular[0], dataWidth * 3);

		}
		stream.close();
	}


	std::vector<Shape> read(std::ifstream& stream) {
		//Logger clogger = Darknec::DLogger.getLogger("CObjLoader");

		int start = SDL_GetTicks();

		const auto dataWidth = sizeof(float); //Size of float


		std::vector<ObjLoader::Shape> shapes;

		int shapeCount = 0;
		stream.read((char*) &shapeCount, sizeof(int));
		shapes.resize(shapeCount);

		for (int shapeIndex = 0; shapeIndex < shapeCount; shapeIndex++) {
			int indexCount = 0;
			int vertexCount = 0;
			int normalCount = 0;

			stream.read((char*) &indexCount, dataWidth);
			stream.read((char*) &vertexCount, dataWidth);
			stream.read((char*) &normalCount, dataWidth);

			shapes[shapeIndex].mesh.indices.resize(indexCount);
			shapes[shapeIndex].mesh.vertices.resize(vertexCount);
			shapes[shapeIndex].mesh.normals.resize(normalCount);


			stream.read((char*) &shapes[shapeIndex].mesh.indices[0], indexCount * dataWidth);
			stream.read((char*) &shapes[shapeIndex].mesh.vertices[0], vertexCount * dataWidth);
			stream.read((char*) &shapes[shapeIndex].mesh.normals[0], normalCount * dataWidth);

			stream.read((char*) &shapes[shapeIndex].material.ambient[0], 3 * dataWidth);
			stream.read((char*) &shapes[shapeIndex].material.diffuse[0], 3 * dataWidth);
			stream.read((char*) &shapes[shapeIndex].material.specular[0], 3 * dataWidth);
		}

		Darknec::logger("COBJLoader", LogLevel::LOG_INFO, "Took %i milliseconds", SDL_GetTicks() - start);

		stream.close();
		return shapes;
	}
}
