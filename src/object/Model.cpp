#include <object/Model.h>
#include <State.h>

Mesh::Mesh() {
	hasNormals_ = false;
	hasNormals_ = false;
	numIndices_ = 0;
	numNormals_ = 0;
	numUVs_ = 0;
	numVertices_ = 0;
}


void Mesh::setupGLBuffers() {
	glGenVertexArrays(1, &this->vao_);
	glBindVertexArray(this->vao_);

	glGenBuffers(1, &this->ibo_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->numIndices_ * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);


	glEnableVertexAttribArray(0);
	glGenBuffers(1, &this->vboVertices_);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboVertices_);
	glBufferData(GL_ARRAY_BUFFER, this->numVertices_ * sizeof(float), &this->vertices_[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);

	if (this->hasNormals_) {
		glEnableVertexAttribArray(1);
		glGenBuffers(1, &this->vboNormals_);
		glBindBuffer(GL_ARRAY_BUFFER, this->vboNormals_);
		glBufferData(GL_ARRAY_BUFFER, this->numNormals_ * sizeof(float), &this->normals_[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	}

	if (this->hasUVS_) {
		glEnableVertexAttribArray(2);
		glGenBuffers(1, &this->vboUVs_);
		glBindBuffer(GL_ARRAY_BUFFER, this->vboUVs_);
		glBufferData(GL_ARRAY_BUFFER, this->numUVs_ * sizeof(float), &this->UVs_[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	}
}

std::string Material::getName() {
	return name_;
}

bool Material::get(MATERIALDATA dataKey, Texture& textureRef) {
	if (dataKey >= MATERIALDATA::TEXTURE_DIFFUSE && dataKey <= MATERIALDATA::TEXTURE_PARALLAX) {
		textureRef = this->textures_[dataKey];
		return true;
	}
	return false;
}
bool Material::get(MATERIALDATA dataKey, glm::vec3& colourRef) {
	if (dataKey >= MATERIALDATA::COLOUR_AMBIENT && dataKey <= MATERIALDATA::COLOUR_TRANSPARANT) {
		colourRef = this->colours_[dataKey];
		return true;
	}
	return false;
}
bool Material::get(MATERIALDATA dataKey, float& scalarRef) {
	if (dataKey >= MATERIALDATA::COLSCA_SHININESS && dataKey <= MATERIALDATA::COLSCA_SHININESS_STRENGTH) {
		scalarRef = this->colourScalars_[dataKey];
		return true;
	}
	return false;
}





bool Material::set(MATERIALDATA dataKey, Texture textureRef) {
	if (dataKey >= MATERIALDATA::TEXTURE_DIFFUSE && dataKey <= MATERIALDATA::TEXTURE_PARALLAX) {
		this->textures_[dataKey] = textureRef;
		return true;
	}
	return false;
}
bool Material::set(MATERIALDATA dataKey, glm::vec3 colourRef) {
	if (dataKey >= MATERIALDATA::COLOUR_AMBIENT && dataKey <= MATERIALDATA::COLOUR_TRANSPARANT) {
		this->colours_[dataKey] = colourRef;
		return true;
	}
	return false;
}
bool Material::set(MATERIALDATA dataKey, float scalarRef) {
	if (dataKey >= MATERIALDATA::COLSCA_SHININESS && dataKey <= MATERIALDATA::COLSCA_SHININESS_STRENGTH) {
		this->colourScalars_[dataKey] = scalarRef;
		return true;
	}
	return false;
}


Model::Model() {}


Model Model::ConvertAssimpToDarknec(aiScene* scene) {
	Model model;

	model.numMeshes_ = scene->mNumMeshes;
	for (unsigned int mesh = 0; mesh < scene->mNumMeshes; mesh++) {

		Mesh messh = Mesh();
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

		for (unsigned int face = 0; face < scene->mMeshes[mesh]->mNumFaces; face++) {
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
				messh.UVs_.push_back(1- vector.y);
			}
		}

		messh.materialIndex_ = scene->mMeshes[mesh]->mMaterialIndex;
		model.meshes_.push_back(messh);
	}

	model.numMaterials_ = scene->mNumMaterials;

	for (unsigned int mat = 0; mat < scene->mNumMaterials; mat++) {
		Material darkMat;
		aiMaterial* aiMat = scene->mMaterials[mat];


		aiColor3D aiAmb;
		aiMat->Get(AI_MATKEY_COLOR_AMBIENT, aiAmb);
		glm::vec3 darkAmb = glm::vec3(aiAmb.r, aiAmb.g, aiAmb.b);
		darkMat.set(Material::MATERIALDATA::COLOUR_AMBIENT, darkAmb);

		aiColor3D aiDif;
		aiMat->Get(AI_MATKEY_COLOR_AMBIENT, aiDif);
		glm::vec3 darkDif = glm::vec3(aiDif.r, aiDif.g, aiDif.b);
		darkMat.set(Material::MATERIALDATA::COLOUR_DIFFUSE, darkDif);

		aiColor3D aiSpe;
		aiMat->Get(AI_MATKEY_COLOR_AMBIENT, aiSpe);
		glm::vec3 darkSpe = glm::vec3(aiSpe.r, aiSpe.g, aiSpe.b);
		darkMat.set(Material::MATERIALDATA::COLOUR_DIFFUSE, darkSpe);

		model.materials_.push_back(darkMat);
	}


	return model;
}
