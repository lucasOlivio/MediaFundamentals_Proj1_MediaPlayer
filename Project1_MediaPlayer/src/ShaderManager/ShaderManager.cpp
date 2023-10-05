#include "ShaderManager.h"

#include "CommonOpengl.h"	// For all the OpenGL calls, etc.

#include <fstream>
#include <sstream>		// "string stream"
#include <vector>
#include <iostream>

#include <algorithm>		// for the std::copy
#include <iterator>			// for the back_inserter


const unsigned int MAXLINELENGTH = 65536;		// 16x1024

ShaderManager::ShaderManager()
{
	return;
}

ShaderManager::~ShaderManager()
{
	return;
}

bool ShaderManager::SetupShader(std::string programName)
{
	ShaderManager::Shader vertexShader;
	vertexShader.fileName = "vertex" + programName + ".glsl";

	ShaderManager::Shader fragmentShader;
	fragmentShader.fileName = "fragment" + programName + ".glsl";

	if (!this->CreateProgramFromFile(programName, vertexShader, fragmentShader)) {
		std::string error = this->GetLastError();
		printf("Error: Couldn't compile or link: %s", error.c_str());
		return false;
	}

	return true;
}

void ShaderManager::Destroy()
{
	for (std::map< unsigned int, ShaderProgram >::iterator it = this->m_IDToShaderProgram.begin(); it != this->m_IDToShaderProgram.end(); ++it)
	{
		glDeleteProgram(it->first);
	}
}

bool ShaderManager::UseShaderProgram(unsigned int ID)
{
	// Use the number directy... 
	// TODO: Might do a lookup to see if we really have that ID...
	glUseProgram(ID);
	return true;
}

bool ShaderManager::UseShaderProgram(std::string shaderProgramName)
{
	unsigned int shaderProgramID = this->GetIDFromShaderProgramName(shaderProgramName);

	if (shaderProgramID == 0)
	{
		return false;
	}

	glUseProgram(shaderProgramID);

	return true;
}

unsigned int ShaderManager::GetIDFromShaderProgramName(std::string shaderProgramName)
{
	std::map< std::string /*name*/, unsigned int /*ID*/ >::iterator
		itShad = this->m_shaderProgramNameToID.find(shaderProgramName);

	if (itShad == this->m_shaderProgramNameToID.end())
	{	// Didn't find it
		return 0;
	}
	return itShad->second;
}

ShaderManager::ShaderProgram*
ShaderManager::GetShaderProgramFromName(std::string shaderProgramName)
{
	unsigned int shaderProgramID = this->GetIDFromShaderProgramName(shaderProgramName);

	// Now get the actual shader

	std::map< unsigned int /*ID*/, ShaderProgram >::iterator
		itShaderProgram = this->m_IDToShaderProgram.find(shaderProgramID);

	if (itShaderProgram == this->m_IDToShaderProgram.end())
	{	// Didn't find it
		return NULL;		// or 0 or nullptr
	}

	ShaderProgram* pShaderProgram = &(itShaderProgram->second);

	return pShaderProgram;
}

void ShaderManager::SetBasePath(std::string basepath)
{
	this->m_basepath = basepath;
	return;
}

// Returns bool if didn't load
bool ShaderManager::m_LoadSourceFromFile(Shader& shader)
{
	std::string fullFileName = this->m_basepath + shader.fileName;

	std::ifstream theFile(fullFileName.c_str());
	if (!theFile.is_open())
	{
		return false;
	}

	shader.vecSource.clear();

	char* pLineTemp = new char[MAXLINELENGTH];
	while (theFile.getline(pLineTemp, MAXLINELENGTH))
	{
		std::string tempString(pLineTemp);
		//if ( tempString != "" )
		//{	// Line isn't empty, so add
		shader.vecSource.push_back(tempString);
		//}
	}

	theFile.close();
	return true;		// Return the string (from the sstream)
}

// Returns empty string if no error
// returns false if no error
bool ShaderManager::m_WasThereACompileError(unsigned int shaderID,
	std::string& errorText)
{
	errorText = "";	// No error

	GLint isCompiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		// Fill with zeros, maybe...?
		glGetShaderInfoLog(shaderID, maxLength, &maxLength, pLogText);
		// Copy char array to string
		errorText.append(pLogText);

		this->m_lastError.append("\n");
		this->m_lastError.append(errorText);


		delete[] pLogText;	// Oops

		return true;	// There WAS an error
	}
	return false; // There WASN'T an error
}

