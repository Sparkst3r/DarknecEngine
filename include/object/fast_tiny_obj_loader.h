//
// Copyright 2012-2013, Syoyo Fujita.
//
// Licensed under 2-clause BSD liecense.
//
#ifndef _TINY_OBJ_LOADER_H
#define _TINY_OBJ_LOADER_H

#include <string>
#include <vector>
#include <map>

namespace ObjLoader {

	struct Material {
		std::string name;

		float ambient[3];
		float diffuse[3];
		float specular[3];
		float transmittance[3];
		float emission[3];
		float shininess;
		float ior;                // index of refraction
		float dissolve;           // 1 == opaque; 0 == fully transparent
		// illumination model (see http://www.fileformat.info/format/material/)
		int illum;

		std::string ambient_texname;
		std::string diffuse_texname;
		std::string specular_texname;
		std::string normal_texname;
		std::map<std::string, std::string> unknown_parameter;
	} ;

	struct Mesh {
		std::vector<float>          vertices;
		std::vector<float>          normals;
		std::vector<float>          texcoords;
		std::vector<unsigned int>   indices;
	};

	struct Shape {
		std::string  name;
		Material   material;
		Mesh       mesh;
	};

	class MaterialReader {
	public:
		MaterialReader() {}
		virtual ~MaterialReader() {}

		virtual int operator() (
			const std::string& matId,
			std::map<std::string, Material>& matMap) = 0;
	};

	class MaterialFileReader : public MaterialReader {
	public:
		MaterialFileReader(const std::string& mtl_basepath) : m_mtlBasePath(mtl_basepath) {}
		virtual ~MaterialFileReader() {}
		virtual int operator() (
			const std::string& matId,
			std::map<std::string, Material>& matMap);

	private:
		std::string m_mtlBasePath;
	};

	/// Loads .obj from a file.
	/// 'shapes' will be filled with parsed shape data
	/// The function returns error string.
	/// Returns empty string when loading .obj success.
	/// 'mtl_basepath' is optional, and used for base path for .mtl file.
	int LoadObj(std::vector<Shape>& shapes, const char* filename, const char* mtl_basepath = NULL);

	/// Loads object from a std::istream, uses GetMtlIStreamFn to retrieve
	/// std::istream for materials.
	/// Returns empty string when loading .obj success.
	int LoadObj(std::vector<Shape>& shapes, FILE* file, MaterialReader& readMatFn);

	/// Loads materials into std::map
	/// Returns an empty string if successful
	int LoadMtl(std::map<std::string, Material>& material_map, FILE* file);
}

namespace CObjLoader {
	void write(std::ofstream& stream, const std::vector<ObjLoader::Shape>& shapes);
	std::vector<ObjLoader::Shape> read(std::ifstream& stream);
}

#endif  // _TINY_OBJ_LOADER_H
