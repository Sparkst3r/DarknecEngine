#include <render/shader/Shader.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <SDL.h>


/**
* Shader
* Default Constructor
* GL shader state is NULL at this point.
*/
Shader::Shader() {
}

Shader::~Shader() {
	//TODO fix destructor once stripping out user shader manipulation. Engine hasnt initialised when global variables are being created and causes premature destruction.
}

void Shader::destroy() {
	glDeleteProgram(this->ID);
}

/**
* Bind variable as fragment data output of the shader
* @param location name of variable to assign as fragment data output
*/
void Shader::bindFragmentOutput(const char* location) {
	glBindFragDataLocation(this->ID, 0, location);
}

/**
* Make shader active
*/
void Shader::use() {
	glUseProgram(this->ID);
}

/**
* Shader
*
* Creates a shader program from source code
* @param filename shader file to load
*/
Shader::Shader(const char* filename) {
	std::ifstream file(filename);

	if (file) {
		log(LogLevel::LOG_INFO, "Found shader file: %s", filename);
		std::stringstream buffer;
        buffer << file.rdbuf();

		std::vector<std::string> stages = preprocess(buffer.str());

		ID = createShader(stages, false);
		this->bindFragmentOutput("colour");

        file.close();
	}
	else {
		log(LogLevel::LOG_ERROR, "Couldn't find shader file: %s", filename);
	}

}

