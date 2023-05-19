#pragma once

#include <string>

namespace Apostle {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
	};

	class ShaderLibrary
	{
	public:
		void AddShader(const Ref<Shader>& shader);
		void AddShader(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> LoadShader(const std::string& filepath); // ex: Texture.glsl name = "Texture"
		Ref<Shader> LoadShader(const std::string& name, const std::string& filepath);

		Ref<Shader> GetShader(const std::string& name);

		bool Exists(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};

}