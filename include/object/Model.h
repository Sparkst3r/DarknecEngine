#ifndef DARKNEC_MODEL_H
#define DARKNEC_MODEL_H

#include <Core.h>
#include <assimp/scene.h>
#include <render/Texture.h>

class Mesh {
public:
	Mesh();

	GLuint vao_;
	GLuint vboVertices_;
	GLuint vboNormals_;
	GLuint vboTangents_;
	GLuint vboUVs_;
	
	GLuint ibo_;

	std::vector<float>          vertices_;
	std::vector<float>          normals_;
	std::vector<float>          tangents_;
	std::vector<float>          UVs_;
	std::vector<unsigned int>   indices_;


	int numVertices_;
	int numNormals_;
	int numTangents_;
	int numUVs_;
	int numIndices_;


	bool hasNormals_;
	bool hasTangents_;
	bool hasUVS_;

	int materialIndex_;

	void setupGLBuffers();

};

class Material {
public:
	enum SHADEMODEL {
		FLAT,
		BLINN,
		PHONG,
		GOURAUD,
		TOON,
		MINNAERT,
		COOKTORR,
		ORENNAYAR,
		FRESNEL,
		PHYSICAL,
		NOSHADING,
	};

	enum MATERIALDATA {
		COLOUR_AMBIENT,
		COLOUR_DIFFUSE,
		COLOUR_SPECULAR,
		COLOUR_EMISSIVE,
		COLOUR_TRANSPARANT,
		COLSCA_SHININESS,
		COLSCA_SHININESS_STRENGTH,

		TEXTURE_DIFFUSE,
		TEXTURE_SPECULAR,
		TEXTURE_AMBIENT,
		TEXTURE_EMISSIVE,
		TEXTURE_HEIGHT,
		TEXTURE_NORMAL,
		TEXTURE_SHININESS,
		TEXTURE_OPACITY,
		TEXTURE_DISPLACE,
		TEXTURE_OCCLUSION,
		TEXTURE_PARALLAX,

		PHYS_COLOUR_BASE,
		PHYS_COLOUR_EMISSIVE,
		PHYS_METAL,
		PHYS_ROUGH,
		PHYS_SPECULAR,
		PHYS_REFRACTIND,
		PHYS_OPACITY,


		

		RENDER_OPACITY,
		RENDER_TWOSIDED,
		RENDER_WIREFRAME,
		RENDER_SHADEMODEL,


	};

	std::string getName();

	bool get(MATERIALDATA dataKey, Texture& texture);
	bool get(MATERIALDATA dataKey, glm::vec3& colourRef);
	bool get(MATERIALDATA dataKey, float& scalarRef);

	bool set(MATERIALDATA dataKey, Texture textureRef);
	bool set(MATERIALDATA dataKey, glm::vec3 colourRef);
	bool set(MATERIALDATA dataKey, float scalarRef);

	std::hash_map<MATERIALDATA, Texture> textures_;

	std::hash_map<MATERIALDATA, glm::vec3> colours_;

	std::hash_map<MATERIALDATA, float> colourScalars_;

	std::string name_;

};

class Model {
public:

	Model();

	std::vector<Mesh> meshes_;
	int numMeshes_;

	std::vector<Material> materials_;
	int numMaterials_;

	static Model ConvertAssimpToDarknec(aiScene* scene);

};



#endif // !DARKNEC_MODEL_H
