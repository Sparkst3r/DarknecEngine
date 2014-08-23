#ifndef DARKNEC_COMPONENTMESH_H 
#define DARKNEC_COMPONENTMESH_H

#include <component/Component.h>
#include <component/ComponentTransform.h>
#include <object/Mesh.h>
#include <render/shader/Shader.h>

class ComponentMesh : public Component {
public:
	DECL_COMPONENT_FACTORY(ComponentMesh)

	ComponentMesh(GameObject* container);

	virtual rapidxml::xml_node<>* write(rapidxml::xml_node<>* node);
	virtual void read(rapidxml::xml_node<>* node);
	virtual void update();

	void renderObject();
	void setShader(Shader* shader);
	void setMesh(Mesh* model);
	void setUBO(UBO matrices);
	void setTransform(std::string str);

private:
	std::string trans;

	Mesh* model_ = NULL;
	Shader* shader_;
	UBO matrices_;
};
#endif // !DARKNEC_COMPONENTMESH_H
