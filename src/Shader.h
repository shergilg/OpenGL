#pragma once

#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath; // for debugging
	unsigned int m_RendererID;
	// caching system for uniforms
	std::unordered_map<std::string, int>m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const; //for shaders its called Useprogram but we will call it Bind to keep everything consistent
	void Unbind() const;

	//Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};