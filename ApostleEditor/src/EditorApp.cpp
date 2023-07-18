#include <Apostle.h>
#include <Apostle/Core/EntryPoint.h>

#include "imgui.h"
#include "EditorLayer.h"

namespace Apostle {
	
	class ApostleEditor : public Application
	{
	public:
		ApostleEditor()
			: Application("Apostle Editor")
		{
			
			PushLayer(new EditorLayer());
		}

		~ApostleEditor()
		{

		}
	};


	Apostle::Application* Apostle::CreateApplication()
	{
		return new ApostleEditor();
	}

}