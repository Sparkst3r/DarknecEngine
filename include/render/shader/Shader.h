#ifndef DARKNEC_SHADER_H
#define DARKNEC_SHADER_H

#include <Core.h>

#include <render/GLTypeDef.h>

/**
* Shader
* 
* GLSL shader
*/
class Shader {
public:

	Shader::Shader() {}


	Shader(const char* filename);
	~Shader();


	void destroy();
	void use();
	void reload();

	AttrID getAttribute(const char* attrName);
	UnifID getUniform(const char* uniformName);

	void bindFragmentOutput(const char* location);


#pragma region Single Values
	/**
	* Set float uniform value in shader program
	* @param location name of variable to insert value in to
	* @param value float value
	*/
	void setUniformFloat(const char* location, float value);
	/**
	* Set 2 float uniform value in shader program
	* @param location name of variable to insert value in to
	* @param value float value
	* @param value2 float value 2
	*/
	void setUniformFloat2(const char* location, float value, float value2);
	/**
	* Set 3 float uniform value in shader program
	* @param location name of variable to insert value in to
	* @param value float value
	* @param value2 float value 2
	* @param value3 float value 3
	*/
	void setUniformFloat3(const char* location, float value, float value2, float value3);
	/**
	* Set 4 float uniform value in shader program
	* @param location name of variable to insert value in to
	* @param value float value
	* @param value2 float value 2
	* @param value3 float value 3
	* @param value4 float value 4
	*/
	void setUniformFloat4(const char* location, float value, float value2, float value3, float value4);

	/**
	* Set int uniform value in shader program
	* @param location name of variable to insert value in to
	* @param value int value
	*/
	void setUniformInt(const char* location, int value);
	/**
	* Set 2 int uniform value in shader program
	* @param location name of variable to insert value in to
	* @param value int value
	* @param value2 int value 2
	*/
	void setUniformInt2(const char* location, int value, int value2);
	/**
	* Set 3 int uniform value in shader program
	* @param location name of variable to insert value in to
	* @param value int value
	* @param value2 int value 2
	* @param value3 int value 3
	*/
	void setUniformInt3(const char* location, int value, int value2, int value3);
	/**
	* Set 4 int uniform value in shader program
	* @param location name of variable to insert value in to
	* @param value int value
	* @param value2 int value 2
	* @param value3 int value 3
	* @param value4 int value 4
	*/
	void setUniformInt4(const char* location, int value, int value2, int value3, int value4);

	/**
	* Set unsigned int uniform value in shader program
	* @param location name of variable to insert value in to
	* @param value unsigned int value
	*/
	void setUniformUnsignedInt(const char* location, unsigned int value);
	/**
	* Set 2 unsigned int uniform value in shader program
	* @param location name of variable to insert value in to
	* @param value unsigned int value
	* @param value2 unsigned int value 2
	*/
	void setUniformUnsignedInt2(const char* location, unsigned int value, unsigned int value2);
	/**
	* Set 3 unsigned int uniform value in shader program
	* @param location name of variable to insert value in to
	* @param value unsigned int value
	* @param value2 unsigned int value 2
	* @param value3 unsigned int value 3
	*/
	void setUniformUnsignedInt3(const char* location, unsigned int value, unsigned int value2, unsigned int value3);
	/**
	* Set 4 unsigned int uniform value in shader program
	* @param location name of variable to insert value in to
	* @param value unsigned int value
	* @param value2 unsigned int value 2
	* @param value3 unsigned int value 3
	* @param value4 unsigned int value 4
	*/
	void setUniformUnsignedInt4(const char* location, unsigned int value, unsigned int value2, unsigned int value3, unsigned int value4);


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

private:
	GLuint ID = 0;
	const char* filename;

	//TODO fix shader error handling
	//For sanity. Prevents console spam but messy hack.
	std::vector<std::string> previousErrors;

	std::vector<std::string> preprocess(std::string raw);

	GLuint createShader(std::vector<std::string> stages, bool reload);

};

#endif