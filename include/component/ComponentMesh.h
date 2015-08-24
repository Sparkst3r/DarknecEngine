#ifndef DARKNEC_COMPONENTMESH_H 
#define DARKNEC_COMPONENTMESH_H

#include <State.h>
#include <component/Component.h>
#include <component/ComponentTransform.h>
#include <component/ComponentRWUtils.h>
#include <object/CObjLoader.h>
#include <render/Texture.h>

#include <render/RendererDeferredPhong.h>

class ComponentMesh : public Component {
public:
	DECL_COMPONENT_FACTORY(ComponentMesh)
	DECL_COMPONENTREQUIREMENT_FACTORY(ComponentMesh)

	ComponentMesh(GameObject* container);

	/**
	* init
	*
	* Run after all components are loaded and if validate() returns true
	* ensuring all requirements exist and are init'd.
	*/
	virtual void init();
	virtual void update();
	virtual bool validate();

	virtual XMLNode write(XMLNode node);
	virtual void read(XMLNode node);

private:
	ComponentRequirement<ComponentTransform> transform_;

	Model model_;
	std::string modelFile_;

	std::vector<VAO> vaos_;
	std::vector<VBO> vbosVertex_;
	std::vector<VBO> vbosNormal_;
	std::vector<VBO> vbosUV_;

	std::vector<IBO> ibos_;

	GLuint s;



};
#endif // !DARKNEC_COMPONENTMESH_H

extern Renderer* renderer;
