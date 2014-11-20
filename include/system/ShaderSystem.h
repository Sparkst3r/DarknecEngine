#ifndef DARKNEC_SHADERSYSTEM_H 
#define DARKNEC_SHADERSYSTEM_H

#include <State.h>
#include <render/shader/Shader.h>

class ShaderSystem {
public:

	void registerShader(std::string name, Shader shader);

	void removeShader(std::string name);

	void useShader(std::string name);

	void restorePrevious();

	Shader getShader(std::string name);

	std::string getActiveShaderName();



private:
	std::string activeShader;
	std::string previousShader;

	std::hash_map<std::string, Shader> shaderMap;
};

extern ShaderSystem* sys3;

#endif // !DARKNEC_SHADERSYSTEM_H
