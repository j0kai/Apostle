#include "Sandbox2D.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

#include <time.h>
/// <summary>
/// A templated timer used as a basic profiler.
/// 
///  TODO:
///  Currently taking in an rvalue and then copying it, could implement std::string_view
///	 Might be better to move the lambda into the Timer class instead(?)
/// 
/// </summary>
template<typename Fn>
class Timer
{
public:
	Timer(const char* scopeName, Fn&& func)
		: m_ScopeName(scopeName), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_Func({ m_ScopeName, duration });
	}
private:
	const char* m_ScopeName;
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};

#define PROFILE_SCOPE(scopeName) Timer timer##__LINE__(scopeName, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = Apostle::Texture2D::Create("assets/textures/Checkerboard-Grey.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Apostle::Timestep ts)
{
	//using namespace std::literals;
	PROFILE_SCOPE("Sandbox2D::OnUpdate");
	
	// Update
	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(ts);
	}

	// Render
	{
		PROFILE_SCOPE("Renderer Prep");
		Apostle::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Apostle::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Apostle::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Apostle::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		Apostle::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
		Apostle::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerboardTexture);
		Apostle::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	for (auto& result : m_ProfileResults)
	{
		AP_CORE_ASSERT(strlen(result.Name) <= 40, "Name length exceeds buffer; limit name to 40 characters or less!");
		char label[50];
		strcpy_s(label, 8, "%.3fms "); // "0.000ms " = 8 chars
		strcat_s(label, 40, result.Name);
		ImGui::Text(label, result.Time);
	}

	m_ProfileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Apostle::Event& e)
{
	m_CameraController.OnEvent(e);
}
