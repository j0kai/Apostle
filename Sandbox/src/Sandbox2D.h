#pragma once

#include "Apostle.h"

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
	Apostle::Ref<Apostle::Shader> m_FlatColorShader;
	Apostle::Ref<Apostle::VertexArray> m_SquareVA;

	Apostle::Ref<Apostle::Texture2D> m_CheckerboardTexture;

	Apostle::OrthographicCameraController m_CameraController;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;
};