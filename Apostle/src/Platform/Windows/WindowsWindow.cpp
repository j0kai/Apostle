#include "APpch.h"
#include "WindowsWindow.h"

namespace Apostle {

    static bool s_GLFWInitialised = false;

    Window* Window::Create(const WindowProperties& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProperties& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }
    void WindowsWindow::Init(const WindowProperties& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        AP_CORE_INFO("Creating Window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialised)
        {
            int successful = glfwInit();
            AP_ASSERT(successful, "Could not initialise window!");
        }

        m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void WindowsWindow::SetVSync(bool isEnabled)
    {
        if (!isEnabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        
        m_Data.VSync = isEnabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }


}