#ifndef DARKNEC_MODEL_H

#include <Core.h>
#include <assimp/scene.h>

class Messh {
public:
	Messh();

	std::vector<float>          vertices_;
	std::vector<float>          normals_;
	std::vector<float>          UVs_;
	std::vector<unsigned int>   indices_;


	int numVertices_;
	int numNormals_;
	int numUVs_;
	int numIndices_;


	bool hasNormals_;
	bool hasUVS_;

};

class Model {
public:

	Model();

	std::vector<Messh> meshes_;

	int numMeshes_;

	static Model ConvertAssimpToDarknec(aiScene* scene);

};



#endif // !DARKNEC_MODEL_H
