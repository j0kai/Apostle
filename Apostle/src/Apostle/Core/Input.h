#pragma once

#include "APpch.h"
#include "Apostle/Core/Core.h"

namespace Apostle {

	class APOSTLE_API Input
	{
	public:
		virtual ~Input() = default;
		
		static bool IsKeyPressed(int keycode);

		static bool IsMouseButtonPressed(int button);
		static std::pair<float, float> GetMousePosition();
		static void SetMousePosition(float posX, float posY);
		static void SetCursorInputMode(int value);
		static float GetMouseX();
		static float GetMouseY();
	};

}