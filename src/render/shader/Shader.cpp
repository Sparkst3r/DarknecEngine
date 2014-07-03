#include <render/shader/Shader.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <SDL.h>


/* Default Constructor */
Shader::Shader() {
}

void Shader::destroy() {
	glDeleteProgram(this->ID);
}

/* Shader Constructor */
Shader::Shader(std::vector<ShaderFile*> shaders) {
	ID = glCreateProgram();

	std::vector<GLuint> shaderIDs;
	
	for (ShaderFile* file : shaders) {
		GLuint shader = glCreateShader(file->shaderType);
		std::ifstream shaderFile = std::ifstream(file->shaderFile);
		if (shaderFile.is_open()) {
			log(LogLevel::LOG_INFO, "Loaded shader file: %s", file->shaderFile);
		}
		else {
			log(LogLevel::LOG_ERROR, "Could not load shader file: %s", file->shaderFile);
		}
		std::stringstream shaderData;
		shaderData << shaderFile.rdbuf();
		shaderFile.close();

		const std::string &shaderSourceString = shaderData.str();

		

		const char* shaderSource = shaderSourceString.c_str();

		preprocess(shaderSource);

		glShaderSource(shader, 1, &shaderSource, NULL);
		glCompileShader(shader);

		

		GLint compileStatus;
		GLint infoLogLength;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		LogLevel level = LogLevel::LOG_WARN;
		if (compileStatus == GL_FALSE) {
			level = LogLevel::LOG_ERROR;
		}

		if (infoLogLength > 1) {
			GLchar* strInfoLog = new GLchar[infoLogLength + 1];
			glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
			log(level, "Shader compile log for '%s'\n%s", file->shaderFile, strInfoLog);
			delete strInfoLog;
		}

		


		shaderIDs.push_back(shader);
		glAttachShader(ID, shader);
		delete file;
	}

	glLinkProgram(ID);

	for (GLuint shaderID : shaderIDs) {
		glDetachShader(ID, shaderID);
		glDeleteShader(shaderID);
	}
	this->bindFragmentOutput("colour");
}

Shader::Shader(const char* filename) {
	FILE* file;
	fopen_s(&file, filename, "r");

	if (!file) {

	}

}

std::vector<const char*> Shader::preprocess(const char* raw) {
	std::string versionString = "#version " + Darknec::GLSLVersion;

	std::stringstream stream(raw);

	std::vector<std::string> lines;

	while (stream.good()) {
		std::string SingleLine;
		getline(stream, SingleLine, '\n');
		lines.push_back(SingleLine);
	}

	for (std::string blash : lines) {
		log(LogLevel::LOG_INFO, "%s", blash.c_str());
	}

	std::vector<const char*> blah;
	return blah;
}

AttrID Shader::getAttribute(const char* attrName) {
	AttrID attributeID = glGetAttribLocation(this->ID, attrName);
	if (attributeID == -1) {
		std::string logString = std::string("Could not bind attribute : ") + attrName;
		SDL_LogWarn(0, logString.c_str());
	}
	return attributeID;
}
UnifID Shader::getUniform(const char* uniformName) {
	UnifID uniformID = glGetUniformLocation(this->ID, uniformName);
	
	if (uniformID == -1) {
		log(LogLevel::LOG_ERROR, "Could not bind uniform: %s", uniformName);
	}
	return uniformID;
}

void Shader::bindFragmentOutput(const char* location) {
	glBindFragDataLocation(this->ID, 0, location);
}

void Shader::use() {
	glUseProgram(this->ID);
}

#pragma region Single Values

/**
* Set float uniform value in shader program
* @param location name of variable to insert value in to
* @param value float value
*/
void Shader::setUniformFloat(const char* location, float value) {
	glUniform1f(this->getUniform(location), value);
}
/**
* Set 2 float uniform value in shader program
* @param location name of variable to insert value in to
* @param value float value
* @param value2 float value 2
*/
void Shader::setUniformFloat2(const char* location, float value, float value2) {
	glUniform2f(this->getUniform(location), value, value2);
}
/**
* Set 3 float uniform value in shader program
* @param location name of variable to insert value in to
* @param value float value
* @param value2 float value 2
* @param value3 float value 3
*/
void Shader::setUniformFloat3(const char* location, float value, float value2, float value3) {
	glUniform3f(this->getUniform(location), value, value2, value3);
}
/**
* Set 4 float uniform value in shader program
* @param location name of variable to insert value in to
* @param value float value
* @param value2 float value 2
* @param value3 float value 3
* @param value4 float value 4
*/
void Shader::setUniformFloat4(const char* location, float value, float value2, float value3, float value4) {
	glUniform4f(this->getUniform(location), value, value2, value3, value4);
}

/**
* Set int uniform value in shader program
* @param location name of variable to insert value in to
* @param value int value
*/
void Shader::setUniformInt(const char* location, int value) {
	glUniform1i(this->getUniform(location), value);
}
/**
* Set 2 int uniform value in shader program
* @param location name of variable to insert value in to
* @param value int value
* @param value2 int value 2
*/
void Shader::setUniformInt2(const char* location, int value, int value2) {
	glUniform2i(this->getUniform(location), value, value2);
}
/**
* Set 3 int uniform value in shader program
* @param location name of variable to insert value in to
* @param value int value
* @param value2 int value 2
* @param value3 int value 3
*/
void Shader::setUniformInt3(const char* location, int value, int value2, int value3) {
	glUniform3i(this->getUniform(location), value, value2, value3);
}
/**
* Set 4 int uniform value in shader program
* @param location name of variable to insert value in to
* @param value int value
* @param value2 int value 2
* @param value3 int value 3
* @param value4 int value 4
*/
void Shader::setUniformInt4(const char* location, int value, int value2, int value3, int value4) {
	glUniform4i(this->getUniform(location), value, value2, value3, value4);
}

/**
* Set unsigned int uniform value in shader program
* @param location name of variable to insert value in to
* @param value unsigned int value
*/
void Shader::setUniformUnsignedInt(const char* location, unsigned int value) {
	glUniform1ui(this->getUniform(location), value);
}
/**
* Set 2 unsigned int uniform value in shader program
* @param location name of variable to insert value in to
* @param value unsigned int value
* @param value2 unsigned int value 2
*/
void Shader::setUniformUnsignedInt2(const char* location, unsigned int value, unsigned int value2) {
	glUniform2ui(this->getUniform(location), value, value2);
}
/**
* Set 3 unsigned int uniform value in shader program
* @param location name of variable to insert value in to
* @param value unsigned int value
* @param value2 unsigned int value 2
* @param value3 unsigned int value 3
*/
void Shader::setUniformUnsignedInt3(const char* location, unsigned int value, unsigned int value2, unsigned int value3) {
	glUniform3ui(this->getUniform(location), value, value2, value3);
}
/**
* Set 4 unsigned int uniform value in shader program
* @param location name of variable to insert value in to
* @param value unsigned int value
* @param value2 unsigned int value 2
* @param value3 unsigned int value 3
* @param value4 unsigned int value 4
*/
void Shader::setUniformUnsignedInt4(const char* location, unsigned int value, unsigned int value2, unsigned int value3, unsigned int value4) {
	glUniform4ui(this->getUniform(location), value, value2, value3, value3);
}

#pragma endregion