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

				//Write mesh material index
				stream.write((const char*) &model.meshes_[mesh].materialIndex_, dataWidth);



				//stream.write((const char*) &ambientF[0], dataWidth * 3);
				//stream.write((const char*) &diffuseF[0], dataWidth * 3);
				//stream.write((const char*) &specularF[0], dataWidth * 3);*/
			}

			stream.write((const char*) &model.numMaterials_, dataWidth);

			const int procedeFlag = -1;
			const int colourFlag = -2;

			for (Material mat : model.materials_) {
				//If there are material colours. There should never not be though.
				if (!mat.colours_.empty()) {
					stream.write((const char*) &colourFlag, dataWidth);

					typedef std::hash_map<int, glm::vec3>::iterator ite;
					for (ite iterator = mat.colours_.begin(); iterator != mat.colours_.end(); iterator++) {
						int enumIndex = iterator->first;
						glm::vec3 colour = iterator->second;

						//Write index
						stream.write((const char*) &enumIndex, dataWidth);
						//Write colour floats
						stream.write((const char*) &colour[0], dataWidth * 3);
					}

					//Write procede flag
					stream.write((const char*) &procedeFlag, dataWidth);
				}
				else {
					Darknec::logger(LogLevel::LOG_ERROR, "COBJ file does not contain Material colour data! This is really bad.");
					//Write procede flag
					stream.write((const char*) &procedeFlag, dataWidth);
				}




			}




			stream.close();
		}


		Model read(std::string filename) {
			std::ifstream stream = std::ifstream(filename, std::ios::binary);

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

				//Read mesh material index
				stream.read((char*) &messh.materialIndex_, dataWidth);

				//stream.read((char*) &shapes[shapeIndex].material.ambient[0], 3 * dataWidth);
				//stream.read((char*) &shapes[shapeIndex].material.diffuse[0], 3 * dataWidth);
				//stream.read((char*) &shapes[shapeIndex].material.specular[0], 3 * dataWidth);
				//

				model.meshes_.push_back(messh);
			}

			stream.read((char*) &model.numMaterials_, dataWidth);

			for (int matIndex = 0; matIndex < model.numMaterials_; matIndex++) {
				Material mater;
				int currentIndex = -1;
				stream.read((char*) &currentIndex, dataWidth);

				if (currentIndex == -2) {
					while (currentIndex != -1) {
						stream.read((char*) &currentIndex, dataWidth);
						if (currentIndex != -1) {
							int enumIndex = currentIndex;
							glm::vec3 colour;
							stream.read((char*) &colour[0], dataWidth * 3);
							mater.colours_[enumIndex] = colour;
						}
					}
				}
			}




			Darknec::logger("COBJLoader", Darknec::LogLevel::LOG_LOG, "Loading object: %s, Took %i milliseconds", filename.c_str(), SDL_GetTicks() - start);

			stream.close();
			return model;
		}
	}
}