#include <object/Model.h>
#include <State.h>

Messh::Messh() {
	hasNormals_ = false;
	hasNormals_ = false;
	numIndices_ = 0;
	numNormals_ = 0;
	numUVs_ = 0;
	numVertices_ = 0;
}

Model::Model() {
}


Model Model::ConvertAssimpToDarknec(aiScene* scene) {
	Model model;

	model.numMeshes_ = scene->mNumMeshes;
	Darknec::logger("sss %i", scene->mNumMeshes);
	for (int mesh = 0; mesh < scene->mNumMeshes; mesh++) {

		Messh messh = Messh();
		int vertCount = scene->mMeshes[mesh]->mNumVertices;
		int normCount = scene->mMeshes[mesh]->mNumVertices;
		int indiCount = scene->mMeshes[mesh]->mNumFaces * 3; //All imported models must be comprised of tris.
		int uvCount = scene->mMeshes[mesh]->mNumVertices;


		messh.numIndices_ = indiCount;
		messh.numNormals_ = normCount * 3;
		messh.numVertices_ = vertCount * 3;
		messh.numUVs_ = uvCount * 2;
		messh.hasNormals_ = scene->mMeshes[mesh]->HasNormals();
		messh.hasUVS_ = scene->mMeshes[mesh]->HasTextureCoords(0);

		for (int face = 0; face < scene->mMeshes[mesh]->mNumFaces; face++) {
			aiFace facce = scene->mMeshes[mesh]->mFaces[face];
			messh.indices_.push_back(facce.mIndices[0]);
			messh.indices_.push_back(facce.mIndices[1]);
			messh.indices_.push_back(facce.mIndices[2]);
		}


		for (int vertex = 0; vertex < vertCount; vertex++) {
			aiVector3D vector = scene->mMeshes[mesh]->mVertices[vertex];
			messh.vertices_.push_back(vector.x);
			messh.vertices_.push_back(vector.y);
			messh.vertices_.push_back(vector.z);
		}

		if (messh.hasNormals_) {
			for (int normal = 0; normal < normCount; normal++) {
				aiVector3D vector = scene->mMeshes[mesh]->mNormals[normal];
				messh.normals_.push_back(vector.x);
				messh.normals_.push_back(vector.y);
				messh.normals_.push_back(vector.z);
			}
		}

		if (messh.hasUVS_) {
			for (int uv = 0; uv < uvCount; uv++) {
				aiVector3D vector = scene->mMeshes[mesh]->mTextureCoords[0][uv];
				messh.UVs_.push_back(vector.x);
				messh.UVs_.push_back(vector.y);
			}
		}
		
		model.meshes_.push_back(messh);
		Darknec::logger("Derpy %i", model.meshes_.size());
	}
	


	return model;
}
