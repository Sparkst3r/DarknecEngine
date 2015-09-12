#include <system/ShaderSystem.h>

void ShaderSystem::registerShader(std::string name, Shader shader) {
	this->shaderMap[name] = shader;
}

void ShaderSystem::removeShader(std::string name) {
	std::hash_map<std::string, Shader>::iterator pair = this->shaderMap.find(name);

	this->shaderMap.erase(pair);
}

void ShaderSystem::useShader(std::string name) {
	if (activeShader != name) {
		this->previousShader = this->activeShader;
		this->shaderMap[name].use();
		this->activeShader = name;
	}
}

void ShaderSystem::restorePrevious() {
	if (this->previousShader != std::string("")) {
		this->shaderMap[previousShader].use();
	}
}

Shader ShaderSystem::getShader(std::string name) {
	this->useShader(name);
	return this->shaderMap[name];
}

std::string ShaderSystem::getActiveShaderName() {
	return this->activeShader;
}


ShaderSystem* sys3 = new ShaderSystem();