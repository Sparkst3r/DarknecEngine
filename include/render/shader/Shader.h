#ifndef DARKNEC_SHADER_H
#define DARKNEC_SHADER_H

#include <State.h>
#include <vector>
#include <string>
#include <render/GLTypeDef.h>

struct ShaderFile {
	ShaderFile(GLenum shaderType, const char* shaderFile) {
		this->shaderType = shaderType;
		this->shaderFile = shaderFile;
	}

	GLenum shaderType;
	const char* shaderFile;
};

class Shader {

public:

	Shader();
	Shader(std::vector<ShaderFile*> shaders);

	Shader(const char* filename);



	void destroy();

	AttrID getAttribute(const char* attrName);
	UnifID getUniform(const char* uniformName);

	void use();

	void bindFragmentOutput(const char* location);


#pragma region Single Values
	void setUniformFloat	(const char* location, float value);
	void setUniformFloat2	(const char* location, float value, float value2);
	void setUniformFloat3	(const char* location, float value, float value2, float value3);
	void setUniformFloat4	(const char* location, float value, float value2, float value3, float value4);

	void setUniformInt	(const char* location, int value);
	void setUniformInt2	(const char* location, int value, int value2);
	void setUniformInt3	(const char* location, int value, int value2, int value3);
	void setUniformInt4	(const char* location, int value, int value2, int value3, int value4);

	void setUniformUnsignedInt	(const char* location, unsigned int value);
	void setUniformUnsignedInt2	(const char* location, unsigned int value, unsigned int value2);
	void setUniformUnsignedInt3	(const char* location, unsigned int value, unsigned int value2, unsigned int value3);
	void setUniformUnsignedInt4	(const char* location, unsigned int value, unsigned int value2, unsigned int value3, unsigned int value4);

#pragma endregion

#pragma region Vector

	void setUniformFloatVec2(const char* location, glm::vec2 value);
	void setUniformFloatVec3(const char* location, glm::vec3 value);
	void setUniformFloatVec4(const char* location, glm::vec4 value);

	void setUniformIntVec2(const char* location, glm::vec2 value);
	void setUniformIntVec3(const char* location, glm::vec3 value);
	void setUniformIntVec4(const char* location, glm::vec4 value);

	void setUniformUnsignedIntVec2(const char* location, glm::vec2 value);
	void setUniformUnsignedIntVec3(const char* location, glm::vec3 value);
	void setUniformUnsignedIntVec4(const char* location, glm::vec4 value);

#pragma endregion

#pragma region Matrix
	void setUniformFloatMatrix2(const char* location, bool transpose, glm::mat2 matrix);
	void setUniformFloatMatrix3(const char* location, bool transpose, glm::mat3 matrix);
	void setUniformFloatMatrix4(const char* location, bool transpose, glm::mat4 matrix);

	void setUniformFloatMatrix2x3(const char* location, bool transpose, glm::mat2x3 matrix);
	void setUniformFloatMatrix2x4(const char* location, bool transpose, glm::mat2x4 matrix);

	void setUniformFloatMatrix3x2(const char* location, bool transpose, glm::mat3x2 matrix);
	void setUniformFloatMatrix3x4(const char* location, bool transpose, glm::mat3x4 matrix);

	void setUniformFloatMatrix4x2(const char* location, bool transpose, glm::mat4x2 matrix);
	void setUniformFloatMatrix4x3(const char* location, bool transpose, glm::mat4x3 matrix);

#pragma endregion

private :
	GLuint ID;
	Logger log = Darknec::DLogger.getLogger("Shader");

	std::vector<const char*> preprocess(const char* raw);

};

#endif