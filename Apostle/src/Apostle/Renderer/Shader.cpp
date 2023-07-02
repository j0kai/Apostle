#include "APpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Apostle {
	Ref<Shader> Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetRenderAPI())
		{
			case RendererAPI::API::None: AP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filepath);
		}

		AP_CORE_ASSERT(false, "Unknown render API!");
		return nullptr;
	}

	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetRenderAPI())
		{
			case RendererAPI::API::None: AP_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSource, fragmentSource);
		}

		AP_CORE_ASSERT(false, "Unknown render API!");
		return nullptr;
	}

	void ShaderLibrary::AddShader(const std::string& name, const Ref<Shader>& shader)
	{
		AP_PROFILE_FUNCTION();

		AP_CORE_ASSERT(Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::AddShader(const Ref<Shader>& shader)
	{
		AP_PROFILE_FUNCTION();

		auto& name = shader->GetName();
		AddShader(name, shader);
	}

	Ref<Shader> ShaderLibrary::LoadShader(const std::string& filepath)
	{
		AP_PROFILE_FUNCTION();

		auto shader = Shader::Create(filepath);
		AddShader(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::LoadShader(const std::string& name, const std::string& filepath)
	{
		AP_PROFILE_FUNCTION();

		auto shader = Shader::Create(filepath);
		AddShader(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::GetShader(const std::string& name)
	{
		AP_PROFILE_FUNCTION();

		AP_CORE_ASSERT(!Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name)
	{
		return m_Shaders.find(name) == m_Shaders.end();
	}

}