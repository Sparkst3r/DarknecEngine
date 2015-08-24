#ifndef DARKNEC_COBJLOADER 
#define DARKNEC_COBJLOADER

#include <Core.h>
#include <object/Model.h>

namespace Darknec {
	namespace CObjLoader {
		void write(std::ofstream& stream, Model meshes);
		Model read(Model model, std::string filename);
	}
}


#endif // !DARKNEC_COBJLOADER
