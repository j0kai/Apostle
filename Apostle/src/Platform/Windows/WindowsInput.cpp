#include "APpch.h"
#include "Apostle/Core/Input.h"

#include <GLFW/glfw3.h>
#include "Apostle/Core/Application.h"
#include "Apostle/Events/MouseEvents.h"

namespace Apostle {

    bool Input::IsKeyPressed(int keycode)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::IsMouseButtonPressed(int button)
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    std::pair<float, float> Input::GetMousePosition()
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double posX, posY;
        glfwGetCursorPos(window, &posX, &posY);

        return { (float)posX, (float)posY };
    }

    float Input::GetMouseX()
    {
        auto [x, y] = GetMousePosition();
        return x;
    }

    float Input::GetMouseY()
    {
        auto [x, y] = GetMousePosition();
        return y;
    }

}