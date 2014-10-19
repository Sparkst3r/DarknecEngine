#ifndef DARKNEC_COBJLOADER 
#define DARKNEC_COBJLOADER

#include <Core.h>
#include <object/Model.h>

namespace Darknec {
	namespace CObjLoader {
		void write(std::ofstream& stream, Model meshes);
		Model read(std::ifstream& stream);
	}
}


#endif // !DARKNEC_COBJLOADER