/**
* createShader
* Compile and link shader from parsed source code
* 
* @param stages parsed strings containing the shader stages
* @param reload should the shader replace a previous instance
* @return the shader ID 
*/
GLuint Shader::createShader(std::vector<std::string> stages, bool reload) {
	GLuint tempID = glCreateProgram();

	std::vector<GLuint> shaderIDs;
	std::string shadersUsed;

	//TODO This could really be more efficient as a for loop in itself
	#pragma region Compile stages
	if (stages[0] != "") {
		shadersUsed += "VERT ";
		GLuint stageTemp = glCreateShader(GL_VERTEX_SHADER);
		
		const char* stageChar = stages[0].c_str();
		glShaderSource(stageTemp, 1, &stageChar, NULL);
		glCompileShader(stageTemp);
		glAttachShader(tempID, stageTemp);
		shaderIDs.push_back(stageTemp);
	}
	if (stages[1] != "") {
		shadersUsed += "TESSCONT ";
		GLuint stageTemp;
		stageTemp = glCreateShader(GL_TESS_CONTROL_SHADER);
		const char* stageChar = stages[1].c_str();
		glShaderSource(stageTemp, 1, &stageChar, NULL);
		glCompileShader(stageTemp);
		glAttachShader(tempID, stageTemp);
		shaderIDs.push_back(stageTemp);
	}
	if (stages[2] != "") {
		shadersUsed += "TESSEVAL ";
		GLuint stageTemp;
		stageTemp = glCreateShader(GL_TESS_EVALUATION_SHADER);
		const char* stageChar = stages[2].c_str();
		glShaderSource(stageTemp, 1, &stageChar, NULL);
		glCompileShader(stageTemp);
		glAttachShader(tempID, stageTemp);
		shaderIDs.push_back(stageTemp);
	}
	if (stages[3] != "") {
		shadersUsed += "GEOM ";
		GLuint stageTemp;
		stageTemp = glCreateShader(GL_GEOMETRY_SHADER);
		const char* stageChar = stages[3].c_str();
		glShaderSource(stageTemp, 1, &stageChar, NULL);
		glCompileShader(stageTemp);
		glAttachShader(ID, stageTemp);
		shaderIDs.push_back(stageTemp);
	}
	if (stages[4] != "") {
		shadersUsed += "FRAG ";
		GLuint stageTemp = glCreateShader(GL_FRAGMENT_SHADER);
		const char* stageChar = stages[4].c_str();
		glShaderSource(stageTemp, 1, &stageChar, NULL);
		glCompileShader(stageTemp);
		glAttachShader(tempID, stageTemp);
		shaderIDs.push_back(stageTemp);
	}
	#pragma endregion

	glLinkProgram(tempID);

	#pragma region Print Link Status

	GLint linkStatus;
	GLint infoLogLength;
	glGetProgramiv(tempID, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(tempID, GL_INFO_LOG_LENGTH, &infoLogLength);

	LogLevel level = LogLevel::LOG_WARN;
	if (linkStatus == GL_FALSE) {
		level = LogLevel::LOG_ERROR;
	}

	if (infoLogLength > 1) {
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(tempID, infoLogLength, NULL, strInfoLog);
		log(level, "Shader link log: \n %s", strInfoLog);
		delete strInfoLog;
	}
	else {
		log(LogLevel::LOG_INFO, "Linked shader with stages: %s", shadersUsed.c_str());
	}

	#pragma endregion

	for (GLuint shaderID : shaderIDs) {
		glDetachShader(tempID, shaderID);
		glDeleteShader(shaderID);
	}


	if (reload) {
		glDeleteProgram(this->ID);
	}

	return tempID;
}

/**
* preprocess
* Parse and preprocess raw source code
*
* @param raw raw string from shader source file
* @return parsed strings of the shader stages
*/
std::vector<std::string> Shader::preprocess(std::string raw) {
	std::stringstream stream(raw);

	std::ostringstream convert;
	convert << Darknec::GLSLVersion;
	std::string versionString = "#version " + convert.str() + "\n";

	std::vector<std::string> stages;
	stages.resize(5);

	std::vector<std::string> vertList;
	std::vector<std::string> tessContList;
	std::vector<std::string> tessEvalList;
	std::vector<std::string> geomList;
	std::vector<std::string> fragList;

	std::string currentLine;

	#pragma region Seperate Stages
	while (stream.good()) {

		getline(stream, currentLine, '\n');

		if (currentLine == "#VERT") {
			vertList.push_back(versionString);
			while (currentLine != "#ENDVERT" && stream.good()) {
				getline(stream, currentLine, '\n');
				if (currentLine != "#ENDVERT") {
					vertList.push_back(currentLine);
				}
			}
		}
		else if (currentLine == "#TESSCONT") {
			tessContList.push_back(versionString);
			while (currentLine != "#ENDTESSCONT" && stream.good()) {
				getline(stream, currentLine, '\n');
				if (currentLine != "#ENDTESSCONT") {
					tessContList.push_back(currentLine);
				}
			}
		}
		else if (currentLine == "#TESSEVAL") {
			tessEvalList.push_back(versionString);
			while (currentLine != "#ENDTESSEVAL" && stream.good()) {
				getline(stream, currentLine, '\n');
				if (currentLine != "#ENDTESSEVAL") {
					tessEvalList.push_back(currentLine);
				}
			}
		}
		else if (currentLine == "#GEOM") {
			geomList.push_back(versionString);
			while (currentLine != "#ENDGEOM" && stream.good()) {
				getline(stream, currentLine, '\n');
				if (currentLine != "#ENDGEOM") {
					geomList.push_back(currentLine);
				}
			}
		}
		else if (currentLine == "#FRAG") {
			fragList.push_back(versionString);
			while (currentLine != "#ENDFRAG" && stream.good()) {
				getline(stream, currentLine, '\n');
				if (currentLine != "#ENDFRAG") {
					fragList.push_back(currentLine);
				}
			}
		}
	}
	#pragma endregion

	for (std::string string : vertList) {
		stages[0] += string.c_str();
		stages[0] += "\n";
	}
	for (std::string string : tessContList) {
		stages[1] += string.c_str();
		stages[1] += "\n";
	}
	for (std::string string : tessEvalList) {
		stages[2] += string.c_str();
		stages[2] += "\n";
	}
	for (std::string string : geomList) {
		stages[3] += string.c_str();
		stages[3] += "\n";
	}
	for (std::string string : fragList) {
		stages[4] += string.c_str();
		stages[4] += "\n";
	}





	return stages;
}

/**
* Get attribute location from variable name
* @param location name of variable to return the ID of
* @return ID representing the address of the variable
*/
AttrID Shader::getAttribute(const char* attrName) {
	AttrID attributeID = glGetAttribLocation(this->ID, attrName);
	if (attributeID == -1) {
		std::string logString = std::string("Could not bind attribute : ") + attrName;
		SDL_LogWarn(0, logString.c_str());
	}
	return attributeID;
}
/**
* Get uniform location from variable name
* @param location name of variable to return the ID of
* @return ID representing the address of the variable
*/
UnifID Shader::getUniform(const char* uniformName) {
	UnifID uniformID = glGetUniformLocation(this->ID, uniformName);

	bool shouldPrint = true;

	//TODO seriously fix this >.<
	//////////////////Begin hack /////////////////
	for (std::string str : previousErrors) {

		if (str == (std::string("Could not bind uniform: ") + std::string(uniformName))) {
			shouldPrint = false;
		}
	}
	//////////////////End hack /////////////////

	if (uniformID == -1 && shouldPrint) {
		previousErrors.push_back((std::string("Could not bind uniform: ") + std::string(uniformName)));
		log(LogLevel::LOG_ERROR, "Could not bind uniform: %s", uniformName);
	}
	return uniformID;
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