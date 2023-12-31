#pragma once

#include "Apostle.h"

#include "ParticleSystem.h"

#include <chrono>

class Sandbox2D : public Apostle::Layer
{
public:
	Sandbox2D();
	~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(Apostle::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Apostle::Event& e) override;
private:
	Apostle::ShaderLibrary m_ShaderLibrary;
	Apostle::Ref<Apostle::Shader> m_FlatColorShader;
	Apostle::Ref<Apostle::VertexArray> m_ShaderVA;

	Apostle::Ref<Apostle::Texture2D> m_CheckerboardTexture;

	const std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime = std::chrono::high_resolution_clock::now();

	Apostle::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	ParticleSystem m_ParticleSystem;
	ParticleProps m_Particle;
};