#include "APpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <filesystem>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Apostle {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		AP_PROFILE_FUNCTION();

		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;	

		AP_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		AP_PROFILE_FUNCTION();

		std::string source = ReadFile(filepath);
		auto shaderSources = ParseShader(source);
		CompileShader(shaderSources);

		// Set m_Name to be the shader's file name 
		std::filesystem::path path = filepath;
		m_Name = path.stem().string(); // Returns the file's name stripped of the extension (.glsl)
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
		:	m_Name(name)
	{
		AP_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSource;
		sources[GL_FRAGMENT_SHADER] = fragmentSource;
		CompileShader(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		AP_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		AP_PROFILE_FUNCTION();

		std::string result;
		std::ifstream stream(filepath, std::ios::in | std::ios::binary);
		if (stream)
		{
			stream.seekg(0, std::ios::end);
			result.resize(stream.tellg());
			stream.seekg(0, std::ios::beg);
			stream.read(&result[0], result.size());
			stream.close();
		}
		else
		{
			AP_CORE_ERROR("Could not open file: '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::ParseShader(const std::string& source)
	{
		AP_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t position = source.find(typeToken, 0);
		while (position != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", position);
			AP_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t startOfType = position + typeTokenLength + 1;
			std::string type = source.substr(startOfType, eol - startOfType); /* params: 1 - start location, 2 - no. of characters of substr */
			AP_CORE_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified!");

			size_t nextLinePosition = source.find_first_not_of("\r\n", eol);
			position = source.find(typeToken, nextLinePosition);
			shaderSources[ShaderTypeFromString(type)] = 
				source.substr(nextLinePosition, 
					position - (nextLinePosition == std::string::npos ? source.size() - 1 : nextLinePosition));
		}

		return shaderSources;
	}

	void OpenGLShader::CompileShader(const std::unordered_map<GLenum, std::string>& shaderSources)
	{
		AP_PROFILE_FUNCTION();

		GLuint program = glCreateProgram();
		AP_CORE_ASSERT(shaderSources.size() <= 2, "Apostle only supports two shaders for now");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto&& [type, src] : shaderSources)
		{
			GLuint shader = glCreateShader(type);

			const GLchar* source = src.c_str();
			glShaderSource(shader, 1, &source, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				AP_CORE_ERROR("{0}", infoLog.data());
				AP_CORE_ASSERT(false, "Shader compilation failed!");
				break;
			}
			
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for(auto id : glShaderIDs)
				glDeleteShader(id);

			AP_CORE_ERROR("{0}", infoLog.data());
			AP_CORE_ASSERT(false, "Shader linking failed!");
			return;
		}

		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		m_RendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		AP_PROFILE_FUNCTION();

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		AP_PROFILE_FUNCTION();

		glUseProgram(0);
	}

	void OpenGLShader::SetInt(const std::string& name, int value)
	{
		AP_PROFILE_FUNCTION();

		UploadUniformInt(name, value);
	}

	void OpenGLShader::SetFloat(const std::string& name, float value)
	{
		AP_PROFILE_FUNCTION();

		UploadUniformFloat(name, value);
	}

	void OpenGLShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		AP_PROFILE_FUNCTION();

		UploadUniformFloat2(name, value);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		AP_PROFILE_FUNCTION();

		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		AP_PROFILE_FUNCTION();

		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		AP_PROFILE_FUNCTION();

		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

}