#pragma once

////////////////////////////////////////////////////////////////

/*------------For use by Apostle applications-----------------*/
 
////////////////////////////////////////////////////////////////

#include "Apostle/Core/Application.h"
#include "Apostle/Core/Layer.h"
#include "Apostle/ImGui/ImGuiLayer.h"
#include "Apostle/Core/Log.h"

#include "Apostle/Core/Timestep.h"

#include "Apostle/Core/Input.h"
#include "Apostle/Core/KeyCodes.h"
#include "Apostle/Core/MouseButtonCodes.h"

#include "Apostle/Events/ApplicationEvents.h"
#include "Apostle/Events/KeyEvents.h"
#include "Apostle/Events/MouseEvents.h"


/* ------------- ECS ---------------*/
#include "Apostle/ECS/Scene.h"
#include "Apostle/ECS/Entity.h"
#include "Apostle/ECS/ScriptableEntity.h"
#include "Apostle/ECS/Components.h"


/* ----------Renderer---------------*/
#include "Apostle/Renderer/Renderer.h"
#include "Apostle/Renderer/Renderer2D.h"
#include "Apostle/Renderer/RenderCommand.h"

#include "Apostle/Renderer/Shader.h"
#include "Apostle/Renderer/Texture.h"
#include "Apostle/Renderer/Framebuffer.h"
#include "Apostle/Renderer/SubTexture2D.h"
#include "Apostle/Renderer/VertexArray.h"
#include "Apostle/Renderer/Buffer.h"

#include "Apostle/Renderer/OrthographicCamera.h"
#include "Apostle/Renderer/OrthographicCameraController.h"

#include "Apostle/Renderer/PerspectiveCamera.h"