bool ShaderManager::m_WasThereALinkError(unsigned int programID, std::string& errorText)
{
	errorText = "";	// No error

	GLint wasError = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &wasError);
	if (wasError == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

		char* pLogText = new char[maxLength];
		// Fill with zeros, maybe...?
		glGetProgramInfoLog(programID, maxLength, &maxLength, pLogText);
		// Copy char array to string
		errorText.append(pLogText);

		// Extra code that Michael forgot wasn't there... 
		this->m_lastError.append("\n");
		this->m_lastError.append(errorText);


		delete[] pLogText;

		// There WAS an error
		return true;
	}

	// There WASN'T an error
	return false;
}

std::string ShaderManager::GetLastError(void)
{
	std::string lastErrorTemp = this->m_lastError;
	this->m_lastError = "";
	return lastErrorTemp;
}

bool ShaderManager::m_CompileShaderFromSource(ShaderManager::Shader& shader, std::string& error)
{
	error = "";

	const unsigned int MAXLINESIZE = 8 * 1024;	// About 8K PER LINE, which seems excessive

	unsigned int numberOfLines = static_cast<unsigned int>(shader.vecSource.size());

	// This is an array of pointers to strings. aka the lines of source
	char** arraySource = new char* [numberOfLines];
	// Clear array to all zeros (i.e. '\0' or null terminator)
	memset(arraySource, 0, numberOfLines);

	for (unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++)
	{
		unsigned int numCharacters = (unsigned int)shader.vecSource[indexLine].length();
		// Create an array of chars for each line
		arraySource[indexLine] = new char[numCharacters + 2];		// For the '\n' and '\0' at end
		memset(arraySource[indexLine], 0, numCharacters + 2);;

		// The "secure" version to copy arrays, where you specify the number of chars to copy
		strcpy_s( arraySource[indexLine], 
				  strlen( shader.vecSource[indexLine].c_str() ) + 1,	// Number of char in copy + 0 terminator
				  shader.vecSource[indexLine].c_str() );
		arraySource[indexLine][numCharacters + 0] = '\n';
		arraySource[indexLine][numCharacters + 1] = '\0';
	}

	glShaderSource(shader.ID, numberOfLines, arraySource, NULL);
	glCompileShader(shader.ID);

	// Get rid of the temp source "c" style array
	for (unsigned int indexLine = 0; indexLine != numberOfLines; indexLine++)
	{	// Delete this line
		delete[] arraySource[indexLine];
	}
	// And delete the original char** array
	delete[] arraySource;

	// Did it work? 
	std::string errorText = "";
	if (this->m_WasThereACompileError(shader.ID, errorText))
	{
		std::stringstream ssError;
		ssError << shader.GetShaderTypeString();
		ssError << " compile error: ";
		ssError << errorText;
		error = ssError.str();
		return false;
	}

	return true;
}

bool ShaderManager::CreateProgramFromFile(
	std::string shaderProgramName,
	Shader& vertexShad,
	Shader& fragShader)
{
	std::string errorText = "";


	// Shader loading happening before vertex buffer array
	vertexShad.ID = glCreateShader(GL_VERTEX_SHADER);
	vertexShad.shaderType = Shader::VERTEX_SHADER;

	// Load some text from a file...
	if (!this->m_LoadSourceFromFile(vertexShad))
	{
		return false;
	}

	errorText = "";
	if (!this->m_CompileShaderFromSource(vertexShad, errorText))
	{
		this->m_lastError = errorText;
		return false;
	}

	fragShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
	fragShader.shaderType = Shader::FRAGMENT_SHADER;
	if (!this->m_LoadSourceFromFile(fragShader))
	{
		return false;
	}

	if (!this->m_CompileShaderFromSource(fragShader, errorText))
	{
		this->m_lastError = errorText;
		return false;
	}

	ShaderProgram curProgram;
	curProgram.ID = glCreateProgram();

	glAttachShader(curProgram.ID, vertexShad.ID);
	glAttachShader(curProgram.ID, fragShader.ID);
	glLinkProgram(curProgram.ID);

	// Was there a link error? 
	errorText = "";
	if (this->m_WasThereALinkError(curProgram.ID, errorText))
	{
		std::stringstream ssError;
		ssError << "Shader program link error: ";
		ssError << errorText;
		this->m_lastError = ssError.str();
		return false;
	}

	// At this point, shaders are compiled and linked into a program

	curProgram.name = shaderProgramName;

	// Add the shader to the map
	this->m_IDToShaderProgram[curProgram.ID] = curProgram;
	// Save to other map, too
	this->m_shaderProgramNameToID[curProgram.name] = curProgram.ID;

	return true;
}
