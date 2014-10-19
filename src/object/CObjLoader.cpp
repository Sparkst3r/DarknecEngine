#include <object/CObjLoader.h>
#include <State.h>

namespace Darknec {
	namespace CObjLoader {
		void write(std::ofstream &stream, Model model) {
			const auto dataWidth = sizeof(float);
			const int meshCount = model.numMeshes_;

			stream.write((const char*) &meshCount, sizeof(int));

			for (int mesh = 0; mesh < model.numMeshes_; mesh++) {
				const int indexCount = model.meshes_[mesh].numIndices_;
				const int vertexCount = model.meshes_[mesh].numVertices_;
				
				//Write indices
				stream.write((const char*) &indexCount, dataWidth);
				stream.write((const char*) &model.meshes_[mesh].indices_[0], dataWidth * indexCount);

				//Write vertices
				stream.write((const char*) &vertexCount, dataWidth);
				stream.write((const char*) &model.meshes_[mesh].vertices_[0], dataWidth * vertexCount);


				//Write normals if the mesh has them
				if (model.meshes_[mesh].hasNormals_) {
					const int normalCount = model.meshes_[mesh].numNormals_;
					int hasNorm = 1;
					stream.write((const char*) &hasNorm, dataWidth);
					stream.write((const char*) &normalCount, dataWidth);
					stream.write((const char*) &model.meshes_[mesh].normals_[0], dataWidth * normalCount);
				}
				else {
					int hasNormal = 0;
					stream.write((const char*) &hasNormal, dataWidth);
				}
				
				//Write UVS if the mesh has them
				if (model.meshes_[mesh].hasUVS_) {
					const int uvCount = model.meshes_[mesh].numUVs_;
					int hasUV = 1;
					stream.write((const char*) &hasUV, dataWidth);
					stream.write((const char*) &uvCount, dataWidth);
					stream.write((const char*) &model.meshes_[mesh].UVs_[0], dataWidth * uvCount);
				}
				else {
					int hasUV = 0;
					stream.write((const char*) &hasUV, dataWidth);
				}


				/*aiMaterial* mat = meshes->mMaterials[meshes->mMeshes[mesh]->mMaterialIndex];


				aiColor3D ambient;
				float ambientF[3];
				mat->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
				ambientF[0] = ambient.r;
				ambientF[1] = ambient.g;
				ambientF[2] = ambient.b;

				aiColor3D diffuse;
				float diffuseF[3];
				mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
				diffuseF[0] = diffuse.r;
				diffuseF[1] = diffuse.g;
				diffuseF[2] = diffuse.b;

				aiColor3D specular;
				float specularF[3];
				mat->Get(AI_MATKEY_COLOR_AMBIENT, specular);
				specularF[0] = specular.r;
				specularF[1] = specular.g;
				specularF[2] = specular.b;

				stream.write((const char*) &ambientF[0], dataWidth * 3);
				stream.write((const char*) &diffuseF[0], dataWidth * 3);
				stream.write((const char*) &specularF[0], dataWidth * 3);*/

			}
			stream.close();
		}


		Model read(std::ifstream& stream) {
			Model model = Model();
			const auto dataWidth = sizeof(float); //Size of float

			int start = SDL_GetTicks();

			int meshCount = 0;
			stream.read((char*) &meshCount, sizeof(int));
			model.numMeshes_ = meshCount;
			model.meshes_;


			for (int mesh = 0; mesh < meshCount; mesh++) {
				Messh messh;
				


				//Read indices
				int indexCount = 0;
				stream.read((char*) &indexCount, dataWidth);
				messh.indices_ = std::vector<unsigned int>(indexCount);
				messh.numIndices_ = indexCount;
				stream.read((char*) &messh.indices_[0], indexCount * dataWidth);

				//Read vertices
				int vertexCount = 0;
				stream.read((char*) &vertexCount, dataWidth);
				messh.vertices_ = std::vector<float>(vertexCount);
				messh.numVertices_ = vertexCount;
				stream.read((char*) &messh.vertices_[0], vertexCount * dataWidth);

				int hasNorm = 0;
				stream.read((char*) &hasNorm, dataWidth);
				if (hasNorm) {
					messh.hasNormals_ = true;
					int normalCount = 0;
					stream.read((char*) &normalCount, dataWidth);
					messh.normals_ = std::vector<float>(normalCount);
					messh.numNormals_ = normalCount;
					stream.read((char*) &messh.normals_[0], normalCount * dataWidth);
				}


				int hasUV = 0;
				stream.read((char*) &hasUV, dataWidth);
				if (hasUV) {
					messh.hasUVS_ = true;
					int uvCount = 0;
					stream.read((char*) &uvCount, dataWidth);
					messh.UVs_ = std::vector<float>(uvCount);
					messh.numUVs_ = uvCount;
					stream.read((char*) &messh.UVs_[0], uvCount * dataWidth);
				}

				//stream.read((char*) &shapes[shapeIndex].material.ambient[0], 3 * dataWidth);
				//stream.read((char*) &shapes[shapeIndex].material.diffuse[0], 3 * dataWidth);
				//stream.read((char*) &shapes[shapeIndex].material.specular[0], 3 * dataWidth);
				//

				model.meshes_.push_back(messh);
			}

			Darknec::logger("COBJLoader", Darknec::LogLevel::LOG_INFO, "Took %i milliseconds", SDL_GetTicks() - start);

			stream.close();
			return model;
		}
	}
}